#include "datetime_trigger.h"

void StopwatchTimeTrigger::hour_flip() {
    _hour = true;
}

void StopwatchTimeTrigger::minute_flip() {
    _minute = true;
}

void StopwatchTimeTrigger::second_flip() {
    _second = true;
}

void StopwatchTimeTrigger::ms_flip() {
    _ms = true;
}

void StopwatchTimeTrigger::on_flip() {
    _on = true;
}

void StopwatchTimeTrigger::stoppped_flip() {
    _stoppped = true;
}

void StopwatchTimeTrigger::forced_flip() {
    _forced = true;
}

bool StopwatchTimeTrigger::hour_triggered() const {
    return _hour;
}

bool StopwatchTimeTrigger::minute_triggered() const {
    return _minute;
}

bool StopwatchTimeTrigger::second_triggered() const {
    return _second;
}

bool StopwatchTimeTrigger::ms_triggered() const {
    return _ms;
}

bool StopwatchTimeTrigger::on_triggered() const {
    return _on;
}

bool StopwatchTimeTrigger::stoppped_triggered() const {
    return _stoppped;
}

bool StopwatchTimeTrigger::forced_triggered() const {
    return _forced;
}

bool StopwatchTimeTrigger::time_triggered() const {
    return ms_triggered() or
            second_triggered() or 
            minute_triggered() or 
            hour_triggered() or 
            forced_triggered();
}

void StopwatchTimeTrigger::flop() {
    _hour = false;
    _minute = false;
    _second = false;
    _ms = false;
    _on = false;
    _stoppped = false;
    _forced = false;
}

bool AlarmDateTimeTrigger::hour_triggered() const {
    return _hour;
}

bool AlarmDateTimeTrigger::minute_triggered() const {
    return _minute;
}

bool AlarmDateTimeTrigger::days_triggered() const {
    return _days;
}

bool AlarmDateTimeTrigger::day_pointer_triggered() const {
    return _day_pointer;
}

bool AlarmDateTimeTrigger::on_triggered() const {
    return _on;
}

void AlarmDateTimeTrigger::hour_flip() {
    _hour = true;
}

void AlarmDateTimeTrigger::minute_flip() {
    _minute = true;
}

void AlarmDateTimeTrigger::days_flip() {
    _days = true;
}

void AlarmDateTimeTrigger::day_pointer_flip() {
    _day_pointer = true;
}

void AlarmDateTimeTrigger::on_flip() {
    _on = true;
}

bool AlarmDateTimeTrigger::triggered() const {
    return _on or _hour or _minute or _days or _day_pointer;
}

void AlarmDateTimeTrigger::flop() {
    _on = false;
    _hour = false;
    _minute = false;
    _days = false;
    _day_pointer = false;
}

void TimerDateTimeTrigger::origin_hour_flip() { 
    _origin_hour = true;
}

void TimerDateTimeTrigger::origin_minute_flip() { 
    _origin_minute = true;
}

void TimerDateTimeTrigger::origin_second_flip() { 
    _origin_second = true;
}

void TimerDateTimeTrigger::origin_day_flip() { 
    _origin_day = true;
}

void TimerDateTimeTrigger::origin_month_flip() { 
    _origin_month = true;
}

void TimerDateTimeTrigger::origin_year_flip() { 
    _origin_year = true;
}

void TimerDateTimeTrigger::hour_flip() { 
    _hour = true;
}

void TimerDateTimeTrigger::minute_flip() { 
    _minute = true;
}

void TimerDateTimeTrigger::second_flip() { 
    _second = true;
}

void TimerDateTimeTrigger::day_flip() { 
    _day = true;
}

bool TimerDateTimeTrigger::origin_hour_triggered() const {
    return _origin_hour;
}

bool TimerDateTimeTrigger::origin_minute_triggered() const {
    return _origin_minute;
}

bool TimerDateTimeTrigger::origin_second_triggered() const {
    return _origin_second;
}

bool TimerDateTimeTrigger::origin_day_triggered() const {
    return _origin_day;
}

bool TimerDateTimeTrigger::origin_month_triggered() const {
    return _origin_month;
}

bool TimerDateTimeTrigger::origin_year_triggered() const {
    return _origin_year;
}

bool TimerDateTimeTrigger::hour_triggered() const {
    return _hour;
}

bool TimerDateTimeTrigger::minute_triggered() const {
    return _minute;
}

bool TimerDateTimeTrigger::second_triggered() const {
    return _second;
}

bool TimerDateTimeTrigger::day_triggered() const {
    return _day;
}

bool TimerDateTimeTrigger::triggered() const {
    return _hour or _minute or _second or _day or
            _origin_hour or _origin_minute or _origin_second or 
            _origin_day or _origin_month or _origin_year;
}

void TimerDateTimeTrigger::flop() {
    _origin_hour = false;
    _origin_minute = false;
    _origin_second = false;
    _origin_day = false;
    _origin_month = false;
    _origin_year = false;
    _hour = false;
    _minute = false;
    _second = false;
    _day = false;
}

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
