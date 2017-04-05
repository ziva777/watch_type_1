#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "timer1.h"
#include "timer2.h"
#include "clock.h"
#include "display.h"
#include "clock_controller.h"
#include "settings.h"

class Program {
    public:
        Program();

        void setup();
        void loop();
        
        void timer1_interrupt();
        void attach_timer1_interrupt(Timer1::TimerCallbackFunction func);
        void start_timer1();
        
    private:
        // 100 mS
        Timer1::FCPU_SCALES _timer1_scale {Default::TIMER1_SCALE}; 
        uint16_t _timer1_counts = {Default::TIMER1_COUNTS};

        Clock _clock;
        Display _display;
        ClockController _clock_controller{_clock, _display};
};

#endif // _PROGRAM_H_
