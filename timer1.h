#ifndef _TIMER1_H_
#define _TIMER1_H_

#include <Arduino.h>

class Timer1 {
    public:
        enum FCPU_SCALES : uint8_t {
            TIMER_OFF  =                                0, // timer1 off
            SCALE_1    =                        _BV(CS10), // 1 - 0.0625 uS
            SCALE_8    =            _BV(CS11)            , // 8 - 0.5 uS
            SCALE_64   =            _BV(CS11) | _BV(CS10), // 64 - 4 uS
            SCALE_256  = _BV(CS12)                       , // 256 - 16 uS
            SCALE_1024 = _BV(CS12)            | _BV(CS10)  // 1024 - 640 uS
        };

        typedef void (*TimerCallbackFunction)();
        TimerCallbackFunction timer_callback_function;
        
        static const Timer1 & instance();
        
        void start(Timer1::FCPU_SCALES prescalar, uint16_t counts);
        void attach(TimerCallbackFunction callback);
      
    private:
        Timer1() {};
        Timer1(const Timer1 &);
        Timer1 & operator=(const Timer1 &);

        Timer1::FCPU_SCALES _prescalar;
        uint16_t _counts;
};

#endif // _TIMER1_H_
