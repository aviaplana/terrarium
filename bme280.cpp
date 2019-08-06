#include "bme280.h"
#include <BME280I2C.h>
#include <Wire.h>

bool Bme280::begin() {
    Wire.begin();
    return bme.begin();
}

float Bme280::get_temperature() {
    return bme.temp(BME280::TempUnit_Celsius);
}

float Bme280::get_humidity() {
    return bme.hum();
}

float Bme280::get_pressure() {
    return bme.pres(BME280::PresUnit_Pa);
}
