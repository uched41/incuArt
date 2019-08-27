#include "engine.h"
#include "display.h"
#include "config.h"
#include "logger.h"
#include "date_time.h"

date_time_t* clock;
display_t* mdisplay;
engine_t* engine;
Logger*    logger;

long lastRead = 0;

void setup() {
  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL);
  debugln("Starting device");
  
  clock = new date_time_t();
  mdisplay = new display_t(display_reset_pin, &display_serial);
  mdisplay->init();
  
  logger = new Logger();
  if(!logger->_on) { debugln("Unable to Intialize logger.");}   
  
  engine = new engine_t();
  long stime = 5000, ltime = millis();
  //while(millis()-ltime < stime) engine->loop();     // Run the engine loop for sometime before doing self test, this ensures that we have good values from sensors
  
  //self_test();                                      // Run self test
  debugln("Initialization complete starting loop");
}

void loop() {
  if(millis()-lastRead>1000){
    engine->loop();
    lastRead = millis();
  }
  
  mdisplay->loop();
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
