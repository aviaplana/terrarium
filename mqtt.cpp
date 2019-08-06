#include "mqtt.h"

void Mqtt::set_server(char* host, uint16_t port) {
    client->setServer(host, port);
}

bool Mqtt::connect(char* clientId, char* user, char* password) {
    return client->connect(clientId, user, password);
}

bool Mqtt::is_connected() {
    return client->connected();
}

void Mqtt::send_message(char* topic, char* message) {
    client->publish(topic, message);
}

int8_t Mqtt::get_state() {
    return client->state();
}
