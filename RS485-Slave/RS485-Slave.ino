#include <ModbusRTUSlave.h>
#include <SoftwareSerial.h>

// กำหนดพอร์ต SoftwareSerial
SoftwareSerial mySerial(10, 11);  // RX, TX
const uint8_t slaveID = 6;
// สร้างอินสแตนซ์ ModbusRTUSlave ด้วย SoftwareSerial
ModbusRTUSlave slave(mySerial, 7);  // DE/RE_PIN

void setup() {
  mySerial.begin(9600);  // เริ่มต้น RS485
  slave.begin(6, 9600);  // เริ่มต้น Modbus Slave ด้วย ID 6
  slave.configureHoldingRegisters(new uint16_t[10], 10);  // กำหนด Holding Registers
}

void loop() {
  slave.poll();  // รอคำขอจาก Master
}
