#include <pebble.h>
#include "window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_font_lcd_40;
static GFont s_res_gothic_18_bold;
static GFont s_res_roboto_condensed_21;
static GFont s_res_gothic_24_bold;
static GFont s_res_gothic_18;
static TextLayer *s_textlayer_time;
static TextLayer *s_textlayer_stock1;
static TextLayer *s_textlayer_weekday;
static TextLayer *s_textlayer_day;
static TextLayer *s_textlayer_weather;
static TextLayer *s_textlayer_city;
static TextLayer *s_textlayer_stock2;
static TextLayer *s_textlayer_stock3;
static TextLayer *s_textlayer_stock4;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_font_lcd_40 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LCD_40));
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  // s_textlayer_time
  s_textlayer_time = text_layer_create(GRect(0, 0, 96, 40));
  text_layer_set_background_color(s_textlayer_time, GColorClear);
  text_layer_set_text_color(s_textlayer_time, GColorWhite);
  text_layer_set_text(s_textlayer_time, "14:49");
  text_layer_set_text_alignment(s_textlayer_time, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_time, s_res_font_lcd_40);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_time);
  
  // s_textlayer_stock1
  s_textlayer_stock1 = text_layer_create(GRect(2, 44, 142, 20));
  text_layer_set_background_color(s_textlayer_stock1, GColorClear);
  text_layer_set_text_color(s_textlayer_stock1, GColorWhite);
  text_layer_set_text(s_textlayer_stock1, "DIS: 102.67 (+0.47)");
  text_layer_set_font(s_textlayer_stock1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock1);
  
  // s_textlayer_weekday
  s_textlayer_weekday = text_layer_create(GRect(100, 0, 42, 21));
  text_layer_set_background_color(s_textlayer_weekday, GColorClear);
  text_layer_set_text_color(s_textlayer_weekday, GColorWhite);
  text_layer_set_text(s_textlayer_weekday, "TH");
  text_layer_set_text_alignment(s_textlayer_weekday, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_weekday, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_weekday);
  
  // s_textlayer_day
  s_textlayer_day = text_layer_create(GRect(100, 21, 42, 21));
  text_layer_set_background_color(s_textlayer_day, GColorClear);
  text_layer_set_text_color(s_textlayer_day, GColorWhite);
  text_layer_set_text(s_textlayer_day, "9/30");
  text_layer_set_text_alignment(s_textlayer_day, GTextAlignmentRight);
  text_layer_set_font(s_textlayer_day, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_day);
  
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
  text_layer_set_font(s_textlayer_city, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_city);
  
  // s_textlayer_stock2
  s_textlayer_stock2 = text_layer_create(GRect(2, 63, 142, 20));
  text_layer_set_background_color(s_textlayer_stock2, GColorClear);
  text_layer_set_text_color(s_textlayer_stock2, GColorWhite);
  text_layer_set_text(s_textlayer_stock2, "JPM: 61.42 (+0.45)");
  text_layer_set_font(s_textlayer_stock2, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock2);
  
  // s_textlayer_stock3
  s_textlayer_stock3 = text_layer_create(GRect(2, 82, 142, 20));
  text_layer_set_background_color(s_textlayer_stock3, GColorClear);
  text_layer_set_text_color(s_textlayer_stock3, GColorWhite);
  text_layer_set_text(s_textlayer_stock3, "FIT: 38.05 (+0.36)");
  text_layer_set_font(s_textlayer_stock3, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock3);
  
  // s_textlayer_stock4
  s_textlayer_stock4 = text_layer_create(GRect(2, 101, 142, 20));
  text_layer_set_background_color(s_textlayer_stock4, GColorClear);
  text_layer_set_text_color(s_textlayer_stock4, GColorWhite);
  text_layer_set_text(s_textlayer_stock4, "MDLZ: 42.21 (+0.34)");
  text_layer_set_font(s_textlayer_stock4, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_stock4);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_time);
  text_layer_destroy(s_textlayer_stock1);
  text_layer_destroy(s_textlayer_weekday);
  text_layer_destroy(s_textlayer_day);
  text_layer_destroy(s_textlayer_weather);
  text_layer_destroy(s_textlayer_city);
  text_layer_destroy(s_textlayer_stock2);
  text_layer_destroy(s_textlayer_stock3);
  text_layer_destroy(s_textlayer_stock4);
  fonts_unload_custom_font(s_res_font_lcd_40);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_window(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_window(void) {
  window_stack_remove(s_window, true);
}
