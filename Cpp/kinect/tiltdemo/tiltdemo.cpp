#include "libfreenect.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
 
int main(int argc, char *argv[])
{
 // seed rng generator
 srand(time(0));
 // pointer to the freenect context
 freenect_context *ctx;
 // pointer to the device
 freenect_device *dev;
 
 
 if (freenect_init(&ctx, NULL) < 0)
 {
  std::cout<<"freenect_init() failed\n";
   exit(EXIT_FAILURE);
 }
 // set the highest log level so we can see what is going on
 freenect_set_log_level(ctx, FREENECT_LOG_SPEW);
 
 int nr_devices = freenect_num_devices (ctx);
 std::cout<<"Number of devices found: "<<nr_devices<<"\n";
 // I only have one kinect so open device 0
 if (freenect_open_device(ctx, &dev, 0) < 0)
 {
  std::cout<<"could not open device error\n";
  freenect_shutdown(ctx);
  exit(EXIT_FAILURE);
 }
 // now I'm going to loop and set random value
 // these are basically from the tiltdemo.c that comes with
 // the freenect lib modified not to use the sync lib
 while (1)
 {
  // Pick a random tilt and a random LED state
  freenect_led_options led = (freenect_led_options) (rand() % 6); // explicit cast
  int tilt = (rand() % 30)-15;
  freenect_raw_tilt_state *state = 0;
  double dx, dy, dz;
  // Set the LEDs to one of the possible states
  freenect_set_led(dev,led);
  // Set the tilt angle (in degrees)
  freenect_set_tilt_degs(dev,tilt);
 
  // Get the raw accelerometer values and tilt data
  state=freenect_get_tilt_state(dev);
 
  std::cout<<"led["<<led<<"] tilt["<<tilt<<"]\r" ;
  std::cout.flush();
  sleep(1);
 }
}
