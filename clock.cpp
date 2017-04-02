#include "clock.h"

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
            _substate_curr = S_CLOCK1_EDIT_SECONDS;
            break;

        case S_ALARM1:
        case S_ALARM2:
            _substate_curr = S_ALARM_EDIT_MINUTES;
            break;

        case S_ALARM3:
            _substate_curr = S_ALARM2_EDIT_MINUTES;
            break;

        case S_TIMER1:
        case S_TIMER2:
            _substate_curr = S_TIMER_EDIT_SECONDS;
            break;

        case S_TIMER3:
            _substate_curr = S_TIMER2_EDIT_DAYS;
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
                case S_CLOCK1_EDIT_SECONDS:
                    _substate_curr = S_CLOCK1_EDIT_MINUTES;
                    break;
                case S_CLOCK1_EDIT_MINUTES:
                    _substate_curr = S_CLOCK1_EDIT_HOURS;
                    break;
                case S_CLOCK1_EDIT_HOURS:
                    _substate_curr = S_CLOCK1_EDIT_DAYS;
                    break;
                case S_CLOCK1_EDIT_DAYS:
                    _substate_curr = S_CLOCK1_EDIT_MONTHS;
                    break;
                case S_CLOCK1_EDIT_MONTHS:
                    _substate_curr = S_CLOCK1_EDIT_YEARS;
                    break;
                case S_CLOCK1_EDIT_YEARS:
                    _substate_curr = S_CLOCK1_EDIT_SECONDS;
                    break;
            }
            break;

        case S_ALARM1:
        case S_ALARM2:
            switch (_substate_curr) {
                case S_ALARM_EDIT_MINUTES:
                    _substate_curr = S_ALARM_EDIT_HOURS;
                    break;
                case S_ALARM_EDIT_HOURS:
                    _substate_curr = S_ALARM_EDIT_MINUTES;
                    break;
            }
            break;

        case S_ALARM3:
            switch (_substate_curr) {
                case S_ALARM2_EDIT_MINUTES:
                    _substate_curr = S_ALARM2_EDIT_HOURS;
                    break;
                case S_ALARM2_EDIT_HOURS:
                    _substate_curr = S_ALARM2_EDIT_DAYS_OF_WEEK;
                    break;
                case S_ALARM2_EDIT_DAYS_OF_WEEK:
                    _substate_curr = S_ALARM2_EDIT_MINUTES;
                    break;
            }
            break;

        case S_TIMER1:
        case S_TIMER2:
            switch (_substate_curr) {
                case S_TIMER_EDIT_SECONDS:
                    _substate_curr = S_TIMER_EDIT_MINUTES;
                    break;
                case S_TIMER_EDIT_MINUTES:
                    _substate_curr = S_TIMER_EDIT_HOURS;
                    break;
                case S_TIMER_EDIT_HOURS:
                    _substate_curr = S_TIMER_EDIT_SECONDS;
                    break;
            }
            break;

        case S_TIMER3:
            switch (_substate_curr) {
                case S_TIMER2_EDIT_DAYS:
                    _substate_curr = S_TIMER2_EDIT_MONTHS;
                    break;
                case S_TIMER2_EDIT_MONTHS:
                    _substate_curr = S_TIMER2_EDIT_YEARS;
                    break;
                case S_TIMER2_EDIT_YEARS:
                    _substate_curr = S_TIMER2_EDIT_DAYS;
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

    if (stopwatch.on and !stopwatch.stoppped) {
        stopwatch.tick(tick_size);
    }
}
