#ifndef __RNBEE_H__
#define __RNBEE_H__

#include <Arduino.h>

#define RNBEE_LOG(...)                                                         \
  {                                                                            \
    if (_logSerial)                                                            \
      _logSerial->print(__VA_ARGS__);                                          \
  }
#define RNBEE_LOGLN(...)                                                       \
  {                                                                            \
    if (_logSerial)                                                            \
      _logSerial->println(__VA_ARGS__);                                        \
  }

class RnBee {
public:
  RnBee(uint8_t initPin, HardwareSerial *serial);
  bool setup();
  uint8_t sendRawPayload(unsigned char *data, uint16_t size);

  void sendCmd(char *cmd);

  void waitForResponse();

  void sendData(char *data);

  void init();

  void setLogSerial(HardwareSerial *logSerial) { _logSerial = logSerial; }

private:
  char getHexHi(char ch);
  char getHexLo(char ch);

  uint8_t _txPower;
  HardwareSerial *_loraSerial;
  HardwareSerial *_logSerial;
  uint8_t _initPin;
};

#endif
