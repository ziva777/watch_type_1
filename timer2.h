#ifndef _TIMER2_H_
#define _TIMER2_H_

#include <Arduino.h>

class Timer2 {
    public:
        enum FCPU_SCALES : uint8_t {
            TIMER_OFF  =                                 0, // timer1 off
            SCALE_1    =                         _BV(CS20), // 1 - 0.0625 uS
            SCALE_8    =             _BV(CS21)            , // 8 - 0.5 uS
            SCALE_32   =             _BV(CS21) | _BV(CS20), // 32 - 2 uS
            SCALE_64   = _BV(CS22)                        , // 64 - 4 uS
            SCALE_128  = _BV(CS22) |             _BV(CS20), // 128 - 8 uS
            SCALE_256  = _BV(CS22) | _BV(CS21)            , // 256 - 16 uS
            SCALE_1024 = _BV(CS22) | _BV(CS21) | _BV(CS20)  // 1024 - 640 uS
        };

        typedef void (*TimerCallbackFunction)();
        TimerCallbackFunction timer_callback_function;
        
        static const Timer2 & instance();
        
        void start(Timer2::FCPU_SCALES prescalar, uint8_t counts);
        void attach(TimerCallbackFunction callback);
      
    private:
        Timer2() {};
        Timer2(const Timer2 &);
        Timer2 & operator=(const Timer2 &);

        Timer2::FCPU_SCALES _prescalar;
        uint8_t _counts;
};

#endif // _TIMER2_H_
