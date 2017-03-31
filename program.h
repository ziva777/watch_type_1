#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "button.h"
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
        void oscillate();
        
    private:
        // Display _display;
        // Oscillator _oscillator;
        // DateTime _datetime;
};

#endif // _PROGRAM_H_
