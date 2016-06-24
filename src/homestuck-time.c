#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap[4];

static int tick_tock = 0;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[11];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M:%S" : "%I:%M:%S", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_text_layer, s_buffer);

  tick_tock = (tick_tock + 1) % 4;
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap[tick_tock]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Set window background
  window_set_background_color(window, GColorBlack);

  // Create GBitmap
  s_background_bitmap[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_0);
  s_background_bitmap[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_1);
  s_background_bitmap[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_2);
  s_background_bitmap[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BG_3);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap[0]);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Text layer
  s_text_layer = text_layer_create(GRect(0, (bounds.size.h - 32) / 2, bounds.size.w, 32));
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorWhite);
  text_layer_set_text(s_text_layer, "04:13:00");
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap[0]);
  gbitmap_destroy(s_background_bitmap[1]);
  gbitmap_destroy(s_background_bitmap[2]);
  gbitmap_destroy(s_background_bitmap[3]);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);

  // Destroy TextLayer
  text_layer_destroy(s_text_layer);
}

static void init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
  update_time();
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  deinit();
}
