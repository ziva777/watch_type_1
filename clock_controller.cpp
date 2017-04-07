#include "clock_controller.h"
#include <Arduino.h>

ClockController::ClockController(Clock &c, Display &d)
    : _clock(c), _display(d) 
{

}

void ClockController::setup() {
    _display.setup(Pin::CONTRAST, Pin::BRIGHTNESS);
    _display.set_contrast(Default::CONTRAST);
    _display.set_brightness(Default::BRIGHTNESS);
    _display.print_clock_state(_clock);
    _display.print_clock_datetime(_clock.primary_datetime);
}

void ClockController::_inc_datetime(DateTime &dt, Clock::CLOCK_SUBSTATES state, bool stopped) {
    if (state == Clock::S_CLOCK_EDIT_SECONDS) {
        if (not stopped)
            dt.align_second_up();
    } else if (state == Clock::S_CLOCK_EDIT_MINUTES) {
        dt.inc_minute();
    } else if (state == Clock::S_CLOCK_EDIT_HOURS) {
        dt.inc_hour();
    } else if (state == Clock::S_CLOCK_EDIT_DAYS) {
        dt.inc_day();
    } else if (state == Clock::S_CLOCK_EDIT_MONTHS) {
        dt.inc_month();
    } else if (state == Clock::S_CLOCK_EDIT_YEARS) {
        dt.inc_year();
    }

    dt.resolve_febrary_collision();
}

void ClockController::_dec_datetime(DateTime &dt, Clock::CLOCK_SUBSTATES state, bool stopped) {
    if (state == Clock::S_CLOCK_EDIT_SECONDS) {
        if (not stopped)
            dt.align_second_down();
    } else if (state == Clock::S_CLOCK_EDIT_MINUTES) {
        dt.dec_minute();
    } else if (state == Clock::S_CLOCK_EDIT_HOURS) {
        dt.dec_hour();
    } else if (state == Clock::S_CLOCK_EDIT_DAYS) {
        dt.dec_day();
    } else if (state == Clock::S_CLOCK_EDIT_MONTHS) {
        dt.dec_month();
    } else if (state == Clock::S_CLOCK_EDIT_YEARS) {
        dt.dec_year();
    }

    dt.resolve_febrary_collision();
}

void ClockController::_inc_alarm(AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_ALARM_TYPE1_EDIT_MINUTES or 
        state == Clock::S_ALARM_TYPE2_EDIT_MINUTES)
        dt.inc_minute();
    else 
    if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS or 
        state == Clock::S_ALARM_TYPE2_EDIT_HOURS)
        dt.inc_hour();
    else
    if (state == Clock::S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK)
        dt.switch_day();
}

void ClockController::_dec_alarm(AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_ALARM_TYPE1_EDIT_MINUTES or 
        state == Clock::S_ALARM_TYPE2_EDIT_MINUTES)
        dt.dec_minute();
    else 
    if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS or 
        state == Clock::S_ALARM_TYPE2_EDIT_HOURS)
        dt.dec_hour();
    else 
    if (state == Clock::S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK)
        dt.move_day_pointer();
}

void ClockController::_inc_timer_type1(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS)
        dt.inc_second();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
        dt.inc_minute();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
        dt.inc_hour();
}

void ClockController::_dec_timer_type1(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS)
        dt.dec_second();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
        dt.dec_minute();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
        dt.dec_hour();
}

void ClockController::_inc_timer_type2(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS)
        dt.inc_origin_second();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
        dt.inc_origin_minute();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
        dt.inc_origin_hour();
}

void ClockController::_dec_timer_type2(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS)
        dt.dec_origin_second();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES)
        dt.dec_origin_minute();
    else if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
        dt.dec_origin_hour();
}

void ClockController::_inc_timer_type3(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state, DateTime &curr_dt) {
    dt.normalize();

    if (state == Clock::S_TIMER_TYPE2_EDIT_DAYS)
        dt.inc_origin_day();
    else if (state == Clock::S_TIMER_TYPE2_EDIT_MONTHS)
        dt.inc_origin_month();
    else if (state == Clock::S_TIMER_TYPE2_EDIT_YEARS)
        dt.inc_origin_year(curr_dt.year, Default::TIMER3_MAX_FUTURE_YEAR_OFFSET);
}

void ClockController::_dec_timer_type3(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state, DateTime &curr_dt) {
    dt.normalize();

    if (state == Clock::S_TIMER_TYPE2_EDIT_DAYS)
        dt.dec_origin_day();
    else if (state == Clock::S_TIMER_TYPE2_EDIT_MONTHS)
        dt.dec_origin_month();
    else if (state == Clock::S_TIMER_TYPE2_EDIT_YEARS)
        dt.dec_origin_year(curr_dt.year, Default::TIMER3_MAX_FUTURE_YEAR_OFFSET);
}

void ClockController::_handle_clock_substate(Clock::CLOCK_SUBSTATES substate, DateTime &dt) {
    if (substate == Clock::S_NONE) {
        if (dt.trigger.time_triggered())
            _display.print_clock_time(dt);

        if (dt.trigger.date_triggered())
            _display.print_clock_date(dt);
    } else {
        _display.print_edit_clock(dt, substate);
    }
}

void ClockController::_handle_alarm_type1_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt) {
    if (substate == Clock::S_NONE){
        if (dt.trigger.triggered())
            _display.print_alarm_type1(dt);
    } else {
        _display.print_edit_alarm_type1(dt, substate);
    }
}

void ClockController::_handle_alarm_type2_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt) {
    if (substate == Clock::S_NONE) {
        if (dt.trigger.triggered())
            _display.print_alarm_type2(dt);
    } else {
        _display.print_edit_alarm_type2(dt, substate);
    }
}

void ClockController::_handle_alarm_type3_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt) {
    if (substate == Clock::S_NONE) {
        if (dt.trigger.triggered())
            _display.print_alarm_type3(dt);
    } else {
        _display.print_edit_alarm_type3(dt, substate);
    }
}

void ClockController::_handle_timer_type1_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt) {
    if (substate == Clock::S_NONE) {
        if (dt.trigger.triggered())
            _display.print_timer_type1(dt);
    } else {
        _display.print_edit_timer_type1(dt, substate);   
    }
}

void ClockController::_handle_timer_type2_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt) {
    if (substate == Clock::S_NONE) {
        if (dt.trigger.triggered())
            _display.print_timer_type2(dt);
    } else {
        _display.print_edit_timer_type2(dt, substate);   
    }
}

void ClockController::_handle_timer_type3_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt, DateTime &curr_dt) {
    if (substate == Clock::S_NONE) {
        if (dt.trigger.triggered())
            _display.print_timer_type3(dt);
    } else {
        _display.print_edit_timer_type3(dt, substate);   
    }
}

void ClockController::_handle_stopwatch(StopwatchTime &stopwatch) {
    if (stopwatch.trigger.time_triggered())
        _display.print_stopwatch(stopwatch);
}

void ClockController::sync(){
    _button1.update();
    _button2.update();
    _button3.update();
    _button4.update();

    _process_FSM_logic();
    _process_display_logic();
    _process_stop_logic();

    _clock.flop_state_changed();

    _button1.flop();
    _button2.flop();
    _button3.flop();
    _button4.flop();
}

void ClockController::_process_FSM_logic() {
    Clock::CLOCK_STATES state = _clock.state();
    Clock::CLOCK_SUBSTATES substate = _clock.substate();

    switch (state) {
        case Clock::S_CLOCK1:
        case Clock::S_CLOCK2: {
            if (substate == Clock::S_NONE) {
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                } else
                if (_button2.pressed_hard()) {
                    _clock.begin_substate();
                } else 
                if (_button3.pressed() or _button3.pressed_repeat()) {
                    _display.set_contrast((_display.contrast() + 10) % 100);
                } else 
                if (_button4.pressed() or _button4.pressed_repeat()) {
                    _display.set_brightness((_display.brightness() + 5) % 255);
                }
            } else {
                DateTime &dt = (state == Clock::S_CLOCK1 ? _clock.primary_datetime : 
                                                           _clock.secondary_datetime);
                bool stopped = (state == Clock::S_CLOCK1 ? _clock.primary_datetime_stoped() :
                                                           _clock.secondary_datetime_stoped());

                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
                    _display.print_clock_datetime(dt);
                }

                if (_button3.pressed() or _button3.pressed_repeat()) 
                    _dec_datetime(dt, substate, stopped);

                if (_button4.pressed() or _button4.pressed_repeat()) 
                    _inc_datetime(dt, substate, stopped);
            } 
            break;
        }
        case Clock::S_ALARM1:
        case Clock::S_ALARM2:
        case Clock::S_ALARM3: {
            AlarmDateTime &dt = 
                (state == Clock::S_ALARM1 ? _clock.alaram1_datetime : 
                            (state == Clock::S_ALARM2 ? _clock.alaram2_datetime :
                                                        _clock.alaram3_datetime));

            if (substate == Clock::S_NONE) {
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                } else
                if (_button2.pressed_hard())
                    _clock.begin_substate();
                else 
                if (_button2.pressed()) {
                    dt.on = !dt.on;
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();

                    if (state == Clock::S_ALARM1)
                        _display.print_alarm_type1(dt);
                    else if (state == Clock::S_ALARM2)
                        _display.print_alarm_type2(dt);
                    else if (state == Clock::S_ALARM3)
                        _display.print_alarm_type3(dt);
                }

                if (_button3.pressed() or _button3.pressed_repeat())
                    _dec_alarm(dt, substate);

                if (_button4.pressed() or _button4.pressed_repeat())
                    _inc_alarm(dt, substate);
            }
            break;
        }
        case Clock::S_SIGNAL: {
            if (_button1.pressed()) {
                _clock.next_state();
                _display.print_clock_state(_clock);
            } else
            if (_button2.pressed())
                _clock.hour_signal = not _clock.hour_signal;
            break;
        }
        case Clock::S_TIMER1: {
            TimerDateTime &dt = _clock.timer1_datetime;

            if (substate == Clock::S_NONE) {
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                } else
                if (_button2.pressed_hard()) {
                    _clock.begin_substate();
                    dt.on = false;
                    dt.stoppped = true;
                } else 
                if (_button2.pressed()) {
                    if (dt.stoppped and not dt.on) {
                        dt.on = true;
                        dt.stoppped = false;
                        dt.launch_countdown1();
                    } else
                    if (not dt.stoppped and dt.on) {
                        dt.stoppped = true;
                    } else 
                    if (dt.stoppped and dt.on) {
                        dt.stoppped = false;
                    }   
                } else 
                if (_button3.pressed_hard()) {
                    if (dt.on) {
                        dt.on = false;
                        dt.stoppped = true;
                    }

                    dt.reset_countdown();
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
                    _display.print_timer_type1(dt);
                }

                if (_button3.pressed() or _button3.pressed_repeat())
                    _dec_timer_type1(dt, substate);

                if (_button4.pressed() or _button4.pressed_repeat())
                    _inc_timer_type1(dt, substate);
            }
            break;
        }
        case Clock::S_TIMER2: {
            TimerDateTime &dt = _clock.timer2_datetime;

            if (substate == Clock::S_NONE) {
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                } else
                if (_button2.pressed_hard()) {
                    _clock.begin_substate();
                    dt.on = false;
                    dt.stoppped = true;
                } else 
                if (_button2.pressed()) {
                    if (dt.stoppped and not dt.on) {
                        dt.on = true;
                        dt.stoppped = false;
                        dt.launch_countdown2(_clock.primary_datetime);
                    } else
                    if (not dt.stoppped and dt.on) {
                        dt.stoppped = true;
                    } else 
                    if (dt.stoppped and dt.on) {
                        dt.stoppped = false;
                    }   
                } else 
                if (_button3.pressed_hard()) {
                    if (dt.on) {
                        dt.on = false;
                        dt.stoppped = true;
                    }

                    dt.reset_countdown();
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
                    _display.print_timer_type2(dt);
                }

                if (_button3.pressed() or _button3.pressed_repeat())
                    _dec_timer_type2(dt, substate);

                if (_button4.pressed() or _button4.pressed_repeat())
                    _inc_timer_type2(dt, substate);
            }
            break;
        }
        case Clock::S_TIMER3: {
            TimerDateTime &dt = _clock.timer3_datetime;

            if (substate == Clock::S_NONE) {
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                } else
                if (_button2.pressed_hard()) {
                    _clock.begin_substate();
                    dt.on = false;
                    dt.stoppped = true;
                } else 
                if (_button2.pressed()) {
                    if (dt.stoppped and not dt.on) {
                        dt.on = true;
                        dt.stoppped = false;
                        dt.launch_countdown3(_clock.primary_datetime);
                    } else
                    if (not dt.stoppped and dt.on) {
                        dt.stoppped = true;
                    } else 
                    if (dt.stoppped and dt.on) {
                        dt.stoppped = false;
                    }   
                } else 
                if (_button3.pressed_hard()) {
                    if (dt.on) {
                        dt.on = false;
                        dt.stoppped = true;
                    }

                    dt.reset_countdown();
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
                    _display.print_timer_type3(dt);
                }

                if (_button3.pressed() or _button3.pressed_repeat())
                    _dec_timer_type3(dt, substate, _clock.primary_datetime);

                if (_button4.pressed() or _button4.pressed_repeat())
                    _inc_timer_type3(dt, substate, _clock.primary_datetime);
            }
            break;
        }
        case Clock::S_STOPWATCH: {
            if (_button1.pressed()) {
                _clock.next_state();
                _display.print_clock_state(_clock);
            } else
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
                } else 
                if (_clock.stopwatch.on and _clock.stopwatch.stoppped) {
                    if (_clock.stopwatch.stamps_index_to_show)
                        --_clock.stopwatch.stamps_index_to_show;
                }
            } else
            if (_button3.pressed_hard()) {
                if (_clock.stopwatch.on) {
                    if (_clock.stopwatch.stoppped) {
                        _clock.stopwatch.free_stamp();
                        _clock.stopwatch.reset();
                    }
                }
            } else
            if (_button4.pressed()) {
                if (_clock.stopwatch.on and _clock.stopwatch.stoppped) {
                    if (_clock.stopwatch.stamps_index_to_show < _clock.stopwatch.stamps_index - 1)
                        ++_clock.stopwatch.stamps_index_to_show;
                }
            }
            break;
        }
    }
}

void ClockController::_process_display_logic() {
    Clock::CLOCK_STATES state = _clock.state();
    Clock::CLOCK_SUBSTATES substate = _clock.substate();

    switch (state) {
        case Clock::S_CLOCK1: {
            if (_clock.state_changed())
                _display.print_clock_datetime(_clock.primary_datetime);
            else
                _handle_clock_substate(substate, _clock.primary_datetime);
            break;
        }
        case Clock::S_CLOCK2: {
            if (_clock.state_changed())
                _display.print_clock_datetime(_clock.secondary_datetime);
            else
                _handle_clock_substate(substate, _clock.secondary_datetime);
            break;
        }
        case Clock::S_ALARM1: {
            if (_clock.state_changed())
                _display.print_alarm_type1(_clock.alaram1_datetime);
            else
                _handle_alarm_type1_substate(substate, _clock.alaram1_datetime);
            break;
        }
        case Clock::S_ALARM2: {
            if (_clock.state_changed())
                _display.print_alarm_type2(_clock.alaram2_datetime);
            else
                _handle_alarm_type2_substate(substate, _clock.alaram2_datetime);
            break;
        }
        case Clock::S_ALARM3: {
            if (_clock.state_changed())
                _display.print_alarm_type3(_clock.alaram3_datetime);
            else
                _handle_alarm_type3_substate(substate, _clock.alaram3_datetime);
            break;
        }
        case Clock::S_SIGNAL: {
            _display.print_signal(_clock.hour_signal);
            break;
        }
        case Clock::S_TIMER1: {
            if (_clock.state_changed())
                _display.print_timer_type1(_clock.timer1_datetime);
            else
                _handle_timer_type1_substate(substate, _clock.timer1_datetime);
            break;
        }
        case Clock::S_TIMER2: {
            if (_clock.state_changed())
                _display.print_timer_type2(_clock.timer2_datetime);
            else
                _handle_timer_type2_substate(substate, _clock.timer2_datetime);
            break;
        }
        case Clock::S_TIMER3: {
            if (_clock.state_changed())
                _display.print_timer_type3(_clock.timer3_datetime);
            else
                _handle_timer_type3_substate(substate, _clock.timer3_datetime, _clock.primary_datetime);
            break;
        }
        case Clock::S_STOPWATCH: {
            if (_clock.state_changed())
                _display.print_stopwatch(_clock.stopwatch);
            else
                _handle_stopwatch(_clock.stopwatch);
            break;
        }
    }
}

void ClockController::_process_stop_logic() {
    Clock::CLOCK_STATES state = _clock.state();
    Clock::CLOCK_SUBSTATES substate = _clock.substate();

    if (_button4.is_down() and 
        state == Clock::S_CLOCK1 and 
        substate == Clock::S_CLOCK_EDIT_SECONDS)
        _clock.primary_datetime_stop();
    else
        _clock.primary_datetime_resume();

    if (_button4.is_down() and 
        state == Clock::S_CLOCK2 and 
        substate == Clock::S_CLOCK_EDIT_SECONDS)
        _clock.secondary_datetime_stop();
    else
        _clock.secondary_datetime_resume();
}

void ClockController::timer1_sync() {
    _clock.tick(Default::TICK_SIZE);
}

void ClockController::timer2_sync() {
    // static uint16_t counter = 0;
    // static uint16_t brightness = 0;

    // if (counter % 100 == 0) {
    //     _display.set_brightness(brightness);
    //     ++brightness;
    //     brightness %= 255;
    // }

    // ++counter;
}
