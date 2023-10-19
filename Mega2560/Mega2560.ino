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

#define AC_VOLTAGE1_PIN A3
AcVoltage acVoltageA1(AC_VOLTAGE1_PIN);

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
void serialEvent()
{
  Serial1.print("Mega :");
  while (Serial.available())
  {
    byte incomeByte = Serial.read();
    Serial1.write(incomeByte);
  }
    Serial1.println("");
}
void serialEvent1()
{
  while (Serial1.available())
  {
    byte incomeByte = Serial1.read();
    Serial.write(incomeByte);

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
  Serial1.begin(115200);

  acVoltageA1.begin();

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
  lcd.setCursor(0, 0);
  lcd.print("RMS Voltage:");
  lcd.setCursor(0, 1);
  lcd.clear();
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
  SecondsCounter();
  MicrosCounter();

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
  }
  else if (currentMillis < previousMillis)
  { // Overflow
    previousMillis = currentMillis;
  }
}

void MicrosCounter(void)
{
  unsigned long currentMicros = millis();
  if (currentMicros - previousMicros >= 500) // 500ms
  {
    previousMicros = currentMicros;

    // Get RMS Voltage
    uint32_t rmsVoltage = acVoltageA1.getVoltageRMS();

    // char buf[10];
    // sprintf(buf, "%3u V", rmsVoltage);

    lcd.setCursor(0, 0);
    lcd.print("Voltage: ");
    lcd.setCursor(0, 1);
    lcd.print(rmsVoltage);

    // Serial.print("Voltage: ");
    // Serial.print(rmsVoltage);

    // Serial.print(" |Buf: ");
    // Serial.println(buf);
  }
  else if (currentMicros < previousMicros)
  { // Overflow
    previousMicros = currentMicros;
  }
}

// ------------------   ESC  ------------------ //
void OnPress_ESC(void)
{
  Serial.println("OnPress_ESC");
  tone(BUZZER_PIN, 2500, 100);
  Serial1.println("OnPress_ESC");
  CountNoBacklight = NoBacklightTime;
  lcd.backlight();
}

void OnRelease_ESC(void)
{
}
// ------------------   UP  ------------------ //
void OnPress_UP(void)
{
  Serial.println("OnPress_UP");
  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  lcd.backlight();
}
void OnRelease_UP(void)
{
}

// ------------------   DOWN  ------------------ //
void OnPress_DOWN(void)
{
  Serial.println("OnPress_DOWN");
  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  lcd.backlight();
}

void OnRelease_DOWN(void)
{
}

// ------------------   ENTER  ------------------ //
void OnPress_ENTER(void)
{
  Serial.println("OnPress_ENTER");

  tone(BUZZER_PIN, 2500, 100);
  CountNoBacklight = NoBacklightTime;
  lcd.backlight();
}

void OnRelease_ENTER(void)
{
}