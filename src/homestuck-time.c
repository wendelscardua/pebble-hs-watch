#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xDD, 0xA8, 0x71, 0x95, 0x63, 0x86, 0x40, 0xE7, 0x90, 0x19, 0x6C, 0xD8, 0x98, 0xC2, 0xAB, 0xC5 }
PBL_APP_INFO(MY_UUID,
			 "Homestuck Time", "Wendel Scardua",
			 1, 0, /* App version */
			 RESOURCE_ID_IMAGE_MENU_ICON,
			 APP_INFO_WATCH_FACE);

Window window;
TextLayer text_layer;

#define TIME_STR_BUFFER_BYTES 32
char time_text_buffer[TIME_STR_BUFFER_BYTES];

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
    string_format_time(time_text_buffer, TIME_STR_BUFFER_BYTES, "%H:%M:%S", event->tick_time);
    text_layer_set_text(&text_layer, time_text_buffer);
}

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&window, "Fenestrated Plane");
    window_stack_push(&window, true /* Animated */);

    text_layer_init(&text_layer, window.layer.frame);
	text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_color(&text_layer, GColorWhite);
	text_layer_set_background_color(&text_layer, GColorBlack);
    strcpy(time_text_buffer, "");
    text_layer_set_text(&text_layer, time_text_buffer);
    layer_add_child(&window.layer, &text_layer.layer);
}

void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,
        .tick_info = {
            .tick_handler = &handle_tick,
            .tick_units = SECOND_UNIT
        }
    };
    app_event_loop(params, &handlers);
}