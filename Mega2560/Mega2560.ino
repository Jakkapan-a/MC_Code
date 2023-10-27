#include <TcBUTTON.h>
#include <TcPINOUT.h>
#include <LiquidCrystal_I2C.h>
#include <AcVoltage.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Setting.h"
// ------------------   INPUT   ------------------ //
LiquidCrystal_I2C lcd(0x27, 16, 2); // Module IIC/I2C Interface บางรุ่นอาจจะใช้ 0x3f

#define BTN_ESC_PIN 35
void OnPress_ESC(void);
void OnRelease_ESC(void);
TcBUTTON BTN_ESC(BTN_ESC_PIN);

#define BTN_UP_PIN 36
void OnPress_UP(void);
void OnRelease_UP(void);
TcBUTTON BTN_UP(BTN_UP_PIN);

#define BTN_DOWN_PIN 37
void OnPress_DOWN(void);
void OnRelease_DOWN(void);
TcBUTTON BTN_DOWN(BTN_DOWN_PIN);

#define BTN_ENTER_PIN 38
void OnPress_ENTER(void);
void OnRelease_ENTER(void);
TcBUTTON BTN_ENTER(BTN_ENTER_PIN);

#define SD_CS 10   // | Mega 2560
#define SD_MOSI 51 // | COPI 51
#define SD_MISO 50 // | CIPO 50
#define SD_SCK 52  // |  CSK 52

// ------------------  Analog INPUT   ------------------ //
#define AC_VOLTAGE1_PIN A1
AcVoltage acVoltageA1(AC_VOLTAGE1_PIN);

#define AC_VOLTAGE2_PIN A2
AcVoltage acVoltageA2(AC_VOLTAGE2_PIN);

#define AC_VOLTAGE3_PIN A3
AcVoltage acVoltageA3(AC_VOLTAGE3_PIN);

#define AC_VOLTAGE4_PIN A4
AcVoltage acVoltageA4(AC_VOLTAGE4_PIN);

#define AC_VOLTAGE5_PIN A5
AcVoltage acVoltageA5(AC_VOLTAGE5_PIN);

#define AC_VOLTAGE6_PIN A6
AcVoltage acVoltageA6(AC_VOLTAGE6_PIN);

#define AC_VOLTAGE7_PIN A7
AcVoltage acVoltageA7(AC_VOLTAGE7_PIN);

#define AC_VOLTAGE8_PIN A8
AcVoltage acVoltageA8(AC_VOLTAGE8_PIN);

#define AC_VOLTAGE9_PIN A9

AcVoltage acVoltageA9(AC_VOLTAGE9_PIN);

#define AC_VOLTAGE10_PIN A10
AcVoltage acVoltageA10(AC_VOLTAGE10_PIN);

// ------------------  OUTPUT   ------------------ //
#define LED1_PIN 22
TcPINOUT LED1(LED1_PIN);

#define LED2_PIN 23
TcPINOUT LED2(LED2_PIN);

#define LED3_PIN 24
TcPINOUT LED3(LED3_PIN);

#define LED4_PIN 25
TcPINOUT LED4(LED4_PIN);

#define LED5_PIN 26
TcPINOUT LED5(LED5_PIN);

#define LED6_PIN 27
TcPINOUT LED6(LED6_PIN);

#define LED7_PIN 28
TcPINOUT LED7(LED7_PIN);

#define LED8_PIN 29
TcPINOUT LED8(LED8_PIN);

#define LED9_PIN 30
TcPINOUT LED9(LED9_PIN);

#define LED10_PIN 31
TcPINOUT LED10(LED10_PIN);

#define BUZZER_PIN 7
// ------------------ VARIABLES ------------------ //
unsigned long previousMillis = 0;
unsigned long previousMicros = 0;
int CountNoBacklight = 0;
const int NoBacklightTime = 60; // 60 seconds
int countBackHome = 0;
const int countBackHomeTime = 120; // 60 seconds

bool startReceived = false;
bool endReceived = false;
const int LENGTH = 8;
byte _data[LENGTH];
int dataIndex = 0;

const int _OFFSET = 10000;
const int BUFFER_SIZE = 512; // Adjust this based on your needs
byte _buffer[BUFFER_SIZE];
int bufferIndex = 0;

const byte START_BYTE = 0x02;
const byte END_BYTE = 0x03;

bool stateBTN_ESC = false;
bool stateBTN_UP = false;
bool stateBTN_DOWN = false;
bool stateBTN_ENTER = false;

int currentMenu = 0;
int currentSubMenu = 0;
int cursorPosition = 0;

Setting setting;

int status = 0; // 1.setup 2.update parameter 3.connect wifi 4.connect mqtt 5.ready
int countUpdateParameter = 0;
unsigned long previousMillisUpdateParameter = 0;
const int timeUpdateParameter = 20; // 20ms
// ------------------   FUNCTIONS   ------------------ //
void serialEvent()
{
  Serial1.print("Mega1 :");
  while (Serial.available())
  {
    byte incomeByte = Serial.read();

  }

}
String incomeString = "";
void serialEvent3()
{
  Serial.print("Mega3 :");
  
  while (Serial3.available())
  {
    byte incomeByte = Serial3.read();
    // Serial.print((char)incomeByte);
    incomeString += (char)incomeByte;

    if (incomeByte == START_BYTE)
    {
      startReceived = true;
      endReceived = false;
      dataIndex = 0;
      _data[dataIndex] = incomeByte;
    }
    else if (startReceived && dataIndex < LENGTH - 1)
    {
      dataIndex++;
      _data[dataIndex] = incomeByte;
      if (incomeByte == END_BYTE)
      {
        handleDataChunk(_data, dataIndex + 1); // Include the 0x03 byte in this case
        startReceived = false;
        endReceived = true;
        memset(_data, 0, sizeof(_data));
      }
      else if (dataIndex >= LENGTH - 1)
      {
        handleDataChunk(_data, LENGTH);
        // Reset for the next chunk
        dataIndex = -1; // Setting to -1 because it will be incremented in the next loop cycle
        memset(_data, 0, sizeof(_data));
      }
    }
  }
  Serial.println(incomeString);
  incomeString = "";
}

void handleDataChunk(byte *data, int len)
{
  if (bufferIndex + len > BUFFER_SIZE)
  {
    // Buffer overflow handling
    // For this example, we'll just reset the buffer,
    // but you might want to handle this differently
    bufferIndex = 0;
  }
  // Copy the data to the buffer
  for (int i = 0; i < len; i++)
  {
    _buffer[bufferIndex] = data[i];
    bufferIndex++;
  }
}

// ------------------   SETUP   ------------------ //
void setup()
{
  Serial.begin(115200);
  Serial3.begin(115200);

  acVoltageA1.begin();
  acVoltageA2.begin();
  acVoltageA3.begin();
  acVoltageA4.begin();
  acVoltageA5.begin();
  acVoltageA6.begin();
  acVoltageA7.begin();
  acVoltageA8.begin();
  acVoltageA9.begin();
  acVoltageA10.begin();

  BTN_ESC.setOnPress(OnPress_ESC);
  BTN_ESC.setOnRelease(OnRelease_ESC);

  BTN_UP.setOnPress(OnPress_UP);
  BTN_UP.setOnRelease(OnRelease_UP);

  BTN_DOWN.setOnPress(OnPress_DOWN);
  BTN_DOWN.setOnRelease(OnRelease_DOWN);

  BTN_ENTER.setOnPress(OnPress_ENTER);
  BTN_ENTER.setOnRelease(OnRelease_ENTER);

  analogReference(INTERNAL1V1);

  lcd.begin();
  lcd.backlight();
  // lcd.noBacklight();
  lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("RMS Voltage:");
  // lcd.setCursor(0, 1);
  // lcd.clear();
  // lcd.print("RMS Voltage:");
  // lcd.setCursor(0, 1);
  // lcd.cursor();
  CountNoBacklight = NoBacklightTime;

  if (!SD.begin(SD_CS))
  {
    Serial.println("SD card not found");
    lcd.setCursor(0, 0);
    lcd.print("SD Card");
    lcd.setCursor(0, 1);
    lcd.print("Not found!!");
    while (1)
      ;
  }

  setting.readFromSD();
}
// ------------------   LOOP   ------------------ //
void loop()
{

  handleBufferedData();
  if (status == 1)
  {
    // Send parameter to ESPs
    updateParameterToESP();
    String newDataLine1 = "Connect to WiFi";
    String newDataLine2 = "SSID: " + setting.ssid;

    // Remove \r and \n
    newDataLine1.replace("\r", "");
    newDataLine1.replace("\n", "");
    newDataLine2.replace("\r", "");
    newDataLine2.replace("\n", "");

    // Update to LCD
    updateLCD(newDataLine1, newDataLine2);
  }else if(status == 2){
    // Connect to WiFi
  }
  else if (status == 5)
  {
    BTN_ESC.update();
    BTN_UP.update();
    BTN_DOWN.update();
    BTN_ENTER.update();

    acVoltageA1.update();
    acVoltageA2.update();
    acVoltageA3.update();
    acVoltageA4.update();
    acVoltageA5.update();
    acVoltageA6.update();
    acVoltageA7.update();
    acVoltageA8.update();
    acVoltageA9.update();
    acVoltageA10.update();

    SecondsCounter();
    millisCounter();
  }
}
void updateParameterToESP()
{
  unsigned long currentMillisUpdateParameter = millis();
  if (currentMillisUpdateParameter - previousMillisUpdateParameter >= timeUpdateParameter)
  {
    previousMillisUpdateParameter = currentMillisUpdateParameter;
    countUpdateParameter++;
    if (countUpdateParameter == 1)
    {
      // Device name
      // 02 55 ***** 03
      String deviceName = setting.deviceName;
      // Remove \r and \n
      deviceName.replace("\r", "");
      deviceName.replace("\n", "");

      // Convert to ASCII bytes
      byte deviceNameBytes[deviceName.length()];
      stringToAsciiBytes(deviceName, deviceNameBytes, sizeof(deviceNameBytes));

      // Send to ESP
      byte data[sizeof(deviceNameBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB1;
      memcpy(&data[3], deviceNameBytes, sizeof(deviceNameBytes));
      data[sizeof(deviceNameBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 2)
    {
      // SSID
      // 02 55 ***** 03
      String ssid = setting.ssid;
      // Remove \r and \n
      ssid.replace("\r", "");
      ssid.replace("\n", "");

      // Convert to ASCII bytes
      byte ssidBytes[ssid.length()];
      stringToAsciiBytes(ssid, ssidBytes, sizeof(ssidBytes));

      // Send to ESP
      byte data[sizeof(ssidBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB2;
      memcpy(&data[3], ssidBytes, sizeof(ssidBytes));
      data[sizeof(ssidBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 3)
    {
      // Password
      // 02 55 ***** 03
      String password = setting.password;
      // Remove \r and \n
      password.replace("\r", "");
      password.replace("\n", "");

      // Convert to ASCII bytes
      byte passwordBytes[password.length()];
      stringToAsciiBytes(password, passwordBytes, sizeof(passwordBytes));

      // Send to ESP
      byte data[sizeof(passwordBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB3;
      memcpy(&data[3], passwordBytes, sizeof(passwordBytes));
      data[sizeof(passwordBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 4)
    {
      // MQTT Server
      // 02 55 ***** 03
      String mqtt_server = setting.mqtt_server;
      // Remove \r and \n
      mqtt_server.replace("\r", "");
      mqtt_server.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_serverBytes[mqtt_server.length()];
      stringToAsciiBytes(mqtt_server, mqtt_serverBytes, sizeof(mqtt_serverBytes));

      // Send to ESP
      byte data[sizeof(mqtt_serverBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB4;
      memcpy(&data[3], mqtt_serverBytes, sizeof(mqtt_serverBytes));
      data[sizeof(mqtt_serverBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 5)
    {
      // MQTT Port
      // 02 55 ***** 03
      String mqtt_port = setting.mqtt_port;
      // Remove \r and \n
      mqtt_port.replace("\r", "");
      mqtt_port.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_portBytes[mqtt_port.length()];
      stringToAsciiBytes(mqtt_port, mqtt_portBytes, sizeof(mqtt_portBytes));

      // Send to ESP
      byte data[sizeof(mqtt_portBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB5;
      memcpy(&data[3], mqtt_portBytes, sizeof(mqtt_portBytes));
      data[sizeof(mqtt_portBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 6)
    {
      // MQTT User
      // 02 55 ***** 03
      String mqtt_user = setting.mqtt_user;
      // Remove \r and \n
      mqtt_user.replace("\r", "");
      mqtt_user.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_userBytes[mqtt_user.length()];
      stringToAsciiBytes(mqtt_user, mqtt_userBytes, sizeof(mqtt_userBytes));

      // Send to ESP
      byte data[sizeof(mqtt_userBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB6;
      memcpy(&data[3], mqtt_userBytes, sizeof(mqtt_userBytes));
      data[sizeof(mqtt_userBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 7)
    {
      // MQTT Password
      // 02 55 ***** 03
      String mqtt_password = setting.mqtt_password;
      // Remove \r and \n
      mqtt_password.replace("\r", "");
      mqtt_password.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_passwordBytes[mqtt_password.length()];
      stringToAsciiBytes(mqtt_password, mqtt_passwordBytes, sizeof(mqtt_passwordBytes));

      // Send to ESP
      byte data[sizeof(mqtt_passwordBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB7;
      memcpy(&data[3], mqtt_passwordBytes, sizeof(mqtt_passwordBytes));
      data[sizeof(mqtt_passwordBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 8)
    {
      // MQTT Topic
      // 02 55 ***** 03
      String mqtt_topic = setting.mqtt_topic;
      // Remove \r and \n
      mqtt_topic.replace("\r", "");
      mqtt_topic.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_topicBytes[mqtt_topic.length()];
      stringToAsciiBytes(mqtt_topic, mqtt_topicBytes, sizeof(mqtt_topicBytes));

      // Send to ESP
      byte data[sizeof(mqtt_topicBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB8;
      memcpy(&data[3], mqtt_topicBytes, sizeof(mqtt_topicBytes));
      data[sizeof(mqtt_topicBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 9)
    {
      // MQTT Topic Sub
      // 02 55 ***** 03
      String mqtt_topic_sub = setting.mqtt_topic_sub;
      // Remove \r and \n
      mqtt_topic_sub.replace("\r", "");
      mqtt_topic_sub.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_topic_subBytes[mqtt_topic_sub.length()];
      stringToAsciiBytes(mqtt_topic_sub, mqtt_topic_subBytes, sizeof(mqtt_topic_subBytes));

      // Send to ESP
      byte data[sizeof(mqtt_topic_subBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xB9;
      memcpy(&data[3], mqtt_topic_subBytes, sizeof(mqtt_topic_subBytes));
      data[sizeof(mqtt_topic_subBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 10)
    {
      // MQTT Topic Pub
      // 02 55 ***** 03
      String mqtt_topic_pub = setting.mqtt_topic_pub;
      // Remove \r and \n
      mqtt_topic_pub.replace("\r", "");
      mqtt_topic_pub.replace("\n", "");

      // Convert to ASCII bytes
      byte mqtt_topic_pubBytes[mqtt_topic_pub.length()];
      stringToAsciiBytes(mqtt_topic_pub, mqtt_topic_pubBytes, sizeof(mqtt_topic_pubBytes));

      // Send to ESP
      byte data[sizeof(mqtt_topic_pubBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xBA;
      memcpy(&data[3], mqtt_topic_pubBytes, sizeof(mqtt_topic_pubBytes));
      data[sizeof(mqtt_topic_pubBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 11)
    {
      // IP Address
      // 02 55 ***** 03
      String ip_address = setting.ip_address;
      // Remove \r and \n
      ip_address.replace("\r", "");
      ip_address.replace("\n", "");

      // Convert to ASCII bytes
      byte ip_addressBytes[ip_address.length()];
      stringToAsciiBytes(ip_address, ip_addressBytes, sizeof(ip_addressBytes));

      // Send to ESP
      byte data[sizeof(ip_addressBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xBB;
      memcpy(&data[3], ip_addressBytes, sizeof(ip_addressBytes));
      data[sizeof(ip_addressBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 12)
    {
      // Gateway
      // 02 55 ***** 03
      String gateway = setting.gateway;
      // Remove \r and \n
      gateway.replace("\r", "");
      gateway.replace("\n", "");

      // Convert to ASCII bytes
      byte gatewayBytes[gateway.length()];
      stringToAsciiBytes(gateway, gatewayBytes, sizeof(gatewayBytes));

      // Send to ESP
      byte data[sizeof(gatewayBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xBC;
      memcpy(&data[3], gatewayBytes, sizeof(gatewayBytes));
      data[sizeof(gatewayBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 13)
    {
      // Subnet
      // 02 55 ***** 03
      String subnet = setting.subnet;
      // Remove \r and \n
      subnet.replace("\r", "");
      subnet.replace("\n", "");

      // Convert to ASCII bytes
      byte subnetBytes[subnet.length()];
      stringToAsciiBytes(subnet, subnetBytes, sizeof(subnetBytes));

      // Send to ESP
      byte data[sizeof(subnetBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xBD;
      memcpy(&data[3], subnetBytes, sizeof(subnetBytes));
      data[sizeof(subnetBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 14)
    {
      // Primary DNS
      // 02 55 ***** 03
      String primaryDNS = setting.primaryDNS;
      // Remove \r and \n
      primaryDNS.replace("\r", "");
      primaryDNS.replace("\n", "");

      // Convert to ASCII bytes
      byte primaryDNSBytes[primaryDNS.length()];
      stringToAsciiBytes(primaryDNS, primaryDNSBytes, sizeof(primaryDNSBytes));

      // Send to ESP
      byte data[sizeof(primaryDNSBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xBE;
      memcpy(&data[3], primaryDNSBytes, sizeof(primaryDNSBytes));
      data[sizeof(primaryDNSBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter == 15)
    {
      // Secondary DNS
      // 02 55 ***** 03
      String secondaryDNS = setting.secondaryDNS;
      // Remove \r and \n
      secondaryDNS.replace("\r", "");
      secondaryDNS.replace("\n", "");

      // Convert to ASCII bytes
      byte secondaryDNSBytes[secondaryDNS.length()];
      stringToAsciiBytes(secondaryDNS, secondaryDNSBytes, sizeof(secondaryDNSBytes));

      // Send to ESP
      byte data[sizeof(secondaryDNSBytes) + 4];
      data[0] = START_BYTE;
      data[1] = 0x55;
      data[2] = 0xBF;
      memcpy(&data[3], secondaryDNSBytes, sizeof(secondaryDNSBytes));
      data[sizeof(secondaryDNSBytes) + 3] = END_BYTE;
      Serial3.write(data, sizeof(data));
    }
    else if (countUpdateParameter >= 16)
    {
      // Finish
      // 02 43 C0 00 00 00 00 03
      byte data[8] = {0x02, 0x43, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x03};
      Serial3.write(data, sizeof(data));
      status = 2;
    }
  }else if (currentMillisUpdateParameter < previousMillisUpdateParameter)
  { // Overflow
    previousMillisUpdateParameter = currentMillisUpdateParameter;
  }
}

void stringToAsciiBytes(const String &input, byte *output, unsigned int outputSize)
{
  // Clear the output array (especially important if the string is shorter than the maximum length)
  memset(output, 0, outputSize);

  // Ensure we don't exceed the buffer size
  unsigned int inputLength = input.length();
  if (inputLength > outputSize)
  {
    inputLength = outputSize;
  }

  // Iterate over each character in the string, converting it to ASCII
  for (unsigned int i = 0; i < inputLength; i++)
  {
    output[i] = (byte)input.charAt(i);
  }
}
// ------------------  FUNCTIONS SERIAL  ------------------ //
void handleBufferedData()
{
  if (!endReceived)
  {
    return;
  }
  if (_buffer[0] == START_BYTE && _buffer[bufferIndex - 1] == END_BYTE)
  {
    processBufferedMessage();
  }
  resetBuffer();
}

void processBufferedMessage()
{
  if (bufferIndex == 8)
  {
    // 0x02, 0x55, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03
    if (_buffer[1] == 0x55 && _buffer[2] == 0x53 && _buffer[6] == 0x01)
    {
      // If you have any processing related to this condition, put it here
      // Serial1.write(_buffer, bufferIndex);
      status = 1;
      countUpdateParameter = 0;
      tone(BUZZER_PIN, 2500, 50);
    }
    // 0x02, 0x55, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03
    if (_buffer[1] == 0x55 && _buffer[2] == 0x53 && _buffer[6] == 0xA1)
    {
      // If you have any processing related to this condition, put it here
      // Serial1.write(_buffer, bufferIndex);
      status = 5;
      countUpdateParameter = 0;
      tone(BUZZER_PIN, 2500, 50);
    }

  }
  else if (_buffer[1] == 0x43 && _buffer[2] == 0x44)
  {
    // If you have any processing related to this condition, put it here
    // Serial1.write(_buffer, bufferIndex);
  }
  Serial.write(_buffer, bufferIndex);
  tone(BUZZER_PIN, 2500, 50);
}

void resetBuffer()
{
  endReceived = false;
  bufferIndex = 0;
  memset(_buffer, 0, BUFFER_SIZE);
}

// ------------------   FUNCTIONS   ------------------ //
void SecondsCounter(void)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    if (CountNoBacklight > 0)
    {
      CountNoBacklight--;
      if (CountNoBacklight <= 0)
      {
        lcd.noBacklight();
        CountNoBacklight = 0;
      }
    }
    if (countBackHome > 0)
    {
      countBackHome--;
      if (countBackHome <= 0)
      {
        currentMenu = 0;
        cursorPosition = 0;
        countBackHome = 0;
      }
    }
  }
  else if (currentMillis < previousMillis)
  { // Overflow
    previousMillis = currentMillis;
  }
}

void millisCounter(void)
{
  unsigned long currentMicros = millis();
  if (currentMicros - previousMicros >= 100) // 100 ms
  {
    previousMicros = currentMicros;
    // ESC UP  DOWN  ENTER = 2^4 = 16 case
    // 0 0 0 0
    if (!stateBTN_ESC && !stateBTN_UP && !stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Not press
    }
    // 0 0 0 1
    else if (!stateBTN_ESC && !stateBTN_UP && !stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press ENTER
      Serial.println("ENTER");
      if (currentMenu == 0)
      {
      }
      else if (currentMenu == 1 && currentSubMenu == 0)
      {
        currentSubMenu = 1;
      }
      else if (currentMenu == 1 && currentSubMenu != 0)
      {
        if (cursorPosition == 17 && currentSubMenu == 1)
        {
        }
        else if (cursorPosition == 17 && currentSubMenu == 2)
        {
          // Send data to ESP For update
        }
      }
      {
        currentSubMenu = 1;
      }
    }
    // 0 0 1 0
    else if (!stateBTN_ESC && !stateBTN_UP && stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press DOWN
      Serial.println("DOWN");
      if (currentMenu == 0)
      {
        cursorPosition++;
      }
      if (currentMenu == 1 && currentSubMenu == 0)
      {
        cursorPosition++;
      }
      else if (currentMenu == 1 && currentSubMenu != 0)
      {
        if (cursorPosition == 17)
        {
          currentSubMenu = (currentSubMenu == 1) ? 2 : 1;
        }
      }
    }
    // 0 0 1 1
    else if (!stateBTN_ESC && !stateBTN_UP && stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press DOWN ENTER
      Serial.println("DOWN ENTER");
    }
    // 0 1 0 0
    else if (!stateBTN_ESC && stateBTN_UP && !stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press UP
      Serial.println("UP");
      if (currentMenu == 0)
      {
        cursorPosition--;
      }
      if (currentMenu == 1 && currentSubMenu == 0)
      {
        cursorPosition--;
      }
      else if (currentMenu == 1 && currentSubMenu != 0)
      {
        if (cursorPosition == 17)
        {
          currentSubMenu = (currentSubMenu == 1) ? 2 : 1;
        }
      }
    }
    // 0 1 0 1
    else if (!stateBTN_ESC && stateBTN_UP && !stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press UP ENTER
      Serial.println("UP ENTER");
    }
    // 0 1 1 0
    else if (!stateBTN_ESC && stateBTN_UP && stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press UP DOWN
      Serial.println("UP DOWN");
    }
    // 0 1 1 1
    else if (!stateBTN_ESC && stateBTN_UP && stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press UP DOWN ENTER
      Serial.println("UP DOWN ENTER");
    }
    // 1 0 0 0
    else if (stateBTN_ESC && !stateBTN_UP && !stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press ESC
      Serial.println("ESC");
      if (currentMenu == 0)
      {
        currentMenu = 1;
        cursorPosition = 0;
      }
      else if (currentMenu == 1 && currentSubMenu == 0)
      {
        currentMenu = 0;
        cursorPosition = 0;
      }
      else if (currentMenu == 1 && currentSubMenu != 0)
      {
        currentSubMenu = 0;
      }
    }
    // 1 0 0 1
    else if (stateBTN_ESC && !stateBTN_UP && !stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press ESC ENTER
      Serial.println("ESC ENTER");
    }
    // 1 0 1 0
    else if (stateBTN_ESC && !stateBTN_UP && stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press ESC DOWN
      Serial.println("ESC DOWN");
    }
    // 1 0 1 1
    else if (stateBTN_ESC && !stateBTN_UP && stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press ESC DOWN ENTER
      Serial.println("ESC DOWN ENTER");
    }
    // 1 1 0 0
    else if (stateBTN_ESC && stateBTN_UP && !stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press ESC UP
      Serial.println("ESC UP");
    }
    // 1 1 0 1
    else if (stateBTN_ESC && stateBTN_UP && !stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press ESC UP ENTER
      Serial.println("ESC UP ENTER");
    }
    // 1 1 1 0
    else if (stateBTN_ESC && stateBTN_UP && stateBTN_DOWN && !stateBTN_ENTER)
    {
      // Press ESC UP DOWN
      Serial.println("ESC UP DOWN");
    }
    // 1 1 1 1
    else if (stateBTN_ESC && stateBTN_UP && stateBTN_DOWN && stateBTN_ENTER)
    {
      // Press ESC UP DOWN ENTER
      Serial.println("ESC UP DOWN ENTER");
    }
    updateDisplay();
    // Clear state
    stateBTN_ESC = false;
    stateBTN_UP = false;
    stateBTN_DOWN = false;
    stateBTN_ENTER = false;
  }
  else if (currentMicros < previousMicros)
  { // Overflow
    previousMicros = currentMicros;
  }
}

// ------------------   ESC  ------------------ //
void OnPress_ESC(void)
{
  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  countBackHome = countBackHomeTime;
  lcd.backlight();
  previousMicros = millis();
  stateBTN_ESC = true;
  // Serial3.println("Hi ESP From Mega ESC Press");
}

void OnRelease_ESC(void)
{
}
// ------------------   UP  ------------------ //
void OnPress_UP(void)
{
  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  countBackHome = countBackHomeTime;
  lcd.backlight();
  previousMicros = millis();
  stateBTN_UP = true;
}
void OnRelease_UP(void)
{
}

// ------------------   DOWN  ------------------ //
void OnPress_DOWN(void)
{
  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  countBackHome = countBackHomeTime;
  lcd.backlight();
  previousMicros = millis();
  stateBTN_DOWN = true;
}

void OnRelease_DOWN(void)
{
}

// ------------------   ENTER  ------------------ //
void OnPress_ENTER(void)
{
  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  countBackHome = countBackHomeTime;
  lcd.backlight();
  previousMicros = millis();
  stateBTN_ENTER = true;
}

void OnRelease_ENTER(void)
{
}

// ------------------   END  ------------------ //

void updateDisplay()
{
  updateDisplay1();
  updateDisplaySetting();
  updateDisplaySubSetting();
}
String currentLine1 = "                "; //  16 ตัว
String currentLine2 = "                "; //  16 ตัว

void updateLCD(const String &newDataLine1, const String &newDataLine2)
{
  // Line 1
  for (int i = 0; i < 16; i++)
  {
    if (i < newDataLine1.length())
    {
      if (newDataLine1[i] != currentLine1[i])
      {
        lcd.setCursor(i, 0);
        lcd.print(newDataLine1[i]);
        currentLine1[i] = newDataLine1[i];
      }
    }
    else
    {
      if (currentLine1[i] != ' ')
      {
        lcd.setCursor(i, 0);
        lcd.print(' ');
        currentLine1[i] = ' ';
      }
    }
  }

  // Line 2
  for (int i = 0; i < 16; i++)
  {
    if (i < newDataLine2.length())
    {
      if (newDataLine2[i] != currentLine2[i])
      {
        lcd.setCursor(i, 1);
        lcd.print(newDataLine2[i]);
        currentLine2[i] = newDataLine2[i];
      }
    }
    else
    {
      if (currentLine2[i] != ' ')
      {
        lcd.setCursor(i, 1);
        lcd.print(' ');
        currentLine2[i] = ' ';
      }
    }
  }
}

void updateDisplay1()
{
  if (currentMenu != 0)
  {
    return;
  }
  String newDataLine1 = "";
  String newDataLine2 = "";
  if (cursorPosition == 0)
  {
    newDataLine1 = "CH-1 : " + String(acVoltageA1.getVoltageRMS()) + " V";
    newDataLine2 = "CH-2 : " + String(acVoltageA2.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 1)
  {
    newDataLine1 = "CH-2 : " + String(acVoltageA2.getVoltageRMS()) + " V";
    newDataLine2 = "CH-3 : " + String(acVoltageA3.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 2)
  {
    newDataLine1 = "CH-3 : " + String(acVoltageA3.getVoltageRMS()) + " V";
    newDataLine2 = "CH-4 : " + String(acVoltageA4.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 3)
  {
    newDataLine1 = "CH-4 : " + String(acVoltageA4.getVoltageRMS()) + " V";
    newDataLine2 = "CH-5 : " + String(acVoltageA5.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 4)
  {
    newDataLine1 = "CH-5 : " + String(acVoltageA5.getVoltageRMS()) + " V";
    newDataLine2 = "CH-6 : " + String(acVoltageA6.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 5)
  {
    newDataLine1 = "CH-6 : " + String(acVoltageA6.getVoltageRMS()) + " V";
    newDataLine2 = "CH-7 : " + String(acVoltageA7.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 6)
  {
    newDataLine1 = "CH-7 : " + String(acVoltageA7.getVoltageRMS()) + " V";
    newDataLine2 = "CH-8 : " + String(acVoltageA8.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 7)
  {
    newDataLine1 = "CH-8 : " + String(acVoltageA8.getVoltageRMS()) + " V";
    newDataLine2 = "CH-9 : " + String(acVoltageA9.getVoltageRMS()) + " V";
  }
  else if (cursorPosition == 8)
  {
    newDataLine1 = "CH-9 : " + String(acVoltageA9.getVoltageRMS()) + " V";
    newDataLine2 = "CH-10 : " + String(acVoltageA10.getVoltageRMS()) + " V";
  }
  else if (cursorPosition < 0)
  {
    cursorPosition = 8;
  }
  else
  {
    // set cursorPosition = 0
    cursorPosition = 0;
  }
  // updateLCD
  updateLCD(newDataLine1, newDataLine2);
}
void updateDisplaySetting()
{
  if (currentMenu != 1)
  {
    return;
  }

  if (currentSubMenu > 0)
  {
    return;
  }
  String newDataLine1 = "";
  String newDataLine2 = "";
  // String deviceName;
  // String ssid;

  // String password;
  // String mqtt_server;

  // String mqtt_port;
  // String mqtt_user;

  // String mqtt_password;
  // String mqtt_topic;

  // String mqtt_topic_sub;
  // String mqtt_topic_pub;

  // String ip_address;
  // String gateway;

  // String subnet;
  // String primaryDNS;

  // String secondaryDNS;
  // String time_update;

  // String alarm_limit;
  // BOOT LOADER

  if (cursorPosition == 0)
  {
    newDataLine1 = ">DEVICE NAME";
    newDataLine2 = " SSID";
  }
  else if (cursorPosition == 1)
  {
    newDataLine1 = " DEVICE NAME";
    newDataLine2 = ">SSID";
  }
  else if (cursorPosition == 2)
  {
    newDataLine1 = ">PASSWORD";
    newDataLine2 = " MQTT SERVER";
  }
  else if (cursorPosition == 3)
  {
    newDataLine1 = " PASSWORD";
    newDataLine2 = ">MQTT SERVER";
  }
  else if (cursorPosition == 4)
  {
    newDataLine1 = ">MQTT PORT";
    newDataLine2 = " MQTT USER";
  }
  else if (cursorPosition == 5)
  {
    newDataLine1 = " MQTT PORT";
    newDataLine2 = ">MQTT USER";
  }
  else if (cursorPosition == 6)
  {
    newDataLine1 = ">MQTT PASSWORD";
    newDataLine2 = " MQTT TOPIC";
  }
  else if (cursorPosition == 7)
  {
    newDataLine1 = " MQTT PASSWORD";
    newDataLine2 = ">MQTT TOPIC";
  }
  else if (cursorPosition == 8)
  {
    newDataLine1 = ">MQTT TOPIC SUB";
    newDataLine2 = " MQTT TOPIC PUB";
  }
  else if (cursorPosition == 9)
  {
    newDataLine1 = " MQTT TOPIC SUB";
    newDataLine2 = ">MQTT TOPIC PUB";
  }
  else if (cursorPosition == 10)
  {
    newDataLine1 = ">IP ADDRESS";
    newDataLine2 = " GATEWAY";
  }
  else if (cursorPosition == 11)
  {
    newDataLine1 = " IP ADDRESS";
    newDataLine2 = ">GATEWAY";
  }
  else if (cursorPosition == 12)
  {
    newDataLine1 = ">SUBNET";
    newDataLine2 = " PRIMARY DNS";
  }
  else if (cursorPosition == 13)
  {
    newDataLine1 = " SUBNET";
    newDataLine2 = ">PRIMARY DNS";
  }
  else if (cursorPosition == 14)
  {
    newDataLine1 = ">SECONDARY DNS";
    newDataLine2 = " TIME SEND DATA";
  }
  else if (cursorPosition == 15)
  {
    newDataLine1 = " SECONDARY DNS";
    newDataLine2 = ">TIME SEND DATA";
  }
  else if (cursorPosition == 16)
  {
    newDataLine1 = ">ALARM LIMIT";
    newDataLine2 = " BOOT LOADER";
  }
  else if (cursorPosition == 17)
  {
    newDataLine1 = " ALARM LIMIT";
    newDataLine2 = ">BOOT LOADER";
  }
  else if (cursorPosition < 0)
  {
    cursorPosition = 17;
  }
  else
  {
    cursorPosition = 0;
  }

  // updateLCD
  updateLCD(newDataLine1, newDataLine2);
}

void updateDisplaySubSetting()
{
  if (currentMenu != 1)
  {
    return;
  }

  if (currentSubMenu == 0)
  {
    return;
  }

  String newDataLine1 = "";
  String newDataLine2 = "";

  if (cursorPosition == 0 && currentSubMenu == 1)
  {
    newDataLine1 = "DEVICE NAME :";
    String settingDeviceName = setting.deviceName;
    // Remove \r \n
    settingDeviceName.replace("\r", "");
    settingDeviceName.replace("\n", "");

    newDataLine2 = settingDeviceName;
  }
  else if (cursorPosition == 1 && currentSubMenu == 1)
  {
    newDataLine1 = "SSID :";
    String settingSSID = setting.ssid;
    // Remove \r \n
    settingSSID.replace("\r", "");
    settingSSID.replace("\n", "");

    newDataLine2 = settingSSID;
  }
  else if (cursorPosition == 2 && currentSubMenu == 1)
  {
    newDataLine1 = "PASSWORD :";
    String settingPassword = setting.password;
    // Remove \r \n
    settingPassword.replace("\r", "");
    settingPassword.replace("\n", "");

    newDataLine2 = settingPassword;
  }
  else if (cursorPosition == 3 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT SERVER :";
    String settingMqttServer = setting.mqtt_server;
    // Remove \r \n
    settingMqttServer.replace("\r", "");
    settingMqttServer.replace("\n", "");

    newDataLine2 = settingMqttServer;
  }
  else if (cursorPosition == 4 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT PORT :";
    String settingMqttPort = setting.mqtt_port;
    // Remove \r \n
    settingMqttPort.replace("\r", "");
    settingMqttPort.replace("\n", "");

    newDataLine2 = settingMqttPort;
  }
  else if (cursorPosition == 5 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT USER :";
    String settingMqttUser = setting.mqtt_user;
    // Remove \r \n
    settingMqttUser.replace("\r", "");
    settingMqttUser.replace("\n", "");

    newDataLine2 = settingMqttUser;
  }
  else if (cursorPosition == 6 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT PASSWORD :";
    String settingMqttPassword = setting.mqtt_password;
    // Remove \r \n
    settingMqttPassword.replace("\r", "");
    settingMqttPassword.replace("\n", "");

    newDataLine2 = settingMqttPassword;
  }
  else if (cursorPosition == 7 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT TOPIC :";
    String settingMqttTopic = setting.mqtt_topic;
    // Remove \r \n
    settingMqttTopic.replace("\r", "");
    settingMqttTopic.replace("\n", "");

    newDataLine2 = settingMqttTopic;
  }
  else if (cursorPosition == 8 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT TOPIC SUB :";
    String settingMqttTopicSub = setting.mqtt_topic_sub;
    // Remove \r \n
    settingMqttTopicSub.replace("\r", "");
    settingMqttTopicSub.replace("\n", "");

    newDataLine2 = settingMqttTopicSub;
  }
  else if (cursorPosition == 9 && currentSubMenu == 1)
  {
    newDataLine1 = "MQTT TOPIC PUB :";
    String settingMqttTopicPub = setting.mqtt_topic_pub;
    // Remove \r \n
    settingMqttTopicPub.replace("\r", "");
    settingMqttTopicPub.replace("\n", "");

    newDataLine2 = settingMqttTopicPub;
  }
  else if (cursorPosition == 10 && currentSubMenu == 1)
  {
    newDataLine1 = "IP ADDRESS :";
    String settingIpAddress = setting.ip_address;
    // Remove \r \n
    settingIpAddress.replace("\r", "");
    settingIpAddress.replace("\n", "");

    newDataLine2 = settingIpAddress;
  }
  else if (cursorPosition == 11 && currentSubMenu == 1)
  {
    newDataLine1 = "GATEWAY :";
    String settingGateway = setting.gateway;
    // Remove \r \n
    settingGateway.replace("\r", "");
    settingGateway.replace("\n", "");

    newDataLine2 = settingGateway;
  }
  else if (cursorPosition == 12 && currentSubMenu == 1)
  {
    newDataLine1 = "SUBNET :";
    String settingSubnet = setting.subnet;
    // Remove \r \n
    settingSubnet.replace("\r", "");
    settingSubnet.replace("\n", "");

    newDataLine2 = settingSubnet;
  }
  else if (cursorPosition == 13 && currentSubMenu == 1)
  {
    newDataLine1 = "PRIMARY DNS :";
    String settingPrimaryDNS = setting.primaryDNS;
    // Remove \r \n
    settingPrimaryDNS.replace("\r", "");
    settingPrimaryDNS.replace("\n", "");

    newDataLine2 = settingPrimaryDNS;
  }
  else if (cursorPosition == 14 && currentSubMenu == 1)
  {
    newDataLine1 = "SECONDARY DNS :";
    String settingSecondaryDNS = setting.secondaryDNS;
    // Remove \r \n
    settingSecondaryDNS.replace("\r", "");
    settingSecondaryDNS.replace("\n", "");

    newDataLine2 = settingSecondaryDNS;
  }
  else if (cursorPosition == 15 && currentSubMenu == 1)
  {
    newDataLine1 = "TIME SEND DATA :";
    String settingTimeUpdate = setting.time_update;
    // Remove \r \n
    settingTimeUpdate.replace("\r", "");
    settingTimeUpdate.replace("\n", "");

    newDataLine2 = settingTimeUpdate;
  }
  else if (cursorPosition == 16 && currentSubMenu == 1)
  {
    newDataLine1 = "ALARM LIMIT :";
    String settingAlarmLimit = setting.alarm_limit;
    // Remove \r \n
    settingAlarmLimit.replace("\r", "");
    settingAlarmLimit.replace("\n", "");
    newDataLine2 = settingAlarmLimit;
  }
  else if (cursorPosition == 17 && currentSubMenu == 1)
  {
    newDataLine1 = "BOOT LOADER :";
    newDataLine2 = "NO";
  }
  else if (cursorPosition == 17 && currentSubMenu == 2)
  {
    newDataLine1 = "BOOT LOADER :";
    newDataLine2 = "YES";
  }
  else if (cursorPosition < 0)
  {
    cursorPosition = 11;
  }
  else
  {
    cursorPosition = 0;
  }

  // updateLCD
  updateLCD(newDataLine1, newDataLine2);
}