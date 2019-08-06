#ifndef BME280_H
#define BME280_H

#include <BME280I2C.h>

class Bme280 {
    BME280I2C bme; 

    public:
        float get_temperature();
        float get_humidity();
        float get_pressure();
        bool begin();
};

#endif
