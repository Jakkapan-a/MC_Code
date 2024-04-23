const int analogInPin = A0;  // Analog input pin that the voltage divider is connected to
float voltage;
float temp;
int analogValue;
float r1 = 1000000.0;  //1 Mohm
float r2 = 10000.0;    //10 kOhm
void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
}
#define BUFFER_SIZE_DATA_VOLTAGE 500
int inputStringLengthVoltage = 0;
float rms_voltage[BUFFER_SIZE_DATA_VOLTAGE];

uint32_t lastVoltageMillis = 0;
uint32_t _lastVoltageMillis = 0;
void loop() {
  analogValue = analogRead(A0);
  //Convert analog value from a range of 0-1203 to 0-5
  //NOTE: Arduino analog maximum input voltage is 5V
  temp = (analogValue * 5.0) / 1024.0;
  // 1:100 (10kOhm:1Mohm) voltage divider used to increase voltage range
  //Voltage divider equation used to convert voltage received to input voltage
  voltage = temp / (r2 / (r1 + r2));
  // Print the RMS voltage value to the serial monitor
  // Serial.print("Voltage RMS: ");
  // Serial.print(voltage);
  // Serial.println(" V");

  // delay(100);  // Wait for a second before taking the next reading


  uint32_t currentMillis = millis();
  if (currentMillis - lastVoltageMillis >= 1) {
    _lastVoltageMillis = currentMillis;
    if (inputStringLengthVoltage < BUFFER_SIZE_DATA_VOLTAGE) {
      rms_voltage[inputStringLengthVoltage] = voltage;
      inputStringLengthVoltage++;
    } else {
      inputStringLengthVoltage = 0;
    }
  }
  if 
  
  (currentMillis - lastVoltageMillis >= 1000) {
    lastVoltageMillis = currentMillis;
    // get max and min
    float maxVoltage = 0;
    float minVoltage = 9999999;
    for (int i = 0; i < BUFFER_SIZE_DATA_VOLTAGE; i++) {
      if (rms_voltage[i] > maxVoltage) {
        maxVoltage = rms_voltage[i];
      }
      if (rms_voltage[i] < minVoltage) {
        minVoltage = rms_voltage[i];
      }
    }

    // get the difference
    float diffVoltage = maxVoltage - minVoltage;
    // get the square
    float squareVoltage = diffVoltage * diffVoltage;
    // get the mean
    float meanVoltage = squareVoltage / BUFFER_SIZE_DATA_VOLTAGE;
    // get the root
    float rootVoltage = sqrt(meanVoltage);
    Serial.print(" Voltage Min: ");
    Serial.print(minVoltage);
    Serial.print(" Voltage Max: ");
    Serial.print(maxVoltage);
    Serial.print(" Voltage Diff: ");
    Serial.print(diffVoltage);
    Serial.print(" Voltage Square: ");
    Serial.print(squareVoltage);
    Serial.print(" Voltage Mean: ");
    Serial.print(meanVoltage);

    Serial.print(" Voltage RMS: ");
    Serial.print(rootVoltage);
    Serial.println(" V");
  }
}
