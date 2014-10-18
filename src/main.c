
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pebble.h>
  
Window *window;
TextLayer *text_layer_1, *text_layer_2,*text_layer_3,*text_layer_4;
char tap_text[3];
 
int prevx=0;
int prevy=0;
int prevz=0;
int time_in_air=-2;
  
void accel_handler(AccelData *data, uint32_t num_samples)
{
  // data is an array of num_samples elements.
  // num_samples was set when calling accel_data_service_subscribe.
 
  char str[150]={0};
  //snprintf(str,sizeof(data[0].x), "x: %d", data[0].x);
  
  
  if (abs(prevx-data[0].x)>=500 || abs(prevy-data[0].y)>=500  ||  abs(prevz-data[0].z)>=500 )
    time_in_air++;
  
  
  prevx=data[0].x;
  prevy=data[0].y;
  prevz=data[0].z;
  
  int flying_distance=(int)(0.5*9.80665*time_in_air*time_in_air);
  if (time_in_air>=1)
  printf("%d %d\n",time_in_air,flying_distance);
  //char* str1="WTF";
  
  text_layer_set_text(text_layer_1, str);
/*
  memset(str,sizeof(str),0);
  snprintf(str,100,"y: %d", data->y);
  text_layer_set_text(text_layer_2, str);

  
  memset(str,sizeof(str),0);
  snprintf(str,100, "z: %d", data->z);
  text_layer_set_text(text_layer_3, str);
*/
  
  
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
 
  text_layer_1 = text_layer_create(GRect(0, 0, 144, 20));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_1));
 
  text_layer_2 = text_layer_create(GRect(0, 20, 144, 20));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_2));
  
  text_layer_3 = text_layer_create(GRect(0, 40, 144, 20));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_3));
  
  text_layer_4 = text_layer_create(GRect(0, 60, 144, 20));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_4));
  
  
  
 
  accel_data_service_subscribe(1, accel_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
 
  accel_tap_service_subscribe(tap_handler);
}
 
void window_unload(Window *window)
{
  // Call this before destroying text_layer, because it can change the text
  // and this must only happen while the layer exists.
  accel_data_service_unsubscribe();
  accel_tap_service_unsubscribe();
 
  text_layer_destroy(text_layer_2);
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