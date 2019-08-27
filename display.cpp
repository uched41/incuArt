#include "display.h"
#include "config.h"
#include "engine.h"
#include "logger.h"

const uint8_t mainform = 0;

/*
 * Display object
 */
display_t::display_t(uint8_t pin, HardwareSerial* ser){
  _reset_pin = pin;
  _mySerial = ser;

  for(int i=0; i<no_keyboards; i++){
    keyboards[i] = new dkeyboard(keyboard_strings[i]);
  }
}

void display_t::init(void){
  debugln("Initializing display ...");
  _mySerial->begin(115200);
  while(!*_mySerial);
  genie.Begin(*_mySerial);
  genie.AttachEventHandler(event_handler);

  pinMode(_reset_pin, OUTPUT);  
  digitalWrite(_reset_pin, 0);  // Reset the Display via D4
  delay(100);
  digitalWrite(_reset_pin, 1);  // unReset the Display via D4
  delay (5000); //let the display start up after the reset (This is important)

  genie.WriteContrast(7);
  debugln("Display initialization complete.");
}

void display_t::loop(){
  genie.DoEvents();
}


static void event_handler(void){
  genieFrame Event;
  mdisplay->genie.DequeueEvent(&Event); 

  //***** PARSER *****
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT){

    // FORMS ****
    if (Event.reportObject.object == GENIE_OBJ_FORM){ 
      if (Event.reportObject.index == 0){
        debugln("Form: form0");
        mdisplay->form0.update();
      }
    }
    
    // KNOBS ****
    else if (Event.reportObject.object == GENIE_OBJ_ROTARYSW){ 
      uint8_t ind = Event.reportObject.index;
      int val = mdisplay->genie.GetEventData(&Event);
      debugln("Knob " + String(ind) + String(": ") + String(val));
      mdisplay->knob_values[ind] = val;
    }
    
    // KEYBOARDS ****
    else if (Event.reportObject.object == GENIE_OBJ_KEYBOARD){
      uint8_t ind = Event.reportObject.index;

      uint8_t key = mdisplay->genie.GetEventData(&Event);
      switch(key){
        case 8:
          if( mdisplay->keyboards[ind]->val.length()> 0 ){
            mdisplay->keyboards[ind]->val.remove( mdisplay->keyboards[ind]->val.length() - 1 ) ;
          }
          break;
        case 13:
          if( mdisplay->keyboards[ind]->val.length()> 0 ){
            
            if(ind == 0){     // Set point page ****
              float var =  mdisplay->keyboards[ind]->val.toFloat();
              engine->sp->data.setpoints[ mdisplay->knob_values[ind] ] = var;           // Get sensor_index from knob and use it to set setpoint
              engine->sp->save_data();
              mdisplay->form0.update();   // update form before switching
              //mdisplay->genie.WriteObject(GENIE_OBJ_FORM, mainform, 1);       // change to main page
              mdisplay->keyboards[ind]->val = "save success.";

              String l[4] = {"MSG", "CHANGING SETPOINT " + String(mdisplay->knob_values[ind]), "VALUE", mdisplay->keyboards[ind]->val};
              logger->log(l, 4);
            }
            
          }
          break;
        default:
          if( key>45 && key<58){
            mdisplay->keyboards[ind]->val += (char)key;
          }
          break;
      }
      debug("keyboard: "); debugln(mdisplay->keyboards[ind]->val);
      mdisplay->keyboards[ind]->send_value();
      if(key==13) mdisplay->keyboards[ind]->val = "";
    }
  }
}

void display_t::write_string(uint8_t id, float value){
  genie.WriteStr(id, value); // 2 decimal places
}

void display_t::write_string(uint8_t id, String value){
  genie.WriteStr(id, value); // 2 decimal places
}


/*
 * Keyboard object
 */
dkeyboard::dkeyboard(uint8_t sid){
  val = "";
  string_id = sid;
}

void dkeyboard::send_value(){
  mdisplay->write_string(string_id, val);
}


