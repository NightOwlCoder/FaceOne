#include <pebble.h>
#include "window.h"

static int StockCount = 4;
static int _nextStock;

enum
{
  KEY_TEMPERATURE = 0,
  KEY_CONDITIONS = 1,
  KEY_CITY = 2,
  // STOCKS
  KEY_STOCK_1 = 3,
  KEY_STOCK_2 = 4,
  KEY_STOCK_3 = 5,
  KEY_STOCK_4 = 6,
  // MSGS
  RefreshWeather = 7,
  GetQuotes = 8
};

// returns true when market is open
static bool update_time(char *timetoclose, int countb)
{
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  struct tm *tick_time_utc  = gmtime(&temp);


  APP_LOG(APP_LOG_LEVEL_ERROR, "hora %d corrigida %d utc %d", tick_time_utc->tm_hour, tick_time->tm_hour, (int)clock_is_timezone_set());

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  strftime(buffer, sizeof("00:00"), "%l:%M", tick_time);
  if (buffer[0] == ' ')
    memmove(&buffer[0], &buffer[1], sizeof(buffer) - 1);

  set_time(buffer, tick_time->tm_hour < 12);

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

  bool open = false;

  if (tick_time->tm_hour == 6 && tick_time->tm_min >= 30)
    open = true;
  else if (tick_time->tm_hour > 6 && tick_time->tm_hour < 13)
    open = true;

    if (tick_time->tm_wday == 0 || tick_time->tm_wday == 6)
    open = false;

    APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting mkt open to %d, week day = %d", (int)open, tick_time->tm_wday);

  if (open)
  {
    int closeAt = (13 * 60);
    int current = (tick_time->tm_hour * 60) + tick_time->tm_min;
    int timeToClose = closeAt - current;
    int h = timeToClose / 60;
    int m = timeToClose - (h * 60);

    snprintf(timetoclose, countb, "-%d:%02d", h, m);
  }

  return open;
}

static void fetch_weather()
{
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
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  Tuplet cmd = TupletInteger(GetQuotes, _nextStock);
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
  static char timeToClose[30];

  bool marketOpen = update_time(timeToClose, sizeof(timeToClose));

  APP_LOG(APP_LOG_LEVEL_ERROR, "setting mkt open to %d, %s", (int)marketOpen, timeToClose);

  set_marketOpen(marketOpen, timeToClose);

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
  static char stock1_layer_buffer[99];
  static char stock2_layer_buffer[99];
  static char stock3_layer_buffer[99];
  static char stock4_layer_buffer[99];
  int index = 1;

  APP_LOG(APP_LOG_LEVEL_ERROR, "----got msg------");
  // Read first item
  Tuple *t = dict_read_first(iterator);

  *temperature_buffer=0;
  *conditions_buffer=0;
  *stock1_layer_buffer=0;
  *stock2_layer_buffer=0;
  *stock3_layer_buffer=0;
  *stock4_layer_buffer=0;

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

      case KEY_STOCK_1:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting stock 1 to %s", t->value->cstring);
      snprintf(stock1_layer_buffer, sizeof(stock1_layer_buffer), "%s", t->value->cstring);
      break;

      case KEY_STOCK_2:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting stock 2 to %s", t->value->cstring);
      snprintf(stock2_layer_buffer, sizeof(stock2_layer_buffer), "%s", t->value->cstring);
      break;

      case KEY_STOCK_3:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting stock 3 to %s", t->value->cstring);
      snprintf(stock3_layer_buffer, sizeof(stock3_layer_buffer), "%s", t->value->cstring);
      break;

      case KEY_STOCK_4:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting stock 4 to %s", t->value->cstring);
      snprintf(stock4_layer_buffer, sizeof(stock4_layer_buffer), "%s", t->value->cstring);
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

  if (*stock1_layer_buffer) set_stock(1, stock1_layer_buffer);
  if (*stock2_layer_buffer)   set_stock(2, stock2_layer_buffer);
  if (*stock3_layer_buffer) set_stock(3, stock3_layer_buffer);
  if (*stock4_layer_buffer) set_stock(4, stock4_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", reason);
  if (_nextStock <= StockCount)
    fetch_quote(true);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init()
{  
  _nextStock = 0;

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  show_window();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_minutes);


  // Call handler once to populate initial time display
  time_t current_time = time(NULL);
  tick_handler_minutes(localtime(&current_time), SECOND_UNIT | MINUTE_UNIT);
  set_stock(1, "DIS...");
  set_stock(2, "SKX...");
  set_stock(3, "SBUX...");
  set_stock(4, "FIT...");
  set_weather("waiting...");
  set_city("locating...");
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