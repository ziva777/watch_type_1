#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "timer1.h"

// Pinouts
namespace Pin {
    // Buttons pinouts
    const uint8_t BUTTON1 = 10;
    const uint8_t BUTTON2 = 11;
    const uint8_t BUTTON3 = 12;
    const uint8_t BUTTON4 = A8;

    // Don't know
    const uint8_t CONTRAST = 2;
    const uint8_t BRIGHTNESS = 3;

    // Display pinouts
    namespace Display {
        const uint8_t RS = 8; 
        const uint8_t ENABLE = 9;
        const uint8_t D0 = 4;
        const uint8_t D1 = 5; 
        const uint8_t D2 = A7; 
        const uint8_t D3 = 7;
    };
};

// Default values
namespace Default {
    // For LiquidChristal display
    const uint8_t CONTRAST = 55;
    const uint8_t BRIGHTNESS = 10;

    // For clock oscillate
    const uint16_t TICK_SIZE = 100; // 100 mS
    const Timer1::FCPU_SCALES TIMER1_SCALE = Timer1::SCALE_64; 
    const uint16_t TIMER1_COUNTS = 25000 - 1;

    // For initial clock walues
    namespace PrimaryClock {
        const uint8_t HOUR {0};
        const uint8_t MINUTE {0};
        const uint8_t SECOND {0};
        const uint32_t MS {0};

        const uint8_t DAY {01};
        const uint8_t MONTH {01};
        const uint16_t YEAR {2015};
        const uint8_t DAY_OF_WEEK {3};
    };

    // Max length of stopwatch lap list
    const uint8_t STOPWATCH_LAP_COUNT = 10;
};

#endif // _SETTINGS_H_
