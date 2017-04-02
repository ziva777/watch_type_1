#include "datetime.h"

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

void TimerDateTime::normalize() {
    _month_day_count = (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);

    if (origin_day > _month_day_count[origin_month]) {
        origin_day = 1;
        ++origin_month;

        if (origin_month > 12) {
            origin_month = 1;
            ++origin_year;

            leap = _is_leap(origin_year);
        }
        _month_day_count = (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
    }
}

bool TimerDateTime::_is_leap(uint16_t y) const {
    return ((y % 4) == 0) && (((y % 100)!=0) || ((y % 400) == 0));
}

uint8_t TimerDateTime::_day_of_week(uint16_t d, uint16_t m, uint16_t y) const {
    return (d += m<3 ? y-- : y-2, 23*m/9 + d + 4 + y/4 - y/100 + y/400) % 7;
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
    day_of_week = _day_of_week(day, month, year);
    _leap = _is_leap(year);
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

                    _leap = _is_leap(year);
                }
                _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
            }

            day_of_week = _day_of_week(day, month, year);
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

void DateTime::normalize() {
    trigger.flop();
    _hour_buff = hour;
    _minute_buff = minute;
    _second_buff = second;
    _day_buff = day;
    _month_buff = month;
    _year_buff = year;

    second += ms / 1000;
    ms %= 1000;

    minute += second / 60;
    second %= 60;

    if (minute >= 60) {
        minute %= 60;
        ++hour;
    }

    if (hour == 24) {
        hour = 0;
        ++day;
    }

    if (day > _month_day_count[month]) {
        day = 1;
        ++month;
    }

    day_of_week = _day_of_week(day, month, year);

    if (month > 12) {
        month = 1;
        ++year;

        _leap = _is_leap(year);
    }
    _month_day_count = (_leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);

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

bool DateTime::_is_leap(uint16_t y) const {
    return ((y % 4) == 0) && (((y % 100)!=0) || ((y % 400) == 0));
}

uint8_t DateTime::_day_of_week(uint16_t d, uint16_t m, uint16_t y) const {
    return (d += m<3 ? y-- : y-2, 23*m/9 + d + 4 + y/4 - y/100 + y/400) % 7;
}
