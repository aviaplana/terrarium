#ifndef WIFI_H
#define WIFI_H

#define TIMEOUT_CONNECT 10000

class Wifi {
    private:
        void wait_until_connected_or_timeout();

    public:
        struct ConnectionParams { 
            char ssid[20];
            char password[20];
        };
        bool connect(struct ConnectionParams);
        const char* get_ip();
        bool is_connected();
};

#endif