#include "Setting.h"

void Setting::readFromSD() {
   File dataFile = SD.open("settings.txt", FILE_READ);
  if (dataFile) {
    while (dataFile.available()) {
      String line = dataFile.readStringUntil('\n');
      int separatorIndex = line.indexOf('=');
      String key = line.substring(0, separatorIndex);
      String value = line.substring(separatorIndex + 1);
      assignValue(key, value);
    }
    dataFile.close();
  } else {
    // If file doesn't exist, create a new one with default values
    writeToSD();
  }
}

void Setting::writeToSD() {
      // First, remove the old file
  SD.remove("settings.txt");
    // Then, create a new one
  File dataFile = SD.open("settings.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("deviceName=" + deviceName);
    dataFile.println("ssid=" + ssid);
    dataFile.println("password=" + password);
    dataFile.println("mqtt_server=" + mqtt_server);
    dataFile.println("mqtt_port=" + mqtt_port);
    dataFile.println("mqtt_user=" + mqtt_user);
    dataFile.println("mqtt_password=" + mqtt_password);
    dataFile.println("mqtt_topic=" + mqtt_topic);
    dataFile.println("mqtt_topic_sub=" + mqtt_topic_sub);
    dataFile.println("mqtt_topic_pub=" + mqtt_topic_pub);
    dataFile.println("ip_address=" + ip_address);
    dataFile.println("gateway=" + gateway);
    dataFile.println("subnet=" + subnet);
    dataFile.println("primaryDNS=" + primaryDNS);
    dataFile.println("secondaryDNS=" + secondaryDNS);
    dataFile.println("time_update=" + time_update);
    dataFile.println("alarm_limit=" + alarm_limit);
    dataFile.close();
  } else {
    Serial.println("Error opening file");
  }
}

void Setting::updateValue(String key, String value) {
  assignValue(key, value);
  writeToSD();
}

void Setting::assignValue(String key, String value){
  if (key == "deviceName") {
    deviceName = value;
  } else if (key == "ssid") {
    ssid = value;
  } else if (key == "password") {
    password = value;
  } else if (key == "mqtt_server") {
    mqtt_server = value;
  } else if (key == "mqtt_port") {
    mqtt_port = value;
  } else if (key == "mqtt_user") {
    mqtt_user = value;
  } else if (key == "mqtt_password") {
    mqtt_password = value;
  } else if (key == "mqtt_topic") {
    mqtt_topic = value;
  } else if (key == "mqtt_topic_sub") {
    mqtt_topic_sub = value;
  } else if (key == "mqtt_topic_pub") {
    mqtt_topic_pub = value;
  } else if (key == "ip_address") {
    ip_address = value;
  } else if (key == "gateway") {
    gateway = value;
  } else if (key == "subnet") {
    subnet = value;
  } else if (key == "primaryDNS") {
    primaryDNS = value;
  } else if (key == "secondaryDNS") {
    secondaryDNS = value;
  } else if (key == "time_update") {
    time_update = value;
  } else if (key == "alarm_limit") {
    alarm_limit = value;
  }
}
