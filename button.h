#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Bounce2.h>
#include <Arduino.h>

class Button {
    public:
        Button();
        Button(uint8_t pin);

        void attach(uint8_t pin);
        void update();
        bool pressed_repeat() const;
        bool pressed_hard() const;
        bool pressed() const;
        void flop();

    private:
        const uint16_t BOUNCE_INTERVAL = 5;
        const uint16_t WAIT_FOR_HARD_INTERVAL = 500;
        const uint16_t REPEAT_INTERVAL = 100;
        const uint16_t INTERVAL = 5;

        enum BUTTON_STATES { 
            BUTTON_FREE, 
            BUTTON_PRESSED, 
            BUTTON_PRESSED_HARD, 
            BUTTON_REPEAT, 
            BUTTON_WAIT_FOR_PRESSED_HARD,
            BUTTON_WAIT_FOR_REPEAT 
        } _state;

        Bounce _bounce;
        uint32_t _pressed_time;
        uint8_t _repeat_time_prev;
        uint8_t _repeat_time_curr;
        uint8_t _button_state_prev;
        uint8_t _button_state_curr;
};

#endif // _BUTTON_H_
