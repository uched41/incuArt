#include "value.h"
#include "config.h"
#include "display.h"

/*
 * General value object
 */
value_t::value_t(void){
  old_value = 0;
  new_value = 0;
  setpoint = NULL;
}

void value_t::set_value(float v){
  old_value = new_value;
  new_value = v;
}

float value_t::get_value(){
  return new_value;
}

float value_t::get_old_value(){
  return old_value;
}

bool value_t::compare_value(unsigned char prec){
  if (fabs(new_value - old_value) >= pow(0.1,prec))  {
    return true;
  }
  return false;
}

void value_t::send_value(void){
  mdisplay->write_string(value_str_id, new_value);
}

void value_t::send_setpoint(void){
  if(setpoint_str_id < 127){
    mdisplay->write_string(setpoint_str_id, *setpoint);
  }
}

void value_t::set_pid_koefficients(int p, int i, int d){
  kp = p; ki = i; kd = d;

  _pid = new PID(&pid_input, &pid_output, setpoint, kp, ki, kd, DIRECT); 
  _pid->SetMode(AUTOMATIC);
}

/*
 * Analog values object
 */
analog_value_t::analog_value_t(uint8_t mpin, uint8_t vid, uint8_t sid){
  pin    = mpin; 
  offset = 0.0;
  slope  = 1.0;
  value_str_id = vid;
  setpoint_str_id = sid;
}

void analog_value_t::set_param(float offset, float slope) {
  offset = offset;
  slope = slope;
}

bool analog_value_t::refresh_val() {
  float aux=0.0;
  val_adc = analogRead(pin);
  aux = ( float(val_adc/1023.0 * 5 * this->slope) ) + this->offset;
  set_value(aux);
  //print_val();
  
  if(compare_value(2)){
    return true;
  }else {
    return false;
  }
}

void analog_value_t::print_val(){
  char buff[40];
  char str_val[6];
  dtostrf(get_value(), 4, 2, str_val);
  sprintf(buff, "The value of analog pin %d is: %s", pin, str_val);
  debugln(buff);
}
