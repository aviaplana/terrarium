#include "storage.h";
#include <EEPROM.h>

void Storage::begin() {
  EEPROM.begin(EPROM_MEMORY_SIZE);
}

void Storage::write_long(int address, long value) {
    int size = sizeof(value);
    unsigned char result_array[size];

    for (int i = 0; i < size; i++) {
      result_array[i] = (value >> (i * 8));
    }

    write(address, result_array, size);
}

void Storage::write_bool(int address, bool value) {
    unsigned char array_bytes[1] = {value};
    write(address, array_bytes, 1);
}

void Storage::write(int address, unsigned char* value, int length) {
    unsigned char mask = 0xFF;
    unsigned char result;

    for (int i = 0; i < length; i++) {
        result = value[i]  & mask;

        if (EEPROM.read(address + i) != result) {
            EEPROM.write(address + i, result);
        }
    }

    EEPROM.commit();
}

unsigned long Storage::read_unsigned_long(int address) {
    unsigned long value = 0;

    for (int i = 0; i < sizeof(value); i++) {
        value |= (EEPROM.read(address + i) << (i * 8));
    }

    return value;
}

bool Storage::read_bool(int address) {
    bool result = EEPROM.read(address);
    return result;
}
