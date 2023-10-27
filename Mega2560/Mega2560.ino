#include <TcBUTTON.h>
#include <TcPINOUT.h>
#include <LiquidCrystal_I2C.h>
#include <AcVoltage.h>
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
const int MOTOR_OFFSET = 10000;
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

// ------------------   FUNCTIONS   ------------------ //
void serialEvent()
{
  Serial1.print("Mega1 :");
  while (Serial.available())
  {
    byte incomeByte = Serial.read();
    // Serial.write(incomeByte);
    //  delay(1);
  }
  // Serial1.println("");
}
void serialEvent3()
{
  Serial.println("Mega3 :");
  while (Serial3.available())
  {
    byte incomeByte = Serial3.read();
    Serial.print((char)incomeByte);

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
  // // lcd.cursor();
  CountNoBacklight = NoBacklightTime;
}
// ------------------   LOOP   ------------------ //
void loop()
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

  handleBufferedData();
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
      else if (currentMenu == 1)
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
      if (currentMenu == 1 && currentSubMenu ==0)
      {
        cursorPosition++;
      }else if (currentMenu == 1 && currentSubMenu != 0)
      {
        if(cursorPosition == 11){
          if(currentSubMenu == 1){
            currentSubMenu = 2;
          }else if(currentSubMenu == 2){
            currentSubMenu = 1;
          }
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
      if (currentMenu == 1 && currentSubMenu ==0)
      {
        cursorPosition--;
      }else if (currentMenu == 1 && currentSubMenu != 0)
      {
        if(cursorPosition == 11){
          if(currentSubMenu == 1){
            currentSubMenu = 2;
          }else if(currentSubMenu == 2){
            currentSubMenu = 1;
          }
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
      }else if (currentMenu == 1 && currentSubMenu != 0)
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
  
  if(currentSubMenu > 0){
    return;
  }
  String newDataLine1 = "";
  String newDataLine2 = "";
  // DEVICE NAME
  // SSID
  // PASSWORD
  // MQTT SERVER
  // IP ADDRESS
  // GATEWAY
  // SUBNET
  // PRIMARY DNS
  // SECONDARY DNS
  // TIME FOR SEND DATA
  // ALARM LIMIT
  // BOOT LOADER

  if (cursorPosition == 0)
  {
    newDataLine1 = ">DEVICE NAME";
    newDataLine2 = "SSID";
  }else if(cursorPosition == 1){
    newDataLine1 = "DEVICE NAME";
    newDataLine2 = ">SSID";
  }else if(cursorPosition == 2){
    newDataLine1 = ">PASSWORD";
    newDataLine2 = "MQTT SERVER";
  }else if(cursorPosition == 3){
    newDataLine1 = "PASSWORD";
    newDataLine2 = ">MQTT SERVER";
  }else if(cursorPosition == 4){
    newDataLine1 = ">IP ADDRESS";
    newDataLine2 = "GATEWAY";
  }else if(cursorPosition == 5){
    newDataLine1 = "IP ADDRESS";
    newDataLine2 = ">GATEWAY";
  }else if(cursorPosition == 6){
    newDataLine1 = ">SUBNET";
    newDataLine2 = "PRIMARY DNS";
  }else if(cursorPosition == 7){
    newDataLine1 = "SUBNET";
    newDataLine2 = ">PRIMARY DNS";
  }else if(cursorPosition == 8){
    newDataLine1 = ">SECONDARY DNS";
    newDataLine2 = "TIME SEND DATA";
  }else if(cursorPosition == 9){
    newDataLine1 = "SECONDARY DNS";
    newDataLine2 = ">TIME SEND DATA";
  }else if(cursorPosition == 10){
    newDataLine1 = ">ALARM LIMIT";
    newDataLine2 = "BOOT LOADER";
  }else if(cursorPosition == 11){
    newDataLine1 = "ALARM LIMIT";
    newDataLine2 = ">BOOT LOADER";
  }else if(cursorPosition < 0){
    cursorPosition = 11;
  }else{
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
  
  if(currentSubMenu == 0){
    return;
  }

  String newDataLine1 = "";
  String newDataLine2 = "";

  if(cursorPosition == 0 && currentSubMenu == 1){
    newDataLine1 = "DEVICE NAME :";
    newDataLine2 = "MACHINE1";
  }
  else if(cursorPosition == 1 && currentSubMenu == 1){
    newDataLine1 = "SSID :";
    newDataLine2 = "Internet";
  }else if(cursorPosition == 2 && currentSubMenu == 1){
    newDataLine1 = "PASSWORD :";
    newDataLine2 = "0987654321qw";
  }else if(cursorPosition == 3 && currentSubMenu == 1){
    newDataLine1 = "MQTT SERVER :";
    newDataLine2 = "192.167.2.123";
  }else if(cursorPosition == 4 && currentSubMenu == 1){
    newDataLine1 = "IP ADDRESS :";
    newDataLine2 = "192.168.1.125";
  }else if(cursorPosition == 5 && currentSubMenu == 1){
    newDataLine1 = "GATEWAY :";
    newDataLine2 = "10.10.10.10";
  }else if(cursorPosition == 6 && currentSubMenu == 1){
    newDataLine1 = "SUBNET :";
    newDataLine2 = "255.255.0.0";
  }else if(cursorPosition == 7 && currentSubMenu == 1){
    newDataLine1 = "PRIMARY DNS :";
    newDataLine2 = "8.8.8.8";
  }else if(cursorPosition == 8 && currentSubMenu == 1){
    newDataLine1 = "SECONDARY DNS :";
    newDataLine2 = "4.4.4.4";
  }else if (cursorPosition == 9 && currentSubMenu == 1){
    newDataLine1 = "TIME SEND DATA :";
    newDataLine2 = "10";
  }else if(cursorPosition == 10 && currentSubMenu == 1){
    newDataLine1 = "ALARM LIMIT :";
    newDataLine2 = "10";
  }else if(cursorPosition == 11 && currentSubMenu == 1){
    newDataLine1 = "BOOT LOADER :";
    newDataLine2 = "NO";
  }else if(cursorPosition == 11 && currentSubMenu == 2){
    newDataLine1 = "BOOT LOADER :";
    newDataLine2 = "YES";
  }else if(cursorPosition < 0){
    cursorPosition = 11;
  }else{
    cursorPosition = 0;
  }
 
// updateLCD
  updateLCD(newDataLine1, newDataLine2);
}