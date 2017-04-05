#include "program.h"

Program::Program() {

}

void Program::setup() {
    _clock_controller.setup();
}

void Program::attach_timer1_interrupt(Timer1::TimerCallbackFunction func){
    Timer1::instance().attach(func);
}

void Program::start_timer1() {
    Timer1::instance().start(_timer1_scale, _timer1_counts);
}

void Program::timer1_interrupt() {
    _clock_controller.timer1_sync();
}

void Program::loop() {
    _clock_controller.sync();
}
