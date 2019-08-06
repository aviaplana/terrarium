#include "wifi.h"
#include <ESP8266WiFi.h>

bool Wifi::connect(struct ConnectionParams params) {
    WiFi.setOutputPower(0);
    WiFi.mode(WIFI_STA);
    WiFi.begin(params.ssid, params.password);
    
    wait_until_connected_or_timeout();

    if (is_connected()) {
        return true;
    } else {
        return false;
    }
}

void Wifi::wait_until_connected_or_timeout() {
    unsigned long elapsed_time = millis();

    while (!is_connected() && (millis() - elapsed_time) < TIMEOUT_CONNECT) {
        delay(500);
    }
    return ;
}

bool Wifi::is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

const char* Wifi::get_ip(){
    String stringIp = WiFi.localIP().toString();
    uint16_t length = stringIp.length() + 1;
    char ip[length];
    stringIp.toCharArray(ip, length);

    return ip;
}
