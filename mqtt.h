#ifndef MQTT_H
#define MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class Mqtt {
    WiFiClient espClient;
    PubSubClient* client = new PubSubClient(espClient);
    
    public:
        void set_server(char*, uint16_t);
        bool connect(char*, char*, char*);
        bool is_connected();
        void send_message(char*, char*);
        int8_t get_state();
};

#endif
