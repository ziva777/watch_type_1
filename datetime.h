#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <stdint.h>
#include "datetime_trigger.h"

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
