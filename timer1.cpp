#include "timer1.h"

ISR(TIMER1_COMPA_vect) {
    Timer1::instance().timer_callback_function();
}

const Timer1 & Timer1::instance() {
    static Timer1 timer;
    return timer;
}

void Timer1::start(Timer1::FCPU_SCALES prescalar, uint16_t counts) {
    _prescalar = prescalar;
    _counts = counts;

    noInterrupts();
    TCCR1B = 0;
    TCCR1A = 0;

    TCNT1 = 0;
    OCR1A = _counts;

    TCCR1B = _BV(WGM12) | _prescalar;
    TIMSK1 = _BV(OCIE1A);
    interrupts();
}

void Timer1::shift(uint16_t counts) {
    _counts = counts;
    // noInterrupts();
    TCNT1 = 0;
    OCR1A = _counts;
    // interrupts();
}

void Timer1::attach(TimerCallbackFunction callback) {
    timer_callback_function = callback;
}

uint16_t Timer1::counts() const {
    return _counts;
}
