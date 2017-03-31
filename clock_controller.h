#ifndef _CLOCK_CONTROLLER_H_
#define _CLOCK_CONTROLLER_H_

#include "clock.h"
#include "display.h"
#include "button.h"

namespace Pin {
    const uint8_t BUTTON1 = 10;
    const uint8_t BUTTON2 = 11;
    const uint8_t BUTTON3 = 12;
    const uint8_t BUTTON4 = A8;

    const uint8_t CONTRAST = 2;
    const uint8_t BRIGHTNESS = 3;
};

class ClockController {
    public:
        ClockController(Clock &c, Display &d);

        void setup();
        void sync();

        void timer1_sync(); // 100 mS
        void timer2_sync(); // 144 mS

    private:
        const uint16_t TICK_SIZE = 100; // 100 mS

        Clock &_clock;
        Display &_display;

        Button _button1{Pin::BUTTON1};
        Button _button2{Pin::BUTTON2};
        Button _button3{Pin::BUTTON3};
        Button _button4{Pin::BUTTON4};
};

#endif // _CLOCK_CONTROLLER_H_
