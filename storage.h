#ifndef STORAGE_H
#define STORAGE_H

#define EPROM_MEMORY_SIZE 512

class Storage {
    public:
        void begin();
        unsigned long read_unsigned_long(int address);
        bool read_bool(int address);
        void write(int address, unsigned char* value, int length);
        void write_long(int address, long value);
        void write_bool(int address, bool value);
};

#endif
