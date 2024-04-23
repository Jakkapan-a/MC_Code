

#define LED_A_PIN 2
#define LED_B_PIN 3
#define LED_C_PIN 4
#define LED_D_PIN 5
#define LED_E_PIN 6
#define LED_F_PIN 7
#define LED_G_PIN 8
#define LED_H_PIN 9
#define LED_DP_PIN 10

// Array pin
uint8_t ledPins[] = { LED_A_PIN, LED_B_PIN, LED_C_PIN, LED_D_PIN, LED_E_PIN, LED_F_PIN, LED_G_PIN, LED_H_PIN, LED_DP_PIN };

#define BUFFER_SIZE_DATA 20

char inputString[BUFFER_SIZE_DATA];
int inputStringLength = 0;
boolean startReceived = false;
boolean endReceived = false;

const char startChar = '$';
const char endChar = '#';


void serialEvent() {
  while (Serial.available()) {
    byte inChar = (byte)Serial.read();
    if (inChar == startChar) {
      startReceived = true;
      inputStringLength = 0;
      memset(inputString, 0, sizeof(inputString));
    } else if (startReceived && inChar == endChar) {
      endReceived = true;
    } else if (startReceived) {
      if (inputStringLength < BUFFER_SIZE_DATA - 1) {
        inputString[inputStringLength++] = inChar;
      } else {
        startReceived = false;
        endReceived = false;
        inputStringLength = 0;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  // --- Set all LEDs to LOW (off) --- //
  for (int i = 0; i < 9; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // for(int i = 0; i < 9; i++)
  // {
  //   if(i == 1){
  //     digitalWrite(ledPins[i], HIGH);
  //   }
  // }
  digitalWrite(ledPins[0], 0);
  digitalWrite(ledPins[1], 0);
  digitalWrite(ledPins[2], 0);
  digitalWrite(ledPins[3], 0);
  digitalWrite(ledPins[4], 0);
  digitalWrite(ledPins[5], 0);
  digitalWrite(ledPins[6], !0);
  digitalWrite(ledPins[7], !0);
  digitalWrite(ledPins[8], 0);

  memset(inputString, 0, sizeof(inputString));
  inputStringLength = 0;
}

// the loop function runs over and over again forever
void loop() {
  manageSerial();
}

void manageSerial() {
  if (startReceived && endReceived) {
    startReceived = false;
    endReceived = false;
    inputString[inputStringLength] = '\0';
    Serial.println(inputString);
    // parseData(inputString);
    for (int i = 0; i < 9; i++) {
      if (inputString[i] == '1') {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
    // inputString = "";
    memset(inputString, 0, BUFFER_SIZE_DATA);
    inputStringLength = 0;
  }
}