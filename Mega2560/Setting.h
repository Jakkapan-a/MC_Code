#ifndef SETTING_MG_H
#define SETTING_MG_H

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
// --------------------- Class --------------------- //
class Setting {
public:
    String deviceName;
    String ssid;
    String password;
    String mqtt_server;
    String mqtt_port;
    String mqtt_user;
    String mqtt_password;
    String mqtt_topic;
    String mqtt_topic_sub;
    String mqtt_topic_pub;
    String ip_address;
    String gateway;
    String subnet;
    String primaryDNS;
    String secondaryDNS;
    String interval;
    String alarm_limit;

  Setting() {
    // Default values
    deviceName = "ESP1";
    ssid = "Internet";
    password = "0987654321qw";
    mqtt_server = "192.168.137.15";
    mqtt_port = "1883";
    mqtt_user = "admin";
    mqtt_password = "test";
    mqtt_topic = "mqtt_topic";
    mqtt_topic_sub = "mqtt_topic_sub";
    mqtt_topic_pub = "mqtt_topic_pub";
    ip_address = "0.0.0.0";
    gateway = "10.10.0.1";
    subnet = "255.255.0.0";
    primaryDNS = "8.8.8.8";
    secondaryDNS = "4.4.4.4";
    interval = "10";
    alarm_limit = "6";
  }

  void readFromSD();

  void writeToSD();

  void updateValue(String key, String value);

private:
  void assignValue(String key, String value);
};

#endif