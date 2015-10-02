#include <pebble.h>
  #include "window.h"

  static int _nextStock;

enum
{
  KEY_TEMPERATURE = 0,
  KEY_CONDITIONS = 1,
  KEY_CITY = 2,
  // STOCKS
  KEY_STOCK_INDEX = 3,
  KEY_SYMBOL = 4,
  KEY_PRICE = 5,
  KEY_CHANGE = 6,
  // MSGS
  RefreshWeather = 7,
  GetQuote = 8
};

// returns true when market is open
static bool update_time()
{
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if (clock_is_24h_style() == true)
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  else
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);

  set_time(buffer);

  static char date_weekday[] = "XXX";

  strftime(date_weekday, sizeof(date_weekday), "%a",  tick_time);
  set_weekday(date_weekday);

  static char date_date[] = "XX/XX";  
  static char date_day[] = "XX";  
  static char date_month[] = "XX";

  strftime(date_day, sizeof(date_day), "%d",  tick_time);
  if (date_day[0] == '0')
    memmove(&date_day[0], &date_day[1], sizeof(date_day) - 1);

  strftime(date_month, sizeof(date_month), "%m",  tick_time);
  if (date_month[0] == '0')
    memmove(&date_month[0], &date_month[1], sizeof(date_month) - 1);

  snprintf(date_date, sizeof(date_date), "%s/%s", date_month, date_day);

  set_day(date_date);

  if (tick_time->tm_hour == 6 && tick_time->tm_min > 28)
    return true;  
  else if (tick_time->tm_hour > 6 && tick_time->tm_hour < 14)
    return true;

  return false;
}

static void fetch_weather()
{
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  Tuplet cmd = TupletInteger(RefreshWeather, 1);
  dict_write_tuplet(iter, &cmd); 
  dict_write_end(iter); 

  // Send the message!
  app_message_outbox_send();
}

static bool fetch_quote(bool retry)
{
  if (!retry)
  {
    _nextStock++;
    if (_nextStock > 3)
    { 
      return false;
    }
  }

  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  Tuplet cmd = TupletInteger(GetQuote, _nextStock);
  dict_write_tuplet(iter, &cmd); 
  dict_write_end(iter); 

  // Send the message!
  app_message_outbox_send();

  return true;
}

static void tick_handler_minutes(struct tm *tick_time, TimeUnits units_changed)
{
  static int stockCounter = 0;
  static int weatherCounter = 0;

  bool marketOpen = update_time();
  set_marketOpen(marketOpen);

  if (!marketOpen)
  {
    return;
  }
  
  ++stockCounter;
  ++ weatherCounter;

  // Get stock updates every 10 minutes
  if (stockCounter >= 10)
  {
    stockCounter = 0;
    _nextStock = 0;
    fetch_quote(false);
    return;
  }

  // Get weather update every 15 minutes
  if (weatherCounter >= 15)
  {
    weatherCounter = 0;
    fetch_weather(); 
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
  static char temperature_buffer[99];
  static char conditions_buffer[99];
  static char weather_layer_buffer[99];
  static char symbol_buffer[99];
  static char price_buffer[99];
  static char change_buffer[99];
  static char stock_layer_buffer[99];
  int index = 1;

  APP_LOG(APP_LOG_LEVEL_ERROR, "----got msg------");
  // Read first item
  Tuple *t = dict_read_first(iterator);

  *temperature_buffer=0;
  *conditions_buffer=0;
  *symbol_buffer=0;
  *price_buffer=0;


  while(t != NULL) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d", (int)t->key);

    switch(t->key) {
      case KEY_TEMPERATURE:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting temp with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%s\u00B0", t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_ERROR, "temp bufer is %s, %d", temperature_buffer, strlen(temperature_buffer));
      break;

      case KEY_CONDITIONS:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting conditions with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
      break;

      case KEY_CITY:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting city with %s, %d", t->value->cstring, strlen(t->value->cstring));
      set_city(t->value->cstring);
      break;

      case KEY_STOCK_INDEX:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting index stock to %d", (int)t->value->int32);
      index = t->value->int32;
      break;

      case KEY_SYMBOL:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting symbol with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(symbol_buffer, sizeof(symbol_buffer), "%s", t->value->cstring);
      break;

      case KEY_PRICE:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting price with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(price_buffer, sizeof(price_buffer), "%s", t->value->cstring);
      break;

      case KEY_CHANGE:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting change with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(change_buffer, sizeof(change_buffer), "%s", t->value->cstring);
      break;

      default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }

  if (*temperature_buffer && *conditions_buffer)
  {
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s %s", temperature_buffer, conditions_buffer);
    APP_LOG(APP_LOG_LEVEL_ERROR, "setting weather with [%s]", weather_layer_buffer);
    set_weather(weather_layer_buffer);  
  }

  if (*symbol_buffer && *price_buffer)
  {
    snprintf(stock_layer_buffer, sizeof(stock_layer_buffer), "%s: %s", symbol_buffer, price_buffer);
    APP_LOG(APP_LOG_LEVEL_ERROR, "setting stock to [%s]", stock_layer_buffer);
    set_stock(index, stock_layer_buffer);
  }

  if (_nextStock <= 3)
    fetch_quote(false);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", reason);
  if (_nextStock <= 3)
    fetch_quote(true);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init()
{  
  _nextStock = 0;

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_minutes);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  show_window();
}

static void deinit()
{
  hide_window();
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}