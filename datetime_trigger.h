#ifndef _DATETIME_TRIGGER_H_
#define _DATETIME_TRIGGER_H_

#include <stdint.h>

class StopwatchTimeTrigger {
    public:
        void hour_flip();
        void minute_flip();
        void second_flip();
        void ms_flip();
        void on_flip();
        void stoppped_flip();

        bool time_triggered() const;

        bool hour_triggered() const;
        bool minute_triggered() const;
        bool second_triggered() const;
        bool ms_triggered() const;
        bool on_triggered() const;
        bool stoppped_triggered() const;

        void flop(); // flop all
    private:
        bool _hour;
        bool _minute;
        bool _second;
        bool _ms;
        bool _on;
        bool _stoppped;
};

class DateTimeTrigger {
public:
    bool hour_triggered() const;
    bool minute_triggered() const;
    bool second_triggered() const;
    bool ms_triggered() const;
    bool day_triggered() const;
    bool month_triggered() const;
    bool year_triggered() const;
    bool day_of_week_triggered() const;

    bool time_triggered() const;
    bool date_triggered() const;

    void hour_flip();
    void minute_flip();
    void second_flip();
    void ms_flip();
    void day_flip();
    void month_flip();
    void year_flip();
    void day_of_week_flip();

    void flop(); // flop all

private:
    bool _hour;
    bool _minute;
    bool _second;
    bool _ms;
    bool _day;
    bool _month;
    bool _year;
    bool _day_of_week;
};



#endif // _DATETIME_TRIGGER_H_
