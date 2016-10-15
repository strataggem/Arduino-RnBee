#include "RnBee.h"

void RnBee::sendCmd(char *cmd) {
  RNBEE_LOG(cmd);
  RNBEE_LOG("\n");
  _loraSerial->write(cmd);
  _loraSerial->write("\r\n");
  while (!_loraSerial->available()) {
    delay(100);
  }
  while (_loraSerial->available()) {
    char c = _loraSerial->read();
    RNBEE_LOG(c);
  }
}

void RnBee::waitForResponse() {
  while (!_loraSerial->available()) {
    delay(100);
  }

  while (_loraSerial->available()) {
    char c = _loraSerial->read();
    RNBEE_LOG(_loraSerial->read());
  }
}

char RnBee::getHexHi(char ch) {
  char nibble = ch >> 4;
  return (nibble > 9) ? nibble + 'A' - 10 : nibble + '0';
}
char RnBee::getHexLo(char ch) {
  char nibble = ch & 0x0f;
  return (nibble > 9) ? nibble + 'A' - 10 : nibble + '0';
}

uint8_t RnBee::sendRawPayload(unsigned char *data, uint16_t size) {
  RNBEE_LOG("RnBee::sendRawPayload of size ");
  RNBEE_LOGLN(size);
  RNBEE_LOG("radio tx ");
  _loraSerial->write("radio tx ");
  // Write data as hex characters
  uint32_t cpt = 0;
  uint8_t *ptr = data;
  while (cpt < size) {
    _loraSerial->write(getHexHi(*ptr));
    _loraSerial->write(getHexLo(*ptr));

    RNBEE_LOG(getHexHi(*ptr));
    RNBEE_LOG(getHexLo(*ptr));

    ptr++;
    cpt++;
  }
  _loraSerial->write("\r\n");
  RNBEE_LOG("\n");
  delay(500);

  while (_loraSerial->available()) {
    char c = _loraSerial->read();
    RNBEE_LOG(c);
  }

  return cpt;
}

bool RnBee::setup() {

  digitalWrite(BEE_VCC, HIGH);
  _loraSerial->begin(57600);

  return true;
}

void RnBee::init() {
  sendCmd("sys factoryRESET");

  // For LoRa raw transmission
  sendCmd("mac pause");
  sendCmd("radio set pwr 14");
  sendCmd("radio set sf sf7");
  sendCmd("radio set freq 867100000");
  sendCmd("radio set rxbw 125");
  sendCmd("radio set crc on");
}

RnBee::RnBee(uint8_t initPin, HardwareSerial *serial) {
  _initPin = initPin;
  _loraSerial = serial;
  _logSerial = NULL;
}
