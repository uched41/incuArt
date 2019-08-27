#ifndef CONFIG_H
#define CONFIG_H

#include <Controllino.h>

#define DEBUG_MODE 1
#define DEBUG_SERIAL Serial
#define debug(x) if(DEBUG_MODE){ \
                  Serial.print(x); \
                  }

#define debugln(x) if(DEBUG_MODE){ \
                  Serial.println(x); \
                  }
double mapf(double val, double in_min, double in_max, double out_min, double out_max);

/* 
 * Pin definitions
 */
#define tc1_pin   CONTROLLINO_D0
#define tc2_pin   CONTROLLINO_D1
#define tc3_pin   CONTROLLINO_D2
#define tc4_pin   CONTROLLINO_D3
#define tc5_pin   CONTROLLINO_D4
#define tc6_pin   CONTROLLINO_D5

#define co2_pin   CONTROLLINO_A0
#define o2_pin    CONTROLLINO_A1
#define rh_pin    CONTROLLINO_D6
#define ph_pin    CONTROLLINO_D7

#define tcoil1_pin  CONTROLLINO_R0
#define tcoil2_pin  CONTROLLINO_R1
#define tcoil3_pin  CONTROLLINO_R2
#define tcoil4_pin  CONTROLLINO_R3
#define tcoil5_pin  CONTROLLINO_R4
#define tcoil6_pin  CONTROLLINO_R5

#define co2valve_pin        28
#define o2valve_pin         29
#define co2_main_valve_pin  30
#define o2_main_valve_pin   31

#define co2_pressure_pin  A8
#define o2_pressure_pin   A9

#define ldr1_pin    CONTROLLINO_A2
#define ldr2_pin    CONTROLLINO_A3
#define ldr3_pin    CONTROLLINO_A4
#define ldr4_pin    CONTROLLINO_A5
#define ldr5_pin    CONTROLLINO_A6
#define ldr6_pin    CONTROLLINO_A7

#define uv_lamp_pin CONTROLLINO_A8

#define sd_chip_select    CONTROLLINO_PIN_HEADER_SS    
#define sd_mosi           CONTROLLINO_PIN_HEADER_MOSI    // *** DO NOT CHANGE ***
#define sd_miso           CONTROLLINO_PIN_HEADER_MISO    // *** DO NOT CHANGE ***
#define sd_sck            CONTROLLINO_PIN_HEADER_SCK    // *** DO NOT CHANGE ***

/* 
 * Set points
 */
#define max_sp_temperature  39
#define max_sp_co2          20
#define max_sp_o2           21

#define ldr_open_setpoint     800
#define ldr_close_setpoint    600

#define minimum_gas_pressure          50    // in Kpa


/*
 * Display Configuration
 */
#define display_serial    Serial2
#define display_reset_pin CONTROLLINO_D8


/* 
 * PID Koefficinets
 */
#define TC01_KP     140
#define TC01_KI     141
#define TC01_KD     142

#define TC02_KP     144
#define TC02_KI     145
#define TC02_KD     146

#define TC03_KP     148
#define TC03_KI     149
#define TC03_KD     150

#define TC04_KP     152
#define TC04_KI     153
#define TC04_KD     154

#define TC05_KP     156
#define TC05_KI     157
#define TC05_KD     158

#define TC06_KP     160
#define TC06_KI     161
#define TC06_KD     162

#define CO2_KP      103
#define CO2_KI      104
#define CO2_KD      105

#define O2_KP       111
#define O2_KI       112
#define O2_KD       113


/*
 * Miscellaneous 
 */
#define INT_MAX           32767     // Large constant

#endif
