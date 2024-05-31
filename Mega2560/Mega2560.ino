/**
 * @file Mega2560.ino
 * @author Jakkapan.A
 * @brief
 * @version 0.2
 * @date 2024-03-14
 * Arduino Mega 2560
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <TcBUTTON.h>
#include <TcPINOUT.h>
#include <LiquidCrystal_I2C.h>
// #include <AcVoltage.h>

#include <ModbusMaster.h>
#include <SPI.h>
// #include <SD.h>
#include <Wire.h>
#include <EEPROM.h>

// #include "Setting.h"
// ------------------   INPUT   ------------------ //
LiquidCrystal_I2C lcd(0x27, 16, 2); // Module IIC/I2C Interface

const int led_mod_bus = 8;
ModbusMaster node;
#define MbSlaveID 5 // Slave ID of the Modbus device

#define BTN_ESC_PIN 34
void OnEventPress_ESC(bool state);
TcBUTTON BTN_ESC(BTN_ESC_PIN, false);

#define BTN_UP_PIN 35
void OnEventPress_UP(bool state);
TcBUTTON BTN_UP(BTN_UP_PIN, false);

#define BTN_DOWN_PIN 36
void OnEventPress_DOWN(bool state);
TcBUTTON BTN_DOWN(BTN_DOWN_PIN, false);

#define BTN_ENTER_PIN 37
void OnEventPress_ENTER(bool state);
TcBUTTON BTN_ENTER(BTN_ENTER_PIN, false);

// ------------------  OUTPUT   ------------------ //
#define LED1_RED_PIN 38
TcPINOUT LED1_RED(LED1_RED_PIN);

#define LED2_RED_PIN 39
TcPINOUT LED2_RED(LED2_RED_PIN);

#define LED3_RED_PIN 40
TcPINOUT LED3_RED(LED3_RED_PIN);

#define LED4_RED_PIN 41
TcPINOUT LED4_RED(LED4_RED_PIN);

#define LED5_RED_PIN 42
TcPINOUT LED5_RED(LED5_RED_PIN);

#define LED6_RED_PIN 43
TcPINOUT LED6_RED(LED6_RED_PIN);

#define LED7_RED_PIN 44
TcPINOUT LED7_RED(LED7_RED_PIN);

#define LED8_RED_PIN 45
TcPINOUT LED8_RED(LED8_RED_PIN);

#define LED9_RED_PIN 46
TcPINOUT LED9_RED(LED9_RED_PIN);

#define LED10_RED_PIN 47
TcPINOUT LED10_RED(LED10_RED_PIN);

#define LED11_RED_PIN 48
TcPINOUT LED11_RED(LED11_RED_PIN);

TcPINOUT LED_LIST_RED[] = {LED1_RED, LED2_RED, LED3_RED, LED4_RED, LED5_RED, LED6_RED, LED7_RED, LED8_RED, LED9_RED, LED10_RED, LED11_RED};

// ------------------  LED GREEN ------------------- //
#define LED1_GREEN_PIN A1
TcPINOUT LED1_GREEN(LED1_GREEN_PIN);

#define LED2_GREEN_PIN A2
TcPINOUT LED2_GREEN(LED2_GREEN_PIN);

#define LED3_GREEN_PIN A3
TcPINOUT LED3_GREEN(LED3_GREEN_PIN);

#define LED4_GREEN_PIN A4
TcPINOUT LED4_GREEN(LED4_GREEN_PIN);

#define LED5_GREEN_PIN A5
TcPINOUT LED5_GREEN(LED5_GREEN_PIN);

#define LED6_GREEN_PIN A6
TcPINOUT LED6_GREEN(LED6_GREEN_PIN);

#define LED7_GREEN_PIN A7
TcPINOUT LED7_GREEN(LED7_GREEN_PIN);

#define LED8_GREEN_PIN A8
TcPINOUT LED8_GREEN(LED8_GREEN_PIN);

#define LED9_GREEN_PIN A9
TcPINOUT LED9_GREEN(LED9_GREEN_PIN);

#define LED10_GREEN_PIN A10
TcPINOUT LED10_GREEN(LED10_GREEN_PIN);

#define LED11_GREEN_PIN A11
TcPINOUT LED11_GREEN(LED11_GREEN_PIN);

TcPINOUT LED_LIST_GREEN[] = {LED1_GREEN, LED2_GREEN, LED3_GREEN, LED4_GREEN, LED5_GREEN, LED6_GREEN, LED7_GREEN, LED8_GREEN, LED9_GREEN, LED10_GREEN, LED11_GREEN};
// ---------------------------- //
#define RELAY1_PIN 22
TcPINOUT RELAY1(RELAY1_PIN,true);

#define RELAY2_PIN 23
TcPINOUT RELAY2(RELAY2_PIN,true);

#define RELAY3_PIN 24
TcPINOUT RELAY3(RELAY3_PIN,true);

#define RELAY4_PIN 25
TcPINOUT RELAY4(RELAY4_PIN,true);

#define RELAY5_PIN 26
TcPINOUT RELAY5(RELAY5_PIN,true);

#define RELAY6_PIN 27
TcPINOUT RELAY6(RELAY6_PIN,true);

#define RELAY7_PIN 28
TcPINOUT RELAY7(RELAY7_PIN,true);

#define RELAY8_PIN 29
TcPINOUT RELAY8(RELAY8_PIN,true);

#define RELAY9_PIN 30
TcPINOUT RELAY9(RELAY9_PIN,true);

#define RELAY10_PIN 31
TcPINOUT RELAY10(RELAY10_PIN,true);

#define RELAY11_PIN 32
TcPINOUT RELAY11(RELAY11_PIN,true);

TcPINOUT RELAYS[] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6, RELAY7, RELAY8, RELAY9, RELAY10, RELAY11};
const int numRelays = sizeof(RELAYS) / sizeof(RELAYS[0]);

#define BUZZER_PIN 4
TcPINOUT BUZZER(BUZZER_PIN, true);
// ------------------ VARIABLES ------------------ //
uint32_t previousMillis = 0;

int countNoBacklight = 0;
const int NoBacklightTime = 60; // 60 seconds
int countBackHome = 0;
const int countBackHomeTime = 120; // 60 seconds
int countReset = 0;
const int countResetTime = 2;

#define INDEX_RELAY_MAX 11

int indexRelay = 0;
// -------------------- MENU -------------------- //
int indexMenu = 0; // 0: Home, 1: Setting
int oldIndexMenu = 0;
uint8_t countIndexMenu = 0;

int selectMenu = 0;
int selectSubMenu = 0;
int selectSubMenu1 = 0;
int selectSubMenu2 = 0;
boolean isSave = false;
// -------------------- STATE BUTTON -------------------- //
boolean stateEsc = false;
boolean stateUp = false;
boolean stateDown = false;
boolean stateEnter = false;

boolean currentStateEsc = false;
boolean currentStateUp = false;
boolean currentStateDown = false;
boolean currentStateEnter = false;
boolean stateCensorOnStation = false;

boolean statusServer = false;
boolean oldStateServer = false;

boolean statusETH = false;
uint8_t countDownStatusServer = 0; // Sec 13

#define TIME_UP_COMMUNICATION 10 // 14 seconds
uint8_t countDownStatusETH = 0;  // Sec 12

#define TIME_OUT_COMMUNICATION 20 // 20 seconds
uint8_t countUpCommunication = 0;

// -------------------- END STATE BUTTON -------------------- //
bool isAlarm[11] = {false, false, false, false, false, false, false, false, false, false, false};


#define LIMIT_ADDRESS_EEPROM 10
int limitAlarm, setLimitAlarm = 6;
#define TIME_UPDATE_CH_EEPROM 15
int timeUpdateCH, setTimeUpdateCH = 5; // CH is Channel relay update
#define TIME_UPDATE_DATA_EEPROM 20
int timeUpdateData, setTimeUpdateData = 5;
// ------------------   VARIABLES   ------------------ //
int countUpdateParameter = 0;
unsigned long previousMillisUpdateParameter = 0;
const int timeUpdateParameter = 20; // 20ms
// ------------------   FUNCTIONS   ------------------ //
#define BUFFER_SIZE_DATA 256
// -------------------- SERIAL  -------------------- //
boolean startReceived = false;
boolean endReceived = false;

const char startChar = '$';
const char endChar = '#';

char fileName[8];
// String inputString = "";
char inputString[BUFFER_SIZE_DATA];
int inputStringLength = 0;
void serialEvent()
{
  while (Serial.available())
  {
    byte inChar = (byte)Serial.read();
    if (inChar == startChar)
    {
      startReceived = true;
      inputStringLength = 0;
      memset(inputString, 0, sizeof(inputString));
    }
    else if (startReceived && inChar == endChar)
    {
      endReceived = true;
    }
    else if (startReceived)
    {
      if (inputStringLength < BUFFER_SIZE_DATA - 1)
      {
        inputString[inputStringLength++] = inChar;
      }
      else
      {
        startReceived = false;
        endReceived = false;
        inputStringLength = 0;
      }
    }
  }
}

// -------------------- SERIAL 3 -------------------- //
bool startReceived3 = false;
bool endReceived3 = false;
const char startChar3 = '$';
const char endChar3 = '#';
// String inputString3 = "";
char inputString3[BUFFER_SIZE_DATA];
int inputStringLength3 = 0;
void serialEvent3()
{
  while (Serial3.available())
  {
    byte inChar = (byte)Serial3.read();
    if (inChar == startChar3)
    {
      startReceived3 = true;
      inputStringLength3 = 0;
      memset(inputString3, 0, BUFFER_SIZE_DATA);
    }
    else if (startReceived3 && inChar == endChar3)
    {
      endReceived3 = true;
    }
    else if (startReceived3)
    {
      if (inputStringLength3 < BUFFER_SIZE_DATA - 1)
      {
        inputString3[inputStringLength3++] = inChar;
      }
      else
      {
        startReceived3 = false;
        endReceived3 = false;
        inputStringLength3 = 0;
      }
    }
  }
}

// ------------------   SETUP   ------------------ //
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);

  pinMode(led_mod_bus, OUTPUT);
  node.begin(MbSlaveID, Serial2);

  BTN_ESC.OnEventChange(OnEventPress_ESC);
  BTN_UP.OnEventChange(OnEventPress_UP);
  BTN_DOWN.OnEventChange(OnEventPress_DOWN);
  BTN_ENTER.OnEventChange(OnEventPress_ENTER);

  lcd.begin();
  lcd.backlight();

  lcd.clear();
  countNoBacklight = NoBacklightTime;
  manageRelayByIndex(0);
  // Read data from EEPROM
  limitAlarm = readInt8InEEPROM(LIMIT_ADDRESS_EEPROM);
  timeUpdateCH = readInt8InEEPROM(TIME_UPDATE_CH_EEPROM);

  Serial.println("Limit Alarm: " + String(limitAlarm));
  Serial.println("Time Update CH: " + String(timeUpdateCH));

  // Loop off led
  for (int i = 0; i < numRelays; i++)
  {
    LED_LIST_RED[i].off();
    LED_LIST_GREEN[i].off();
  }

  BUZZER.on();
  delay(100);
  BUZZER.off();
  delay(50);
  BUZZER.on();
  delay(100);
  BUZZER.off();
}
// ------------------   LOOP   ------------------ //
void loop()
{
  BTN_ESC.update();
  BTN_UP.update();
  BTN_DOWN.update();
  BTN_ENTER.update();
  // acVoltageA1.update();
  mainFunction();
  manageSerial();
  manageSerial3();
}

void manageSerial()
{
  if (startReceived && endReceived)
  {
    startReceived = false;
    endReceived = false;
    inputString[inputStringLength] = '\0';
    Serial.println(inputString);
    parseData(inputString);
    memset(inputString, 0, BUFFER_SIZE_DATA);
    inputStringLength = 0;
  }
}

void manageSerial3()
{
  if (startReceived3 && endReceived3)
  {
    startReceived3 = false;
    endReceived3 = false;
    inputString3[inputStringLength3] = '\0';
    Serial.println(inputString3);
    parseData(inputString3);
    memset(inputString3, 0, BUFFER_SIZE_DATA);
    inputStringLength3 = 0;
  }
}

void parseData(String data)
{
  data.trim();
  if (data.indexOf("STATUS_SERVER:") != -1)
  {
    // Send data to MES
    String extract = extractData(data, "STATUS_SERVER:");
    if (extract == "OK")
    {
      statusServer = true;
      countDownStatusServer = TIME_OUT_COMMUNICATION;
    }
    else
    {
      statusServer = false;
    }
    // Response to master
  }
  else if (data.indexOf("STATUS_ETH:") != -1)
  {
    // Send data to MES
    // countDownStatusETH = TIME_OUT_COMMUNICATION;
    String extract = extractData(data, "STATUS_ETH:");
    if (extract == "OK")
    {
      statusETH = true;
      countDownStatusETH = TIME_OUT_COMMUNICATION;
    }
    else
    {
      statusETH = false;
    }
    // Response to master
  }
  else if (data.indexOf("UPDATE_LIMIT:") != -1)
  {
    // Send data to MES
    // countDownStatusETH = TIME_OUT_COMMUNICATION;
    String extract = extractData(data, "UPDATE_LIMIT:");
    // convert to int
    int limit = extract.toInt();
    if (limit > 0 && limit < 250)
    {
      limitAlarm = limit;
      updateEEPROM(LIMIT_ADDRESS_EEPROM, limitAlarm);
    }
  }
}

String extractData(String data, String key)
{
  int keyIndex = data.indexOf(key); // Find the position of the key
  if (keyIndex == -1)
  {
    return ""; // Return 0 if key not found
  }

  int startIndex = keyIndex + key.length();     // Start index for the number
  int endIndex = data.indexOf(",", startIndex); // Find the next comma after the key
  if (endIndex == -1)
  {
    endIndex = data.length(); // If no comma, assume end of string
  }

  String valueStr = data.substring(startIndex, endIndex); // Extract the substring
  return valueStr;                                        // Convert to float and return
}

void manageRelayByIndex(int _index)
{
  for (int i = 0; i < numRelays; i++)
  {
    if (i == _index)
    {
      RELAYS[i].on();
    }
    else
    {
      RELAYS[i].off();
    }
  }
}
int counterSentData = 0;
// float ch = 0;
#define CH_DATA_SIZE 11
float chData[CH_DATA_SIZE];
uint16_t chDataInt[CH_DATA_SIZE];

uint32_t previousMillisSec = 0;
uint8_t nextTimeRelaySec = 0;

void mainFunction(void)
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisSec >= 1000) // 1 seconds
  {
    previousMillisSec = currentMillis;
    // Next Relay after 5 seconds change CH relay
    nextTimeRelaySec++;
    if (nextTimeRelaySec >= 2)
    {
      String data = "$PUB=";
      data += "CH";
      data += String(indexRelay + 1);
      data += ":" + String(chData[indexRelay], 1);
      data += "#";
      Serial.println(data);
    }

    if (nextTimeRelaySec > timeUpdateCH)
    {
      indexRelay++;
      if (indexRelay >= INDEX_RELAY_MAX)
      {
        indexRelay = 0;
      }

      manageRelayByIndex(indexRelay);
      nextTimeRelaySec = 0;
      previousMillis = currentMillis;
    }

    // ---- Backlight ---- //
    if (countNoBacklight > 0)
    {
      countNoBacklight--;
      if (countNoBacklight == 0)
      {
        lcd.noBacklight();
      }
    }
  }
  else if (currentMillis < previousMillisSec)
  {
    previousMillisSec = currentMillis; // Over flow
  }

  // -------------------------- 100 ms -------------------------- //
  if (currentMillis - previousMillis >= 100) // 200 ms
  {
    previousMillis = currentMillis;
    float voltage  =0;
    if (nextTimeRelaySec >= 2)
    {

      voltage = getVoltageRMS();
      chData[indexRelay] = voltage;
      if (chData[indexRelay] > limitAlarm)
      {
        isAlarm[indexRelay] = true;
        LED_LIST_RED[indexRelay].on();
        LED_LIST_GREEN[indexRelay].off();
      }
      else
      {
        isAlarm[indexRelay] = false;
        LED_LIST_RED[indexRelay].off();
        LED_LIST_GREEN[indexRelay].on();

        // If not index led green off
        for (int i = 0; i < numRelays; i++)
        {
          if (i != indexRelay)
          {
            LED_LIST_GREEN[i].off();
          }
        }
      }

      bool isAlarmAll = false;
      for (int i = 0; i < numRelays; i++)
      {
        if (isAlarm[i] == true)
        {
          isAlarmAll = true;
          break;
        }
      }

      if (isAlarmAll)
      {
        BUZZER.on();
      }
      else
      {
        BUZZER.off();
      }
    }
    else
    {
      LED_LIST_GREEN[indexRelay].off();
      // If not index led green off
      for (int i = 0; i < numRelays; i++)
      {
        if (i != indexRelay)
        {
          LED_LIST_GREEN[i].off();
        }
      }
    }

    if (indexMenu == 0)
    {
      String data = "CH";
      data += indexRelay + 1;
      data += ":";
      data += String(voltage, 1);
      data += "V";
      // updateLCD("Measurement", data);
      updateLCD(">-MEASUREMENT-<", data);
    }
    else
    {
      // Display setting
      settingPage();
    }
    stateButtonPressed();
    resetStateButton();
  }
  else if (currentMillis < previousMillis)
  {
    previousMillis = currentMillis; // Over flow
  }
}

char currentLine1[17] = "                "; // 16 characters + null terminator
char currentLine2[17] = "                "; // 16 characters + null terminator
void updateLCD(const String newDataLine1, const String newDataLine2)
{
  updateLCD(newDataLine1.c_str(), newDataLine2.c_str());
}
void updateLCD(const char *newDataLine1, const char *newDataLine2)
{
  updateLCDLine(newDataLine1, currentLine1, 0);
  updateLCDLine(newDataLine2, currentLine2, 1);
}

void updateLCDLine(const char *newData, char (&currentLine)[17], int row)
{
  int i;
  // Update characters as long as they are different or until newData ends
  for (i = 0; i < 16 && newData[i]; i++)
  {
    if (newData[i] != currentLine[i])
    {
      lcd.setCursor(i, row);
      lcd.print(newData[i]);
      currentLine[i] = newData[i];
    }
  }
  // Clear any remaining characters from the previous display
  for (; i < 16; i++)
  {
    if (currentLine[i] != ' ')
    {
      lcd.setCursor(i, row);
      lcd.print(' ');
      currentLine[i] = ' ';
    }
  }
}

void OnEventPress_ESC(bool state)
{
  currentStateEsc = !state;
  if (!state)
  {
    stateEsc = true;
    Serial.println("ESC");
    // tone(BUZZER_PIN, 2000, 100);
    previousMillis = millis();
    countNoBacklight = NoBacklightTime;
    lcd.backlight();
  }
}

void OnEventPress_UP(bool state)
{
  currentStateUp = !state;
  if (!state)
  {
    stateUp = true;
    Serial.println("UP");
    // tone(BUZZER_PIN, 2000, 100);
    previousMillis = millis();
    countNoBacklight = NoBacklightTime;
    lcd.backlight();
  }
}

void OnEventPress_DOWN(bool state)
{
  currentStateDown = !state;
  if (!state)
  {
    stateDown = true;
    Serial.println("DOWN");
    // tone(BUZZER_PIN, 2000, 100);
    previousMillis = millis();
    countNoBacklight = NoBacklightTime;
    lcd.backlight();
  }
}

void OnEventPress_ENTER(bool state)
{
  currentStateEnter = !state;
  if (!state)
  {
    stateEnter = true;
    Serial.println("ENTER");
    // tone(BUZZER_PIN, 2000, 100);
    previousMillis = millis();
    countNoBacklight = NoBacklightTime;
    lcd.backlight();
  }
}

void stateButtonPressed()
{
  // 0 0 0 0
  if (!stateEsc && !stateUp && !stateDown && !stateEnter)
  {
    // not pressed
    return;
  }
  else
    // 1 1 1 0
    if (stateEsc && stateUp && stateDown && !stateEnter)
    {
    }
    else
      // 1 1 0 0
      if (stateEsc && stateUp && !stateDown && !stateEnter)
      {
      }
      else
        // 0 0 0 1
        if (!stateEsc && !stateUp && !stateDown && stateEnter)
        {
          btnEnterOnEventPressed();
        }
        else
          // 0 0 1 0
          if (!stateEsc && !stateUp && stateDown && !stateEnter)
          {
            btnDownOnEventPressed();
          }
          else
            // 0 1 0 0
            if (!stateEsc && stateUp && !stateDown && !stateEnter)
            {
              btnUpOnEventPressed();
            }
            else
              // 0 1 1 0
              if (!stateEsc && stateUp && stateDown && !stateEnter)
              {
                // btnUpOnEventPressed();
                btnUpDownOnEventPressed();
                // Serial.println("UP DOWN");
              }
              else
                // 1 0 0 0
                if (stateEsc && !stateUp && !stateDown && !stateEnter)
                {
                  btnEscOnEventPressed();
                }

#if 1
  Serial.print("indexMenu: ");
  Serial.println(indexMenu);
  Serial.print("selectMenu: ");
  Serial.println(selectMenu);
  Serial.print("selectSubMenu: ");
  Serial.println(selectSubMenu);
  Serial.print("selectSubMenu1: ");
  Serial.println(selectSubMenu1);
  Serial.print("selectSubMenu2: ");
  Serial.println(selectSubMenu2);
  Serial.println(" --------------------- ");
#endif
}
void resetStateButton()
{
  stateEsc = false;
  stateUp = false;
  stateDown = false;
  stateEnter = false;
}
void btnEscOnEventPressed()
{
  if (selectSubMenu2 > 0)
  {
    selectSubMenu2 = 0;
  }
  else if (selectSubMenu1 > 0)
  {
    selectSubMenu1 = 0;
  }
  else if (selectSubMenu > 0)
  {
    selectSubMenu = 0;
  }
  else if (selectMenu > 0)
  {
    selectMenu = 0;
  }
  else if (indexMenu == 0)
  {
    indexMenu = 1;
  }
  else if (indexMenu == 2)
  {
    indexMenu = 0;
  }
  else if (indexMenu == 1)
  {
    indexMenu = 0;
  }
}

void btnUpOnEventPressed()
{
  if (indexMenu == 1)
  {
    // SELECT MENU VOLTAGE LIST
    if (selectMenu == 0 && selectSubMenu > 0)
    {
      selectSubMenu1++;
    }

    // SELECT MENU SETTING LIMIT
    else if (selectMenu == 1 && selectSubMenu > 0)
    {
      setLimitAlarm++;
    }

    // SELECT MENU UPDATE CH
    else if (selectMenu == 2 && selectSubMenu > 0)
    {
      setTimeUpdateCH++;
    }

    else if (selectSubMenu == 0)
    {
      selectMenu--;
    }
  }
}

void btnDownOnEventPressed()
{
  if (indexMenu == 1)
  {
    // SELECT MENU VOLTAGE LIST
    if (selectMenu == 0 && selectSubMenu > 0)
    {
      selectSubMenu1--;
    }

    // SELECT MENU SETTING LIMIT
    else if (selectMenu == 1 && selectSubMenu > 0)
    {
      setLimitAlarm--;
    }

    // SELECT MENU UPDATE CH
    else if (selectMenu == 2 && selectSubMenu > 0)
    {
      setTimeUpdateCH--;
    }

    else if (selectSubMenu == 0)
    {
      selectMenu++;
    }
  }
}

void btnEnterOnEventPressed()
{
  if (indexMenu == 0)
  {
    return;
  }
  // SELECT MENU VOLTAGE LIST
  if (selectMenu == 0 && selectSubMenu == 0)
  {
    selectSubMenu = 1;
  }
  else if (selectMenu == 0 && selectSubMenu > 0)
  {
    selectSubMenu = 0;
  }

  // SELECT MENU SETTING LIMIT
  else if (selectMenu == 1 && selectSubMenu == 0)
  {
    selectSubMenu = 1;
    // Read data from EEPROM
    limitAlarm = readInt8InEEPROM(LIMIT_ADDRESS_EEPROM);
    setLimitAlarm = limitAlarm;

    Serial.println("Limit Alarm: " + String(limitAlarm));
    Serial.println("Set Limit Alarm: " + String(setLimitAlarm));
  }
  else if (selectMenu == 1 && selectSubMenu > 0)
  {

    // Save data to EEPROM
    updateEEPROM(LIMIT_ADDRESS_EEPROM, setLimitAlarm);
    updateLCD("Save...........", "");
    delay(1000);
    // Update
    limitAlarm = setLimitAlarm;
    selectSubMenu = 0;
  }
  // SELECT MENU UPDATE CH
  else if (selectMenu == 2 && selectSubMenu == 0)
  {
    selectSubMenu = 1;
    // Read data from EEPROM
    timeUpdateCH = readInt8InEEPROM(TIME_UPDATE_CH_EEPROM);
    setTimeUpdateCH = timeUpdateCH;
  }
  else if (selectMenu == 2 && selectSubMenu > 0)
  {
    // Save data to EEPROM
    updateEEPROM(TIME_UPDATE_CH_EEPROM, setTimeUpdateCH);
    updateLCD("Save...........", "");
    delay(1000);
    // Update
    timeUpdateCH = setTimeUpdateCH;
    selectSubMenu = 0;
  }
}

void btnUpDownOnEventPressed()
{
}

void settingPage()
{
  String line1 = "";
  String line2 = "";
  if (indexMenu != 1)
  {
    return;
  }

  if (selectMenu == 0)
  {
    line1 = ">VOLTAGE LIST";
    line2 = " SETTTING LIMIT";

    if (selectSubMenu > 0)
    {
      showVoltageList(selectSubMenu1, line1, line2);
    }
  }
  else if (selectMenu == 1)
  {
    line1 = " VOLTAGE LIST";
    line2 = ">SETTTING LIMIT";

    if (selectSubMenu > 0)
    {
      showSettingLimit(setLimitAlarm, line1, line2);
    }
  }
  else if (selectMenu == 2)
  {
    line1 = ">UPDATE CH";
    line2 = " SERVER : " + String(statusServer ? "ONLINE" : "OFFLINE");

    if (selectSubMenu > 0)
    {
      showSettingUpdateCH(setTimeUpdateCH, line1, line2);
    }
  }
  else if (selectMenu == 3)
  {
    line1 = " UPDATE CH";
    // line2 = ">UPDATE DATA";
    line2 = ">SERVER : " + String(statusServer ? "ONLINE" : "OFFLINE");
    if (selectSubMenu > 0)
    {
      showSettingUpdateData(setTimeUpdateData, line1, line2);
    }
  }
  else if (selectMenu == 4)
  {
    line1 = ">ETH : " + String(statusETH ? "TRUE" : "FALSE");
    line2 = " ";
  }

  else if (selectMenu > 4)
  {
    selectMenu = 0;
  }
  else if (selectMenu < 0)
  {
    selectMenu = 4;
  }
  updateLCD(line1, line2);
}

void showVoltageList(int &_index, String &line1, String &line2)
{
  if (selectSubMenu == 0)
  {
    return;
  }
  if (_index >= INDEX_RELAY_MAX)
  {
    _index = 0;
  }
  else if (_index < 0)
  {
    _index = INDEX_RELAY_MAX - 1;
  }

  line1 = " Curr CH" + String(indexRelay + 1) + ": " + String(chData[indexRelay], 0) + "V";
  line2 = ">CH" + String(_index + 1) + ": " + String(chData[_index], 0) + "V";
}

void showSettingLimit(int &myLimit, String &line1, String &line2)
{
  if (selectSubMenu == 0)
  {
    return;
  }

  if (myLimit < 1)
  {
    myLimit = 250;
  }
  else if (myLimit > 250)
  {
    myLimit = 1;
  }

  line1 = " LIMIT: " + String(limitAlarm);
  line2 = ">SET : " + String(myLimit) + "V";
}

void showSettingUpdateCH(int &myTime, String &line1, String &line2)
{
  if (selectSubMenu == 0)
  {
    return;
  }
  if (myTime < 2)
  {
    myTime = 60;
  }
  else if (myTime > 60)
  {
    myTime = 2;
  }
  line1 = " UPDATE CH: " + String(timeUpdateCH);
  line2 = ">SET : " + String(myTime) + "s";
}

void showSettingUpdateData(int &myTime, String &line1, String &line2)
{
  if (selectSubMenu == 0)
  {
    return;
  }

  if (myTime > 60)
  {
    myTime = 1;
  }
  else if (myTime < 1)
  {
    myTime = 60;
  }

  line1 = " UPDATE DATA: " + String(timeUpdateData);
  line2 = ">SET : " + String(myTime) + "s";
}

// -------------------- EEPROM -------------------- //
void updateEEPROM(int index, uint8_t data)
{
  EEPROM.update(index, data);
}

void updateInt16ToEEPROM(int address, uint16_t data)
{
  EEPROM.update(address, data >> 8);       // Store the higher byte
  EEPROM.update(address + 1, data & 0xFF); // Store the lower byte
}

void updateEEPROM(int index, String data)
{
  int len = data.length();
  for (int i = 0; i < len; i++)
  {
    EEPROM.update(index + i, data[i]);
  }
}

uint8_t readInt8InEEPROM(int index)
{
  return EEPROM.read(index);
}

uint16_t readInt16CInEEPROM(int index)
{
  int data = 0;
  data = EEPROM.read(index) << 8; // Read the higher byte and shift it
  data |= EEPROM.read(index + 1); // Read the lower byte and OR it
  return data;
}

float getVoltageRMS()
{
  uint16_t MBdata[10];
  uint8_t j, result;
  digitalWrite(led_mod_bus, 1);

  result = node.readHoldingRegisters(0, 8);
  bool isError = false;
  // If read is successful, print the registers
  if (result == node.ku8MBSuccess)
  {
    for (j = 0; j < 10; j++)
    {
      MBdata[j] = node.getResponseBuffer(j);
    }
  }
  else
  {
    isError = true;
    Serial.print("Error reading registers. Code: ");
    Serial.println(result);
  }
  digitalWrite(led_mod_bus, 0);
  /*
  10:25:45.711 -> Register 0: 1 Status of Alarm
  10:25:45.744 -> Register 1: 0
  10:25:45.744 -> Register 2: 125 Voltage (Decimal Point = 1, V = 12.5)
  10:25:45.778 -> Register 3: 0
  10:25:45.778 -> Register 4: 151
  10:25:45.811 -> Register 5: 0
  10:25:45.811 -> Register 6: 1
  10:25:45.845 -> Register 7: 10
  10:25:45.845 -> Register 8: 0
  10:25:45.884 -> Register 9: 0
  */
  float voltage = 0;
  if (!isError)
  {
    // for (j = 0; j < 10; j++)
    // {
    //   Serial.print("Register ");
    //   Serial.print(j);
    //   Serial.print(": ");
    //   Serial.println(MBdata[j]);
    // }
    // Serial.println("====================================");
    // Serial.print("Status of Alarm: ");
    // Serial.println(MBdata[0]);
    // Serial.print("Voltage: ");
    voltage = MBdata[2] / 10.0;
    // Serial.println(voltage, 1);
    chDataInt[indexRelay] = MBdata[2];
  }

  return voltage;
}