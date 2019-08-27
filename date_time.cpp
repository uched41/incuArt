#include <Controllino.h>
#include "date_time.h" 

/*
 * RTC Object
 */
date_time_t::date_time_t(void){
  Controllino_RTC_init(0);
  Controllino_SetTimeDate(5,2,6,19,8,37,23);  // (Day of the month, Day of the week, Month, Year, Hour, Minute, Second)
}

void date_time_t::get_time(void){
  day_month = Controllino_GetDay();
  seconds = Controllino_GetSecond(); 
  minute = Controllino_GetMinute(); 
  hour = Controllino_GetHour();
  day_week = Controllino_GetWeekDay();
  month = Controllino_GetMonth();
  year = Controllino_GetYear();
}

void date_time_t::loop(void){
  get_time();
}

