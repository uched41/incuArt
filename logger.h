#ifndef LOGGER_H
#define LOGGER_H

#include <ArduinoJson.h>
#include <SD.h>

class Json{
  DynamicJsonBuffer*  jsonBuffer;
  
  public:
    Json(void);
    char* generate(String* dict, int no_pairs, int& len2);
  
};


class Logger : public Json{
  const char* log_file = "log.txt";
  File _file;
    
  public:
    Logger(void);
    bool log(String* dict, int len);

    bool _on = false;     // true if initialization was successful
};


extern Logger* logger;

#endif

