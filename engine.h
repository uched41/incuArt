#ifndef ENGINE_H
#define ENGINE_H

#include "sensors.h"
#include "value.h"
#include "setpoints.h"

#define no_sensors  11

class engine_t{
  value_t* sensors[no_sensors];           // array of all sensors in chamber, for easy iteration
  public:
    analog_value_t* rh;
    analog_value_t* ph;
    temperature_chamber_t* tc[6];    // array of class pointers
    co2_sensor_t* co2;
    o2_sensor_t* o2;
    valve_t* co2_main_valve;
    valve_t* o2_main_valve;
    uv_lamp_t* uv_lamp;
    
    setpoints_t* sp;
    engine_t();
    void read_inputs();
    void loop();
    void send_setpoints();
    void send_vals();
};

extern engine_t* engine;

void self_test(void);


#endif
