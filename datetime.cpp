#include "datetime.h"
#include <Arduino.h>

namespace scale {
    // Seconds consts
    namespace mseconds_per {
        const uint16_t SECOND = 1000U;
        const uint16_t MINUTE = 60000U;
    };

    // Mseconds consts
    namespace seconds_per {
        const uint32_t MINUTE = 60UL;
        const uint32_t HOUR = 3600UL;
        const uint32_t DAY = 86400UL;
    };
};

// Defines for consts
namespace defines {
    const uint8_t SECONDS_MAX = 59U;
    const uint8_t MINUTES_MAX = 59U;
    const uint8_t HOURS_MAX = 23U;

    const uint8_t SECONDS_OVERFLOW = SECONDS_MAX + 1U;
    const uint8_t MINUTES_OVERFLOW = MINUTES_MAX + 1U;
    const uint8_t HOURS_OVERFLOW = HOURS_MAX + 1U;

    const uint8_t HALF_A_MINUTE = 30U; // seconds

    const uint8_t COUNTDOWN_TIMER_HOURS_MAX = 99U;
    const uint8_t COUNTDOWN_TIMER_HOURS_OVERFLOW = COUNTDOWN_TIMER_HOURS_MAX + 1U;

    const uint32_t COUNTDOWN_TIMER_MAX_WITHOUT_SECONDS = 
                        COUNTDOWN_TIMER_HOURS_MAX * scale::seconds_per::HOUR +
                            MINUTES_MAX * scale::seconds_per::MINUTE;
};

namespace aux {
    uint8_t MONTH_LENGTH[] = {
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

    uint8_t MONTH_LENGTH_LEAP[] = {
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

    inline uint8_t month_day_count(bool leap) {
        return (leap ? MONTH_LENGTH_LEAP : MONTH_LENGTH);
    }

    template<typename T, int BOUND>
    inline T & increment(T &v) {
        return (v = ++v % BOUND);
    }

    template<typename T, int BOUND>
    inline T & decrement(T &v) {
        return (v ? --v : v=BOUND);
    }

    auto decrement_second = decrement<uint8_t, defines::SECONDS_MAX>;
    auto decrement_minute = decrement<uint8_t, defines::MINUTES_MAX>;
    auto decrement_hour = decrement<uint8_t, defines::HOURS_MAX>;

    template<typename T>
    inline T & decrement_day(T &v, T BOUND) {
        return (v > 1 ? --v : v=BOUND);
    }

    template<typename T>
    inline T & decrement_month(T &v) {
        return (v > 1 ? --v : v=12);
    }

    auto increment_second = increment<uint8_t, defines::SECONDS_OVERFLOW>;
    auto increment_minute = increment<uint8_t, defines::MINUTES_OVERFLOW>;
    auto increment_hour = increment<uint8_t, defines::HOURS_OVERFLOW>;

    template<typename T>
    inline T & increment_day(T &v, T BOUND) {
        return (v = v % (BOUND) + 1);
    }

    template<typename T>
    inline T & increment_month(T &v) {
        return (v = v % (12) + 1);
    }
}

void AlarmDateTime::inc_minute() {
    aux::increment_minute(minute);
    trigger.minute_flip();
}

void AlarmDateTime::inc_hour() {
    aux::increment_hour(hour);
    trigger.hour_flip();
}

void AlarmDateTime::dec_minute() {
    aux::decrement_minute(minute);
    trigger.minute_flip();
}

void AlarmDateTime::dec_hour() {
    aux::decrement_hour(hour);
    trigger.hour_flip();
}

void AlarmDateTime::move_day_pointer() {
    aux::increment<
            uint8_t, 
            7
        >(day_pointer);
    trigger.day_pointer_flip();
}

void AlarmDateTime::switch_day() {
    days[day_pointer] = (days[day_pointer] == 1 ? 0 : 1);
    trigger.day_pointer_flip();
}

void AlarmDateTime::tick1(const DateTime &dt, uint16_t tick_size) {
    trigger.flop();

    if (dt.second == 0 and dt.hour == hour) {   
        if (dt.minute == minute and no_lock) {
            no_lock = false;
            ringing = true;
        } else {
            no_lock = (dt.minute != minute);
        }
    }
}

void AlarmDateTime::tick2(const DateTime &dt, uint16_t tick_size) {
    trigger.flop();

    if (dt.second == 0 and dt.hour == hour) {
        if (dt.minute == minute and no_lock) {
            no_lock = false;
            ringing = true;
            trigger.on_flip();
        } else {
            no_lock = (dt.minute != minute);
        }
    }
}

void AlarmDateTime::tick3(const DateTime &dt, uint16_t tick_size) {
    trigger.flop();

    if (days[dt.day_of_week]) {
        if (dt.second == 0 and dt.hour == hour) {   
            if (dt.minute == minute and no_lock) {
                no_lock = false;
                ringing = true;
            } else {
                no_lock = (dt.minute != minute);
            }
        }
    }
}

uint8_t TimerDateTime::days_in_month() const {
    return _month_day_count[origin_month];
}

void TimerDateTime::normalize() {
    leap = aux::is_leap(origin_year);
    _month_day_count = aux::month_day_count(leap);
}

void TimerDateTime::inc_second() {
    if (++origin_second == defines::SECONDS_OVERFLOW) {
        origin_second = 0;
        inc_minute();
    }
}

void TimerDateTime::inc_minute() {
    if (++origin_minute == defines::MINUTES_OVERFLOW) {
        origin_minute = 0;
        inc_hour();
    }
}

void TimerDateTime::inc_hour() {
    aux::increment<
            uint8_t, 
            defines::COUNTDOWN_TIMER_HOURS_OVERFLOW
        >(origin_hour);
}

uint32_t TimerDateTime::_get_total_origin_sec() const {
    uint32_t total_sec = origin_hour * scale::seconds_per::HOUR + 
                            origin_minute * scale::seconds_per::MINUTE + 
                              origin_second;
    return total_sec;
}

void TimerDateTime::_set_total_origin_sec(uint32_t total_sec) {
    origin_hour = total_sec / scale::seconds_per::HOUR;
    origin_minute = (total_sec - origin_hour * scale::seconds_per::HOUR) / 
                        scale::seconds_per::MINUTE;
    origin_second = total_sec - 
                        origin_hour * scale::seconds_per::HOUR - 
                            origin_minute * scale::seconds_per::MINUTE;
}

uint32_t TimerDateTime::_get_total_sec() const {
    uint32_t total_sec = hour * scale::seconds_per::HOUR + 
                            minute * scale::seconds_per::MINUTE + 
                              second;
    return total_sec;
}

bool TimerDateTime::_set_total_sec(uint32_t total_sec) {
    uint8_t s = second;
    hour = total_sec / scale::seconds_per::HOUR;
    minute = (total_sec - hour * scale::seconds_per::HOUR) / 
                scale::seconds_per::MINUTE;
    second = total_sec - 
                hour * scale::seconds_per::HOUR - 
                    minute * scale::seconds_per::MINUTE;
    return s != second;
}

void TimerDateTime::dec_second() {
    uint32_t total_sec = _get_total_origin_sec();

    if (total_sec) {
        --total_sec;
    } else {
        total_sec = defines::COUNTDOWN_TIMER_MAX_WITHOUT_SECONDS + 
                        defines::SECONDS_MAX;
    }

    _set_total_origin_sec(total_sec);
}

void TimerDateTime::dec_minute() {
    uint32_t total_sec = _get_total_origin_sec();

    if (total_sec >= scale::seconds_per::MINUTE) {
        total_sec -= scale::seconds_per::MINUTE;
    } else {
        total_sec = defines::COUNTDOWN_TIMER_MAX_WITHOUT_SECONDS + 
                        origin_second;
    }

    _set_total_origin_sec(total_sec);
}

void TimerDateTime::dec_hour() {
    aux::decrement<
            uint8_t, 
            defines::COUNTDOWN_TIMER_HOURS_MAX
        >(origin_hour);
}

void TimerDateTime::inc_origin_day() {
    aux::increment_day(origin_day, days_in_month());
}

void TimerDateTime::inc_origin_month() {
    aux::increment_month(origin_month);
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

void TimerDateTime::inc_origin_year(uint16_t year_curr, uint8_t year_max_offset) {
    (origin_year < year_curr + year_max_offset ? 
        ++origin_year : 
        origin_year -= year_max_offset);
    resolve_febrary_collision();
    normalize();
}

void TimerDateTime::inc_origin_second() {
    aux::increment_second(origin_second);

    if (origin_second == 0)
        inc_origin_minute();
}

void TimerDateTime::inc_origin_minute() {
    aux::increment_minute(origin_minute);

    if (origin_minute == 0)
        inc_origin_hour();
}

void TimerDateTime::inc_origin_hour() {
    aux::increment_hour(origin_hour);
}

void TimerDateTime::dec_origin_second() {
    aux::decrement_second(origin_second);

    if (origin_second == defines::SECONDS_MAX) // after dec become 59
        dec_origin_minute();
}

void TimerDateTime::dec_origin_minute() {
    aux::decrement_minute(origin_minute);

    if (origin_minute == defines::MINUTES_MAX) // after dec become 59
        aux::decrement_hour(origin_hour);
}

void TimerDateTime::dec_origin_hour() {
    aux::decrement_hour(origin_hour);
}

void TimerDateTime::dec_origin_day() {
    normalize();
    aux::decrement_day<uint8_t>(origin_day, days_in_month());
}

void TimerDateTime::dec_origin_month() {
    normalize();
    aux::decrement_month(origin_month);
}

void TimerDateTime::dec_origin_year() {
    --origin_year;
    resolve_febrary_collision();
    normalize();
}

void TimerDateTime::dec_origin_year(uint16_t year_curr, uint8_t year_max_offset) {
    (origin_year > year_curr ? 
        --origin_year : 
        origin_year += year_max_offset);
    resolve_febrary_collision();
    normalize();
}

void TimerDateTime::launch_countdown1() {
    hour = origin_hour;
    minute = origin_minute;
    second = origin_second;
    ms = 0;
    trigger.hour_flip();
    trigger.minute_flip();
    trigger.second_flip();
}

void TimerDateTime::launch_countdown2(DateTime &dt) {
    ms = 0;
    trigger.second_flip();
}

void TimerDateTime::launch_countdown3(DateTime &dt) {
    hour = 0;
    minute = 0;
    second = 0;
    ms = 0; 
    trigger.second_flip();  
}

void TimerDateTime::tick_countdown1(uint16_t tick_size) {
    trigger.flop();

    ms += tick_size;

    if (ms > scale::mseconds_per::SECOND) {
        ms %= scale::mseconds_per::SECOND;
        
        uint32_t total_sec = _get_total_sec();

        if (total_sec > 1)
            --total_sec;
        else {
            total_sec = 0;
            on = false;
            stoppped = true;
            ringing = true;
        }

        _set_total_sec(total_sec);
        trigger.second_flip();
    }
}

void TimerDateTime::tick_countdown2(const DateTime &dt, uint16_t tick_size) {
    trigger.flop();

    uint32_t total_sec_curr = dt.hour * scale::seconds_per::HOUR + 
                                dt.minute * scale::seconds_per::MINUTE + 
                                    dt.second;
    uint32_t total_sec_target = _get_total_origin_sec();
    uint32_t total_sec = (total_sec_curr < total_sec_target ? 
                            (total_sec_target - total_sec_curr) : 
                            (scale::seconds_per::DAY - total_sec_curr + total_sec_target));

    if (_set_total_sec(total_sec))
        trigger.second_flip();

    if (total_sec == scale::seconds_per::DAY) {
        on = false;
        stoppped = true;
        ringing = true;
    }
}

void TimerDateTime::tick_countdown3(const DateTime &dt, uint16_t tick_size) {
    trigger.flop();

    if ((origin_year > dt.year) or
        (origin_year == dt.year and origin_month > dt.month) or
        (origin_year == dt.year and origin_month == dt.month and origin_day > dt.day) ) {
        uint8_t s = second;
        uint16_t total_day = 0;
        uint16_t day_curr = aux::day_number(dt.day, dt.month, dt.year);
        uint16_t day_target = aux::day_number(origin_day, origin_month, origin_year);

        for (uint16_t i = dt.year; i != origin_year; ++i)
            total_day += (aux::is_leap(i) ? 366 : 365);

        total_day = (day_curr < day_target ? 
                        (day_target - day_curr) : 
                        ((aux::is_leap(dt.year) ? 366 : 365) - day_curr + day_target));

        day = total_day - 1;

        hour = defines::HOURS_MAX - dt.hour;
        minute = defines::MINUTES_MAX - dt.minute;
        second = defines::SECONDS_MAX - dt.second;

        if (day == 0)
            if ((hour == 0) and (minute == 0) and (second == 0)) {
                on = false;
                stoppped = true;
                ringing = true;
            }

        if (s != second)
            trigger.second_flip();
    } else {
        // no sence
        day = 0;
        hour = 0;
        minute = 0;
        second = 0;
        on = false;
        stoppped = true;
        trigger.second_flip();
    }
}

void TimerDateTime::reset_countdown() {
    hour = 0;
    minute = 0;
    second = 0;
    ms = 0;
}

void StopwatchTime::HMSm::tick(uint16_t tick_size) {
    _ms += tick_size;

    _second += _ms / scale::mseconds_per::SECOND;
    _ms %= scale::mseconds_per::SECOND;

    _minute += _second / defines::SECONDS_OVERFLOW;
    _second %= defines::SECONDS_OVERFLOW;

    if (_minute >= defines::MINUTES_OVERFLOW) {
        _minute %= defines::MINUTES_OVERFLOW;
        _hour = ++_hour % defines::COUNTDOWN_TIMER_HOURS_OVERFLOW;
    }
}

void StopwatchTime::HMSm::reset() {
    _hour = 0;
    _minute = 0;
    _second = 0;
    _ms = 0;
}

const StopwatchTime::HMSm & StopwatchTime::counter() const {
    return _counter;
}

void StopwatchTime::tick(uint16_t tick_size) {
    trigger.flop();

    uint8_t hour_buff = _counter.hour();
    uint8_t minute_buff = _counter.minute();
    uint8_t second_buff = _counter.second();
    uint32_t ms_buff = _counter.ms();

    _counter.tick(tick_size);

    if (_counter.hour() != hour_buff)
        trigger.hour_flip();

    if (_counter.minute() != minute_buff)
        trigger.minute_flip();

    if (_counter.second() != second_buff)
        trigger.second_flip();

    if (_counter.ms() != ms_buff)
        trigger.ms_flip();
}

void StopwatchTime::stamp_it() {
    _stamps[_stamps_index].lap_counter = _counter;
    _stamps[_stamps_index].ready = true;
    _stamps_index_to_show = _stamps_index;

    if (_stamps_index != _STAMPS_COUNT - 1)
        ++_stamps_index;

    trigger.forced_flip();
}

void StopwatchTime::free_stamp() {
    for (uint16_t i = 0; i != _STAMPS_COUNT; ++i)
        _stamps[i].ready = false;

    reset();
    trigger.forced_flip();
}

void StopwatchTime::reset() {
    on = false;   
    stoppped = true;
    _stamps_index = 0;
    _counter.reset();
}

const StopwatchTime::HMSmStamp & StopwatchTime::current_stamp() const {
    return _stamps[_stamps_index_to_show];
}

uint8_t StopwatchTime::current_stamp_index() const {
    return _stamps_index_to_show;
}

void StopwatchTime::inc_current_stamp_index() {
    if (_stamps_index_to_show < _stamps_index - 1)
        ++_stamps_index_to_show;

    trigger.forced_flip();
}

void StopwatchTime::dec_current_stamp_index() {
    if (_stamps_index_to_show)
        --_stamps_index_to_show;

    trigger.forced_flip();
}

DateTime::DateTime() 
    : _month_day_count(aux::month_day_count(_leap))
{
    day_of_week = aux::day_of_week(day, month, year);
    _leap = aux::is_leap(year);
    _month_day_count = (aux::month_day_count(_leap));
}

void DateTime::tick(uint16_t tick_size) {
    trigger.flop();

    ms += tick_size;
    uint8_t tmp = ms / scale::mseconds_per::SECOND;
    ms %= scale::mseconds_per::SECOND;

    for(uint16_t i = 0; i != tmp; ++i) {
        inc_second();

        if (second == 0) {
            inc_minute();

            if (minute == 0) {
                inc_hour();

                if (hour == 0) {
                    inc_day();

                    if (day == 1) {
                        inc_month();

                        if (month == 1) {
                            inc_year();
                        }
                    }
                }
            }
        }
    }
}

void DateTime::align_second_up() {
    // (second < 30 ? second = 30 : second = 0, inc_minute());

    if (second > defines::HALF_A_MINUTE) {
        second = 0;
        ms = 0;
        tick(scale::mseconds_per::MINUTE);
    } else if (second <= defines::HALF_A_MINUTE) {
        second = defines::HALF_A_MINUTE;
        ms = 0;
    }
}

void DateTime::align_second_down() {
    (second < defines::HALF_A_MINUTE ? 
        second = 0 : 
        second = defines::HALF_A_MINUTE);
    ms = 0;
}

void DateTime::inc_second() {
    aux::increment_second(second);
    trigger.second_flip();
}

void DateTime::inc_minute() {
    aux::increment_minute(minute);
    trigger.minute_flip();
}

void DateTime::inc_hour() {
    aux::increment_hour(hour);
    trigger.hour_flip();
}

void DateTime::inc_day() {
    aux::increment_day(day, days_in_month());
    day_of_week = aux::day_of_week(day, month, year);
    trigger.day_flip();
}

void DateTime::inc_month() {
    aux::increment_month(month);
    day_of_week = aux::day_of_week(day, month, year);
    trigger.month_flip();
}

void DateTime::inc_year() {
    ++year;
    _leap = aux::is_leap(year);
    _month_day_count = aux::month_day_count(_leap);
    day_of_week = aux::day_of_week(day, month, year);
    trigger.year_flip();
}

void DateTime::dec_second() {
    aux::decrement_second(second);
    trigger.second_flip();
}

void DateTime::dec_minute() {
    aux::decrement_minute(minute);
    trigger.minute_flip();
}

void DateTime::dec_hour() {
    aux::decrement_hour(hour);
    day_of_week = aux::day_of_week(day, month, year);
    trigger.hour_flip();
}

void DateTime::dec_day() {
    aux::decrement_day<uint8_t>(day, days_in_month());
    day_of_week = aux::day_of_week(day, month, year);
    trigger.day_flip();
}

void DateTime::dec_month() {
    aux::decrement_month(month);
    day_of_week = aux::day_of_week(day, month, year);
    trigger.month_flip();
}

void DateTime::dec_year() {
    --year;
    _leap = aux::is_leap(year);
    _month_day_count = aux::month_day_count(_leap);
    day_of_week = aux::day_of_week(day, month, year);
    trigger.year_flip();
}

uint8_t DateTime::days_in_month() const {
    return _month_day_count[month];
}

void DateTime::resolve_febrary_collision() {
    if (day > days_in_month()) {
        day = days_in_month();
    }
}
