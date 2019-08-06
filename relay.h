#ifndef RELAY_H
#define RELAY_H

class Relay {
    public:
        Relay(unsigned char pin);
        void begin(bool initial_state);
        void setup();
        void turn_on();
        void turn_off();
        void toggle();
        bool is_on();

    private:
        unsigned char  pin;
};

#endif
