
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pebble.h>
  
Window *window;
TextLayer *text_layer_1, *text_layer_2,*text_layer_3,*text_layer_4;
char tap_text[3];
char str[150]={0};
int prevx=0;
int prevy=0;
int prevz=0;
int time_in_air=-2;
uint64_t last_change=0;
  
void accel_handler(AccelData *data, uint32_t num_samples)
{
  // data is an array of num_samples elements.
  // num_samples was set when calling accel_data_service_subscribe.
 
  
  //snprintf(str,sizeof(data[0].x), "x: %d", data[0].x);
  
  
  if (abs(prevx-data[0].x)>=600 || abs(prevy-data[0].y)>=600  ||  abs(prevz-data[0].z)>=600 )
  {
    time_in_air++;
    last_change=data[0].timestamp;
  }
  
  
  prevx=data[0].x;
  prevy=data[0].y;
  prevz=data[0].z;
  
  
  int flying_distance=(int)(0.5*9.80665*time_in_air*time_in_air/400);
  if (time_in_air>=1)
  {
    memset(str,sizeof(str),0);
   // printf("%d",flying_distance);
    snprintf(str,sizeof(str),"Reached %d m", flying_distance);
   // printf("The string is : %s",str);
    
    
  }

  text_layer_set_text(text_layer_1, str);
  
  if (abs(last_change-data[0].timestamp)>=5000)
    {
    time_in_air=-2;
   // printf("Clear");
  }
  
    
 
  
}
 
void tap_handler(AccelAxisType axis, int32_t direction)
{
  // Build a short message one character at a time to cover all possible taps.
 
  if (direction > 0)
  {
    tap_text[0] = '+';
  } else {
    tap_text[0] = '-';
  }
 
  if (axis == ACCEL_AXIS_X)
  {
    tap_text[1] = 'X';
  } else if (axis == ACCEL_AXIS_Y)
  {
    tap_text[1] = 'Y';
  } else if (axis == ACCEL_AXIS_Z)
  {
    tap_text[1] = 'Z';
  }
 
  // The last byte must be zero to indicate end of string.
  tap_text[2] = 0;
 
  text_layer_set_text(text_layer_2, tap_text);
}
 
void window_load(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
 
  text_layer_1 = text_layer_create(GRect(0, 55, 144, 50));
  
   text_layer_set_font(text_layer_1, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(text_layer_1, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(text_layer_1));
  
  
   text_layer_set_text(text_layer_1, "THROW YOUR PEBBLE!!!");
  accel_data_service_subscribe(1, accel_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
 
}
 
void window_unload(Window *window)
{
  // Call this before destroying text_layer, because it can change the text
  // and this must only happen while the layer exists.
  accel_data_service_unsubscribe(); 
  text_layer_destroy(text_layer_1);
}
 
int main()
{
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers)
  {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  app_event_loop();
  window_destroy(window);
}