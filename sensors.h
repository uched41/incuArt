#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Controllino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "value.h"

class digital_control_t {
  uint8_t pin;

  public:
    digital_control_t(uint8_t p);
    void on();
    void off();
};


class heating_coil_t : public digital_control_t{
  public:
    heating_coil_t(uint8_t p);
};


class valve_t : public digital_control_t{
  public:
    valve_t(uint8_t p);
};


class ldr_t : public analog_value_t{
  bool open;
  public:
    ldr_t(uint8_t pin);
    void loop(void);
    bool is_open();
    void log_error(uint8_t _index=0);
};


class pressure_sensor_t : public analog_value_t{
  public:
    pressure_sensor_t(uint8_t p, String name);
    String _name;
    bool refresh_val(void);
    bool is_on();
    void log_error(uint8_t _index=0);
};


class temperature_chamber_t : public value_t{ 
  OneWire* _oneWire;
  DallasTemperature* _sensor;
  bool connected;
  heating_coil_t* heating_coil;
  
  public:
    uint8_t _pin;  
    ldr_t* ldr;  
    temperature_chamber_t(uint8_t pin, uint8_t hpin, uint8_t lpin, double* spoint, uint8_t vid, uint8_t sid);
    void read_temperature();
    bool refresh_val();
    void print_val();
    bool is_connected(void) { return connected;}
    void loop(void);
    void log_error(uint8_t _index=0);
};


class co2_sensor_t : public analog_value_t{
  valve_t* valve;
  
  public:
    pressure_sensor_t* pressure_sensor;
    co2_sensor_t(uint8_t mpin, uint8_t vpin, uint8_t ppin, double* spoint, uint8_t vid=127, uint8_t sid=127);
    bool refresh_val();
    bool is_connected(void) { return true;}
    void loop(void);
    void log_error(uint8_t _index=0);
};


class o2_sensor_t : public analog_value_t{
  valve_t* valve;
  public:
    pressure_sensor_t* pressure_sensor;
    o2_sensor_t(uint8_t mpin, uint8_t vpin, uint8_t ppin, double* spoint, uint8_t vid=127, uint8_t sid=127);
    bool refresh_val();
    bool is_connected(void) { return true;}
    void loop(void);
    void log_error(uint8_t _index=0);
};


class uv_lamp_t : public analog_value_t{
  public:
    uv_lamp_t(uint8_t mpin);
    void loop(void);
    void log_error(uint8_t _index=0);
};

#endif
