#ifndef TIMER_H
#define TIMER_H

class Timer {
    private: 
        unsigned long stop_millis[5];
        int num_stops = 0;
        int current_stop;
        unsigned long counter = 0;

        unsigned long duration;
        bool active;
        void (*callback) (void);
        unsigned long calculate_stop_millis();

    public:
        void begin(unsigned long, void (*callback) (void));
        bool is_active() { return active; }
        bool get_current_stop() { return current_stop; }
        void set_current_stop(int);
        void set_counter_with_next_stop();
        void add_stop(unsigned long);
        void set_duration(unsigned long duration);
        void set_current_remaining(unsigned long remaining);
        void start();
        void stop();
        bool check();
        bool has_remaining_time();
        unsigned long get_remaining_millis();
};

#endif
