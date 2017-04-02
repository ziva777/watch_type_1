#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <stdint.h>
#include "datetime_trigger.h"

class AlarmDateTime {
    public:
        uint8_t hour{0};
        uint8_t minute{0};
        uint8_t days[7]{0, 1, 1, 1, 1, 1, 0}; // 0 - sunday, 1 - monday, etc
        uint8_t day_pointer = 1; // 1 - monday

        bool on{false};
        bool stoppped{false};
    private:
};

class TimerDateTime {
    public:
        uint8_t origin_hour{0};
        uint8_t origin_minute{0};
        uint8_t origin_second{1};

        uint16_t origin_day{0};
        uint16_t origin_month{0};
        uint16_t origin_year{0};

        uint8_t hour{0};
        uint8_t minute{0};
        uint8_t second{0};
        uint16_t day{0};

        bool on{false};
        bool stoppped{false};
        bool leap{false};

        void normalize();
    private:
        uint8_t *_month_day_count;

        bool _is_leap(uint16_t y) const;
        uint8_t _day_of_week(uint16_t d, uint16_t m, uint16_t y) const;
};

class StopwatchTime {
    public:
        struct Stamp{
            uint8_t hour{0};
            uint8_t minute{0};
            uint8_t second{0};
            uint32_t ms{0};
            bool ready{false};
        };
        // Stamp stamp;

        uint8_t hour{0};
        uint8_t minute{0};
        uint8_t second{0};
        uint32_t ms{0};

        bool on{false};
        bool stoppped{false};

        StopwatchTimeTrigger trigger; // on change

        static const uint8_t STAMPS_COUNT {10};
        Stamp stamps[STAMPS_COUNT];
        uint8_t stamps_index {0};
        uint8_t stamps_index_to_show {0};

        void tick(uint16_t tick_size); // tick_size in mS
        void stamp_it(); // stamp current time in stamp var
        void free_stamp(); // reset stamp var
        void reset();

    private:
        uint8_t _hour_buff{0};
        uint8_t _minute_buff{0};
        uint8_t _second_buff{0};
        uint32_t _ms_buff{0};
};

class DateTime {
    public:
        uint8_t hour{0};
        uint8_t minute{0};
        uint8_t second{0};
        uint32_t ms{0};

        uint8_t day{01}; // 1 ... 31
        uint8_t month{01}; // 1 ... 12
        uint16_t year{2010}; // 2010 ... MAX(uint16_t)
        uint8_t day_of_week; // 0 - sunday, 1 - monday, etc

        DateTimeTrigger trigger; // on date and time change

        DateTime();

        void tick(uint16_t tick_size); // tick_size in mS
        void normalize();

        bool leap() const { return _leap; };

    private:
        bool _leap{false};
        uint8_t *_month_day_count;

        bool _is_leap(uint16_t y) const;
        uint8_t _day_of_week(uint16_t d, uint16_t m, uint16_t y) const;

        uint8_t _hour_buff{0};
        uint8_t _minute_buff{0};
        uint8_t _second_buff{0};
        uint8_t _day_buff{0};
        uint8_t _month_buff{0};
        uint16_t _year_buff{0};
};

#endif // _DATETIME_H_
