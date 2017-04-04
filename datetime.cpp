#include "datetime.h"
#include <Arduino.h>

#define CURR_HOUR ((__TIME__[0] - 48) * 10 + (__TIME__[1] - 48))
#define CURR_MINUTE ((__TIME__[3] - 48) * 10 + (__TIME__[4] - 48))
#define CURR_SECOND ((__TIME__[6] - 48) * 10 + (__TIME__[7] - 48))

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

void TimerDateTime::normalize() {
    leap = is_leap(origin_year);
    _month_day_count = (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

void TimerDateTime::inc_origin_day() {
    origin_day = origin_day % _month_day_count[origin_month] + 1;
}

void TimerDateTime::inc_origin_month() {
    origin_month = origin_month % 12 + 1;
}

void TimerDateTime::inc_origin_year() {
    ++origin_year;

    if (leap and origin_month == 2 and origin_day == _month_day_count[origin_month]) {
        origin_day = 1;
        ++origin_month;
    }

    leap = is_leap(origin_year);
    _month_day_count = (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

void TimerDateTime::dec_origin_day() {
    if (origin_day == 1)
        origin_day = _month_day_count[origin_month];
    else
        --origin_day;
}

void TimerDateTime::dec_origin_month() {
    if (origin_month == 1)
        origin_month = 12;
    else
        --origin_month;
}

void TimerDateTime::dec_origin_year() {
    --origin_year;

    if (leap and origin_month == 2 and origin_day == _month_day_count[origin_month]) {
        origin_day = 1;
        ++origin_month;
    }

    leap = is_leap(origin_year);
    _month_day_count = (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

void TimerDateTime::launch_countdown() {
    hour = origin_hour;
    minute = origin_minute;
    second = origin_second;

    

    ms = 0;
}

void TimerDateTime::launch_countdown(DateTime &dt) {
    origin_hour = dt.hour;
    origin_minute = dt.minute;
    origin_second = dt.second-1;
    ms = 0;
}

void TimerDateTime::tick_countdown(uint16_t tick_size) {
    ms += tick_size;

    if (ms > 1000) {
        ms %= 1000;
        
        uint32_t total_sec = hour * 3600UL + 
                                minute * 60UL + 
                                  second;

        if (total_sec)
            --total_sec;
        else {
            on = false;
            stoppped = true;
            tone(6, 440, 50);
            // tone(6, 14400, 150);
            // tone(6, 440, 50);
        }

        hour = total_sec / 3600UL;
        minute = (total_sec - hour * 3600UL) / 60UL;
        second = total_sec - hour * 3600UL - minute * 60UL;
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
    hour = CURR_HOUR;
    minute = CURR_MINUTE;
    second = CURR_SECOND + 5;

    day = 31;
    month = 3;
    year = 2017;
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

            if (day > _month_day_count[month]) {
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
    minute = ++minute % 60;
}

void DateTime::inc_hour() {
    hour = ++hour % 24;
}

void DateTime::inc_day() {
    (day < days_in_month() ? ++day : day = 1);
}

void DateTime::inc_month() {
    (month < 12 ? ++month : month = 1);
}

void DateTime::inc_year() {
    ++year;
    _leap = ::is_leap(year);
    _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

void DateTime::dec_second() {
    (second < 30 ? second = 0 : second = 30);
}

void DateTime::dec_minute() {
    (minute ? --minute : minute=59);
}

void DateTime::dec_hour() {
    (hour ? --hour : hour=23);
}

void DateTime::dec_day() {
    (day > 1 ? --day : day = days_in_month());
}

void DateTime::dec_month() {
    (month > 1 ? --month : month = 1);
}

void DateTime::dec_year() {
    --year;
    _leap = ::is_leap(year);
    _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
}

uint8_t DateTime::days_in_month() const {
    return _month_day_count[month];
}

void DateTime::resolve_febrary_collision() {
    if (day > _month_day_count[month]) {
        // ++month;
        // day = 1;
        day = _month_day_count[month];
    }
}

// void DateTime::normalize() {
//     trigger.flop();
//     _hour_buff = hour;
//     _minute_buff = minute;
//     _second_buff = second;
//     _day_buff = day;
//     _month_buff = month;
//     _year_buff = year;

//     second += ms / 1000;
//     ms %= 1000;

//     minute += second / 60;
//     second %= 60;

//     if (minute >= 60) {
//         minute %= 60;
//         ++hour;
//     }

//     if (hour == 24) {
//         hour = 0;
//         ++day;
//     }

//     if (day > _month_day_count[month]) {
//         day = 1;
//         ++month;
//     }

//     day_of_week = ::day_of_week(day, month, year);

//     if (month > 12) {
//         month = 1;
//         ++year;

//         _leap = ::is_leap(year);
//     }
//     _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);

//     if (hour != _hour_buff)
//         trigger.hour_flip();

//     if (minute != _minute_buff)
//         trigger.minute_flip();

//     if (second != _second_buff)
//         trigger.second_flip();

//     if (day != _day_buff)
//         trigger.day_flip();

//     if (month != _month_buff)
//         trigger.month_flip();

//     if (year != _year_buff)
//         trigger.year_flip();
// }
