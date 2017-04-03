#include "display.h"

// For LiquidCrystal
static const uint8_t _RS_ = 8; 
static const uint8_t _ENABLE_ = 9;
static const uint8_t _D0_ = 4;
static const uint8_t _D1_ = 5; 
static const uint8_t _D2_ = A7; 
static const uint8_t _D3_ = 7;

static char *DAY_NAME[7] = {
    "Su",
    "Mo",
    "Tu",
    "We",
    "Th",
    "Fr",
    "Sa"
};

Display::Display() 
    : _lcd(_RS_, _ENABLE_, _D0_, _D1_, _D2_, _D3_) 
{
    
}

void Display::setup(uint8_t contrast_pin, uint8_t brightness_pin) {
    _contrast_pin = contrast_pin;
    _brightness_pin = brightness_pin;

    pinMode(_contrast_pin, OUTPUT);
    pinMode(_brightness_pin, OUTPUT);

    _lcd.begin(16, 2);
}

void Display::print_text(uint8_t row, uint8_t column, char *s) {
    _lcd.setCursor(column, row);
    _lcd.print(s);
}

void Display::print_clock_state(const Clock &clock) {
    switch (clock.state()) {
        case Clock::S_CLOCK1:
            print_text(0, 0, "CL1");
            break;
        case Clock::S_CLOCK2:
            print_text(0, 0, "CL2");
            break;
        case Clock::S_ALARM1:
            print_text(0, 0, "AL1");
            break;
        case Clock::S_ALARM2:
            print_text(0, 0, "AL2");
            break;
        case Clock::S_ALARM3:
            print_text(0, 0, "AL3");
            break;
        case Clock::S_SIGNAL:
            print_text(0, 0, "SG ");
            break;
        case Clock::S_TIMER1:
            print_text(0, 0, "T1 ");
            break;
        case Clock::S_TIMER2:
            print_text(0, 0, "T2 ");
            break;
        case Clock::S_TIMER3:
            print_text(0, 0, "T3 ");
            break;
        case Clock::S_STOPWATCH:
            print_text(0, 0, "SW ");
            break;
    }
}

void Display::print_clock_time(const DateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, " %2u:%02u:%02u ", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(0, 3, buff);
}

void Display::print_clock_date(const DateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u.%02u.%04u %s", 
                  dt.day,
                  dt.month,
                  dt.year,
                  DAY_NAME[dt.day_of_week]);
    print_text(1, 3, buff);
}

void Display::print_clock_datetime(const DateTime &dt) {
    print_clock_time(dt);
    print_clock_date(dt);
}

void Display::print_alarm_type1(const AlarmDateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u  %s", 
                  dt.hour,
                  dt.minute,
                  (dt.on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', sizeof(buff) - 1);
    print_text(1, 0, buff);
}

void Display::print_alarm_type2(const AlarmDateTime &dt) {
    print_alarm_type1(dt);
}

void Display::print_alarm_type3(const AlarmDateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u  %s", 
                  dt.hour,
                  dt.minute,
                  (dt.on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', 16);
    buff[10 + 1] = (dt.days[0] ? 'S' : 's');
    buff[ 4 + 1] = (dt.days[1] ? 'M' : 'm');
    buff[ 5 + 1] = (dt.days[2] ? 'T' : 't');
    buff[ 6 + 1] = (dt.days[3] ? 'W' : 'w');
    buff[ 7 + 1] = (dt.days[4] ? 'T' : 't');
    buff[ 8 + 1] = (dt.days[5] ? 'F' : 'f');
    buff[ 9 + 1] = (dt.days[6] ? 'S' : 's');
    print_text(1, 0, buff);
}

void Display::print_signal(bool on) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%s       ", (on ? "on" : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', sizeof(buff) - 1);
    print_text(1, 0, buff);
}

void Display::print_timer_type1(const TimerDateTime &dt) {
    char buff[16];

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u:%02u %s", 
                  dt.origin_hour,
                  dt.origin_minute,
                  dt.origin_second,
                  (dt.on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u:%02u  ", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(1, 4, buff);
}

void Display::print_timer_type2(const TimerDateTime &dt) {
    print_timer_type1(dt);
}

void Display::print_timer_type3(const TimerDateTime &dt) {
    char buff[16];

    memset(buff, 0, 16);
    sprintf(buff, "%2u.%02u.%02u %s", 
                  dt.origin_day,
                  dt.origin_month,
                  dt.origin_year % 100,
                  (dt.on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, 0, 16);
    sprintf(buff, "%3u %2u:%02u:%02u", 
                  dt.day,
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(1, 0, buff);
}

void Display::print_stopwatch(const StopwatchTime &t) {
    char buff[17];

    memset(buff, 0, 17);
    sprintf(buff, "%02u:%02u:%02u.%01u   ", 
                  t.hour,
                  t.minute,
                  t.second,
                  t.ms / 100);
    print_text(0, 3, buff);

    memset(buff, 0, 17);

    if (!t.stamps[t.stamps_index_to_show].ready) {
        sprintf(buff, "   --:--:--.-  ");
    } else {
        sprintf(buff, "%2u %02u:%02u:%02u.%01u   ", 
                      t.stamps_index_to_show + 1,
                      t.stamps[t.stamps_index_to_show].hour,
                      t.stamps[t.stamps_index_to_show].minute,
                      t.stamps[t.stamps_index_to_show].second,
                      t.stamps[t.stamps_index_to_show].ms / 100);
    }
      
    print_text(1, 0, buff);
}

void Display::set_contrast(uint8_t contrast) {
    _contrast = contrast;
    analogWrite(_contrast_pin, _contrast);
}

void Display::set_brightness(uint8_t brightness) {
    _brightness = brightness;
    analogWrite(_brightness_pin, _brightness);
}

void Display::clear() {
  _lcd.clear();
}

bool Display::_blink() const {
    return ((millis() % 1000) < 250);
}

void Display::print_edit_clock(const DateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[2][17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_CLOCK_EDIT_SECONDS) {
            sprintf(buff[0], " %2u:%02u:  ", dt.hour, dt.minute);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_MINUTES) {
            sprintf(buff[0], " %2u:  :%02u", dt.hour, dt.second);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_HOURS) {
            sprintf(buff[0], "   :%02u:%02u", dt.minute, dt.second);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_DAYS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "  .%02u.%04u %s", dt.month, dt.year, DAY_NAME[dt.day_of_week]);
        } else
        if (state == Clock::S_CLOCK_EDIT_MONTHS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "%2u.  .%04u %s", dt.day, dt.year, DAY_NAME[dt.day_of_week]);
        } else 
        if (state == Clock::S_CLOCK_EDIT_YEARS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "%2u.%02u.     %s", dt.day, dt.month, DAY_NAME[dt.day_of_week]);
        }

        print_text(0, 3, buff[0]);
        print_text(1, 3, buff[1]);
    } else {
        print_clock_datetime(dt);
    }
}

void Display::print_edit_alarm_type1(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_ALARM_TYPE1_EDIT_MINUTES)
            sprintf(buff, "%2u:    %s", 
                          dt.hour,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS)
            sprintf(buff, "  :%02u  %s", 
                          dt.minute,
                          (dt.on ? "on " : "off"));

        print_text(0, 4, buff);
    } else {
        print_alarm_type1(dt);
    }
}

void Display::print_edit_alarm_type2(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    print_edit_alarm_type1(dt, state);
}

void Display::print_edit_alarm_type3(const AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    char buff[17];
    memset(buff, 0, sizeof(buff));

    if (_blink()) {
        if (state == Clock::S_ALARM_TYPE2_EDIT_MINUTES)
            sprintf(buff, "%2u:    %s", 
                          dt.hour,
                          // dt.minute,
                          (dt.on ? "on" : "off"));
        else
        if (state == Clock::S_ALARM_TYPE2_EDIT_HOURS)
            sprintf(buff, "  :%02u  %s", 
                          // dt.hour,
                          dt.minute,
                          (dt.on ? "on " : "off"));
        else
            sprintf(buff, "%2u:%02u  %s", 
                      dt.hour,
                      dt.minute,
                      (dt.on ? "on " : "off"));
    } else {
        sprintf(buff, "%2u:%02u  %s", 
                      dt.hour,
                      dt.minute,
                      (dt.on ? "on " : "off"));
    }

    print_text(0, 4, buff);

    memset(buff, ' ', 16);

    if (state == Clock::S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK) {
        if (((millis() % 1000) < 250)) {
            buff[10 + 1] = (dt.day_pointer == 0 ? ' ' : (dt.days[0] ? 'S' : 's'));
            buff[ 4 + 1] = (dt.day_pointer == 1 ? ' ' : (dt.days[1] ? 'M' : 'm'));
            buff[ 5 + 1] = (dt.day_pointer == 2 ? ' ' : (dt.days[2] ? 'T' : 't'));
            buff[ 6 + 1] = (dt.day_pointer == 3 ? ' ' : (dt.days[3] ? 'W' : 'w'));
            buff[ 7 + 1] = (dt.day_pointer == 4 ? ' ' : (dt.days[4] ? 'T' : 't'));
            buff[ 8 + 1] = (dt.day_pointer == 5 ? ' ' : (dt.days[5] ? 'F' : 'f'));
            buff[ 9 + 1] = (dt.day_pointer == 6 ? ' ' : (dt.days[6] ? 'S' : 's'));
        } else {
            buff[10 + 1] = (dt.days[0] ? 'S' : 's');
            buff[ 4 + 1] = (dt.days[1] ? 'M' : 'm');
            buff[ 5 + 1] = (dt.days[2] ? 'T' : 't');
            buff[ 6 + 1] = (dt.days[3] ? 'W' : 'w');
            buff[ 7 + 1] = (dt.days[4] ? 'T' : 't');
            buff[ 8 + 1] = (dt.days[5] ? 'F' : 'f');
            buff[ 9 + 1] = (dt.days[6] ? 'S' : 's');
        }
    } else {
            buff[10 + 1] = (dt.days[0] ? 'S' : 's');
            buff[ 4 + 1] = (dt.days[1] ? 'M' : 'm');
            buff[ 5 + 1] = (dt.days[2] ? 'T' : 't');
            buff[ 6 + 1] = (dt.days[3] ? 'W' : 'w');
            buff[ 7 + 1] = (dt.days[4] ? 'T' : 't');
            buff[ 8 + 1] = (dt.days[5] ? 'F' : 'f');
            buff[ 9 + 1] = (dt.days[6] ? 'S' : 's');
    }
    print_text(1, 0, buff);
}

void Display::print_edit_timer_type1(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS)
            sprintf(buff, "%2u:%02u:   %s", 
                          dt.origin_hour,
                          dt.origin_minute,
                          //dt.origin_second,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
            sprintf(buff, "%2u:  :%02u %s", 
                          dt.origin_hour,
                          // dt.origin_minute,
                          dt.origin_second,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
            sprintf(buff, "  :%02u:%02u %s", 
                          // dt.origin_hour,
                          dt.origin_minute,
                          dt.origin_second,
                          (dt.on ? "on " : "off"));

        print_text(0, 4, buff);
    } else {
        print_timer_type1(dt);
    }
}

void Display::print_edit_timer_type2(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    print_edit_timer_type1(dt, state);
}

void Display::print_edit_timer_type3(const TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (_blink()) {
        char buff[17];
        memset(buff, 0, sizeof(buff));

        if (state == Clock::S_TIMER_TYPE2_EDIT_DAYS)
            sprintf(buff, "  .%02u.%02u %s", 
                          // dt.origin_day,
                          dt.origin_month,
                          dt.origin_year % 100,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_TIMER_TYPE2_EDIT_MONTHS)
            sprintf(buff, "%2u.  .%02u %s", 
                          dt.origin_day,
                          // dt.origin_month,
                          dt.origin_year % 100,
                          (dt.on ? "on " : "off"));
        else
        if (state == Clock::S_TIMER_TYPE2_EDIT_YEARS)
            sprintf(buff, "%2u.%02u.   %s", 
                      dt.origin_day,
                      dt.origin_month,
                      // dt.origin_year,
                      (dt.on ? "on " : "off"));

        print_text(0, 4, buff);
    } else {
        print_timer_type3(dt);
    }
}

uint8_t Display::contrast() const {
    return _contrast;
}

uint8_t Display::brightness() const {
    return _brightness;
}
