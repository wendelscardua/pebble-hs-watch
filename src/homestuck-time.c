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

BmpContainer background_image;

const int BACKGROUND_RESOURCE_IDS[] = {
	RESOURCE_ID_IMAGE_BACKGROUND_0,
	RESOURCE_ID_IMAGE_BACKGROUND_1,
	RESOURCE_ID_IMAGE_BACKGROUND_2,
	RESOURCE_ID_IMAGE_BACKGROUND_3,
};

static int bg_count = 0;

#define TIME_STR_BUFFER_BYTES 32
char time_text_buffer[TIME_STR_BUFFER_BYTES];

void set_container_image(BmpContainer *bmp_container, const int resource_id, GPoint origin) {

	layer_remove_from_parent(&bmp_container->layer.layer);
	layer_remove_from_parent(&text_layer.layer);
	bmp_deinit_container(bmp_container);

	bmp_init_container(resource_id, bmp_container);

	GRect frame = layer_get_frame(&bmp_container->layer.layer);
	frame.origin.x = origin.x;
	frame.origin.y = origin.y;
	layer_set_frame(&bmp_container->layer.layer, frame);

	layer_add_child(&window.layer, &bmp_container->layer.layer);
	layer_add_child(&window.layer, &text_layer.layer);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *event) {
	set_container_image(&background_image, BACKGROUND_RESOURCE_IDS[bg_count], GPoint(0, 0));
	bg_count = (bg_count + 1) % 4;
    string_format_time(time_text_buffer, TIME_STR_BUFFER_BYTES, "%H:%M:%S", event->tick_time);
    text_layer_set_text(&text_layer, time_text_buffer);
}

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&window, "Fenestrated Plane");
    window_stack_push(&window, true /* Animated */);

	resource_init_current_app(&APP_RESOURCES);

	// background time symbol
	bmp_init_container(BACKGROUND_RESOURCE_IDS[3], &background_image);
    layer_add_child(&window.layer, &background_image.layer.layer);


    text_layer_init(&text_layer, GRect(28, 54, 100, 32));
	text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_color(&text_layer, GColorWhite);
	text_layer_set_background_color(&text_layer, GColorBlack);
	
    strcpy(time_text_buffer, "");
    text_layer_set_text(&text_layer, time_text_buffer);
    layer_add_child(&window.layer, &text_layer.layer);
}


void handle_deinit(AppContextRef ctx) {
	(void)ctx;

    bmp_deinit_container(&background_image);

}
void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,
	    .deinit_handler = &handle_deinit,
        .tick_info = {
            .tick_handler = &handle_tick,
            .tick_units = SECOND_UNIT
        }
    };
    app_event_loop(params, &handlers);
}