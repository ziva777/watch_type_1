#include "datetime.h"
#include <Arduino.h>

// #define CURR_HOUR ((__TIME__[0] - 48) * 10 + (__TIME__[1] - 48))
// #define CURR_MINUTE ((__TIME__[3] - 48) * 10 + (__TIME__[4] - 48))
// #define CURR_SECOND ((__TIME__[6] - 48) * 10 + (__TIME__[7] - 48))

// template<typename T, int BOUND>
// inline void DECREMENT(T &v) {
//     (v ? --v : v=BOUND);
// }
// auto minute_decrement = DECREMENT<uint8_t, 59>;

#define DECREMENT(V, BOUND) (V ? --V : V=BOUND)
#define DECREMENT_1(V, BOUND) (V > 1 ? --V : V=BOUND)

#define DECREMENT_MINUTE(V) DECREMENT(V, 59)
#define DECREMENT_HOUR(V) DECREMENT(V, 23)
#define DECREMENT_DAY(V, F) DECREMENT_1(V, F)
#define DECREMENT_MONTH(V) DECREMENT_1(V, 12)

#define INCREMENT(V, BOUND) (V = ++V % BOUND)
#define INCREMENT_MINUTE(V) INCREMENT(V, 60)
#define INCREMENT_HOUR(V) INCREMENT(V, 24)
#define INCREMENT_DAY(V, F) (V = V % (F) + 1)
#define INCREMENT_MONTH(V) (V = V % 12 + 1)

static uint8_t MONTH_LENGTH[] = {
    0, // dymmy
    31, // 1   January   31 days
    28, // 2   February  28 days
    31, // 3   March     31 days
    30, // 4   April     30 days
    31, // 5   May       31 days
    30, // 6   June      30 days
    31, // 7   July      31 days
    31, // 8   August    31 days
    30, // 9   September 30 days
    31, // 10  October   31 days
    30, // 11  November  30 days
    31  // 12  December  31 days
};

static uint8_t MONTH_LENGTH_LEAP[] = {
    0, // dymmy
    31, // 1   January   31 days
    29, // 2   February  29 in leap years
    31, // 3   March     31 days
    30, // 4   April     30 days
    31, // 5   May       31 days
    30, // 6   June      30 days
    31, // 7   July      31 days
    31, // 8   August    31 days
    30, // 9   September 30 days
    31, // 10  October   31 days
    30, // 11  November  30 days
    31  // 12  December  31 days
};

bool is_leap(uint16_t y) {
    return ((y % 4) == 0) && (((y % 100)!=0) || ((y % 400) == 0));
}

uint8_t day_of_week(uint16_t d, uint16_t m, uint16_t y) {
    return (d += m<3 ? y-- : y-2, 23*m/9 + d + 4 + y/4 - y/100 + y/400) % 7;
}

uint16_t day_number(uint16_t d, uint16_t m, uint16_t y) {
    return 365*y + y/4 - y/100 + (m*306 + 5)/10 + (d - 1);
}

void AlarmDateTime::inc_minute() {
    INCREMENT_MINUTE(minute);
}

void AlarmDateTime::inc_hour() {
    INCREMENT_HOUR(hour);
}

void AlarmDateTime::dec_minute() {
    DECREMENT_MINUTE(minute);
}

void AlarmDateTime::dec_hour() {
    DECREMENT_HOUR(hour);
}

void AlarmDateTime::move_day_pointer() {
    INCREMENT(day_pointer, 7);
}

void AlarmDateTime::switch_day() {
    days[day_pointer] = (days[day_pointer] == 1 ? 0 : 1);
}

void AlarmDateTime::tick1(const DateTime &dt, uint16_t tick_size) {
    if (dt.hour == hour)
        if (dt.minute == minute and no_lock) {
            no_lock = false;
            ringing = true;
        } else {
            no_lock = (dt.minute != minute);
        }
}

void AlarmDateTime::tick2(const DateTime &dt, uint16_t tick_size) {
    tick1(dt, tick_size);
}

void AlarmDateTime::tick3(const DateTime &dt, uint16_t tick_size) {
    if (days[dt.day_of_week])
        if (dt.hour == hour) {   
            if (dt.minute == minute and no_lock) {
                no_lock = false;
                ringing = true;
            } else {
                no_lock = (dt.minute != minute);
            }
        }
}

uint8_t TimerDateTime::days_in_month() const {
    return _month_day_count[origin_month];
}

void TimerDateTime::normalize() {
    leap = is_leap(origin_year);
    _month_day_count = (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

void TimerDateTime::inc_origin_day() {
    INCREMENT_DAY(origin_day, days_in_month());
}

void TimerDateTime::inc_origin_month() {
    INCREMENT_MONTH(origin_month);
}

void TimerDateTime::resolve_febrary_collision() {
    if (leap and origin_month == 2 and origin_day == days_in_month()) {
        origin_day = 1;
        ++origin_month;
    }
}

void TimerDateTime::inc_origin_year() {
    ++origin_year;
    resolve_febrary_collision();
    normalize();
}

void TimerDateTime::dec_origin_day() {
    normalize();
    DECREMENT_DAY(origin_day, days_in_month());
}

void TimerDateTime::dec_origin_month() {
    normalize();
    DECREMENT_MONTH(origin_month);
}

void TimerDateTime::dec_origin_year() {
    --origin_year;
    resolve_febrary_collision();
    normalize();
}

void TimerDateTime::launch_countdown1() {
    hour = origin_hour;
    minute = origin_minute;
    second = origin_second;
    ms = 0;
}

void TimerDateTime::launch_countdown2(DateTime &dt) {
    ms = 0;
}

void TimerDateTime::launch_countdown3(DateTime &dt) {
    ms = 0;   
}

void TimerDateTime::tick_countdown1(uint16_t tick_size) {
    ms += tick_size;

    if (ms > 1000) {
        ms %= 1000;
        
        uint32_t total_sec = hour * 3600UL + 
                                minute * 60UL + 
                                  second;

        if (total_sec > 1)
            --total_sec;
        else {
            total_sec = 0;
            on = false;
            stoppped = true;
            ringing = true;
        }

        hour = total_sec / 3600UL;
        minute = (total_sec - hour * 3600UL) / 60UL;
        second = total_sec - hour * 3600UL - minute * 60UL;
    }
}

void TimerDateTime::tick_countdown2(const DateTime &dt, uint16_t tick_size) {
    uint32_t total_sec_curr = dt.hour * 3600UL + dt.minute * 60UL + dt.second;
    uint32_t total_sec_target = origin_hour * 3600UL + origin_minute * 60UL + origin_second;
    uint32_t total_sec = (total_sec_curr < total_sec_target ? 
                (total_sec_target - total_sec_curr) : 
                (86400UL - total_sec_curr + total_sec_target));

    hour = total_sec / 3600UL;
    minute = (total_sec - hour * 3600UL) / 60UL;
    second = total_sec - hour * 3600UL - minute * 60UL;

    if (total_sec == 86400UL) {
        on = false;
        stoppped = true;
        ringing = true;
    }
}

void TimerDateTime::tick_countdown3(const DateTime &dt, uint16_t tick_size) {
    if (origin_year >= dt.year and origin_month >= dt.month and origin_day >= dt.day) {
        uint16_t total_day = 0;
        uint16_t day_curr = day_number(dt.day, dt.month, dt.year);
        uint16_t day_target = day_number(origin_day, origin_month, origin_year);

        for (uint16_t i = dt.year; i != origin_year; ++i)
            total_day += (::is_leap(i) ? 366 : 365);

        total_day = (day_curr < day_target ? 
                (day_target - day_curr) : 
                ((::is_leap(dt.year) ? 366 : 365) - day_curr + day_target));

        day = total_day - 1;

        hour = 23 - dt.hour;
        minute = 59 - dt.minute;
        second = 59 - dt.second;

        if (day == 0)
            if ((hour == 0) and (minute == 0) and (second == 0)) {
                on = false;
                stoppped = true;
                ringing = true;
            }
    } else {
        // no sence
        day = 0;
        hour = 0;
        minute = 0;
        second = 0;
        on = false;
        stoppped = true;
    }
}

void TimerDateTime::reset_countdown() {
    hour = 0;
    minute = 0;
    second = 0;
    ms = 0;
}

void StopwatchTime::tick(uint16_t tick_size) {
    trigger.flop();
    _hour_buff = hour;
    _minute_buff = minute;
    _second_buff = second;
    _ms_buff = ms;

    ms += tick_size;

    second += ms / 1000;
    ms %= 1000;

    minute += second / 60;
    second %= 60;

    if (minute >= 60) {
        minute %= 60;
        ++hour;
        hour %= 100;
    }

    if (hour != _hour_buff)
        trigger.hour_flip();

    if (minute != _minute_buff)
        trigger.minute_flip();

    if (second != _second_buff)
        trigger.second_flip();

    if (ms != _ms_buff)
        trigger.ms_flip();
}

void StopwatchTime::stamp_it() {
    stamps[stamps_index].hour = hour;
    stamps[stamps_index].minute = minute;
    stamps[stamps_index].second = second;
    stamps[stamps_index].ms = ms;
    stamps[stamps_index].ready = true;
    stamps_index_to_show = stamps_index;

    if (stamps_index != STAMPS_COUNT - 1)
        ++stamps_index;
}

void StopwatchTime::free_stamp() {
    for (uint16_t i = 0; i != STAMPS_COUNT; ++i)
        stamps[i].ready = false;
}

void StopwatchTime::reset() {
    on = false;   
    stoppped = true;
    hour = 0;
    minute = 0;
    second = 0;
    ms = 0;
    stamps_index = 0;
}

DateTime::DateTime() 
    : _month_day_count((_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH))
{
    day_of_week = ::day_of_week(day, month, year);
    _leap = ::is_leap(year);
    _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

void DateTime::tick(uint16_t tick_size) {
    trigger.flop();
    _hour_buff = hour;
    _minute_buff = minute;
    _second_buff = second;
    _day_buff = day;
    _month_buff = month;
    _year_buff = year;

    ms += tick_size;

    second += ms / 1000;
    ms %= 1000;

    minute += second / 60;
    second %= 60;

    if (minute >= 60) {
        minute %= 60;
        ++hour;

        if (hour == 24) {
            hour = 0;
            ++day;

            if (day > days_in_month()) {
                day = 1;
                ++month;

                if (month > 12) {
                    month = 1;
                    ++year;

                    _leap = ::is_leap(year);
                }
                _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
            }

            day_of_week = ::day_of_week(day, month, year);
        }
    }

    if (hour != _hour_buff)
        trigger.hour_flip();

    if (minute != _minute_buff)
        trigger.minute_flip();

    if (second != _second_buff)
        trigger.second_flip();

    if (day != _day_buff)
        trigger.day_flip();

    if (month != _month_buff)
        trigger.month_flip();

    if (year != _year_buff)
        trigger.year_flip();
}

void DateTime::inc_second() {
    (second < 30 ? second = 30 : second = 60);
}

void DateTime::inc_minute() {
    INCREMENT_MINUTE(minute);
}

void DateTime::inc_hour() {
    INCREMENT_HOUR(hour);
}

void DateTime::inc_day() {
    INCREMENT_DAY(day, days_in_month());
    day_of_week = ::day_of_week(day, month, year);
}

void DateTime::inc_month() {
    INCREMENT_MONTH(month);
    day_of_week = ::day_of_week(day, month, year);
}

void DateTime::inc_year() {
    ++year;
    _leap = ::is_leap(year);
    _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
    day_of_week = ::day_of_week(day, month, year);
}

void DateTime::dec_second() {
    (second < 30 ? second = 0 : second = 30);
}

void DateTime::dec_minute() {
    DECREMENT_MINUTE(minute);
}

void DateTime::dec_hour() {
    DECREMENT_HOUR(hour);
    day_of_week = ::day_of_week(day, month, year);
}

void DateTime::dec_day() {
    DECREMENT_DAY(day, days_in_month());
    day_of_week = ::day_of_week(day, month, year);
}

void DateTime::dec_month() {
    DECREMENT_MONTH(month);
    day_of_week = ::day_of_week(day, month, year);
}

void DateTime::dec_year() {
    --year;
    _leap = ::is_leap(year);
    _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
    day_of_week = ::day_of_week(day, month, year);
}

uint8_t DateTime::days_in_month() const {
    return _month_day_count[month];
}

void DateTime::resolve_febrary_collision() {
    if (day > days_in_month()) {
        day = days_in_month();
    }
}
