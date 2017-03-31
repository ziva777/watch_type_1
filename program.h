#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <Arduino.h>

#include "timer1.h"
#include "timer2.h"
#include "clock.h"
#include "display.h"
#include "clock_controller.h"

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
        // 100 mS
        Timer1::FCPU_SCALES _timer1_scale = Timer1::SCALE_64; 
        uint16_t _timer1_counts = 25000 - 1;

        // 144 mS
        Timer2::FCPU_SCALES _timer2_scale = Timer2::SCALE_1024;
        uint16_t _timer2_scale2 = 1;
        uint8_t _timer2_counts = 225;

        Clock _clock;
        Display _display;
        ClockController _clock_controller{_clock, _display};
};

#endif // _PROGRAM_H_
