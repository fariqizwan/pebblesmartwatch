#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_name_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_hour_font;
static GFont s_minute_font;
static GFont s_name_font;
static GFont s_date_font;
//Background image pointer
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static Layer *s_draw_layer;

typedef enum {
  ahad=0,isnin,selasa,rabu,
  khamis,jumaat,sabtu
}Hari;

void get_hari(Hari hari,char *haribuff){
  printf("%d",hari);
  switch(hari){
    case ahad:
      snprintf(haribuff,10,"%s","Ahad");
      break;
    case isnin:
      snprintf(haribuff,10,"%s","Isnin");
      break;
    case selasa:
      snprintf(haribuff,10,"%s","Selasa");
      break;
    case rabu:
      snprintf(haribuff,10,"%s","Rabu");
      break;
    case khamis:
      snprintf(haribuff,10,"%s","Khamis");
      break;
    case jumaat:
      snprintf(haribuff,10,"%s","Jumaat");
      break;
    case sabtu:
      snprintf(haribuff,10,"%s","Sabtu");
      break;
    default:
      snprintf(haribuff,10,"%s","HariHari");
  }
}

static void layer_update_proc(Layer *layer,GContext *ctx){
  //Custom drawing happens here
  graphics_context_set_stroke_color(ctx,GColorBlack);
  //draw horizontal line
  graphics_draw_line(ctx,GPoint(19,44),GPoint(123,44));
}

static void main_window_load(Window *window){
  //Get information about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Set bitmap layer
  //s_background_layer = bitmap_layer_create(bounds);
  
  //Set draw layer
  s_draw_layer = layer_create(bounds);
  
  //Set this layer's update procedure
  layer_set_update_proc(s_draw_layer,layer_update_proc);
  layer_add_child(window_layer,s_draw_layer);
  
  //Create the textlayer with specific bounds
  s_time_layer = text_layer_create(
            GRect(0,PBL_IF_ROUND_ELSE(58,52),bounds.size.w,50));
  s_hour_layer = text_layer_create(
            GRect(0,PBL_IF_ROUND_ELSE(58,52),bounds.size.w/2,50));
  s_minute_layer = text_layer_create(
            GRect(bounds.size.w/2-5,PBL_IF_ROUND_ELSE(58,52),bounds.size.w/2,50));
  s_name_layer = text_layer_create(
            GRect(0,PBL_IF_ROUND_ELSE(26,20),bounds.size.w,50));
  s_date_layer = text_layer_create(
            GRect(0,PBL_IF_ROUND_ELSE(116,110),bounds.size.w,50));
  
  /*//Set the interface for s_time_layer
  text_layer_set_background_color(s_time_layer,GColorWhite);
  text_layer_set_text_color(s_time_layer,GColorBlack);*/
  //Set the interface for s_hour_layer
  text_layer_set_background_color(s_hour_layer,GColorWhite);
  text_layer_set_text_color(s_hour_layer,GColorBlack);
  //Set the interface for s_minute_layer
  text_layer_set_background_color(s_minute_layer,GColorWhite);
  text_layer_set_text_color(s_minute_layer,GColorBlack);
  //Set the interface for s_name_layer
  text_layer_set_background_color(s_name_layer,GColorClear);
  text_layer_set_text_color(s_name_layer,GColorBlack);
  //Set the interface for s_date_layer
  text_layer_set_background_color(s_date_layer,GColorWhite);
  text_layer_set_text_color(s_date_layer,GColorBlack);
  //Create GFont
  //s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_VGA_30));
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_VGA_40));
  s_minute_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_VGA_30));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_VGA_20));
  s_name_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_BEYOND_MOUNTAINS_20));
  //Apply custom font to text layer;
  //text_layer_set_font(s_time_layer,s_time_font);
  text_layer_set_font(s_hour_layer,s_hour_font);
  text_layer_set_text_alignment(s_hour_layer,GTextAlignmentCenter);
  text_layer_set_font(s_minute_layer,s_minute_font);
  text_layer_set_text_alignment(s_minute_layer,GTextAlignmentCenter);
  text_layer_set_font(s_name_layer,s_name_font);
  text_layer_set_text_alignment(s_name_layer,GTextAlignmentCenter);
  text_layer_set_font(s_date_layer,s_date_font);
  text_layer_set_text_alignment(s_date_layer,GTextAlignmentCenter);
  static char name[] = "Fariq";
  text_layer_set_text(s_name_layer,name);
  
  //add it as a child layer to window's root layer
  //layer_add_child(window_layer,text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer,text_layer_get_layer(s_hour_layer));
  layer_add_child(window_layer,text_layer_get_layer(s_minute_layer));
  layer_add_child(window_layer,text_layer_get_layer(s_name_layer));
  layer_add_child(window_layer,text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window){
  //text_layer_destroy(s_time_layer);
  text_layer_destroy(s_hour_layer);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_name_layer);
  text_layer_destroy(s_date_layer);
  //fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_hour_font);
  fonts_unload_custom_font(s_minute_font);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_name_font);
  //Destroy GBitmap;
  gbitmap_destroy(s_background_bitmap);
  //Destroy Bitmaplayer
  bitmap_layer_destroy(s_background_layer);
  layer_destroy(s_draw_layer);
}
static void update_time(struct tm *tick_time) {
  //write the current hours and minutes into a buffer
  static char s_hour_buffer[4];
  strftime(s_hour_buffer,sizeof(s_hour_buffer),clock_is_24h_style() ? "%H" : "%l:",tick_time);
  static char s_minute_buffer[8];
  strftime(s_minute_buffer,sizeof(s_minute_buffer),clock_is_24h_style() ? "%M" : "%M%P",tick_time);
  
  /*//Change the color according to current time
  int current_hour = tick_time->tm_hour;
  if (current_hour > 12) {
    text_layer_set_text_color(s_time_layer,GColorOrange);
  }*/
  
  //Display this time on the TextLayer
  //text_layer_set_text(s_time_layer,s_buffer);
  text_layer_set_text(s_hour_layer,s_hour_buffer);
  text_layer_set_text(s_minute_layer,s_minute_buffer);
}

static void update_date(struct tm *tick_date){
  //write the current date into a buffer
  static char s_datebuffer[20];
  strftime(s_datebuffer,sizeof(s_datebuffer),"%d %b %Y",tick_date);
  char harini[12];
  static char s_both[24];
  get_hari(tick_date->tm_wday,harini);
  printf("TODAY %s", harini);
  snprintf(s_both,30,"%s %s",harini,s_datebuffer);
  
  //Display date on the s_date_layer
  text_layer_set_text(s_date_layer,s_both);
}

static void tick_handler(struct tm *tick_time,TimeUnits units_changed){
  if (units_changed & MINUTE_UNIT){
    update_time(tick_time); 
  }
  if (units_changed & HOUR_UNIT){
    vibes_short_pulse();
  }
  if (units_changed & DAY_UNIT){
    update_date(tick_time);
  }
}
static void init(void){
  //create the main window
  s_main_window = window_create();
  //set handlers to manage the elements inside the window
  window_set_window_handlers(s_main_window,(WindowHandlers){
      .load = main_window_load,
      .unload = main_window_unload
  });
  //Set main window background color to match background image
  window_set_background_color(s_main_window,GColorClear);
  //show the window on the watch
  window_stack_push(s_main_window,true);
  //Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  update_time(tick_time);
  update_date(tick_time);
  
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


