#ifndef TERRARIUM_DATA_H
#define TERRARIUM_DATA_H

struct TerrariumData {
        float temperature;
        float humidity;
        int goal_humidity;
        bool light;
        bool humidifier;
        bool fan_flow;
        bool fan_intake;

        const float INVALID_DATA =  -99.;

        char* to_string();
        bool has_valid_data();
        const char* string_temperature();
        const char* string_humidity();
        const char* string_goal_humidity();
        const char* string_light();
        const char* string_fan_flow();
        const char* string_fan_intake();
        const char* string_humidifier();
        bool is_light_on() { return light; }
        bool is_humidifier_on() { return humidifier; }
        bool is_fan_flow_on() { return fan_flow; }
        bool is_fan_intake_on() { return fan_intake; }
};

#endif