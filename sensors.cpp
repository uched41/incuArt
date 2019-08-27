#include "sensors.h"
#include "config.h"
#include "logger.h"
#include <PID_v1.h>

/*
 * Temperature Sensor
 */
temperature_chamber_t::temperature_chamber_t(uint8_t pin, uint8_t hpin, uint8_t lpin, double* spoint, uint8_t vid, uint8_t sid){
  _pin = pin;
  _oneWire = new OneWire(_pin);
  _sensor = new DallasTemperature(_oneWire);
  _sensor->begin();
  debugln("Locating temperature sensor on pin " + String(_pin)); 

  int cnt = _sensor->getDeviceCount();
  debugln(String(cnt) + " temperature sensors detected");
  _sensor->setResolution(9);
  cnt ? (connected=true) : (connected=false) ;
  
  setpoint = spoint;
  value_str_id = vid;
  setpoint_str_id = sid;

  if(connected){
    send_setpoint();
    heating_coil = new heating_coil_t(hpin);
    _pid->SetOutputLimits(0, max_sp_temperature);

    ldr = new ldr_t(lpin);
  }
}
 
void temperature_chamber_t::read_temperature(){
  _sensor->requestTemperatures();
  float ans = _sensor->getTempCByIndex(0);
  //float ans = random(50);
  set_value(ans);
}

bool temperature_chamber_t::refresh_val(){
  if(!connected) return false;
  read_temperature();
  //print_val();
  if(compare_value(2)){
    send_value();       // if value has changed update screen
    return true;
  }else {
    return false;
  }
}

void temperature_chamber_t::print_val(){
  char buff[60];
  char str_val[8];
  dtostrf(get_value(), 5, 2, str_val);
  sprintf(buff,"The value of temperature sensor %d is: %s C", _pin, str_val);
  debugln(buff);
}

void temperature_chamber_t::loop(){
  if(!connected) return; 
  pid_input = get_value();
  _pid->Compute();

  (pid_output < *setpoint) ? (heating_coil->on()) : (heating_coil->off()) ;

  ldr->loop();    
}

void temperature_chamber_t::log_error(uint8_t _index){
  String l[2] = {"ERROR", "CHAMBER " + String(_index) + " TEMP SENSOR FAILED."};
  logger->log(l, 2);
}


/*
 * CO2 Sensor
 */
co2_sensor_t::co2_sensor_t(uint8_t mpin, uint8_t vpin, uint8_t ppin, double* spoint, uint8_t vid=127, uint8_t sid=127): analog_value_t(mpin, vid, sid){
  debugln("Initializing CO2 Sensor");
  debug(vid); debug(" "); debugln(sid);
  setpoint = spoint;
  send_setpoint();

  _pid->SetOutputLimits(0, max_sp_co2);
  valve = new valve_t(vpin);

  pressure_sensor = new pressure_sensor_t(ppin, "CO2");
}

bool co2_sensor_t::refresh_val(){
  int val = analogRead(pin);
  float val_1 = mapf(val, 0, 1023, 0, 20);   // map value from 0 to 20%
  set_value(val_1);

  pressure_sensor->refresh_val();
  
  if(compare_value(2)){
    send_value();       // if value has changed update screen
    return true;
  }else {
    return false;
  }
}

void co2_sensor_t::loop(){
  if(!pressure_sensor->is_on()){
    pressure_sensor->log_error();
    return;
  }
  pid_input = get_value();
  _pid->Compute();

  (pid_output < *setpoint) ? (valve->on()) : (valve->off()) ;
}

void co2_sensor_t::log_error(uint8_t _index){
  String l[2] = {"ERROR", "CO2 SENSOR FAILED."};
  logger->log(l, 2);
}


/*
 * O2 Sensor
 */
o2_sensor_t::o2_sensor_t(uint8_t mpin, uint8_t vpin, uint8_t ppin, double* spoint, uint8_t vid=127, uint8_t sid=127): analog_value_t(mpin, vid, sid){
  debugln("Initializing O2 Sensor");
  setpoint = spoint;
  send_setpoint();

  _pid->SetOutputLimits(0, max_sp_o2);
  valve = new valve_t(vpin);

  pressure_sensor = new pressure_sensor_t(ppin, "O2");
}

bool o2_sensor_t::refresh_val(){
  int val = analogRead(pin);
  float val_1 = mapf(val, 0, 1023, 0, 100);   // map value from 0 to 20%
  set_value(val_1);

  pressure_sensor->refresh_val();
  
  if(compare_value(2)){
    send_value();       // if value has changed update screen
    return true;
  }else {
    return false;
  }
}

void o2_sensor_t::loop(){
  if(!pressure_sensor->is_on()){
    pressure_sensor->log_error();
    return;
  }
  pid_input = get_value();
  _pid->Compute();

  (pid_output < *setpoint) ? (valve->on()) : (valve->off()) ;
}

void o2_sensor_t::log_error(uint8_t _index){
  String l[2] = {"ERROR", "O2 SENSOR FAILED."};
  logger->log(l, 2);
}


/*
 * Digital Control object
 */
digital_control_t::digital_control_t(uint8_t p){
  pin = p;
  pinMode(p, OUTPUT);
}

void digital_control_t::on(){
  digitalWrite(pin, HIGH);
}

void digital_control_t::off(){
  digitalWrite(pin, LOW);
}


/*
 * Heating coil object
 */
heating_coil_t::heating_coil_t(uint8_t p) : digital_control_t(p){
  debugln("Initialing heating foil on pin: " + String(p) );
}


/*
 * Valve object
 */
valve_t::valve_t(uint8_t p) : digital_control_t(p){
  debugln("Initialing valve on pin: " + String(p) );
}


/*
 * Light Sensor object
 */
ldr_t::ldr_t(uint8_t pin) : analog_value_t(pin){
  debugln("Initialing Light Sensor: " + String(pin) );
}

bool ldr_t::is_open(void){
  return open;
}

void ldr_t::loop(void){
  refresh_val();
  if(val_adc >= ldr_open_setpoint) open = true;
  if(val_adc <= ldr_close_setpoint) open = false;
}

void ldr_t::log_error(uint8_t _index){
  String l[2] = {"ERROR", "CHAMBER OPEN"};
  logger->log(l, 2);
}


/*
 * Pressure Sensor
 */
pressure_sensor_t::pressure_sensor_t(uint8_t p, String name) : analog_value_t(p){
  debugln("Initializing pressure sensor.");
  _name = name;
}

bool pressure_sensor_t::refresh_val(){
  float aux=0.0;
  val_adc = analogRead(pin);
  aux = ( float(val_adc/1023.0 * 5 * this->slope) ) + this->offset;
  aux = mapf(aux, 0.2, 4.7, 0, 100);   // output range 0.2v to 4.7v, 0 to 100kpa 
  set_value(aux);
  
  if(compare_value(2)){
    return true;
  }else {
    return false;
  }
}

bool pressure_sensor_t::is_on(){
  if(get_value() < minimum_gas_pressure){
    return false;
  }
  else return true;
}

void pressure_sensor_t::log_error(uint8_t _index){
  String l[2] = {"ERROR", "NO PRESSURE IN " + _name + " SUPPLY TUBE."};
  logger->log(l, 2);
}


/*
 * UV Lamp
 */
uv_lamp_t::uv_lamp_t(uint8_t mpin) : analog_value_t(mpin){
  debugln("Initializing UV Lamp");
}

void uv_lamp_t::loop(void){
  auto ans = get_value();
  if(ans < 1.2) return;

  log_error();
}

void uv_lamp_t::log_error(uint8_t _index=0){
  String l[2] = {"ERROR", "UV LAMP DEAD."};
  logger->log(l, 2);
}



