#include "clock.h"
#include <Arduino.h>

void Clock::next_state() {
    _state_prev = _state_curr;

    switch (_state_curr) {
        case S_CLOCK1:
            _state_curr = S_CLOCK2;
            break;
        case S_CLOCK2:
            _state_curr = S_ALARM1;
            break;
        case S_ALARM1:
            _state_curr = S_ALARM2;
            break;
        case S_ALARM2:
            _state_curr = S_ALARM3;
            break;
        case S_ALARM3:
            _state_curr = S_SIGNAL;
            break;
        case S_SIGNAL:
            _state_curr = S_TIMER1;
            break;
        case S_TIMER1:
            _state_curr = S_TIMER2;
            break;
        case S_TIMER2:
            _state_curr = S_TIMER3;
            break;
        case S_TIMER3:
            _state_curr = S_STOPWATCH;
            break;
        case S_STOPWATCH:
            _state_curr = S_CLOCK1;
            break;
    }
}

void Clock::begin_substate() {
    _substate_prev = _substate_curr;
    CLOCK_STATES s = state();

    switch (s) {
        case S_CLOCK1:
        case S_CLOCK2:
            _substate_curr = S_CLOCK_EDIT_SECONDS;
            break;

        case S_ALARM1:
        case S_ALARM2:
            _substate_curr = S_ALARM_TYPE1_EDIT_MINUTES;
            break;

        case S_ALARM3:
            _substate_curr = S_ALARM_TYPE2_EDIT_MINUTES;
            break;

        case S_TIMER1:
        case S_TIMER2:
            _substate_curr = S_TIMER_TYPE1_EDIT_SECONDS;
            break;

        case S_TIMER3:
            _substate_curr = S_TIMER_TYPE2_EDIT_DAYS;
            break;

        default:
            _substate_curr = S_NONE;
            break;
    }
}

void Clock::next_substate() {
    _substate_prev = _substate_curr;
    CLOCK_STATES s = state();

    switch (s) {
        case S_CLOCK1:
        case S_CLOCK2:
            switch (_substate_curr) {
                case S_CLOCK_EDIT_SECONDS:
                    _substate_curr = S_CLOCK_EDIT_MINUTES;
                    break;
                case S_CLOCK_EDIT_MINUTES:
                    _substate_curr = S_CLOCK_EDIT_HOURS;
                    break;
                case S_CLOCK_EDIT_HOURS:
                    _substate_curr = S_CLOCK_EDIT_YEARS;
                    break;
                case S_CLOCK_EDIT_YEARS:
                    _substate_curr = S_CLOCK_EDIT_MONTHS;
                    break;
                case S_CLOCK_EDIT_MONTHS:
                    _substate_curr = S_CLOCK_EDIT_DAYS;
                    break;
                case S_CLOCK_EDIT_DAYS:
                    _substate_curr = S_CLOCK_EDIT_SECONDS;
                    break;
            }
            break;

        case S_ALARM1:
        case S_ALARM2:
            switch (_substate_curr) {
                case S_ALARM_TYPE1_EDIT_MINUTES:
                    _substate_curr = S_ALARM_TYPE1_EDIT_HOURS;
                    break;
                case S_ALARM_TYPE1_EDIT_HOURS:
                    _substate_curr = S_ALARM_TYPE1_EDIT_MINUTES;
                    break;
            }
            break;

        case S_ALARM3:
            switch (_substate_curr) {
                case S_ALARM_TYPE2_EDIT_MINUTES:
                    _substate_curr = S_ALARM_TYPE2_EDIT_HOURS;
                    break;
                case S_ALARM_TYPE2_EDIT_HOURS:
                    _substate_curr = S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK;
                    break;
                case S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK:
                    _substate_curr = S_ALARM_TYPE2_EDIT_MINUTES;
                    break;
            }
            break;

        case S_TIMER1:
        case S_TIMER2:
            switch (_substate_curr) {
                case S_TIMER_TYPE1_EDIT_SECONDS:
                    _substate_curr = S_TIMER_TYPE1_EDIT_MINUTES;
                    break;
                case S_TIMER_TYPE1_EDIT_MINUTES:
                    _substate_curr = S_TIMER_TYPE1_EDIT_HOURS;
                    break;
                case S_TIMER_TYPE1_EDIT_HOURS:
                    _substate_curr = S_TIMER_TYPE1_EDIT_SECONDS;
                    break;
            }
            break;

        case S_TIMER3:
            switch (_substate_curr) {
                case S_TIMER_TYPE2_EDIT_DAYS:
                    _substate_curr = S_TIMER_TYPE2_EDIT_MONTHS;
                    break;
                case S_TIMER_TYPE2_EDIT_MONTHS:
                    _substate_curr = S_TIMER_TYPE2_EDIT_YEARS;
                    break;
                case S_TIMER_TYPE2_EDIT_YEARS:
                    _substate_curr = S_TIMER_TYPE2_EDIT_DAYS;
                    break;
            }
            break;
    }
}

void Clock::end_substate() {
    _substate_prev = _substate_curr;
    _substate_curr = S_NONE;
}

Clock::CLOCK_STATES Clock::state() const {
    return _state_curr;
}

Clock::CLOCK_STATES Clock::state_prev() const {
    return _state_prev;
}

Clock::CLOCK_SUBSTATES Clock::substate() const {
    return _substate_curr;
}

Clock::CLOCK_SUBSTATES Clock::substate_prev() const {
    return _substate_prev;
}

void Clock::tick(uint16_t tick_size) {
    primary_datetime.tick(tick_size);
    secondary_datetime.tick(tick_size);

    if (alaram1_datetime.on) {
        alaram1_datetime.tick(primary_datetime, tick_size);

        if (alaram1_datetime.ringing) {
            alaram1_datetime.ringing = false;
            tone(6, 440, 100);
        }
    }

    if (alaram2_datetime.on) {
        alaram2_datetime.tick(primary_datetime, tick_size);

        if (alaram2_datetime.ringing) {
            alaram2_datetime.ringing = false;
            alaram2_datetime.on = false;
            tone(6, 440 * 2, 100);
        }
    }

    if (alaram3_datetime.on) {
        alaram3_datetime.tick3(primary_datetime, tick_size);

        if (alaram3_datetime.ringing) {
            alaram3_datetime.ringing = false;
            tone(6, 440 * 3, 100);
        }
    }

    if (primary_datetime.minute == 0) {
        if (substate() == S_NONE)   
            if (hour_signal and not hour_signal_done) {
                hour_signal_done = true;
                tone(6, 440 * 4, 100);
            }
    } else 
    if (primary_datetime.minute == 59)
        hour_signal_done = false;

    if (stopwatch.on and !stopwatch.stoppped) {
        stopwatch.tick(tick_size);
    }

    if (timer1_datetime.on and !timer1_datetime.stoppped) {
        timer1_datetime.tick_countdown(tick_size);

        if (timer1_datetime.ringing) {
            timer1_datetime.ringing = false;
            tone(6, 440 * 5, 100);
        }
    }

    if (timer2_datetime.on and !timer2_datetime.stoppped) {
        timer2_datetime.tick_countdown2(primary_datetime, tick_size);

        if (timer2_datetime.ringing) {
            timer2_datetime.on = false;
            timer2_datetime.stoppped = true;
            timer2_datetime.ringing = false;
            tone(6, 440 * 5, 150);
        }
    }

    if (timer3_datetime.on and !timer3_datetime.stoppped) {
        timer3_datetime.tick_countdown3(primary_datetime, tick_size);

        if (timer3_datetime.ringing) {
            timer3_datetime.on = false;
            timer3_datetime.stoppped = true;
            timer3_datetime.ringing = false;
            tone(6, 440 * 5, 150);
        }
    }
}
