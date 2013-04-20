#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xDD, 0xA8, 0x71, 0x95, 0x63, 0x86, 0x40, 0xE7, 0x90, 0x19, 0x6C, 0xD8, 0x98, 0xC2, 0xAB, 0xC5 }
PBL_APP_INFO(MY_UUID,
			 "Homestuck Time", "Wendel Scardua",
			 1, 0, /* App version */
			 RESOURCE_ID_IMAGE_MENU_ICON,
			 APP_INFO_WATCH_FACE);

Window s_window;
TextLayer s_text;

#define TIME_STR_BUFFER_BYTES 32
char s_time_str_buffer[TIME_STR_BUFFER_BYTES];

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
    string_format_time(s_time_str_buffer, TIME_STR_BUFFER_BYTES, "%H:%M:%S", event->tick_time);
    text_layer_set_text(&s_text, s_time_str_buffer);
}

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&s_window, "Fenestrated Plane");
    window_stack_push(&s_window, true /* Animated */);

    text_layer_init(&s_text, s_window.layer.frame);
	text_layer_set_font(&s_text, fonts_get_system_font(FONT_KEY_GOTHAM_42_MEDIUM_NUMBERS));
    strcpy(s_time_str_buffer, "");
    text_layer_set_text(&s_text, s_time_str_buffer);
    layer_add_child(&s_window.layer, &s_text.layer);
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