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
    String time_update;
    String alarm_limit;
private:
};

#endif