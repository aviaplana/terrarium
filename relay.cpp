#include <Arduino.h>;
#include "relay.h";

Relay::Relay(unsigned char relay_pin) {
    pin = relay_pin;
}

void Relay::begin(bool initial_state) {
    pinMode(pin, OUTPUT);
    
    if (initial_state) {
        turn_on();
    } else {
        turn_off();
    }
}

void Relay::turn_off() {
    digitalWrite(pin, LOW);
}

void Relay::turn_on() {
    digitalWrite(pin, HIGH);
}

void Relay::toggle() {
    digitalWrite(pin, !is_on());
}

bool Relay::is_on() {
    return digitalRead(pin);
}
