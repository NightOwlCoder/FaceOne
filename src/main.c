#include <pebble.h>
#include "window.h"

static int stockCounter = 0;
static int weatherCounter = 0;
static int StockCount = 4;
static int _nextStock;
static char temperature_buffer[10];
static char conditions_buffer[80];
static char weather_layer_buffer[99];
static char city_buffer[80];
static char stock_layer_buffer[4][20];

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
	struct tm *tick_time_utc = gmtime(&temp);

	APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "hora %d corrigida %d utc %d", tick_time_utc->tm_hour, tick_time->tm_hour, (int)clock_is_timezone_set());

	// Create a long-lived buffer
	static char buffer[] = "00:00";

	strftime(buffer, sizeof("00:00"), "%l:%M", tick_time);
	if (buffer[0] == ' ')
		memmove(&buffer[0], &buffer[1], sizeof(buffer) - 1);

	set_time(buffer, tick_time->tm_hour < 12);

	static char date_weekday[] = "XXX";

	strftime(date_weekday, sizeof(date_weekday), "%a", tick_time);
	set_weekday(date_weekday);

	static char date_date[] = "XX/XX";
	static char date_day[] = "XX";
	static char date_month[] = "XX";

	strftime(date_day, sizeof(date_day), "%d", tick_time);
	if (date_day[0] == '0')
		memmove(&date_day[0], &date_day[1], sizeof(date_day) - 1);

	strftime(date_month, sizeof(date_month), "%m", tick_time);
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

	APP_LOG(APP_LOG_LEVEL_INFO, "setting mkt open to %d, week day = %d", (int)open, tick_time->tm_wday);

	if (open)
	{
		int closeAt = (13 * 60);
		int current = (tick_time->tm_hour * 60) + tick_time->tm_min;
		int timeToClose = closeAt - current;
		int h = timeToClose / 60;
		int m = timeToClose - (h * 60);

		snprintf(timetoclose, countb, "-%d:%02d %d/%d", h, m, 20-weatherCounter, 10-stockCounter);
	}
  else
 		snprintf(timetoclose, countb, "cl %d/%d", 20-weatherCounter, 10-stockCounter);


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

	// Send the message
	app_message_outbox_send();

	return true;
}

static void tick_handler_minutes(struct tm *tick_time, TimeUnits units_changed)
{
	static char timeToClose[30];

	++weatherCounter;
	if (stockCounter < 10)
	  ++stockCounter;
  
	// Get weather update every 20 minutes
	if (weatherCounter >= 20)
	{
		weatherCounter = 0;
		fetch_weather();
	}

	bool marketOpen = update_time(timeToClose, sizeof(timeToClose));

	set_marketOpen(marketOpen, timeToClose);

	if (!marketOpen)
	{
		return;
	}

	// Get stock updates every 10 minutes
	if (stockCounter >= 10)
	{
		stockCounter = 0;
		_nextStock = 0;
		fetch_quote(false);
		return;
	}
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
	int index = 1;

	APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "----got msg------");
	// Read first item
	Tuple *t = dict_read_first(iterator);

	*temperature_buffer = 0;
	*conditions_buffer = 0;
	*city_buffer = 0;
	for (int i = 0; i <= 3; i++)
		*stock_layer_buffer[i] = 0;

	while (t != NULL) {
		APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Key %d", (int)t->key);

		switch (t->key) {
		case KEY_TEMPERATURE:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting temp with %s, %d", t->value->cstring, strlen(t->value->cstring));
			snprintf(temperature_buffer, sizeof(temperature_buffer), "%s\u00B0", t->value->cstring);
			break;

		case KEY_CONDITIONS:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting conditions with %s, %d", t->value->cstring, strlen(t->value->cstring));
			snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
			break;

		case KEY_CITY:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting city with %s, %d", t->value->cstring, strlen(t->value->cstring));
			snprintf(city_buffer, sizeof(city_buffer), "%s", t->value->cstring);
			persist_write_string(KEY_CITY, city_buffer);
			break;

		case KEY_STOCK_1:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting stock 1 to %s", t->value->cstring);
			snprintf(stock_layer_buffer[0], sizeof(stock_layer_buffer[0]), "%s", t->value->cstring);
			persist_write_string(KEY_STOCK_1, stock_layer_buffer[0]);
			break;

		case KEY_STOCK_2:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting stock 2 to %s", t->value->cstring);
			snprintf(stock_layer_buffer[1], sizeof(stock_layer_buffer[1]), "%s", t->value->cstring);
			persist_write_string(KEY_STOCK_2, stock_layer_buffer[1]);
			break;

		case KEY_STOCK_3:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting stock 3 to %s", t->value->cstring);
			snprintf(stock_layer_buffer[2], sizeof(stock_layer_buffer[2]), "%s", t->value->cstring);
			persist_write_string(KEY_STOCK_3, stock_layer_buffer[2]);
			break;

		case KEY_STOCK_4:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting stock 4 to %s", t->value->cstring);
			snprintf(stock_layer_buffer[3], sizeof(stock_layer_buffer[3]), "%s", t->value->cstring);
			persist_write_string(KEY_STOCK_4, stock_layer_buffer[3]);
			break;

		default:
			APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "Key %d not recognized!", (int)t->key);
			break;
		}

		// Look for next item
		t = dict_read_next(iterator);
	}

	if (*temperature_buffer && *conditions_buffer)
	{
		snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s %s", temperature_buffer, conditions_buffer);
		APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "setting weather with [%s]", weather_layer_buffer);
		set_weather(weather_layer_buffer);
		persist_write_string(KEY_TEMPERATURE, weather_layer_buffer);
	}

	if (*city_buffer) set_city(city_buffer);
	for (int i = 0; i <= 3; i++)
		if (*stock_layer_buffer[i]) set_stock(i + 1, stock_layer_buffer[i]);
}

static void printAppResult(AppMessageResult err)
{
	if (err == APP_MSG_OK) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_OK");
	else if (err == APP_MSG_ALREADY_RELEASED) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_ALREADY_RELEASED");
	else if (err == APP_MSG_BUFFER_OVERFLOW) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_BUFFER_OVERFLOW");
	else if (err == APP_MSG_BUSY) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_BUSY");
	else if (err == APP_MSG_CALLBACK_ALREADY_REGISTERED) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_CALLBACK_ALREADY_REGISTERED");
	else if (err == APP_MSG_CALLBACK_NOT_REGISTERED) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_CALLBACK_NOT_REGISTERED");
	else if (err == APP_MSG_INVALID_ARGS) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_INVALID_ARGS");
	else if (err == APP_MSG_NOT_CONNECTED) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_NOT_CONNECTED");
	else if (err == APP_MSG_OUT_OF_MEMORY) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_OUT_OF_MEMORY");
	else if (err == APP_MSG_SEND_REJECTED) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_SEND_REJECTED");
	else if (err == APP_MSG_SEND_TIMEOUT) APP_LOG(APP_LOG_LEVEL_DEBUG, "APP_MSG_SEND_TIMEOUT");
}

static void inbox_dropped_callback(AppMessageResult reason, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! Reason: %d", reason);
	printAppResult(reason);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", reason);
	printAppResult(reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void setStockFromPersist(int stock)
{
	int index = KEY_STOCK_1 + stock - 1;
	if (persist_exists(index))
	{
		persist_read_string(index, stock_layer_buffer[index], sizeof(stock_layer_buffer[index]));
		set_stock(stock, stock_layer_buffer[index]);
	}
	else
		set_stock(stock, "");
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
	int r = app_message_open(128, 128);
	APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "MessageOpen result: %d", r);

	show_window();

	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_minutes);

	// Call handler once to populate initial time display
	time_t current_time = time(NULL);
	tick_handler_minutes(localtime(&current_time), MINUTE_UNIT);

	if (persist_exists(KEY_TEMPERATURE))
	{
		persist_read_string(KEY_TEMPERATURE, temperature_buffer, sizeof(temperature_buffer));
		set_weather(temperature_buffer);
	}
	else
		set_weather("waiting...");

	if (persist_exists(KEY_CITY))
	{
		persist_read_string(KEY_CITY, city_buffer, sizeof(city_buffer));
		set_city(city_buffer);
	}
	else
		set_city("locating...");

	for (int i = 1; i <= 4; i++)
		setStockFromPersist(i);
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