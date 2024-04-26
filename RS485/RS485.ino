/**
 * @file RS485.ino
 * @author Jakkapan
 * @brief 
 * @version 0.1
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 * @note MODBUS RTU RS485 Communication with DIGITAL AC VOLT METER TVM-94N
 */

#include <ModbusMaster.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
ModbusMaster node;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  node.begin(2, mySerial); // Node ID 1 
}

void loop() {

  /**
   Modbus Function Code 
   Code   | Operation                 | Broadcast
   04     | Read Input Registers      | No
   06     | Read Holding Registers    | Yes
   08     | Loop Back Diagnostic      | No 
   */

  /**
   Modbus Exception Code
    Code   | Name                 | Meaning
    01     | ILLEGAL FUNCTION     | The function code received in the query is not an allowable action for the server (or slave).
    02     | ILLEGAL DATA ADDRESS | The data address received in the query is not an allowable address for the server (or slave).
    03     | ILLEGAL DATA VALUE   | A value contained in the query data field is not an allowable value for the server (or slave).
   */

  /**
  Modbus RTU Table TVM-94N(HEX)
  Address | Content            | Format | Word | Access
  0       | Status of Alarm    | int    | 1    | Read/Write
  1       | PV                | long  | 2    | Read Only
  2       | PV                | long  | 2    | Read Only
  3       | PV Peak Hold      | long  | 2    | Read Only
  4       | PV Peak Hold      | long  | 2    | Read Only
  5       | SV               | int  | 1    | Read/Write
  6       | Multiplier       | int  | 1    | Read/Write
  7       | Divisor          | int  | 1    | Read/Write
  8       | PV Adjust       | int  | 1    | Read/Write
  9       | PV Filter       | int  | 1    | Read/Write
  A      | Decimal Point   | int  | 1    | Read/Write
  B      | Lock Key Function | int  | 1    | Read/Write
  C      | Function Alarm 1 | int  | 1    | Read/Write
  D      | High Alarm 1     | int  | 1    | Read/Write
  E      | Low Alarm 1      | int  | 1    | Read/Write
  F      | Hysterisis Alarm 1 | int  | 1    | Read/Write
  10     | Delay On Alarm 1  | int  | 1    | Read/Write
  11     | Delay Off Alarm 1  | int  | 1    | Read/Write
  12     | Function Alarm 2  | int  | 1    | Read/Write
  13     | High Alarm 2      | int  | 1    | Read/Write
  14     | Low Alarm 2       | int  | 1    | Read/Write
  15     | Hysterisis Alarm 2 | int  | 1    | Read/Write
  16     | Delay On Alarm 2  | int  | 1    | Read/Write
  17     | Delay Off Alarm 2  | int  | 1    | Read/Write
  18     | Function Alarm 3  | int  | 1    | Read/Write
  19     | High Alarm 3      | int  | 1    | Read/Write
  1A     | Low Alarm 3       | int  | 1    | Read/Write
  1B     | Hysterisis Alarm 3 | int  | 1    | Read/Write
  1C     | Delay On Alarm 3  | int  | 1    | Read/Write
  1D     | Delay Off Alarm 3  | int  | 1    | Read/Write

  */

  /**
    Example of a Cllent Request and Server Exceptlon Response
     Request
    Field Name | Hex Value
    Slave Address | 01
    Function Code | 04
    Starting Address Hi | 00
    Starting Address Lo | 00
    Quantity of Input Reg. Hi | 00
    Quantity of Input Reg. Lo | 1E
    CRC Hi | 70
    CRC Lo | 02

    Response
    Field Name      | Hex Value
    Slave Address   | 01
    Function Code   | 84
    Exception Code  | 02
    CRC Hi          | C2
    CRC Lo          | C1
  */

  /**
   * Example of a Request to Wrlte Input Reglster 2 to 00 08 hex:
   
    Request
    Field Name | Hex Value
    Slave Address | 01
    Function Code | 06
    Register Address Hi | 00
    Register Address Lo | 04
    Register Value Hi | 03
    Register Value Lo | E8
    CRC Hi | C8
    CRC Lo | B5

    Response
    Field Name | Hex Value
    Slave Address | 01
    Function Code | 06
    Register Address Hi | 00
    Register Address Lo | 00
    Register Value Hi | 03
    Register Value Lo | E8
    CRC Hi | C8
    CRC Lo | B5
   */
  
 // Define a variable to store the result
  uint8_t result;
  uint32_t voltage;

  // Read two words starting from register address 1 (assuming voltage data starts here)
  result = node.readHoldingRegisters(1, 2);

  if (result == node.ku8MBSuccess) {
    voltage = (node.getResponseBuffer(0) << 16) | node.getResponseBuffer(1);

    Serial.print("Voltage: ");
    Serial.println(voltage);
  } else {
    // Provide specific error details
    Serial.print("Failed to read voltage. Error code: ");
    Serial.println(result);
  }
  delay(1000);
}
