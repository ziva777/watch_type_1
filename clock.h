#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "datetime.h"

class Clock {
    public:
        enum CLOCK_STATES { 
            S_CLOCK1,
            S_CLOCK2,
            S_ALARM1,
            S_ALARM2,
            S_ALARM3,
            S_SIGNAL,
            S_TIMER1,
            S_TIMER2,
            S_TIMER3,
            S_STOPWATCH
        };

        DateTime primary_datetime;
        DateTime secondary_datetime;

        DateTime alaram1_datetime;
        DateTime alaram2_datetime;
        DateTime alaram3_datetime;

        DateTime timer1_datetime;
        DateTime timer2_datetime;
        DateTime timer3_datetime;

        bool hour_signal;

        DateTime stopwatch_datetime;

        void tick(uint16_t tick_size); // tick_size in mS

        void next_state();
        Clock::CLOCK_STATES state() const;
        
    private:
        Clock::CLOCK_STATES _state_curr{S_CLOCK1};
        Clock::CLOCK_STATES _state_prev{S_CLOCK1};
};

#endif // _CLOCK_H_
