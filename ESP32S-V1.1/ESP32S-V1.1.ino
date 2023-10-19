
void serialEvent()
{
  Serial.print("Received :");
  while (Serial.available())
  {
    byte incomeByte = Serial.read();
    Serial.write(incomeByte);
  }
    Serial.println("");
}
void setup() {
    Serial.begin(115200);
    // Serial1.begin(115200);

    Serial.println("Hello World!");
    // Serial1.println("Hello World!");
}
int count = 0;
void loop() {

    // count++;
    // Serial.print(count);
    //  Serial.println("Hello World! ");
    //  if(count > 10000){
    //   count = 0;
    //  }
    //  delay(1000);
}