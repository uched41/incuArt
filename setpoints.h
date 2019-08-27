#ifndef SETPOINTS_H
#define SETPOINTS_H

#include <Controllino.h>

struct setpoints_data{
  /*
   * 1 - 6, temperature sensor
   * 7, co2 sensor
   * 8, o2, sensor
   */
  double setpoints[10];
  double offsets[10];
  
  unsigned long int filter_timer;
  unsigned long int o2_timer;
  unsigned long int uv_timer;
};

class setpoints_t{
  uint8_t data_address;
  
  public:
    setpoints_t() : data_address(0) {};
    setpoints_data data;
    void save_data();
    void get_data();
};

#endif
