#include "program.h"

Program::Program() {

}

void Program::setup() {
    _clock_controller.setup();
    // tone(6, 440, 50);
}

void Program::attach_timer1_interrupt(Timer1::TimerCallbackFunction func){
    Timer1::instance().attach(func);
}

void Program::attach_timer2_interrupt(Timer2::TimerCallbackFunction func){
    Timer2::instance().attach(func);
}

void Program::start_timer1() {
    Timer1::instance().start(_timer1_scale, _timer1_counts);
}

void Program::start_timer2() {
    Timer2::instance().start(_timer2_scale, _timer2_counts);
}

void Program::timer1_interrupt() {
    _clock_controller.timer1_sync();
}

void Program::timer2_interrupt() {
    static uint16_t counter = 0;

    if (_timer2_scale2 == counter) {
        counter = 0;
        _clock_controller.timer2_sync();
    }
    ++counter;
}

void Program::loop() {
    _clock_controller.sync();
}
