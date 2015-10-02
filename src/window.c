#include <pebble.h>
  #include "window.h"

  // BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_gothic_18;
static GFont s_res_segoe_16;
static GFont s_res_roboto_condensed_21;
static GFont s_res_segoe_12;
static TextLayer *s_textlayer_time;
static TextLayer *s_textlayer_stock1;
static TextLayer *s_textlayer_weekday;
static TextLayer *s_textlayer_weather;
static TextLayer *s_textlayer_city;
static TextLayer *s_textlayer_stock2;
static TextLayer *s_textlayer_day;
static TextLayer *s_textlayer_stock3;
static TextLayer *s_textlayer_market;
static TextLayer *s_textlayer_line;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_segoe_16 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SEGOE_16));
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_segoe_12 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SEGOE_12));
  // s_textlayer_time
  s_textlayer_time = text_layer_create(GRect(0, -5, 104, 43));
  text_layer_set_background_color(s_textlayer_time, GColorClear);
  text_layer_set_text_color(s_textlayer_time, GColorWhite);
  text_layer_set_text(s_textlayer_time, "12:00");
  text_layer_set_font(s_textlayer_time, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_time);
  
  // s_textlayer_stock1
  s_textlayer_stock1 = text_layer_create(GRect(2, 42, 142, 20));
  text_layer_set_background_color(s_textlayer_stock1, GColorClear);
  text_layer_set_text_color(s_textlayer_stock1, GColorWhite);
  text_layer_set_text(s_textlayer_stock1, "DIS: 102.67 (+0.27)");
  text_layer_set_text_alignment(s_textlayer_stock1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_stock1, s_res_segoe_16);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock1);
  
  // s_textlayer_weekday
  s_textlayer_weekday = text_layer_create(GRect(99, 20, 44, 18));
  text_layer_set_background_color(s_textlayer_weekday, GColorClear);
  text_layer_set_text_color(s_textlayer_weekday, GColorWhite);
  text_layer_set_text(s_textlayer_weekday, "THU");
  text_layer_set_text_alignment(s_textlayer_weekday, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_weekday, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_weekday);
  
  // s_textlayer_weather
  s_textlayer_weather = text_layer_create(GRect(44, 140, 100, 16));
  text_layer_set_background_color(s_textlayer_weather, GColorClear);
  text_layer_set_text_color(s_textlayer_weather, GColorWhite);
  text_layer_set_text(s_textlayer_weather, "81o, Clouds");
  text_layer_set_text_alignment(s_textlayer_weather, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_weather, s_res_segoe_16);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_weather);
  
  // s_textlayer_city
  s_textlayer_city = text_layer_create(GRect(44, 156, 100, 12));
  text_layer_set_background_color(s_textlayer_city, GColorClear);
  text_layer_set_text_color(s_textlayer_city, GColorWhite);
  text_layer_set_text(s_textlayer_city, "Stevenson Ranch");
  text_layer_set_text_alignment(s_textlayer_city, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_city, s_res_segoe_12);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_city);
  
  // s_textlayer_stock2
  s_textlayer_stock2 = text_layer_create(GRect(1, 63, 142, 21));
  text_layer_set_background_color(s_textlayer_stock2, GColorClear);
  text_layer_set_text_color(s_textlayer_stock2, GColorWhite);
  text_layer_set_text(s_textlayer_stock2, "JPM: 59.99");
  text_layer_set_text_alignment(s_textlayer_stock2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_stock2, s_res_segoe_16);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock2);
  
  // s_textlayer_day
  s_textlayer_day = text_layer_create(GRect(99, 0, 44, 21));
  text_layer_set_background_color(s_textlayer_day, GColorClear);
  text_layer_set_text_color(s_textlayer_day, GColorWhite);
  text_layer_set_text(s_textlayer_day, "0/00");
  text_layer_set_text_alignment(s_textlayer_day, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_day, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_day);
  
  // s_textlayer_stock3
  s_textlayer_stock3 = text_layer_create(GRect(2, 84, 142, 21));
  text_layer_set_background_color(s_textlayer_stock3, GColorClear);
  text_layer_set_text_color(s_textlayer_stock3, GColorWhite);
  text_layer_set_text(s_textlayer_stock3, "FIT:38.68");
  text_layer_set_text_alignment(s_textlayer_stock3, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_stock3, s_res_segoe_16);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock3);
  
  // s_textlayer_market
  s_textlayer_market = text_layer_create(GRect(0, 152, 40, 16));
  text_layer_set_text(s_textlayer_market, "closed");
  text_layer_set_text_alignment(s_textlayer_market, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_market, s_res_segoe_12);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_market);
  
  // s_textlayer_line
  s_textlayer_line = text_layer_create(GRect(0, 40, 144, 1));
  text_layer_set_text(s_textlayer_line, "Text layer");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_line);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_time);
  text_layer_destroy(s_textlayer_stock1);
  text_layer_destroy(s_textlayer_weekday);
  text_layer_destroy(s_textlayer_weather);
  text_layer_destroy(s_textlayer_city);
  text_layer_destroy(s_textlayer_stock2);
  text_layer_destroy(s_textlayer_day);
  text_layer_destroy(s_textlayer_stock3);
  text_layer_destroy(s_textlayer_market);
  text_layer_destroy(s_textlayer_line);
  fonts_unload_custom_font(s_res_segoe_16);
  fonts_unload_custom_font(s_res_segoe_12);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window)
{
  destroy_ui();
}

char *upcase(char *str)
{
  for (int i = 0; str[i] != 0; i++)
  {
    if (str[i] >= 'a' && str[i] <= 'z')
      str[i] -= 0x20;
  }
  return str;
}

void show_window(void)
{
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_window(void)
{
  window_stack_remove(s_window, true);
}

static TextLayer* cl_init_text_layer(GRect location, GColor colour, GColor background, const char *res_id, GTextAlignment alignment)
{
  TextLayer *layer = text_layer_create(location);
  text_layer_set_text_color(layer, colour);
  text_layer_set_background_color(layer, background);
  text_layer_set_font(layer, fonts_get_system_font(res_id));
  text_layer_set_text_alignment(layer, alignment);

  return layer;
} 

void set_time(char * value, bool am)
{  
  text_layer_set_text(s_textlayer_time, value);

//  GSize size = text_layer_get_content_size(s_textlayer_time);

//  text_layer_destroy(s_textlayer_ampm);

//  s_textlayer_ampm = text_layer_create(GRect(size.w+2, 2, 20, 16));
//  if (am)
//    text_layer_set_text(s_textlayer_ampm, "AM");
//  else
//    text_layer_set_text(s_textlayer_ampm, "PM");
//  text_layer_set_text_alignment(s_textlayer_ampm, GTextAlignmentCenter);
//  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_ampm);
}

void set_weekday(char * value)
{  
  text_layer_set_text(s_textlayer_weekday, upcase(value));
}

void set_day(char * value)
{
  text_layer_set_text(s_textlayer_day, value);
}

void set_weather(char * value)
{  
  text_layer_set_text(s_textlayer_weather, value);
}

void set_city(char * value)
{  
  text_layer_set_text(s_textlayer_city, value);
}

void set_marketOpen(bool open, char * timetoclose)
{
  if (open)
  {
    text_layer_set_background_color(s_textlayer_market, GColorClear);
    text_layer_set_text_color(s_textlayer_market, GColorWhite);
    text_layer_set_text(s_textlayer_market, timetoclose);
  }
  else
  {
    text_layer_set_background_color(s_textlayer_market, GColorWhite);
    text_layer_set_text_color(s_textlayer_market, GColorBlack);
    text_layer_set_text(s_textlayer_market, "closed");
  }
}

static char* stock1_buffer = NULL;
static char* stock2_buffer = NULL;
static char* stock3_buffer = NULL;

void alocate_and_tranfer(TextLayer *text_layer, char* buffer, char* source_buffer)
{
  if (buffer != NULL)
  {
    free(buffer);    
  }
  int size = strlen(source_buffer);
  buffer = malloc(size+1);
  strncpy(buffer, source_buffer, size );

  text_layer_set_text(text_layer, buffer);
}

void set_stock(int index, char * value)
{  
  switch (index)
  {
    case 1:
    alocate_and_tranfer(s_textlayer_stock1, stock1_buffer, value);
    break;    
    case 2:
    alocate_and_tranfer(s_textlayer_stock2, stock2_buffer, value);
    break;    
    case 3:
    alocate_and_tranfer(s_textlayer_stock3, stock3_buffer, value);
    break;
  }
}
