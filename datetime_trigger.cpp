#include "datetime_trigger.h"

bool DateTimeTrigger::hour_triggered() const {
    return _hour;
}

bool DateTimeTrigger::minute_triggered() const {
    return _minute;
}

bool DateTimeTrigger::second_triggered() const {
    return _second;
}

bool DateTimeTrigger::ms_triggered() const {
    return _ms;
}

bool DateTimeTrigger::day_triggered() const {
    return _day;
}

bool DateTimeTrigger::month_triggered() const {
    return _month;
}

bool DateTimeTrigger::year_triggered() const {
    return _year;
}

bool DateTimeTrigger::day_of_week_triggered() const {
    return _day_of_week;
}

bool DateTimeTrigger::time_triggered() const {
    return second_triggered() or 
            minute_triggered() or 
            hour_triggered();
}

bool DateTimeTrigger::date_triggered() const {
    return day_triggered() or 
            month_triggered() or 
            year_triggered();
}

void DateTimeTrigger::hour_flip() {
    _hour = true;
}

void DateTimeTrigger::minute_flip() {
    _minute = true;
}

void DateTimeTrigger::second_flip() {
    _second = true;
}

void DateTimeTrigger::ms_flip() {
    _ms = true;
}

void DateTimeTrigger::day_flip() {
    _day = true;
}

void DateTimeTrigger::month_flip() {
    _month = true;
}

void DateTimeTrigger::year_flip() {
    _year = true;
}

void DateTimeTrigger::day_of_week_flip() {
    _day_of_week = true;
}

void DateTimeTrigger::flop() {
    _hour = false;
    _minute = false;
    _second = false;
    _ms = false;
    _day = false;
    _month = false;
    _year = false;
    _day_of_week = false;
}
