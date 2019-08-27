#ifndef DISPLAY_H
#define DISPLAY_H

#include <Controllino.h>
#include <genieArduino.h>
#include "value.h"

#define no_keyboards 3
#define no_knobs     3

// Object for individual screens
template<int T>
class Form{  
  public:
    uint8_t len = T;
    value_t* elements[T];
    void update(){
      for(uint8_t i=0; i<T; i++){
        elements[i]->send_value();
        elements[i]->send_setpoint();
      }
    }
    
};

class dkeyboard{
  public:
    dkeyboard(uint8_t sid);
    void send_value();
    String val;
    uint8_t string_id;
};

// Displayd device object
class display_t{
  uint8_t _reset_pin;
  HardwareSerial* _mySerial;
  
  public:
    Genie genie;
    uint8_t knob_values[no_knobs] = {0};       // array index is the knob number
    dkeyboard* keyboards[no_keyboards];        // Note: this setup assumes that there is a corresponding knob for every keyboard, with the same index number.

    Form<10> form0;     // form 0 has 18 elements which we want to store
    
    display_t(uint8_t pin, HardwareSerial* ser);
    void init();
    void loop();

    void write_string(uint8_t id, float value);
    void write_string(uint8_t id, String value);
    void update_keyboard_value(String value);
};

extern display_t* mdisplay;

static void event_handler(void);

/* 
 *  String IDS
 */
static const uint8_t tempvalue_to_str[6]    = {2, 9, 10, 11, 12, 13};   // matches temperature sensor to string in display
static const uint8_t tempsetpoint_to_str[6] = {3, 4, 5, 6, 7, 8 };      // matches temp sensor set point to string in display
static const uint8_t keyboard_strings[3]    = {20, 21, 22};
#define co2_val_string           18
#define o2_val_string            19
#define co2_setpoint_string      16
#define o2_setpoint_string       17
#define rh_string                14
#define ph_string                15



#endif
