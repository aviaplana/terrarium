
#include "mqtt.h"
#include "config.h"
#include "wifi.h"
#include "relay.h"
#include "bme280.h"
#include "timer.h"
#include "terrarium_data.h"
#include "storage.h"

/*
 * 
 * TEMP -> 21 - 24 º
 * HUM -> 95 - 100 %
 * FAN -> 2 - 3 TIMES
 */

/*
 */

#define EEPROM_MILLIS_ADDRESS 0
#define EEPROM_LIGHT_ADDRESS 4
#define RESET_BUTTON_PIN D4
#define HUMIDIRY_TOLERANCE 2

#define LIGHT_TIME 43200000           // 12 HOURS
#define FAN_FLOW_ON_TIME 60000        // 1 MINUTE
#define FAN_FLOW_OFF_TIME 3600000     // 1 HOUR
#define FAN_INTAKE_ON_TIME 300000     // 5 MINUTES
#define FAN_INTAKE_OFF_TIME 28800000  // 8 HOURS
#define RECONNECT_TIME 5000           // 5 SECONDS
#define PUBLISH_TIME 5000             // 5 SECONDS
#define READ_TIME 5000                // 5 SECONDS
#define INTERRUPT_TIME 5000           // 500 MILLISECONDS
#define STORE_TIME 600000             // 10 MINUTES

#define GOAL_HUMIDITY 97

Wifi wifi;
Mqtt mqtt;
Bme280 sensor;
Relay light(D7);
Relay fan_intake(D5);
Relay fan_flow(D6);
Relay humidifier(D8);
TerrariumData data;
Storage storage;

Timer timer_light;
Timer timer_fan_intake;
Timer timer_fan_flow;
Timer timer_reconnect;
Timer timer_check_sensor;
Timer timer_publish;
Timer timer_interrupt;
Timer timer_store_data;

const float min_humidity = 95.0;
const float max_humidity = 98.0;

void setup() {
  Serial.begin(9600);
  data.goal_humidity = GOAL_HUMIDITY;
  storage.begin();
  initialize_timers();
  initialize_relays();
  initialize_interruptions();
  connect_wifi_and_mqtt();
  read_stored_status();
  wait_unitl_sensor_initializes();
  
  delay(100);
}

void initialize_timers() {
  timer_light.begin(LIGHT_TIME, toggle_light_and_store_status);
  timer_fan_intake.begin(FAN_INTAKE_OFF_TIME, callback_fan_intake);
  timer_fan_flow.add_stop(FAN_INTAKE_ON_TIME);
  timer_fan_flow.begin(FAN_FLOW_OFF_TIME, callback_fan_flow);
  timer_fan_flow.add_stop(FAN_FLOW_ON_TIME);
  timer_reconnect.begin(RECONNECT_TIME, reconnect_if_required);
  timer_check_sensor.begin(READ_TIME, read_sensor_and_operate_humidifier);
  timer_publish.begin(PUBLISH_TIME, publish_data_mqtt);
  timer_interrupt.begin(INTERRUPT_TIME, toggle_light_and_store_status);
  timer_store_data.begin(STORE_TIME, store_data);
}

void callback_fan_intake() {
  Serial.println("Toggle fan intake.");
  fan_intake.toggle();
  data.fan_intake = fan_intake.is_on();
}

void callback_fan_flow() {
  Serial.println("Toggle fan flow.");
  fan_flow.toggle();
  data.fan_flow = fan_flow.is_on();
}

void initialize_relays() {
  light.begin(true);
  fan_intake.begin(false);
  fan_flow.begin(false);
  humidifier.begin(true);
}

void toggle_light_and_store_status() {
  Serial.println("Toggle light.");
  light.toggle();
  data.light = light.is_on();
  store_data();
}

void initialize_interruptions() {
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RESET_BUTTON_PIN), interruption_reset_button, RISING);
}

void interruption_reset_button() {
  timer_interrupt.start();
}

void wait_unitl_sensor_initializes() {
  while(!sensor.begin()) {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}

bool connect_wifi_and_mqtt() {
    if (!wifi.is_connected()) {
      if (connect_wifi()) {
        Serial.println(F("Success!"));
      } else {
        Serial.println(F("Couldn't connect."));
      }
    }

    if (wifi.is_connected()) {
      bool success = connect_mqtt();
      
      if (!success) {
        Serial.print(F("Trying again in "));
        Serial.print(RECONNECT_TIME / 1000);
        Serial.println(F(" seconds"));
      } 
    
      return true;
    } 

    return false;
}

bool connect_wifi() {
  Serial.print(F("Connecting to Wifi..."));
  struct Wifi::ConnectionParams params;
   strcpy(params.ssid, WIFI_SSID);
   strcpy(params.password, WIFI_PASSWORD);
   return wifi.connect(params);
}

bool connect_mqtt() {
  Serial.print(F("Attempting MQTT connection as "));
  Serial.print(MQTT_USER);
  Serial.print(F("... "));

  bool success = mqtt.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD);
  
  if (success) {
    Serial.println(F("Connected successfully!"));
  } else {
    Serial.print(F("Couldn't connect. RC="));
    Serial.print(mqtt.get_state());
    Serial.print(F("\n"));
  }

  return success;
}

void reconnect_if_required() {
  if (!mqtt.is_connected()) {
    connect_wifi_and_mqtt();
  }
}

void publish_data_mqtt() {
  Serial.println(F("Sending data to mqtt..."));
 
  char* message = data.to_string();
  mqtt.send_message(MQTT_TOPIC, message);
                                
  Serial.print(F("Sent: "));
  Serial.println(message);

  free(message);
}

void store_data() {
  storage.write_long(EEPROM_MILLIS_ADDRESS, timer_light.get_remaining_millis());
  storage.write_bool(EEPROM_LIGHT_ADDRESS, light.is_on());
}

// TODO: Implement PID
void read_sensor_and_operate_humidifier() {
    read_sensor_data();
    
    if (data.has_valid_data()) {    
      set_humidifier_if_required();
      print_data();
    } else {
      Serial.println(F("Failed to read from sensor"));
    }
}

void read_sensor_data() {
  Serial.println("Reading sensor...");

  data.temperature = sensor.get_temperature();
  data.humidity = sensor.get_humidity();
}

void set_humidifier_if_required() {
  float humidity = data.humidity;
  float goal_humidity = data.goal_humidity;

  if (is_humidity_above_goal_range()) {
    turn_humidifier_on_and_set_data();
  } else if (is_humidity_below_goal_range()) {
    turn_humidifier_off_and_set_data();
  }
}

bool is_humidity_above_goal_range() {
  return (data.humidity - data.goal_humidity) > HUMIDIRY_TOLERANCE;
}

void turn_humidifier_on_and_set_data() { 
  Serial.println("Turning on humidifier");
  humidifier.turn_on();
  fan_flow.turn_on();
  timer_fan_flow.stop();
  data.humidifier = true;
  data.fan_flow = true;
}

bool is_humidity_below_goal_range() {
  return data.humidity <= (data.goal_humidity - HUMIDIRY_TOLERANCE);
}

void turn_humidifier_off_and_set_data() {
  Serial.println("Turning off humidifier");
  humidifier.turn_off();
  set_fan_flow_off();
  timer_fan_flow.start();
  data.humidifier = false;
  data.fan_flow = false;
}

void set_fan_flow_off() {
  fan_flow.turn_off();
  timer_fan_flow.set_current_stop(0);
}

void print_data() {
  char* string_data = data.to_string();
  Serial.println(string_data);
  free(string_data);
}

void read_stored_status() {
  unsigned long remaining = storage.read_unsigned_long(EEPROM_MILLIS_ADDRESS);
  timer_light.set_current_remaining(remaining);

  bool light_status = storage.read_bool(EEPROM_LIGHT_ADDRESS);

  if (light_status) {
    light.turn_on();
  } else {
    light.turn_off();
  }

  Serial.print(F("Readed from storage -> light_stat: "));
  Serial.print(light_status ? "1" : "0");
  Serial.print(F(" Remaining millis: "));
  Serial.println(remaining);
}

void loop() {
  if (timer_interrupt.check()) {
    timer_interrupt.stop();
  }

  timer_reconnect.check();
  timer_check_sensor.check();
  timer_light.check();
  timer_fan_flow.check();
  timer_fan_intake.check();
  timer_store_data.check();

  delay(100);
}

