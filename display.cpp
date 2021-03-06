#include "display.h"
#include "settings.h"

// #define ENG_LANG
#define RUS_LANG

namespace letter {
#if defined(RUS_LANG)
    static const uint8_t CAPITAL_PEH = 168;
    static const uint8_t CAPITAL_VEH =  66;
    static const uint8_t CAPITAL_ESS = 67;
    static const uint8_t CAPITAL_CH = 171;

    static const uint8_t ESS = 99;
    static const uint8_t TEH = 191;
    static const uint8_t REH = 112;
    static const uint8_t BEH = 178;
    static const uint8_t ENN = 189;

    static const uint8_t *RUS_DAY_NAME[7][3] = {
        {CAPITAL_VEH, ESS},
        {CAPITAL_PEH, ENN},
        {CAPITAL_VEH, TEH},
        {CAPITAL_ESS, REH},
        {CAPITAL_CH,  TEH},
        {CAPITAL_PEH, TEH},
        {CAPITAL_ESS, BEH}
    };

    static const uint8_t RUS_SHORT_DAY_NAME_ON[7] = {
        CAPITAL_VEH,
        CAPITAL_PEH,
        CAPITAL_VEH,
        CAPITAL_ESS,
        CAPITAL_CH,
        CAPITAL_PEH,
        CAPITAL_ESS
    };

    static const uint8_t RUS_SHORT_DAY_NAME_OFF[7] = {
        179, //VEH,
        190, //PEH,
        179, //VEH,
        'c', //ESS,
        193, //CH,
        190, //PEH,
        'c'  //ESS
    };
#elif defined(ENG_LANG)
    static const char *DAY_NAME[7] = {
        "Su",
        "Mo",
        "Tu",
        "We",
        "Th",
        "Fr",
        "Sa"
    };

    static const char SHORT_DAY_NAME_ON[7] = {
        'S',
        'M',
        'T',
        'W',
        'T',
        'F',
        'S'
    };

    static const char SHORT_DAY_NAME_OFF[7] = {
        's',
        'm',
        't',
        'w',
        't',
        'f',
        's'
    };
#endif

    char * long_day_name(uint8_t day_no) {
        static char buff[3];
        buff[0] = buff[1] = buff[2] = 0;
#if defined(RUS_LANG)
        sprintf(buff, "%c%c", RUS_DAY_NAME[day_no][0],
                                RUS_DAY_NAME[day_no][1]);
#elif defined(ENG_LANG)
        sprintf(buff, "%c%c", DAY_NAME[day_no][0],
                                DAY_NAME[day_no][1]);
#endif
        return buff;
    }

    uint8_t day_char_name(uint8_t day_no, int armed) {
#if defined(RUS_LANG)
        return (armed ? letter::RUS_SHORT_DAY_NAME_ON[day_no] : letter::RUS_SHORT_DAY_NAME_OFF[day_no]);
#elif defined(ENG_LANG)
        return (armed ? letter::SHORT_DAY_NAME_ON[day_no] : letter::SHORT_DAY_NAME_OFF[day_no]);
#endif
    }

};

Display::Display() 
    : _lcd(Pin::Display::RS, Pin::Display::ENABLE, 
            Pin::Display::D0, Pin::Display::D1, Pin::Display::D2, Pin::Display::D3) 
{
    
}

void Display::print_edit_clock_tuning(uint32_t timeshift) {
    char buff[17];
    memset(buff, 0, sizeof(buff));

    print_text(0, 0, "TU              ");
    sprintf(buff, "%11d uS/S", timeshift);
    print_text(1, 0, buff);

    // _blink_char();
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

    print_text(1, 0, "    ");
}

void Display::_blink_char() {
    static uint8_t c = 238;
    _lcd.setCursor(0, 1);
    _lcd.print((char)c);
    (c == 239 ? c = 238 : ++c);

    // static uint8_t c = 180;
    // _lcd.setCursor(0, 0);
    // _lcd.print((char)c);
    // (c == 255 ? c = 0 : ++c);

    // _lcd.setCursor(0, 1);
    // _lcd.print(c);
}

void Display::print_clock_time(const DateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, " %2u:%02u:%02u ", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(0, 3, buff);
    _blink_char();
}

void Display::print_clock_date(const DateTime &dt) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u.%02u.%04u %s", 
                  dt.day,
                  dt.month,
                  dt.year,
                  letter::long_day_name(dt.day_of_week));
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
    _blink_char();
}

void Display::print_alarm_type2(const AlarmDateTime &dt) {
    print_alarm_type1(dt);
}

char * Display::_fill_str_with_day_names(const AlarmDateTime &dt, char *buff, uint8_t buff_offset) {
    buff[buff_offset++] = letter::day_char_name(1, dt.days[1]);
    buff[buff_offset++] = letter::day_char_name(2, dt.days[2]);
    buff[buff_offset++] = letter::day_char_name(3, dt.days[3]);
    buff[buff_offset++] = letter::day_char_name(4, dt.days[4]);
    buff[buff_offset++] = letter::day_char_name(5, dt.days[5]);
    buff[buff_offset++] = letter::day_char_name(6, dt.days[6]);
    buff[buff_offset++] = letter::day_char_name(0, dt.days[0]);
    return buff;
}

char * Display::_fill_str_with_day_names_blink(const AlarmDateTime &dt, char *buff, uint8_t buff_offset) {
    buff[buff_offset++] = (dt.day_pointer == 1 ? ' ' : letter::day_char_name(1, dt.days[1]));
    buff[buff_offset++] = (dt.day_pointer == 2 ? ' ' : letter::day_char_name(2, dt.days[2]));
    buff[buff_offset++] = (dt.day_pointer == 3 ? ' ' : letter::day_char_name(3, dt.days[3]));
    buff[buff_offset++] = (dt.day_pointer == 4 ? ' ' : letter::day_char_name(4, dt.days[4]));
    buff[buff_offset++] = (dt.day_pointer == 5 ? ' ' : letter::day_char_name(5, dt.days[5]));
    buff[buff_offset++] = (dt.day_pointer == 6 ? ' ' : letter::day_char_name(6, dt.days[6]));
    buff[buff_offset++] = (dt.day_pointer == 0 ? ' ' : letter::day_char_name(0, dt.days[0]));
    return buff;
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
    print_text(1, 0, _fill_str_with_day_names(dt, buff, 5));
    _blink_char();
}

void Display::print_signal(bool on) {
    char buff[17];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%s       ", (on ? "on " : "off"));
    print_text(0, 4, buff);

    memset(buff, ' ', sizeof(buff) - 1);
    print_text(1, 0, buff);
    _blink_char();
}

void Display::print_timer_type1(const TimerDateTime &dt) {
    char buff[16];

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u:%02u %s", 
                  dt.origin_hour,
                  dt.origin_minute,
                  dt.origin_second,
                  (dt.stoppped ? "off" : "on "));
    print_text(0, 4, buff);

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%2u:%02u:%02u  ", 
                  dt.hour,
                  dt.minute,
                  dt.second);
    print_text(1, 4, buff);
    _blink_char();
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
                  (dt.stoppped ? "off" : "on "));
    print_text(0, 4, buff);

    memset(buff, 0, 16);

    if (dt.day < 1000) {
        sprintf(buff, "%3u %2u:%02u:%02u", 
                      dt.day,
                      dt.hour,
                      dt.minute,
                      dt.second);
        print_text(1, 0, buff);
    } else {
        _lcd.setCursor(0, 1);
        _lcd.write((char)123);
        sprintf(buff, "%2u %2u:%02u:%02u", 
                      dt.day % 100,
                      dt.hour,
                      dt.minute,
                      dt.second);
        print_text(2, 1, buff);
    }
    _blink_char();
}

void Display::print_stopwatch(const StopwatchTime &t) {
    char buff[17];

    memset(buff, 0, 17);
    sprintf(buff, "%02u:%02u:%02u.%01u   ", 
                  t.counter().hour(),
                  t.counter().minute(),
                  t.counter().second(),
                  t.counter().ms() / 100);
    print_text(0, 3, buff);

    memset(buff, 0, 17);

    if (!t.current_stamp().ready) {
        sprintf(buff, "   --:--:--.-  ");
    } else {
        sprintf(buff, "%2u %02u:%02u:%02u.%01u   ", 
                      t.current_stamp_index() + 1,
                      t.current_stamp().lap_counter.hour(),
                      t.current_stamp().lap_counter.minute(),
                      t.current_stamp().lap_counter.second(),
                      t.current_stamp().lap_counter.ms() / 100);
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
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, 
                                letter::long_day_name(dt.day_of_week));
        } else if (state == Clock::S_CLOCK_EDIT_MINUTES) {
            sprintf(buff[0], " %2u:  :%02u", dt.hour, dt.second);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, 
                                letter::long_day_name(dt.day_of_week));
        } else if (state == Clock::S_CLOCK_EDIT_HOURS) {
            sprintf(buff[0], "   :%02u:%02u", dt.minute, dt.second);
            sprintf(buff[1], "%2u.%02u.%04u %s", dt.day, dt.month, dt.year, 
                                letter::long_day_name(dt.day_of_week));
        } else if (state == Clock::S_CLOCK_EDIT_DAYS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "  .%02u.%04u %s", dt.month, dt.year, 
                                letter::long_day_name(dt.day_of_week));
        } else if (state == Clock::S_CLOCK_EDIT_MONTHS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "%2u.  .%04u %s", dt.day, dt.year, 
                                letter::long_day_name(dt.day_of_week));
        } else if (state == Clock::S_CLOCK_EDIT_YEARS) {
            sprintf(buff[0], " %2u:%02u:%02u", dt.hour, dt.minute, dt.second);
            sprintf(buff[1], "%2u.%02u.     %s", dt.day, dt.month, 
                                letter::long_day_name(dt.day_of_week));
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
        else if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS)
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
    bool blink = _blink();

    if (blink) {
        if (state == Clock::S_ALARM_TYPE2_EDIT_MINUTES)
            sprintf(buff, "%2u:    %s", 
                          dt.hour,
                          // dt.minute,
                          (dt.on ? "on" : "off"));
        else if (state == Clock::S_ALARM_TYPE2_EDIT_HOURS)
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
        if (blink)
            _fill_str_with_day_names_blink(dt, buff, 5);
        else
            _fill_str_with_day_names(dt, buff, 5);
    } else {
            _fill_str_with_day_names(dt, buff, 5);
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
                          (dt.stoppped ? "off" : "on "));
        else if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
            sprintf(buff, "%2u:  :%02u %s", 
                          dt.origin_hour,
                          // dt.origin_minute,
                          dt.origin_second,
                          (dt.stoppped ? "off" : "on "));
        else if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
            sprintf(buff, "  :%02u:%02u %s", 
                          // dt.origin_hour,
                          dt.origin_minute,
                          dt.origin_second,
                          (dt.stoppped ? "off" : "on "));

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
        else if (state == Clock::S_TIMER_TYPE2_EDIT_MONTHS)
            sprintf(buff, "%2u.  .%02u %s", 
                          dt.origin_day,
                          // dt.origin_month,
                          dt.origin_year % 100,
                          (dt.on ? "on " : "off"));
        else if (state == Clock::S_TIMER_TYPE2_EDIT_YEARS)
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
