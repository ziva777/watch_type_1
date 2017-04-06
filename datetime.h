#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <stdint.h>
#include "datetime_trigger.h"
#include "settings.h"

class DateTime;

class AlarmDateTime {
    public:
        uint8_t hour {0};
        uint8_t minute {0};
        uint8_t days[7] {0, 1, 1, 1, 1, 1, 0}; // 0 - sunday, 1 - monday, etc
        uint8_t day_pointer {1}; // 1 - monday

        bool on {false};
        bool no_lock {true};
        bool ringing {false};

        void inc_minute();
        void inc_hour();
        void dec_minute();
        void dec_hour();

        void move_day_pointer();
        void switch_day();

        void tick1(const DateTime &dt, uint16_t tick_size);
        void tick2(const DateTime &dt, uint16_t tick_size);
        void tick3(const DateTime &dt, uint16_t tick_size);

    private:
};

class TimerDateTime {
    public:
        uint8_t origin_hour {0};
        uint8_t origin_minute {0};
        uint8_t origin_second {0};

        uint16_t origin_day {Default::PrimaryClock::DAY};
        uint16_t origin_month {Default::PrimaryClock::MONTH};
        uint16_t origin_year {Default::PrimaryClock::YEAR};

        uint8_t hour {0};
        uint8_t minute {0};
        uint8_t second {0};
        uint32_t ms {0};
        uint16_t day {0};

        bool on {false};
        bool stoppped {true};
        bool ringing {false};
        bool leap {false};

        void normalize();

        void inc_second();
        void inc_minute();
        void inc_hour();

        void dec_second();
        void dec_minute();
        void dec_hour();

        void inc_origin_second();
        void inc_origin_minute();
        void inc_origin_hour();
        void inc_origin_day();
        void inc_origin_month();
        void inc_origin_year();
        void inc_origin_year(uint16_t year_curr, uint8_t year_max_offset);

        void dec_origin_second();
        void dec_origin_minute();
        void dec_origin_hour();
        void dec_origin_day();
        void dec_origin_month();
        void dec_origin_year();
        void dec_origin_year(uint16_t year_curr, uint8_t year_max_offset);

        void launch_countdown1();
        void launch_countdown2(DateTime &dt);
        void launch_countdown3(DateTime &dt);
        void tick_countdown1(uint16_t tick_size);
        void tick_countdown2(const DateTime &dt, uint16_t tick_size);
        void tick_countdown3(const DateTime &dt, uint16_t tick_size);
        void reset_countdown();

        void resolve_febrary_collision();
        uint8_t days_in_month() const;

    private:
        uint8_t *_month_day_count;

        uint32_t _get_total_origin_sec() const;
        void _set_total_origin_sec(uint32_t total_sec);
        uint32_t _get_total_sec() const;
        void _set_total_sec(uint32_t total_sec);
};

class StopwatchTime {
    public:
        struct Stamp{
            uint8_t hour {0};
            uint8_t minute {0};
            uint8_t second {0};
            uint32_t ms {0};
            bool ready {false};
        };

        uint8_t hour {0};
        uint8_t minute {0};
        uint8_t second {0};
        uint32_t ms {0};

        bool on {false};
        bool stoppped {false};

        StopwatchTimeTrigger trigger; // on change

        // Lap counter
        static const uint8_t STAMPS_COUNT {Default::STOPWATCH_LAP_COUNT};
        Stamp stamps[STAMPS_COUNT];
        uint8_t stamps_index {0};
        uint8_t stamps_index_to_show {0};

        void tick(uint16_t tick_size); // tick_size in mS
        void stamp_it(); // stamp current time in stamps var
        void free_stamp(); // reset stamps var
        void reset();

    private:
        uint8_t _hour_buff {0};
        uint8_t _minute_buff {0};
        uint8_t _second_buff {0};
        uint32_t _ms_buff {0};
};

class DateTime {
    public:
        uint8_t hour {Default::PrimaryClock::HOUR};
        uint8_t minute {Default::PrimaryClock::MINUTE};
        uint8_t second {Default::PrimaryClock::SECOND};
        uint32_t ms {Default::PrimaryClock::MS};
        uint8_t day {Default::PrimaryClock::DAY}; // 1 ... 31
        uint8_t month {Default::PrimaryClock::MONTH}; // 1 ... 12
        uint16_t year {Default::PrimaryClock::YEAR}; // 0 ... MAX(uint16_t)
        uint8_t day_of_week {Default::PrimaryClock::DAY_OF_WEEK}; // 0 - sunday, 1 - monday, etc

        DateTimeTrigger trigger; // on date and time change

        DateTime();

        void tick(uint16_t  tick_size); // tick_size in mS
        uint8_t days_in_month() const;
        bool leap() const { return _leap; };

        void inc_second();
        void inc_minute();
        void inc_hour();
        void inc_day();
        void inc_month();
        void inc_year();

        void dec_second();
        void dec_minute();
        void dec_hour();
        void dec_day();
        void dec_month();
        void dec_year();

        void resolve_febrary_collision();

    private:
        bool _leap {false};
        uint8_t *_month_day_count;

        uint8_t _hour_buff {0};
        uint8_t _minute_buff {0};
        uint8_t _second_buff {0};
        uint8_t _day_buff {0};
        uint8_t _month_buff {0};
        uint16_t _year_buff {0};
};

#endif // _DATETIME_H_
