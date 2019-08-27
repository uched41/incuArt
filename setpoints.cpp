#include "setpoints.h"
#include <EEPROM.h>
#include "config.h"

void setpoints_t::get_data(){
  debugln("Getting set points from EEPROM.");
  EEPROM.get(data_address, data);
}


void setpoints_t::save_data(){
  debugln("Saving setpoints to EEPROM.");
  EEPROM.put(data_address, data);
}


