#include "terrarium_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* TerrariumData::to_string() {
    char* message = (char*) calloc(120, sizeof(char));

    strcat(message, "{\"temperature\": ");
    strcat(message, string_temperature());

    strcat(message, ", \"humidity\": ");
    strcat(message, string_humidity());

    strcat(message, ", \"light\": ");
    strcat(message, string_light());

    strcat(message, ", \"humidifier\": ");
    strcat(message, string_humidifier());

    strcat(message, ", \"fan_intake\": ");
    strcat(message, string_fan_intake());

    strcat(message, ", \"fan_flow\": ");
    strcat(message, string_fan_flow());

    strcat(message, ", \"goal\": ");
    strcat(message, string_goal_humidity());
    
    strcat(message, "}");

    return message;
}

bool TerrariumData::has_valid_data() {
    return humidity != INVALID_DATA;
}

const char* TerrariumData::string_temperature() {
    if (temperature != INVALID_DATA) {
        char char_temp[6];
        gcvt(temperature, 4, char_temp);
        return char_temp;
    } else {
        return "nan";
    }
}

const char* TerrariumData::string_humidity() {
    if (humidity != INVALID_DATA) {
        char char_humidity[6];
        gcvt(humidity, 4, char_humidity);
        return char_humidity;
    } else {
        return "nan";
    }
}

const char* TerrariumData::string_light() {
    return light ? "1" : "0";
}

const char* TerrariumData::string_fan_intake() {
    return fan_intake ? "1" : "0";
}

const char* TerrariumData::string_fan_flow() {
    return fan_flow ? "1" : "0";
}

const char* TerrariumData::string_humidifier() {
    return humidifier ? "1" : "0";
}

const char* TerrariumData::string_goal_humidity() {
    char char_goal[4];
    sprintf(char_goal, "%d", goal_humidity);
    return char_goal;
}
