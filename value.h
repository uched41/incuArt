#ifndef VALUE_T_H
#define VALUE_T_H

#include <Controllino.h>
#include <PID_v1.h>

class value_t{
  float  old_value;
  float  new_value;

  int kp, ki, kd;   // PID Koefficients
    
  public:
    value_t(void);
    double* setpoint;
    uint8_t value_str_id;
    uint8_t setpoint_str_id;
    
    void    set_value(float v);
    float   get_value();
    float   get_old_value();
    bool    compare_value(unsigned char prec);
    virtual bool refresh_val() = 0;

    void set_pid_koefficients(int p, int i, int d);
    double pid_input, pid_output;
    PID* _pid;
    virtual void loop(void) = 0;
    
    void send_value(void);
    void send_setpoint(void);
    void log_error(uint8_t _index=0);
};

class analog_value_t : public value_t {    
  public:
    float   offset;
    float   slope;
    uint8_t pin;
    int val_adc;
    analog_value_t(uint8_t mpin, uint8_t vid=127, uint8_t sid=127);
    void set_param(float offset, float slope);
    void print_val();
    bool refresh_val();

    void loop(void){};
};

#endif
