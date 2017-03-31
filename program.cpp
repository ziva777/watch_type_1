#include "program.h"
#include "button.h"

uint16_t msec_counter = 0;
uint16_t msec_counter_tmp = 0;

Button button1 = Button(Pin::BUTTON1);
Button button2 = Button(Pin::BUTTON2);
Button button3 = Button(Pin::BUTTON3);
Button button4 = Button(Pin::BUTTON4);

Program::Program() {

}

void Program::setup() {
    _display.setup(Pin::CONTRAST, Pin::BRIGHTNESS);
    _display.set_contrast(55);
    _display.set_brightness(10);
    _display.print_clock_state(_clock);
    _display.print_datetime(_clock.primary_datetime);
}

void Program::attach_timer1_interrupt(Timer1::TimerCallbackFunction func){
    Timer1::instance().attach(func);
}

void Program::attach_timer2_interrupt(Timer2::TimerCallbackFunction func){
    Timer2::instance().attach(func);
}

void Program::start_timer1() {
    Timer1::instance().start(Timer1::SCALE_64, 25000 - 1);
}

void Program::start_timer2() {
    Timer2::instance().start(Timer2::SCALE_1024, 200);
}

void Program::timer1_interrupt() {
    ++msec_counter;
    _clock.tick(100);
}

void Program::timer2_interrupt() {
    static uint16_t counter = 0;
    static uint16_t brightness = 0;

    if (counter % 100 == 0) {
        _display.set_brightness(brightness);
        ++brightness;
        brightness %= 255;
    }

    ++counter;
}

void Program::loop() {
    button1.update();
    if (button1.pressed()) {
        _clock.next_state();
        _display.print_clock_state(_clock);
    }
    button1.flop();

    button4.update();
    if (button4.pressed()) {
        // _clock.primary_datetime.tick(1000);
        if (_clock.primary_datetime.second > 30)
            _clock.primary_datetime.second = 59;
        else
        _clock.primary_datetime.second = 0;
    }
    button4.flop();

    button3.update();
    if (button3.pressed()) {
        // _clock.primary_datetime.tick(60 * 1000);
        _clock.primary_datetime.minute = 
                ++_clock.primary_datetime.minute % 60;
        _display.print_time(_clock.primary_datetime);
    } if (button3.pressed_repeat()) {
        _clock.primary_datetime.minute = 
                ++_clock.primary_datetime.minute % 60;
        _display.print_time(_clock.primary_datetime);
    }
    button3.flop();

    /*button2.update();
    if (button2.pressed()) {
        msec_counter += 1;
    } else
    if (button2.pressed_hard()) {
        msec_counter += 1;
    } else
    if (button2.pressed_repeat()) {
        msec_counter += 1;
    }
    button2.flop();

    button3.update();
    if (button3.pressed()) {
        msec_counter -= 1;
    }
    button3.flop();

    button4.update();
    if (button4.pressed()) {
        msec_counter = 65535;
    }
    button4.flop();

    */

    // if (msec_counter_tmp != msec_counter) { 
    //     msec_counter_tmp = msec_counter;
    //     char buff[16];
    //     memset(buff, 0, 16);
    //     sprintf(buff, "V=%5u", msec_counter);
    //     _display.print_text(1, 0, buff);
    // }

    if (_clock.primary_datetime.trigger.time_triggered()) {
        _display.print_time(_clock.primary_datetime);

        if (_clock.primary_datetime.trigger.date_triggered()) {
            _display.print_date(_clock.primary_datetime);
        }

        if (_clock.primary_datetime.trigger.time_triggered() or
            _clock.primary_datetime.trigger.date_triggered()) 
            _clock.primary_datetime.trigger.flop();
    }
}
