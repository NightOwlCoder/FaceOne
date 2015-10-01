#include <pebble.h>
#include "window.h"
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_weather_city_layer;
static TextLayer *s_symbol_layer;
static TextLayer *s_symbol_price_layer;

static GFont s_time_font;

enum {
  KEY_TEMPERATURE = 0,
  KEY_CONDITIONS = 1,
  KEY_CITY = 2,
  // STOCKS
  KEY_SYMBOL = 3,
  KEY_PRICE = 4,
  // MSGS
  RefreshWeather = 5,
  GetQuote = 6
};

static void update_time() {
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

  text_layer_set_text(s_time_layer, buffer);
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

static void fetch_quote()
{
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  Tuplet cmd = TupletCString(GetQuote, "DIS");
  dict_write_tuplet(iter, &cmd); 
  dict_write_end(iter); 

  // Send the message!
  app_message_outbox_send();
}

static void main_window_load(Window *window) {

  //Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCD_40));

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(4, 0, 140, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, s_time_font);

  // Create symbol TextLayer
  s_symbol_layer = text_layer_create(GRect(4, 45, 140, 50));
  text_layer_set_background_color(s_symbol_layer, GColorClear);
  text_layer_set_text_color(s_symbol_layer, GColorBlack);
  text_layer_set_font(s_symbol_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  // Create Weather Layer
  s_weather_layer = text_layer_create(GRect(0, 127, 140, 35));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorBlack);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentRight);
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(s_weather_layer, "guessing...");

  // Create Weather City Layer
  s_weather_city_layer = text_layer_create(GRect(0, 147, 140, 35));
  text_layer_set_background_color(  s_weather_city_layer, GColorClear);
  text_layer_set_text_color(  s_weather_city_layer, GColorBlack);
  text_layer_set_text_alignment(  s_weather_city_layer, GTextAlignmentRight);
  text_layer_set_font(s_weather_city_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  // Add to child layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_symbol_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_city_layer));

  update_time();
  fetch_weather();
  fetch_quote();
}

static void main_window_unload(Window *window) {
  //Unload GFont
  fonts_unload_custom_font(s_time_font);

  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_symbol_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_weather_city_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time();

  // Get stock updates every 5 minutes
  if (tick_time->tm_min % 5 == 0)
    fetch_quote();

  // Get weather update every 15 minutes
  if (tick_time->tm_min % 15 == 0)
    fetch_weather();  
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  static char temperature_buffer[99];
  static char conditions_buffer[99];
  static char weather_layer_buffer[99];
  static char symbol_buffer[99];
  static char price_buffer[99];
  static char stock_layer_buffer[99];

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
      text_layer_set_text(s_weather_city_layer, t->value->cstring);    
      break;

      case KEY_SYMBOL:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting symbol with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(symbol_buffer, sizeof(symbol_buffer), "%s", t->value->cstring);
      break;

      case KEY_PRICE:
      APP_LOG(APP_LOG_LEVEL_ERROR, "setting price with %s, %d", t->value->cstring, strlen(t->value->cstring));
      snprintf(price_buffer, sizeof(price_buffer), "%s", t->value->cstring);
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
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
    APP_LOG(APP_LOG_LEVEL_ERROR, "setting weather with [%s]", weather_layer_buffer);
    text_layer_set_text(s_weather_layer, weather_layer_buffer);  
  }

  if (*symbol_buffer && *price_buffer)
  {
    snprintf(stock_layer_buffer, sizeof(stock_layer_buffer), "%s: %s (0)", symbol_buffer, price_buffer);
    APP_LOG(APP_LOG_LEVEL_ERROR, "setting stock to [%s]", stock_layer_buffer);
    text_layer_set_text(s_symbol_layer, stock_layer_buffer);  
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);  
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

