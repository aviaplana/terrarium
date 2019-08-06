#include <Arduino.h>;
#include "timer.h";

void Timer::begin(const unsigned long duration, void (*timer_callback) (void)) {
    current_stop = 0;
    add_stop(duration);
    counter = calculate_stop_millis();
    active = true;
    callback = timer_callback;
}

bool Timer::check() {
    if (is_active() && !has_remaining_time()) {
            callback();
            set_counter_with_next_stop();
            return true;
    } else {
        return false;
    }
}

void Timer::add_stop(unsigned long duration) {
    stop_millis[num_stops] = duration;
    num_stops++;
}

void Timer::stop() {
    active = false;
}

void Timer::start() {
    active = true;
}

void Timer::set_counter_with_next_stop() {
    if (current_stop >= num_stops-1) {
        current_stop = 0;
    } else {
        current_stop++;
    }

    counter = calculate_stop_millis();
}

void Timer::set_current_stop(int stop) {
  current_stop = stop;
  counter = calculate_stop_millis();
}

unsigned long Timer::calculate_stop_millis() {
    return millis() + stop_millis[current_stop];
}

unsigned long Timer::get_remaining_millis() {
    long remaining = counter - millis();

    if (remaining < 0) {
        remaining = 0;
    }

    return (unsigned long) remaining;
}

void Timer::set_current_remaining(unsigned long remaining) {
    counter = millis() + remaining;
}

bool Timer::has_remaining_time() {
    return millis() < counter;
}
