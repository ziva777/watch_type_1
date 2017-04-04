#include "button.h"

Button::Button() {
}

Button::Button(uint8_t pin) {
    attach(pin);
}

void Button::attach(uint8_t pin) {
    pinMode(pin, INPUT_PULLUP);
    _bounce.attach(pin);
    _bounce.interval(BOUNCE_INTERVAL);
    _button_state_prev = HIGH;
    _button_state_curr = HIGH;
    _state = BUTTON_FREE;
}

void Button::update() {
    _bounce.update();
    _button_state_curr = _bounce.read();

    if (_button_state_curr == LOW and _button_state_prev == HIGH) {
        _state = BUTTON_PRESSED;
        _pressed_time = millis();
    } else 
    if (_state == BUTTON_WAIT_FOR_PRESSED_HARD and _button_state_curr == LOW) {
        if (millis() - _pressed_time >= WAIT_FOR_HARD_INTERVAL) {
            _state = BUTTON_PRESSED_HARD;
            _repeat_time_prev = 0;
        }
    } else
    if (_state == BUTTON_WAIT_FOR_REPEAT and _button_state_curr == LOW) {
        _repeat_time_curr = (millis() % 1000) / REPEAT_INTERVAL;
        if (_repeat_time_curr != _repeat_time_prev) {
            _repeat_time_prev = _repeat_time_curr;
            _state = BUTTON_REPEAT;
            
        }
    }
    else {
        _state = BUTTON_FREE;
    }
}

bool Button::pressed_repeat() const {
    return _state == BUTTON_REPEAT;
}
bool Button::pressed_hard() const {
    return _state == BUTTON_PRESSED_HARD;
}
bool Button::pressed() const {
    return _state == BUTTON_PRESSED;
}

void Button::flop() {
    _button_state_prev = _button_state_curr;

    if (_button_state_curr == LOW)
        if (_state == BUTTON_PRESSED)
            _state = BUTTON_WAIT_FOR_PRESSED_HARD;
        else if (_state == BUTTON_PRESSED_HARD)
            _state = BUTTON_WAIT_FOR_REPEAT;
        else if (_state == BUTTON_REPEAT)
            _state = BUTTON_WAIT_FOR_REPEAT;
}

bool Button::is_down() const {
    return _button_state_curr == LOW;
}

bool Button::is_up() const {
    return !is_down();
}
