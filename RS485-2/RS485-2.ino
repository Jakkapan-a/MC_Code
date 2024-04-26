
#include <ModbusMaster.h>
#include <math.h> 

ModbusMaster node;

const int led = 13;
#define MAX485_DE      7
#define MAX485_RE_NEG  6

#define MbSlaveID 5

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}
void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  pinMode(led, OUTPUT);
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

//=====================Modbus master==============================//
  node.begin(MbSlaveID, Serial3);
  handleRoot();
}
// Declare global variables to avoid scope issues


void loop() {
  handleRoot();
  delay(100);  // Delay between readings
}

void handleRoot() 
{
  uint16_t MBdata[10];
  uint8_t j, result;
  digitalWrite(led, 1);

  // Callbacks allow us to configure the RS485 transceiver correctly
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
   // Read 16 registers starting at 0
  result = node.readHoldingRegisters(0, 8);
  bool isError = true;
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
    isError =false;
    Serial.print("Error reading registers. Code: ");
    Serial.println(result);
  }
  digitalWrite(led, 0);
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
  if(isError){
    // for (j = 0; j < 10; j++)
    // {
    //   Serial.print("Register ");
    //   Serial.print(j);
    //   Serial.print(": ");
    //   Serial.println(MBdata[j]);
    // }
    Serial.println("====================================");
    Serial.print("Status of Alarm: ");
    Serial.println(MBdata[0]);
    Serial.print("Voltage: ");
    float voltage = MBdata[2]/10.0;
    Serial.println(voltage,1);
  }
}
