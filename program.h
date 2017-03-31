#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <Arduino.h>
#include "clock.h"
#include "display.h"
#include "timer1.h"
#include "timer2.h"

namespace Pin {
    const uint8_t BUTTON1 = 10;
    const uint8_t BUTTON2 = 11;
    const uint8_t BUTTON3 = 12;
    const uint8_t BUTTON4 = A8;

    const uint8_t CONTRAST = 2;
    const uint8_t BRIGHTNESS = 3;
};

class Program {
    public:
        Program();

        void setup();
        void loop();
        void timer1_interrupt();
        void timer2_interrupt();

        void attach_timer1_interrupt(Timer1::TimerCallbackFunction func);
        void attach_timer2_interrupt(Timer2::TimerCallbackFunction func);
        void start_timer1();
        void start_timer2();
        
    private:
        Clock _clock;
        Display _display;
        // Oscillator _oscillator;
        // DateTime _datetime;

        
};

#endif // _PROGRAM_H_
