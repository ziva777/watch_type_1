#ifndef _CLOCK_CONTROLLER_H_
#define _CLOCK_CONTROLLER_H_

#include "clock.h"
#include "display.h"
#include "button.h"

namespace Pin {
    const uint8_t BUTTON1 = 10;
    const uint8_t BUTTON2 = 11;
    const uint8_t BUTTON3 = 12;
    const uint8_t BUTTON4 = A8;

    const uint8_t CONTRAST = 2;
    const uint8_t BRIGHTNESS = 3;
};

class ClockController {
    public:
        ClockController(Clock &c, Display &d);

        void setup();
        void sync();

        void timer1_sync(); // 100 mS
        void timer2_sync(); // 144 mS

    private:
        const uint16_t TICK_SIZE = 100; // 100 mS

        Clock &_clock;
        Display &_display;

        Button _button1{Pin::BUTTON1};
        Button _button2{Pin::BUTTON2};
        Button _button3{Pin::BUTTON3};
        Button _button4{Pin::BUTTON4};

        void _inc_datetime(DateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _dec_datetime(DateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _inc_alarm(AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _dec_alarm(AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _inc_timer_type1(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _dec_timer_type1(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _inc_timer_type2(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _dec_timer_type2(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state);
        void _inc_timer_type3(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state, DateTime &curr_dt);
        void _dec_timer_type3(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state, DateTime &curr_dt);

        void _handle_clock_substate(Clock::CLOCK_SUBSTATES substate, DateTime &dt);
        void _handle_alarm_type1_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt);
        void _handle_alarm_type2_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt);
        void _handle_alarm_type3_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt);
        void _handle_timer_type1_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt);
        void _handle_timer_type2_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt);
        void _handle_timer_type3_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt, DateTime &curr_dt);
        void _handle_stopwatch(StopwatchTime &stopwatch);
};

#endif // _CLOCK_CONTROLLER_H_
