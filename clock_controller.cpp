#include "clock_controller.h"

ClockController::ClockController(Clock &c, Display &d)
    : _clock(c), _display(d) 
{

}

void ClockController::setup(){
    _display.setup(Pin::CONTRAST, Pin::BRIGHTNESS);
    _display.set_contrast(55);
    _display.set_brightness(50);
    _display.print_clock_state(_clock);
    _display.print_clock_datetime(_clock.primary_datetime);
}

void ClockController::_inc_datetime(DateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_CLOCK_EDIT_SECONDS)
        dt.second = 60;
    else 
    if (state == Clock::S_CLOCK_EDIT_MINUTES)
        dt.minute += 1;
    else 
    if (state == Clock::S_CLOCK_EDIT_HOURS)
        dt.hour += 1;
    else 
    if (state == Clock::S_CLOCK_EDIT_DAYS)
        dt.day += 1;
    else 
    if (state == Clock::S_CLOCK_EDIT_MONTHS)
        dt.month += 1;
    else 
    if (state == Clock::S_CLOCK_EDIT_YEARS)
        dt.year += 1;

    dt.normalize();
}

void ClockController::_dec_datetime(DateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_CLOCK_EDIT_SECONDS)
        dt.second = 0;
    else 
    if (state == Clock::S_CLOCK_EDIT_MINUTES)
        dt.dec_minute();
    else 
    if (state == Clock::S_CLOCK_EDIT_HOURS)
        dt.dec_hour();
    else 
    if (state == Clock::S_CLOCK_EDIT_DAYS)
        dt.dec_day();
    else 
    if (state == Clock::S_CLOCK_EDIT_MONTHS)
        dt.dec_month();
    else 
    if (state == Clock::S_CLOCK_EDIT_YEARS)
        dt.dec_year();
}

void ClockController::_inc_alarm(AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_ALARM_TYPE1_EDIT_MINUTES or state == Clock::S_ALARM_TYPE2_EDIT_MINUTES) {
        ++dt.minute;

        if (dt.minute == 60) {
            dt.minute = 0;
            dt.hour = ++dt.hour % 24;
        }
    }
    else 
    if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS or state == Clock::S_ALARM_TYPE2_EDIT_HOURS)
        dt.hour = ++dt.hour % 24;
    else
    if (state == Clock::S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK) {
        dt.days[dt.day_pointer] = (dt.days[dt.day_pointer] == 1 ? 0 : 1);
    }
}

void ClockController::_dec_alarm(AlarmDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_ALARM_TYPE1_EDIT_MINUTES or state == Clock::S_ALARM_TYPE2_EDIT_MINUTES) {
        if (dt.minute != 0)
            --dt.minute;
        else {
            dt.minute = 59;

            if (dt.hour)
                --dt.hour;
            else
                dt.hour = 23;
        }
    } else 
    if (state == Clock::S_ALARM_TYPE1_EDIT_HOURS or state == Clock::S_ALARM_TYPE2_EDIT_HOURS) {
        if (dt.hour)
            --dt.hour;
        else
            dt.hour = 23;
    } else 
    if (state == Clock::S_ALARM_TYPE2_EDIT_DAYS_OF_WEEK) {
        dt.day_pointer = ++dt.day_pointer % 7 ;
    }
}

void ClockController::_inc_timer_type1(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS) {
        ++dt.origin_second;

        if (dt.origin_second == 60) {
            dt.origin_second = 0;
            ++dt.origin_minute;

            if (dt.origin_minute == 60) {
                dt.origin_minute = 0;
                ++dt.origin_hour;

                if (dt.origin_hour == 100)
                    dt.origin_hour = 0;
            }
        }
    } else
    if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES) {
        ++dt.origin_minute;

        if (dt.origin_minute == 60) {
            dt.origin_minute = 0;
            ++dt.origin_hour;

            if (dt.origin_hour == 100)
                dt.origin_hour = 0;
        }
    } else
    if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS) {
        ++dt.origin_hour;

        if (dt.origin_hour == 100)
            dt.origin_hour = 0;
    }
}

void ClockController::_dec_timer_type1(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS) {
        uint32_t total_sec = dt.origin_hour * 3600UL + 
                                dt.origin_minute * 60UL + 
                                  dt.origin_second;

        if (total_sec) {
            --total_sec;
        } else {
            total_sec = 99 * 3600UL + 60 * 59UL + 59UL;
        };

        dt.origin_hour = total_sec / 3600UL;
        dt.origin_minute = (total_sec - dt.origin_hour * 3600UL) / 60UL;
        dt.origin_second = total_sec - dt.origin_hour * 3600UL - dt.origin_minute * 60UL;
    } else
    if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES) {
        uint32_t total_sec = dt.origin_hour * 3600UL + 
                                dt.origin_minute * 60UL + 
                                  dt.origin_second;
        uint8_t tmp = dt.origin_second;

        if (total_sec >= 60) {
            total_sec -= 60;
        } else {
            total_sec = 99 * 3600UL + 60 * 59UL + tmp;
        };

        dt.origin_hour = total_sec / 3600UL;
        dt.origin_minute = (total_sec - dt.origin_hour * 3600UL) / 60UL;
        dt.origin_second = total_sec - dt.origin_hour * 3600UL - dt.origin_minute * 60UL;
    } else
    if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS) {
        if (dt.origin_hour == 0)
            dt.origin_hour = 99;
        else
            --dt.origin_hour;
    }
}

void ClockController::_inc_timer_type2(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS) {
        ++dt.origin_second;

        if (dt.origin_second == 60) {
            dt.origin_second = 0;

            ++dt.origin_minute;

            if (dt.origin_minute == 60) {
                dt.origin_minute = 0;
                dt.origin_hour = ++dt.origin_hour % 24;
            }
        }
    }
    else
    if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES) {
        ++dt.origin_minute;

        if (dt.origin_minute == 60) {
            dt.origin_minute = 0;
            dt.origin_hour = ++dt.origin_hour % 24;
        }
    }
    else 
    if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS)
        dt.origin_hour = ++dt.origin_hour % 24;
}

void ClockController::_dec_timer_type2(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state) {
    if (state == Clock::S_TIMER_TYPE1_EDIT_SECONDS) {
        if (dt.origin_second != 0)
            --dt.origin_second;
        else {
            dt.origin_second = 59;

            if (dt.origin_minute != 0)
                --dt.origin_minute;
            else {
                dt.origin_minute = 59;

                if (dt.origin_hour)
                    --dt.origin_hour;
                else
                    dt.origin_hour = 23;
            }
        }
    }
    else
    if (state == Clock::S_TIMER_TYPE1_EDIT_MINUTES) {
        if (dt.origin_minute != 0)
            --dt.origin_minute;
        else {
            dt.origin_minute = 59;

            if (dt.origin_hour)
                --dt.origin_hour;
            else
                dt.origin_hour = 23;
        }
    } else 
    if (state == Clock::S_TIMER_TYPE1_EDIT_HOURS) {
        if (dt.origin_hour)
            --dt.origin_hour;
        else
            dt.origin_hour = 23;
    } 
}

void ClockController::_inc_timer_type3(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state, DateTime &curr_dt) {
    if (state == Clock::S_TIMER_TYPE2_EDIT_DAYS)
        ++dt.origin_day;
    else
    if (state == Clock::S_TIMER_TYPE2_EDIT_MONTHS)
        ++dt.origin_month;
    else
    if (state == Clock::S_TIMER_TYPE2_EDIT_YEARS) {
        if (dt.origin_year == curr_dt.year)
            dt.origin_year = curr_dt.year + 1;
        else
            dt.origin_year = curr_dt.year;
    }

    dt.normalize();
}

void ClockController::_dec_timer_type3(TimerDateTime &dt, Clock::CLOCK_SUBSTATES state, DateTime &curr_dt) {

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
    if (substate == Clock::S_NONE)
        _display.print_alarm_type1(dt);
    else
        _display.print_edit_alarm_type1(dt, substate);
}

void ClockController::_handle_alarm_type2_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt) {
    if (substate == Clock::S_NONE)
        _display.print_alarm_type2(dt);
    else
        _display.print_edit_alarm_type2(dt, substate);
}

void ClockController::_handle_alarm_type3_substate(Clock::CLOCK_SUBSTATES substate, AlarmDateTime &dt) {
    if (substate == Clock::S_NONE)
        _display.print_alarm_type3(dt);
    else
        _display.print_edit_alarm_type3(dt, substate);
}

void ClockController::_handle_timer_type1_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt) {
    if (substate == Clock::S_NONE)
        _display.print_timer_type1(dt);
    else
        _display.print_edit_timer_type1(dt, substate);   
}

void ClockController::_handle_timer_type2_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt) {
    if (substate == Clock::S_NONE)
        _display.print_timer_type2(dt);
    else
        _display.print_edit_timer_type2(dt, substate);   
}

void ClockController::_handle_timer_type3_substate(Clock::CLOCK_SUBSTATES substate, TimerDateTime &dt, DateTime &curr_dt) {
    if (substate == Clock::S_NONE)
        _display.print_timer_type3(dt);
    else
        _display.print_edit_timer_type3(dt, substate);   
}

void ClockController::_handle_stopwatch(StopwatchTime &stopwatch) {
    if (stopwatch.trigger.time_triggered())
        _display.print_stopwatch(stopwatch);
}

void ClockController::sync(){
    static Clock::CLOCK_STATES clock_state_curr = _clock.state();
    static Clock::CLOCK_STATES clock_state_prev = _clock.state();
    bool state_cnanged;

    _button1.update();
    _button2.update();
    _button3.update();
    _button4.update();

    Clock::CLOCK_STATES state = _clock.state();
    Clock::CLOCK_SUBSTATES substate = _clock.substate();

    clock_state_curr = state;
    state_cnanged = clock_state_prev != clock_state_curr;

    switch (state) {
        case Clock::S_CLOCK1:
        case Clock::S_CLOCK2: {
            if (substate == Clock::S_NONE) {
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                } else
                if (_button2.pressed_hard())
                    _clock.begin_substate();
            } else {
                DateTime &dt = (state == Clock::S_CLOCK1 ? _clock.primary_datetime : _clock.secondary_datetime);

                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
                    _display.print_clock_datetime(dt);
                }

                if (_button3.pressed() or _button3.pressed_repeat())
                    _dec_datetime(dt, substate);

                if (_button4.pressed() or _button4.pressed_repeat())
                    _inc_datetime(dt, substate);
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
                } else 
                if (_button2.pressed()) {
                    dt.on = !dt.on;
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
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
                } else 
                if (_button2.pressed()) {
                    dt.on = !dt.on;
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
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

                    dt.origin_day = _clock.primary_datetime.day;
                    dt.origin_month = _clock.primary_datetime.month;
                    dt.origin_year = _clock.primary_datetime.year;
                    dt.leap = _clock.primary_datetime.leap();
                } else 
                if (_button2.pressed()) {
                    dt.on = !dt.on;
                }
            } else {
                if (_button1.pressed())
                    _clock.next_substate();

                if (_button2.pressed()) {
                    _clock.end_substate();
                    substate = _clock.substate();
                }

                if (_button3.pressed() or _button3.pressed_repeat())
                    _dec_timer_type3(dt, substate, _clock.primary_datetime);

                if (_button4.pressed() or _button4.pressed_repeat())
                    _inc_timer_type3(dt, substate, _clock.primary_datetime);
            }
            break;
        }
        case Clock::S_STOPWATCH: {
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
        }
        default:
            if (substate == Clock::S_NONE)
                if (_button1.pressed()) {
                    _clock.next_state();
                    _display.print_clock_state(_clock);
                }
            break;
    }

    switch (_clock.state()) {
        case Clock::S_CLOCK1: {
                if (state_cnanged)
                    _display.print_clock_datetime(_clock.primary_datetime);
                else
                    _handle_clock_substate(substate, _clock.primary_datetime);
            }
            break;
        case Clock::S_CLOCK2: {
                if (state_cnanged)
                    _display.print_clock_datetime(_clock.secondary_datetime);
                else
                    _handle_clock_substate(substate, _clock.secondary_datetime);
            }
            break;
        case Clock::S_ALARM1:
            _handle_alarm_type1_substate(substate, _clock.alaram1_datetime);
            break;
        case Clock::S_ALARM2:
            _handle_alarm_type2_substate(substate, _clock.alaram2_datetime);
            break;
        case Clock::S_ALARM3:
            _handle_alarm_type3_substate(substate, _clock.alaram3_datetime);
            break;
        case Clock::S_SIGNAL:
            _display.print_signal(_clock.hour_signal);
            break;
        case Clock::S_TIMER1:
            _handle_timer_type1_substate(substate, _clock.timer1_datetime);
            break;
        case Clock::S_TIMER2:
            _handle_timer_type2_substate(substate, _clock.timer2_datetime);
            break;
        case Clock::S_TIMER3:
            _handle_timer_type3_substate(substate, _clock.timer3_datetime, _clock.primary_datetime);
            break;
        case Clock::S_STOPWATCH: {
                if ((clock_state_prev != clock_state_curr))
                    _display.print_stopwatch(_clock.stopwatch);
                _handle_stopwatch(_clock.stopwatch);
                break;
            }
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
