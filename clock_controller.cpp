#include "clock_controller.h"

ClockController::ClockController(Clock &c, Display &d)
    : _clock(c), _display(d) {

}

void ClockController::setup(){
    _display.setup(Pin::CONTRAST, Pin::BRIGHTNESS);
    _display.set_contrast(55);
    _display.set_brightness(10);
    _display.print_clock_state(_clock);
    _display.print_datetime(_clock.primary_datetime);
}

void ClockController::sync(){
    static Clock::CLOCK_STATES clock_state_curr = _clock.state();
    static Clock::CLOCK_STATES clock_state_prev = _clock.state();

    _button1.update();
    _button2.update();
    _button3.update();
    _button4.update();

    if (_button1.pressed()) {
        _clock.next_state();
        clock_state_curr = _clock.state();

        // if (clock_state_prev != clock_state_curr) {
        //     _display.clear();
        // }

        _display.print_clock_state(_clock);

        if (_clock.state() == Clock::S_CLOCK1)
            _display.print_datetime(_clock.primary_datetime);
        else
        if (_clock.state() == Clock::S_CLOCK2)
            _display.print_datetime(_clock.secondary_datetime);
    }

    if (_clock.state() == Clock::S_CLOCK1) {
        if (_clock.primary_datetime.trigger.time_triggered())
            _display.print_time(_clock.primary_datetime);

        if (_clock.primary_datetime.trigger.date_triggered())
            _display.print_date(_clock.primary_datetime);
    } else
    if (_clock.state() == Clock::S_CLOCK2) {
        if (_clock.secondary_datetime.trigger.time_triggered())
            _display.print_time(_clock.secondary_datetime);

        if (_clock.secondary_datetime.trigger.date_triggered())
            _display.print_date(_clock.secondary_datetime);
    } else
    if (_clock.state() == Clock::S_ALARM1) {
        _display.print_alarm1(_clock.alaram1_datetime);
    } else
    if (_clock.state() == Clock::S_ALARM2) {
        _display.print_alarm2(_clock.alaram2_datetime);
    } else
    if (_clock.state() == Clock::S_ALARM3) {
        _display.print_alarm3(_clock.alaram3_datetime);
    } else
    if (_clock.state() == Clock::S_SIGNAL) {
        _display.print_signal(_clock.hour_signal);
    } else
    if (_clock.state() == Clock::S_TIMER1) {
        _display.print_timer1(_clock.timer1_datetime);
    } else
    if (_clock.state() == Clock::S_TIMER2) {
        _display.print_timer2(_clock.timer2_datetime);
    } else
    if (_clock.state() == Clock::S_TIMER3) {
        _display.print_timer3(_clock.timer3_datetime);
    } else
    if (_clock.state() == Clock::S_STOPWATCH) {
        if (clock_state_prev != clock_state_curr) {
            _display.print_stopwatch(_clock.stopwatch);
        } else {
            if (_button2.pressed()) {
                if (_clock.stopwatch.on) {
                    _clock.stopwatch.stoppped = !_clock.stopwatch.stoppped;
                } else {
                    _clock.stopwatch.on = true;
                    _clock.stopwatch.stoppped = false;
                }
            } else
            if (_button3.pressed()) {
                if (_clock.stopwatch.on and not _clock.stopwatch.stoppped) {
                    _clock.stopwatch.stamp_it();
                }
            } else
            if (_button3.pressed_hard()) {
                if (_clock.stopwatch.on) {
                    if (_clock.stopwatch.stoppped) {
                        _clock.stopwatch.on = false;   
                        _clock.stopwatch.stoppped = true;
                        _clock.stopwatch.hour = 0;
                        _clock.stopwatch.minute = 0;
                        _clock.stopwatch.second = 0;
                        _clock.stopwatch.ms = 0;
                        _clock.stopwatch.free_stamp();
                    }
                }
            }

            if (_clock.stopwatch.trigger.time_triggered())
                _display.print_stopwatch(_clock.stopwatch);
        }
        // if (_button2.pressed()) {
        //     if (_clock.stopwatch.on) {
        //         _clock.stopwatch.stoppped = !_clock.stopwatch.stoppped;
        //     } else {
        //         _clock.stopwatch.on = true;   
        //         _clock.stopwatch.stoppped = false;   
        //     }
        // } else
        // if (_button3.pressed_hard()) {
        //     _clock.stopwatch.on = false;   
        //     _clock.stopwatch.stoppped = true;
        //     _clock.stopwatch.hour = 0;
        //     _clock.stopwatch.minute = 0;
        //     _clock.stopwatch.second = 0;
        //     _clock.stopwatch.ms = 0;
        // }
        
    }

    clock_state_prev = clock_state_curr;

    _button1.flop();
    _button2.flop();
    _button3.flop();
    _button4.flop();
}

void ClockController::timer1_sync() {
    _clock.tick(TICK_SIZE);
}

void ClockController::timer2_sync() {
    static uint16_t counter = 0;
    static uint16_t brightness = 0;

    if (counter % 100 == 0) {
        _display.set_brightness(brightness);
        ++brightness;
        brightness %= 255;
    }

    ++counter;
}
