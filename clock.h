#ifndef _CLOCK_H_
#define _CLOCK_H_

class Clock {
    public:
        enum CLOCK_STATES { 
            S_CLOCK_MAIN,
            S_ALARM1,
            S_ALARM2,
            S_ALARM3,
            S_SIGNAL,
            S_TIMER1,
            S_TIMER2,
            S_TIMER3,
            S_STOPWATCH
        };

        void next_state();
        Clock::CLOCK_STATES state() const;
        
    private:
        Clock::CLOCK_STATES _state_curr{S_CLOCK_MAIN};
        Clock::CLOCK_STATES _state_prev{S_CLOCK_MAIN};
};

#endif // _CLOCK_H_
