#include "clock.h"
#include "timer1.h"
#include "settings.h"
#include <Arduino.h>

bool Clock::tuning() const { 
    return _tuning;
}

void Clock::begin_tuning() { 
    _tuning = true;
}

void Clock::end_tuning() { 
    _tuning = false;
    _write_timeshift();
}

void Clock::inc_timeshift() {
    if (_timeshift < 65535L * Default::TIMER1_TICKS_PER_SECOND)
        _timeshift += 1;
}

void Clock::dec_timeshift() {
    if (_timeshift > -65536L * Default::TIMER1_TICKS_PER_SECOND)
        _timeshift -= 1;
}

void Clock::set_timeshift(int32_t shift) {
    _timeshift = shift;
}

int32_t Clock::timeshift() {
    return _timeshift;
}

void Clock::_write_timeshift() {
    uint16_t counts = Default::TIMER1_COUNTS;
    counts += _timeshift / Default::TIMER1_TICKS_PER_SECOND;
    _timeshift_ticks = 0;

    if (_timeshift % Default::TIMER1_TICKS_PER_SECOND)
        _timeshift_ticks_bound = abs(Default::TICK_SIZE * 1000L / (_timeshift % Default::TIMER1_TICKS_PER_SECOND));
    else
        _timeshift_ticks_bound = 0;

    Timer1::instance().shift(counts);

    Serial.print("ticks = ");
    Serial.print(_timeshift_ticks);
    Serial.print("; bound = ");
    Serial.println(_timeshift_ticks_bound);
}

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

    _state_change_trigger = _state_prev != _state_curr;
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
            _substate_curr = S_TIMER_TYPE2_EDIT_YEARS;
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
        case S_CLOCK2: {
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
        }

        case S_ALARM1:
        case S_ALARM2: {
            switch (_substate_curr) {
                case S_ALARM_TYPE1_EDIT_MINUTES:
                    _substate_curr = S_ALARM_TYPE1_EDIT_HOURS;
                    break;
                case S_ALARM_TYPE1_EDIT_HOURS:
                    _substate_curr = S_ALARM_TYPE1_EDIT_MINUTES;
                    break;
            }
            break;
        }

        case S_ALARM3: {
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
        }

        case S_TIMER1:
        case S_TIMER2: {
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
        }

        case S_TIMER3: {
            switch (_substate_curr) {
                case S_TIMER_TYPE2_EDIT_YEARS:
                    _substate_curr = S_TIMER_TYPE2_EDIT_MONTHS;
                    break;
                case S_TIMER_TYPE2_EDIT_MONTHS:
                    _substate_curr = S_TIMER_TYPE2_EDIT_DAYS;
                    break;
                case S_TIMER_TYPE2_EDIT_DAYS:
                    _substate_curr = S_TIMER_TYPE2_EDIT_YEARS;
                    break;
                
            }
            break;
        }
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

bool Clock::state_changed() const {
    return _state_change_trigger;
}

void Clock::flop_state_changed() {
    _state_change_trigger = false;
}

void Clock::primary_datetime_stop() {
    _primary_datetime_stop = true;
}

void Clock::primary_datetime_resume() {
    _primary_datetime_stop = false;
}

bool Clock::primary_datetime_stoped() const {
    return _primary_datetime_stop;   
}

void Clock::secondary_datetime_stop() {
    _secondary_datetime_stop = true;
}

void Clock::secondary_datetime_resume() {
    _secondary_datetime_stop = false;
}

bool Clock::secondary_datetime_stoped() const {
    return _secondary_datetime_stop;   
}

void Clock::tick(uint16_t tick_size) {
    if (_timeshift != 0 and _timeshift_ticks_bound) {
        if (++_timeshift_ticks == _timeshift_ticks_bound) {
            _timeshift_ticks = 0;

            if (_timeshift > 0)
                tick_size += 100;
            else if (_timeshift < 0)
                return;
        }
    }

    if (not _primary_datetime_stop)
        primary_datetime.tick(tick_size);
    
    if (not _secondary_datetime_stop)
        secondary_datetime.tick(tick_size);

    if (alaram1_datetime.on) {
        alaram1_datetime.tick1(primary_datetime, tick_size);

        if (alaram1_datetime.ringing) {
            alaram1_datetime.ringing = false;
            tone(6, 440, 100);
        }
    }

    if (alaram2_datetime.on) {
        alaram2_datetime.tick2(primary_datetime, tick_size);

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
        timer1_datetime.tick_countdown1(tick_size);

        if (timer1_datetime.ringing) {
            timer1_datetime.ringing = false;
            // timer1_datetime.stoppped = true;
            // timer1_datetime.ringing = false;
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

void Clock::flop_datetimes() {
    primary_datetime.trigger.flop();
    secondary_datetime.trigger.flop();
    alaram1_datetime.trigger.flop();
    alaram2_datetime.trigger.flop();
    alaram3_datetime.trigger.flop();
    timer1_datetime.trigger.flop();
    timer2_datetime.trigger.flop();
    timer3_datetime.trigger.flop();
    stopwatch.trigger.flop();
}
