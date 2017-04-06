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
        bool _hour {false};
        bool _minute {false};
        bool _second {false};
        bool _ms {false};
        bool _on {false};
        bool _stoppped {false};
};

class AlarmDateTimeTrigger {
    public:
        void hour_flip();
        void minute_flip();
        void days_flip();
        void day_pointer_flip();
        void on_flip();
        
        bool hour_triggered() const;
        bool minute_triggered() const;
        bool days_triggered() const;
        bool day_pointer_triggered() const;
        bool on_triggered() const;

        bool triggered() const;

        void flop(); // flop all

    private:
        bool _hour {false};
        bool _minute {false};
        bool _days {false};
        bool _day_pointer {false};
        bool _on {false};
};

class TimerDateTimeTrigger {
    public:
        void origin_hour_flip();
        void origin_minute_flip();
        void origin_second_flip();
        void origin_day_flip();
        void origin_month_flip();
        void origin_year_flip();
        void hour_flip();
        void minute_flip();
        void second_flip();
        void day_flip();

        bool origin_hour_triggered() const;
        bool origin_minute_triggered() const;
        bool origin_second_triggered() const;
        bool origin_day_triggered() const;
        bool origin_month_triggered() const;
        bool origin_year_triggered() const;
        bool hour_triggered() const;
        bool minute_triggered() const;
        bool second_triggered() const;
        bool day_triggered() const;

        bool triggered() const;

        void flop(); // flop all

    private:
        bool _origin_hour {false};
        bool _origin_minute {false};
        bool _origin_second {false};
        bool _origin_day {false};
        bool _origin_month {false};
        bool _origin_year {false};
        bool _hour {false};
        bool _minute {false};
        bool _second {false};
        bool _day {false};
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
        bool _hour {false};
        bool _minute {false};
        bool _second {false};
        bool _ms {false};
        bool _day {false};
        bool _month {false};
        bool _year {false};
        bool _day_of_week {false};
};

#endif // _DATETIME_TRIGGER_H_
