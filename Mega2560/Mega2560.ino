#include <TcBUTTON.h>
#include <TcPINOUT.h>

// ------------------   INPUT   ------------------ //
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

// ------------------   SETUP   ------------------ //
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  BTN_ESC.setOnPress(OnPress_ESC);
  BTN_ESC.setOnRelease(OnRelease_ESC);

  BTN_UP.setOnPress(OnPress_UP);
  BTN_UP.setOnRelease(OnRelease_UP);

  BTN_DOWN.setOnPress(OnPress_DOWN);
  BTN_DOWN.setOnRelease(OnRelease_DOWN);

  BTN_ENTER.setOnPress(OnPress_ENTER);
  BTN_ENTER.setOnRelease(OnRelease_ENTER);

  
}
// ------------------   LOOP   ------------------ //
void loop() {
  BTN_ESC.update();
  BTN_UP.update();
  BTN_DOWN.update();
  BTN_ENTER.update();
}

// ------------------   FUNCTIONS OF BUTTON   ------------------ //
// ------------------   ESC  ------------------ //
void OnPress_ESC(void) {
  Serial.println("OnPress_ESC");
  tone(BUZZER_PIN, 2500, 100);
  Serial1.println("OnPress_ESC");
}

void OnRelease_ESC(void) {
}
// ------------------   UP  ------------------ //
void OnPress_UP(void) {
  Serial.println("OnPress_UP");
  tone(BUZZER_PIN, 2500, 100);
}
void OnRelease_UP(void) {
}

// ------------------   DOWN  ------------------ //
void OnPress_DOWN(void) {
  Serial.println("OnPress_DOWN");
  tone(BUZZER_PIN, 2500, 100);
}

void OnRelease_DOWN(void) {
}

// ------------------   ENTER  ------------------ //
void OnPress_ENTER(void) {
  Serial.println("OnPress_ENTER");
  tone(BUZZER_PIN, 2500, 100);
}

void OnRelease_ENTER(void) {
}