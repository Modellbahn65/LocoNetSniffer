
#define ARDUINO_ESP32_MCU_esp32

#include <LocoNetStreamESP32.h>

LocoNetBus bus;

#define LOCONET_PIN_RX 22
#define LOCONET_PIN_TX 23
#define LOCONET_UART_SIGNAL_INVERT_RX false
#define LOCONET_UART_SIGNAL_INVERT_TX true

#include <LocoNetStream.h>
LocoNetDispatcher parser(&bus);
LocoNetStreamESP32 lnStream(1, LOCONET_PIN_RX, LOCONET_PIN_TX, LOCONET_UART_SIGNAL_INVERT_RX, LOCONET_UART_SIGNAL_INVERT_TX, &bus);


// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all Sensor messages
void notifySensor( uint16_t Address, uint8_t State ) {
  Serial.print("Sensor: ");
  Serial.print(Address, DEC);
  Serial.print(" - ");
  Serial.println( State ? "Active" : "Inactive" );
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all Switch Request messages
void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction ) {
  Serial.print("Switch Request: ");
  Serial.print(Address, DEC);
  Serial.print(':');
  Serial.print(Direction ? "Closed" : "Thrown");
  Serial.print(" - ");
  Serial.println(Output ? "On" : "Off");
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all Switch Output Report messages
void notifySwitchOutputsReport( uint16_t Address, uint8_t ClosedOutput, uint8_t ThrownOutput) {
  Serial.print("Switch Outputs Report: ");
  Serial.print(Address, DEC);
  Serial.print(": Closed - ");
  Serial.print(ClosedOutput ? "On" : "Off");
  Serial.print(": Thrown - ");
  Serial.println(ThrownOutput ? "On" : "Off");
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all Switch Sensor Report messages
void notifySwitchReport( uint16_t Address, uint8_t State, uint8_t Sensor ) {
  Serial.print("Switch Sensor Report: ");
  Serial.print(Address, DEC);
  Serial.print(':');
  Serial.print(Sensor ? "Switch" : "Aux");
  Serial.print(" - ");
  Serial.println( State ? "Active" : "Inactive" );
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all Switch State messages
void notifySwitchState( uint16_t Address, uint8_t Output, uint8_t Direction ) {
  Serial.print("Switch State: ");
  Serial.print(Address, DEC);
  Serial.print(':');
  Serial.print(Direction ? "Closed" : "Thrown");
  Serial.print(" - ");
  Serial.println(Output ? "On" : "Off");
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all Power messages
void notifyPower(uint8_t State) {
  Serial.print("Layout Power State: ");
  Serial.println(State ? "On" : "Off");
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all MultiSensePower messages
void notifyMultiSensePower(uint8_t BoardID, uint8_t Subdistrict, uint8_t Mode, uint8_t Direction) {
  Serial.print("MultiSensePower: Board ID: ");
  Serial.print(BoardID, DEC);
  Serial.print(" Sub District: ");
  Serial.print(Subdistrict, DEC);
  Serial.print(" Mode: ");
  Serial.print(Mode, DEC);
  Serial.print(" Direction: ");
  Serial.println(Direction, DEC);
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all notifyMultiSenseTransponder messages
void notifyMultiSenseTransponder(uint16_t Address, uint8_t Zone, uint16_t LocoAddress, uint8_t Present) {
  Serial.print("MultiSenseTransponder: Address: ");
  Serial.print(Address, DEC);
  Serial.print(" Zone: ");
  Serial.print(Zone, DEC);
  Serial.print(" Loco Address: ");
  Serial.print(LocoAddress, DEC);
  Serial.print(" Present: ");
  Serial.println(Present, DEC);
}

// This call-back function is called from LocoNet.processSwitchSensorMessage
// for all LongAck messages

void notifyLongAck(uint8_t d1, uint8_t d2) {
  Serial.print("LongACK : Data Byte 1: ");
  Serial.print(d1, DEC);
  Serial.print(" Data Byte 2: ");
  Serial.println(d2, DEC);

}

void setup() {
  
  Serial.begin(115200);
  Serial.println("LocoNet2 Basic Demo");

  lnStream.start();

  parser.onPacket(CALLBACK_FOR_ALL_OPCODES, [](const lnMsg *rxPacket) {
      char tmp[100];
      formatMsg(*rxPacket, tmp, sizeof(tmp));
      Serial.printf("onPacket: %s\n", tmp);
  });

  parser.onSensorChange(notifySensor);
  parser.onSwitchRequest(notifySwitchRequest);
  parser.onSwitchReport(notifySwitchReport);
  parser.onSwitchState(notifySwitchState);
  parser.onPowerChange(notifyPower);
  parser.onMultiSenseTransponder(notifyMultiSenseTransponder);
  parser.onMultiSenseDeviceInfo(notifyMultiSensePower);

}

uint16_t sensorIndex = 0;

void loop() {
  lnStream.process();

  delay(10);
}
