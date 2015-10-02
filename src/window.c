#include <pebble.h>
  #include "window.h"

  // BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_gothic_24_bold;
static GFont s_res_gothic_18;
static GFont s_res_gothic_18_bold;
static TextLayer *s_textlayer_time;
static TextLayer *s_textlayer_stock1;
static TextLayer *s_textlayer_weekday;
static TextLayer *s_textlayer_weather;
static TextLayer *s_textlayer_city;
static TextLayer *s_textlayer_stock2;
static TextLayer *s_textlayer_day;
static TextLayer *s_textlayer_stock3;
static TextLayer *s_textlayer_market;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_textlayer_time
  s_textlayer_time = text_layer_create(GRect(0, -8, 104, 43));
  text_layer_set_background_color(s_textlayer_time, GColorClear);
  text_layer_set_text_color(s_textlayer_time, GColorWhite);
  text_layer_set_text(s_textlayer_time, "55:55");
  text_layer_set_font(s_textlayer_time, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_time);
  
  // s_textlayer_stock1
  s_textlayer_stock1 = text_layer_create(GRect(2, 46, 142, 24));
  text_layer_set_background_color(s_textlayer_stock1, GColorClear);
  text_layer_set_text_color(s_textlayer_stock1, GColorWhite);
  text_layer_set_text(s_textlayer_stock1, "DIS: 102.67");
  text_layer_set_text_alignment(s_textlayer_stock1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_stock1, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock1);
  
  // s_textlayer_weekday
  s_textlayer_weekday = text_layer_create(GRect(104, 0, 40, 18));
  text_layer_set_background_color(s_textlayer_weekday, GColorClear);
  text_layer_set_text_color(s_textlayer_weekday, GColorWhite);
  text_layer_set_text(s_textlayer_weekday, "THU");
  text_layer_set_text_alignment(s_textlayer_weekday, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_weekday, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_weekday);
  
  // s_textlayer_weather
  s_textlayer_weather = text_layer_create(GRect(42, 123, 100, 24));
  text_layer_set_background_color(s_textlayer_weather, GColorClear);
  text_layer_set_text_color(s_textlayer_weather, GColorWhite);
  text_layer_set_text(s_textlayer_weather, "81o, Clouds");
  text_layer_set_text_alignment(s_textlayer_weather, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_weather, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_weather);
  
  // s_textlayer_city
  s_textlayer_city = text_layer_create(GRect(42, 148, 100, 20));
  text_layer_set_background_color(s_textlayer_city, GColorClear);
  text_layer_set_text_color(s_textlayer_city, GColorWhite);
  text_layer_set_text(s_textlayer_city, "Stevenson Ranch");
  text_layer_set_text_alignment(s_textlayer_city, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_city, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_city);
  
  // s_textlayer_stock2
  s_textlayer_stock2 = text_layer_create(GRect(2, 70, 142, 24));
  text_layer_set_background_color(s_textlayer_stock2, GColorClear);
  text_layer_set_text_color(s_textlayer_stock2, GColorWhite);
  text_layer_set_text(s_textlayer_stock2, "JPM: 59.99");
  text_layer_set_text_alignment(s_textlayer_stock2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_stock2, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock2);
  
  // s_textlayer_day
  s_textlayer_day = text_layer_create(GRect(104, 18, 40, 18));
  text_layer_set_background_color(s_textlayer_day, GColorClear);
  text_layer_set_text_color(s_textlayer_day, GColorWhite);
  text_layer_set_text(s_textlayer_day, "7/9");
  text_layer_set_text_alignment(s_textlayer_day, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_day, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_day);
  
  // s_textlayer_stock3
  s_textlayer_stock3 = text_layer_create(GRect(2, 94, 142, 24));
  text_layer_set_background_color(s_textlayer_stock3, GColorClear);
  text_layer_set_text_color(s_textlayer_stock3, GColorWhite);
  text_layer_set_text(s_textlayer_stock3, "FIT:38.68");
  text_layer_set_text_alignment(s_textlayer_stock3, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_stock3, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock3);
  
  // s_textlayer_market
  s_textlayer_market = text_layer_create(GRect(0, 152, 40, 16));
  text_layer_set_text(s_textlayer_market, "closed");
  text_layer_set_text_alignment(s_textlayer_market, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_market);
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

void set_time(char * value)
{  
  text_layer_set_text(s_textlayer_time, value);
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

void set_marketOpen(bool open)
{
  if (open)
  {
    text_layer_set_text(s_textlayer_market, "open");
    text_layer_set_background_color(s_textlayer_market, GColorClear);
    text_layer_set_text_color(s_textlayer_market, GColorWhite);
  }
  else
  {
    text_layer_set_text(s_textlayer_market, "closed");
    text_layer_set_background_color(s_textlayer_market, GColorWhite);
    text_layer_set_text_color(s_textlayer_market, GColorBlack);
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
