#include <pebble.h>

static Window *s_main_window;
static Layer *s_hand_layer;
static TextLayer *s_date_layer;
static TextLayer *s_name_layer;
static int current_hour;
static int current_minute;
static int current_date;
static int current_month;
static GFont s_minute_font;
static GFont s_hour_font;
static GFont s_date_font;
static GFont s_name_font;

//convert hour into angle
static int32_t get_angle_for_hour(int hour,int minute){
  return ((hour*360)/12) + ((minute *30) / 60);//why 30? because hour hand only move 30 deg in 1 hour(360/12)
}
//convert minutes into angle
static int32_t get_angle_for_minute(int minute){
  return (minute * 360) / 60;
}

static void hand_layer_update_proc(Layer *layer,GContext *ctx){
  GRect bounds = layer_get_bounds(layer);
  GRect hourframe = grect_inset(bounds,GEdgeInsets(30));
  GRect hourdigitframe = grect_inset(bounds,GEdgeInsets(40));//hour eq to 12,set insets to 40 else 20
  GPoint centerpoint = GPoint(bounds.size.w/2,bounds.size.h/2);
  
  //for analog 12 hours only
  current_hour -= (current_hour>12) ? 12 : 0;
  
  //set the hour hand and hour digit
  printf("Current HOUR: %d",current_hour);
  int hour_angle = get_angle_for_hour(current_hour,current_minute);
  //int hour_digit_angle = get_angle_for_hour_digit(12);
  printf("hour angle: %d",hour_angle);
  GPoint hpos = gpoint_from_polar(hourframe,GOvalScaleModeFitCircle,DEG_TO_TRIGANGLE(hour_angle));
  graphics_context_set_stroke_color(ctx,GColorWhite);
  graphics_context_set_stroke_width(ctx,5);
  graphics_draw_line(ctx,centerpoint,hpos);
  GRect hr = grect_centered_from_polar(hourdigitframe, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(360),GSize(100, 100));
  char hdigit[4];
  snprintf(hdigit,4,"%d",current_hour);
  graphics_context_set_text_color(ctx,GColorWhite);
  graphics_draw_text(ctx,hdigit, s_hour_font, hr, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  GPoint hdpoint = gpoint_from_polar(hourframe,GOvalScaleModeFitCircle,DEG_TO_TRIGANGLE(360));
  graphics_context_set_stroke_width(ctx,1);
  graphics_draw_circle(ctx, GPoint(hdpoint.x,hdpoint.y-30), (current_hour> 9) ? 12 : 10);
  
  //set the minute hand and minute digit
  printf("Current MINUTE: %d",current_minute);
  int minute_angle = get_angle_for_minute(current_minute);
  printf("minute angle: %d",minute_angle);
  GPoint mpos = gpoint_from_polar(bounds,GOvalScaleModeFitCircle,DEG_TO_TRIGANGLE(minute_angle));
  graphics_context_set_stroke_color(ctx,GColorWhite);
  graphics_context_set_stroke_width(ctx,2);
  graphics_draw_line(ctx,centerpoint,mpos);
  GRect mr = GRect(centerpoint.x-20,centerpoint.y-40,50,50);
  char mdigit[4];
  snprintf(mdigit,4,"%02d",current_minute);
  graphics_context_set_text_color(ctx,GColorWhite);
  graphics_draw_text(ctx,mdigit, s_minute_font, mr, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

static void main_window_load(Window *window){
  //Get information about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);//get the size of the main window
  
  //Set the font
  s_minute_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_VGA_30));
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_BEYOND_MOUNTAINS_20));
  
  //set clock hand layer
  s_hand_layer = layer_create(bounds);
  //set this layer's update procedure
  layer_set_update_proc(s_hand_layer,hand_layer_update_proc);
  layer_add_child(window_layer,s_hand_layer);
  
  //set date layer
  s_date_layer = text_layer_create(
            GRect(0,PBL_IF_ROUND_ELSE(36,30),bounds.size.w,20));
  //Set the interface for s_date_layer
  text_layer_set_background_color(s_date_layer,GColorClear);
  text_layer_set_text_color(s_date_layer,GColorWhite);
  //Create date GFont
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DOS_VGA_15));
  //Apply custom font to text layer;
  text_layer_set_font(s_date_layer,s_date_font);
  text_layer_set_text_alignment(s_date_layer,GTextAlignmentCenter);
  //text_layer_set_text(s_date_layer,"fg");
  layer_add_child(window_layer,text_layer_get_layer(s_date_layer));
  
  //set the name layer
  s_name_layer = text_layer_create(GRect(0,PBL_IF_ROUND_ELSE(116, 110), bounds.size.w, 30));
  text_layer_set_background_color(s_name_layer, GColorClear);
  text_layer_set_text_color(s_name_layer,GColorWhite);
  s_name_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_BEYOND_MOUNTAINS_20));
  text_layer_set_font(s_name_layer, s_name_font);
  text_layer_set_text_alignment(s_name_layer,GTextAlignmentCenter);
  text_layer_set_text(s_name_layer, "Fariq");
  layer_add_child(window_layer, text_layer_get_layer(s_name_layer));
}

static void main_window_unload(Window *window){
  layer_destroy(s_hand_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_name_layer);
  fonts_unload_custom_font(s_minute_font);
  fonts_unload_custom_font(s_hour_font);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_name_font);
}

static void update_time(void){
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  current_hour = t->tm_hour;
  current_minute = t->tm_min;
  current_date = t->tm_mday;
  current_month = t->tm_mon + 1;
  
  layer_mark_dirty(s_hand_layer);
  
  //Display date on the s_date_layer
  static char date[20];
  //snprintf(date, 20, "%d/%d",current_date,current_month);
  strftime(date,sizeof(date),"%d %b %Y",t);
  printf("%s",date);
  text_layer_set_text(s_date_layer,date);
}

static void tick_handler(struct tm *tick_time,TimeUnits units_changed){
  update_time();
}

static void init(void){
  //create the main window
  s_main_window = window_create();
  //set handlers to manage the elements inside the window
  window_set_window_handlers(s_main_window,(WindowHandlers){
      .load = main_window_load,
      .unload = main_window_unload
  });
  //Set the main window background color to black
  window_set_background_color(s_main_window,GColorBlack);
  //show the window on the watch with animated true
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
