#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Setting.h"
// Update these with values suitable for your network.
const char *ssid = "Internet";
const char *password = "0987654321qw";
const char *mqtt_server = "broker_ip_address";

// Set your static IP address
IPAddress local_IP(192, 168, 1, 184);  // your static ip
IPAddress gateway(192, 168, 1, 1);     // your gateway (router) ip
IPAddress subnet(255, 255, 255, 0);    // your network subnet
IPAddress primaryDNS(8, 8, 8, 8);      // optional: your preferred DNS server
IPAddress secondaryDNS(8, 8, 4, 4);    // optional: secondary DNS server

// --------------------- Variable --------------------- //
const byte START_BYTE = 0x02;
const byte END_BYTE = 0x03;

bool startReceived = false;
bool endReceived = false;
const int LENGTH = 8;
byte _data[LENGTH];
int dataIndex = 0;

const int _OFFSET = 10000;
const int BUFFER_SIZE = 512;  // Adjust this based on your needs
byte _buffer[BUFFER_SIZE];
int bufferIndex = 0;

Setting setting;
// --------------------- Function --------------------- //
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Hostname defaults to esp8266-[ChipID]
  String hostname = "ESP8266-Device-01";
  WiFi.hostname(hostname);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_wifi(const String &ssid, const String &password) {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Hostname defaults to esp8266-[ChipID]
  String hostname = setting.deviceName;
  WiFi.hostname(hostname);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  byte data[8] = { 0x02, 0x55, 0x53, 0x00, 0x00, 0x00, 0xA1, 0x03 };
  Serial.write(data, sizeof(data));
}

void serialEvent() {
  while (Serial.available()) {
    byte incomeByte = Serial.read();

    if (incomeByte == START_BYTE) {
      startReceived = true;
      endReceived = false;
      dataIndex = 0;
      _data[dataIndex] = incomeByte;
    } else if (startReceived && dataIndex < LENGTH - 1) {
      dataIndex++;
      _data[dataIndex] = incomeByte;
      if (incomeByte == END_BYTE) {
        handleDataChunk(_data, dataIndex + 1);  // Include the 0x03 byte in this case
        startReceived = false;
        endReceived = true;
        memset(_data, 0, sizeof(_data));
      } else if (dataIndex >= LENGTH - 1) {
        handleDataChunk(_data, LENGTH);
        // Reset for the next chunk
        dataIndex = -1;  // Setting to -1 because it will be incremented in the next loop cycle
        memset(_data, 0, sizeof(_data));
      }
    }
  }
}

void handleDataChunk(byte *data, int len) {
  if (bufferIndex + len > BUFFER_SIZE) {
    // Buffer overflow handling
    // For this example, we'll just reset the buffer,
    // but you might want to handle this differently
    bufferIndex = 0;
  }
  // Copy the data to the buffer
  for (int i = 0; i < len; i++) {
    _buffer[bufferIndex] = data[i];
    bufferIndex++;
  }
}

void setup() {
  Serial.begin(115200);
  // setup_wifi();
  delay(1000);
  byte data[8] = { 0x02, 0x55, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03 };
  Serial.write(data, sizeof(data));
}

void loop() {
  handleBufferedData();
}

// ------------------  FUNCTIONS SERIAL  ------------------ //
void handleBufferedData() {
  if (!endReceived) {
    return;
  }
  if (_buffer[0] == START_BYTE && _buffer[bufferIndex - 1] == END_BYTE) {
    processBufferedMessage();
  }
  resetBuffer();
}

void processBufferedMessage() {
  if (bufferIndex == 8) {
    // Command
    CommandSerial();  
    // Update
    UpdateParameter();
  } else if (_buffer[1] == 0x43 && _buffer[2] == 0x44) {
  } else {
    UpdateParameter();
    CommandSerial();
  }
  // Serial.write(_buffer, bufferIndex);
}
void CommandSerial(){
  if(_buffer[1] != 0x43){
    return;
  }

  if(_buffer[2] == 0xC0){
    // Connect wifi
    setup_wifi(setting.ssid, setting.password);
  }
}
void UpdateParameter() {
  // if new 55 B*
  if (_buffer[1] != 0x55) {
    return;
  }
  if (_buffer[2] == 0xB1) {
    // Update device name
    String deviceName = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      deviceName += (char)_buffer[i];
    }
    setting.deviceName = deviceName;
    ;
  } else if (_buffer[2] == 0xB2) {
    // Update SSID
    String ssid = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      ssid += (char)_buffer[i];
    }
    setting.ssid = ssid;
  } else if (_buffer[2] == 0xB3) {
    // Update password
    String password = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      password += (char)_buffer[i];
    }
    setting.password = password;
  } else if (_buffer[2] == 0xB4) {
    // Update mqtt_server
    String mqtt_server = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_server += (char)_buffer[i];
    }
    setting.mqtt_server = mqtt_server;
  } else if (_buffer[2] == 0xB5) {
    // Update mqtt_port
    String mqtt_port = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_port += (char)_buffer[i];
    }
    setting.mqtt_port = mqtt_port;
  } else if (_buffer[2] == 0xB6) {
    // Update mqtt_user
    String mqtt_user = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_user += (char)_buffer[i];
    }
    setting.mqtt_user = mqtt_user;
  } else if (_buffer[2] == 0xB7) {
    // Update mqtt_password
    String mqtt_password = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_password += (char)_buffer[i];
    }
    setting.mqtt_password = mqtt_password;
  } else if (_buffer[2] == 0xB8) {
    // Update mqtt_topic
    String mqtt_topic = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_topic += (char)_buffer[i];
    }
    setting.mqtt_topic = mqtt_topic;
  } else if (_buffer[2] == 0xB9) {
    // Update mqtt_topic_sub
    String mqtt_topic_sub = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_topic_sub += (char)_buffer[i];
    }
    setting.mqtt_topic_sub = mqtt_topic_sub;
  } else if (_buffer[2] == 0xBA) {
    // Update mqtt_topic_pub
    String mqtt_topic_pub = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      mqtt_topic_pub += (char)_buffer[i];
    }
    setting.mqtt_topic_pub = mqtt_topic_pub;
  } else if (_buffer[2] == 0xBB) {
    // Update ip_address
    String ip_address = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      ip_address += (char)_buffer[i];
    }
    setting.ip_address = ip_address;
  } else if (_buffer[2] == 0xBC) {
    // Update gateway
    String gateway = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      gateway += (char)_buffer[i];
    }
    setting.gateway = gateway;
  } else if (_buffer[2] == 0xBD) {
    // Update subnet
    String subnet = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      subnet += (char)_buffer[i];
    }
    setting.subnet = subnet;
  } else if (_buffer[2] == 0xBE) {
    // Update primaryDNS
    String primaryDNS = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      primaryDNS += (char)_buffer[i];
    }
    setting.primaryDNS = primaryDNS;
  } else if (_buffer[2] == 0xBF) {
    // Update secondaryDNS
    String secondaryDNS = "";
    for (int i = 3; i < bufferIndex - 1; i++) {
      secondaryDNS += (char)_buffer[i];
    }
    setting.secondaryDNS = secondaryDNS;
  } else if (_buffer[2] == 0xC0) {
   
  }
}

void resetBuffer() {
  endReceived = false;
  bufferIndex = 0;
  memset(_buffer, 0, BUFFER_SIZE);
}
