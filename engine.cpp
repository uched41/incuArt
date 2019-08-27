#include "engine.h"
#include "config.h"
#include "display.h"
#include "logger.h"

engine_t::engine_t(){
  sp = new setpoints_t();
  sp->get_data();
  
  rh    = new analog_value_t(rh_pin, rh_string, 127);
  ph    = new analog_value_t(ph_pin, rh_string, 127);

  tc[0] = new temperature_chamber_t(tc1_pin, tcoil1_pin, ldr1_pin, &(sp->data.setpoints[0]), tempvalue_to_str[0], tempsetpoint_to_str[0]);
  tc[1] = new temperature_chamber_t(tc2_pin, tcoil2_pin, ldr2_pin, &(sp->data.setpoints[1]), tempvalue_to_str[1], tempsetpoint_to_str[1]);
  tc[2] = new temperature_chamber_t(tc3_pin, tcoil3_pin, ldr3_pin, &(sp->data.setpoints[2]), tempvalue_to_str[2], tempsetpoint_to_str[2]);
  tc[3] = new temperature_chamber_t(tc4_pin, tcoil4_pin, ldr4_pin, &(sp->data.setpoints[3]), tempvalue_to_str[3], tempsetpoint_to_str[3]);
  tc[4] = new temperature_chamber_t(tc5_pin, tcoil5_pin, ldr5_pin, &(sp->data.setpoints[4]), tempvalue_to_str[4], tempsetpoint_to_str[4]);
  tc[5] = new temperature_chamber_t(tc6_pin, tcoil6_pin, ldr6_pin, &(sp->data.setpoints[5]), tempvalue_to_str[5], tempsetpoint_to_str[5]);

  tc[0]->set_pid_koefficients(TC01_KP, TC01_KI, TC01_KD);
  tc[1]->set_pid_koefficients(TC02_KP, TC02_KI, TC02_KD);
  tc[2]->set_pid_koefficients(TC03_KP, TC03_KI, TC03_KD);
  tc[3]->set_pid_koefficients(TC04_KP, TC04_KI, TC04_KD);
  tc[4]->set_pid_koefficients(TC05_KP, TC05_KI, TC05_KD);
  tc[5]->set_pid_koefficients(TC06_KP, TC06_KI, TC06_KD);

  co2   = new co2_sensor_t(co2_pin, co2valve_pin, co2_pressure_pin, &(sp->data.setpoints[6]), co2_val_string, co2_setpoint_string);
  o2    = new o2_sensor_t(o2_pin, o2valve_pin, o2_pressure_pin, &(sp->data.setpoints[7]), o2_val_string, o2_setpoint_string);

  co2->set_pid_koefficients(CO2_KP, CO2_KI, CO2_KD);
  o2 ->set_pid_koefficients(O2_KP, O2_KI, O2_KD);
  
  co2_main_valve = new valve_t(co2_main_valve_pin);
  o2_main_valve  = new valve_t(o2_main_valve_pin);
  co2_main_valve->on();
  o2_main_valve->on();

  uv_lamp = new uv_lamp_t(uv_lamp_pin);

  // Add elements to form object so that we can remember them when we switch pages.
  // Also add the elements to our sensor array for easy iteration
  int i=0;
  for(i; i<6; i++){
    sensors[i] = mdisplay->form0.elements[i] = tc[i];
  }
  sensors[6] = mdisplay->form0.elements[6] = co2;
  sensors[7] = mdisplay->form0.elements[7] = o2;
  sensors[8] = mdisplay->form0.elements[8] = rh;
  sensors[9] = mdisplay->form0.elements[9] = ph;
  sensors[10] = uv_lamp;
}

void engine_t::read_inputs(){
  for(uint8_t i=0; i<no_sensors; i++){
    sensors[i]->refresh_val();
  }
}

void engine_t::loop(){
  read_inputs();
  for(uint8_t i=0; i<no_sensors; i++){
    sensors[i]->loop();   // call the pid loop for each sensor
  }
}


/* 
 * Self test function after initialization
 */
void self_test(void){
  debugln("Starting Self-Test");
  int count = 0;
  for(int i=0; i<6; i++){
    if(!engine->tc[i]->is_connected()){
      engine->tc[i]->log_error(i+1);
      engine->tc[i]->ldr->is_open() ? (engine->tc[i]->ldr->log_error()) : ({}); 
      count++;
    }
  }

  if(!engine->co2->is_connected()){
    engine->co2->log_error();
    engine->co2->pressure_sensor->is_on() ? ({}) : engine->co2->pressure_sensor->log_error();
    count++;
  }

  if(!engine->o2->is_connected()){
    engine->o2->log_error();
    engine->o2->pressure_sensor->is_on() ? ({}) : engine->o2->pressure_sensor->log_error();
    count++;
  }

  String l1[2] = {"MSG", "SELF-TEST COMPLETE."};
  logger->log(l1, 2);
}

