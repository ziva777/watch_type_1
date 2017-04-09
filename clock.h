#ifndef _CLOCK_H_
#define _CLOCK_H_

// Clock i.e. FSM + values

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

        enum CLOCK_SUBSTATES {
            S_NONE,
            
            // primary_datetime, secondary_datetime
            S_CLOCK_EDIT_SECONDS,
            S_CLOCK_EDIT_MINUTES,
            S_CLOCK_EDIT_HOURS,
            S_CLOCK_EDIT_DAYS,
            S_CLOCK_EDIT_MONTHS,
            S_CLOCK_EDIT_YEARS,

            // alaram1_datetime, alaram2_datetime
            S_ALARM_TYPE1_EDIT_MINUTES,
            S_ALARM_TYPE1_EDIT_HOURS,

            // alaram3_datetime
            S_ALARM_TYPE2_EDIT_MINUTES,
            S_ALARM_TYPE2_EDIT_HOURS,
            S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK,

            // timer1_datetime, timer2_datetime
            S_TIMER_TYPE1_EDIT_SECONDS,
            S_TIMER_TYPE1_EDIT_MINUTES,
            S_TIMER_TYPE1_EDIT_HOURS,

            // timer3_datetime
            S_TIMER_TYPE2_EDIT_DAYS,
            S_TIMER_TYPE2_EDIT_MONTHS,
            S_TIMER_TYPE2_EDIT_YEARS
        };

        DateTime primary_datetime;
        DateTime secondary_datetime;

        AlarmDateTime alaram1_datetime;
        AlarmDateTime alaram2_datetime;
        AlarmDateTime alaram3_datetime;

        TimerDateTime timer1_datetime;
        TimerDateTime timer2_datetime;
        TimerDateTime timer3_datetime;

        StopwatchTime stopwatch;

        bool hour_signal;
        bool hour_signal_done;

        void tick(uint16_t tick_size); // tick_size in mS
        void flop_datetimes();

        void next_state();
        Clock::CLOCK_STATES state() const;
        Clock::CLOCK_STATES state_prev() const;
        bool state_changed() const;
        void flop_state_changed();

        void begin_substate();
        void next_substate();
        void end_substate();
        Clock::CLOCK_SUBSTATES substate() const;
        Clock::CLOCK_SUBSTATES substate_prev() const;

        void primary_datetime_stop();
        void primary_datetime_resume();
        bool primary_datetime_stoped() const;

        void secondary_datetime_stop();
        void secondary_datetime_resume();
        bool secondary_datetime_stoped() const;

        bool tuning() const;
        void begin_tuning();
        void end_tuning();

        void inc_timeshift();
        void dec_timeshift();
        void set_timeshift(int32_t shift); // uS per second
        int32_t timeshift();
        
    private:
        Clock::CLOCK_STATES _state_curr{S_CLOCK1};
        Clock::CLOCK_STATES _state_prev{S_CLOCK1};

        Clock::CLOCK_SUBSTATES _substate_curr{S_NONE};
        Clock::CLOCK_SUBSTATES _substate_prev{S_NONE};

        bool _state_change_trigger {false};
        volatile bool _primary_datetime_stop {false};
        volatile bool _secondary_datetime_stop {false};
        bool _tuning {false};

        int32_t _timeshift {0};
        int32_t _timeshift_ticks {0};
        int32_t _timeshift_ticks_bound {0};

        void _write_timeshift();
};

#endif // _CLOCK_H_
