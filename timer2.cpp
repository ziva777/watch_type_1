#include "timer2.h"

ISR(TIMER2_COMPA_vect) {
    Timer2::instance().timer_callback_function();
}

const Timer2 & Timer2::instance() {
    static Timer2 timer;
    return timer;
}

void Timer2::start(Timer2::FCPU_SCALES prescalar, uint8_t counts) {
    _prescalar = prescalar;
    _counts = counts;

    noInterrupts();
    TCCR2B = 0;
    TCCR2A = 0;

    TCNT2 = 0;
    OCR2A = _counts;

    TCCR2B = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
    TCCR2A = _prescalar;
    TIMSK2 = _BV(OCIE2A);
    interrupts();
}

void Timer2::attach(TimerCallbackFunction callback) {
    timer_callback_function = callback;
}
