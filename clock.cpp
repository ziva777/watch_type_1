#include "clock.h"

void Clock::next_state() {
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

Clock::CLOCK_STATES Clock::state() const {
    return _state_curr;
}

void Clock::tick(uint16_t tick_size) {
    primary_datetime.tick(tick_size);
    secondary_datetime.tick(tick_size);
}
