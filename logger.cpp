#include "logger.h"
#include "config.h"
#include <SPI.h>
#include "date_time.h"

/*
 * Log format
 * { LOG:  "OBJECT INITIALIZED" }
 * { ERROR: "CO2 SENSOR FAILED" }
 */
 
/*
 * Logger object 
 */
Logger::Logger(void){
  debugln("Initializing logger");
  if(!SD.begin(sd_chip_select)) {
    debugln("Card failed, or not present");
    return;
  }

  if(SD.exists(log_file)) {
    debugln("log file exists.");
  } else {
    debugln("log file not found, creating log file.");
    _file = SD.open(log_file, FILE_WRITE);
    _file.close();
  }

  debugln("Logger initialized");
  _on = true;
}


bool Logger::log(String* dict, int len){
  if(!_on){
    //debugln("Error, Logger not initialized!");
    return 0;
  }

  if(len%2){
    debugln("Error, Uneven number of values!");
    return 0;
  }
  
  char* ans;
  int len2 = 0;
  ans = generate(dict, len/2, len2);
  
  _file = SD.open(log_file, FILE_WRITE);
  _file.seek(0);          // add new entries at beginning of file, so that we can easily read latest
  _file.write(ans, len2);
  _file.println();
  _file.close();

  debugln(ans);
  free(ans);
}


/*
 * JSON engine object
 */
Json::Json(void){
  debugln("Initializing JSON Engine");
  jsonBuffer = new DynamicJsonBuffer(200);
}

char* Json::generate(String* dict, int no_pairs, int& len2){
  JsonObject& root = jsonBuffer->createObject();
  root["date"] = String(clock->day_month) + "." + String(clock->month) + "." + String(clock->year) ;
  root["time"] = String(clock->hour) + ":" + String(clock->minute) + ":" + String(clock->seconds) ;
  
  // sample input Array of strings [ "type", "error", "value", "2" ]
  int j=0;
  for(int i=0; i<no_pairs; i++){
    root[dict[j]] = dict[j+1];
    j+=2;
  }

  char* result = NULL;
  len2 = root.measureLength()+1;
  result = malloc(len2);
  if(result == NULL){
    debugln("JSON: unable to allocate memory");
    return NULL;
  }
  root.printTo(result, len2);
  return result;
}


