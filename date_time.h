#ifndef DATETIME_H
#define DATETIME_H

class date_time_t{ 
  public:
    int seconds, minute, hour, day_week, day_month, week, month, year ;
    date_time_t(void);
    void get_time(void);
    void loop();
};

extern date_time_t* clock;

#endif
