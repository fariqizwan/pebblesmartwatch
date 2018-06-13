#include <pebble.h>

typedef struct {
    int age;
    int age2;
    int age3;
} Age;

static Window *s_main_window;
static TextLayer *s_time_layer;

static void main_window_load(Window *window){
  //Get information about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Create the textlayer with specific bounds
  s_time_layer = text_layer_create(
            GRect(0,PBL_IF_ROUND_ELSE(58,52),bounds.size.w,50));
  //improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer,GColorGreen);
  text_layer_set_text_color(s_time_layer,GColorBlack);
  //text_layer_set_text(s_time_layer,"00:00");
  text_layer_set_font(s_time_layer,fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer,GTextAlignmentCenter);
  
  //add it as a child layer to window's root layer
  layer_add_child(window_layer,text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window){
  text_layer_destroy(s_time_layer);
  
}
static void update_time() {
  //Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  //write the current hours and minutes into a buffer
  static char s_buffer[12];
  strftime(s_buffer,sizeof(s_buffer),clock_is_24h_style() ? "%H:%M" : "%I:%M",tick_time);
  
  //Display this time on the TextLayer
  text_layer_set_text(s_time_layer,s_buffer);
}
static void tick_handler(struct tm *tick_time,TimeUnits units_changed){
   update_time(); 
}
static void init(void){
  //just for testing log
  printf("Hello there...\n");
  Age ages = {.age2=1,.age=2,.age3=3};
  printf("age=%d,age2=%d,age3=%d",ages.age,ages.age2,ages.age3);
  
  //create the main window
  s_main_window = window_create();
  //set handlers to manage the elements inside the window
  window_set_window_handlers(s_main_window,(WindowHandlers){
      .load = main_window_load,
      .unload = main_window_unload
  });
  //show the window on the watch
  window_stack_push(s_main_window,true);
  update_time();
  
  //register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT,tick_handler);
}

static void deinit(void){
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
  return 0;
}
