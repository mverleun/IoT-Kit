/*
  03-04-2018   1.0.0 Initial version
*/

#include <Homie.h>

#define STATUS_LED 12
#define P044_BUFFER_SIZE 1024
#define NETBUF_SIZE 600
#define DISABLED 0
#define WAITING 1
#define READING 2
#define CHECKSUM 3
#define DONE 4

boolean serialdebug = false;
char* Plugin_044_serial_buf;
unsigned int bytes_read = 0;
boolean CRCcheck = false;
unsigned int currCRC = 0;
int checkI = 0;
boolean success = false;
static int state = DISABLED;

HomieNode electricityNode("electricity", "current");
HomieNode gasNode("gas", "current");


bool textHandler(HomieRange range, String value) {
  Homie.getLogger() << "Received text " << value << endl;
  return true;
}

/*
   validP1char
       checks whether the incoming character is a valid one for a P1 datagram. Returns false if not, which signals corrupt datagram
*/
bool validP1char(char ch) {
  if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '.') || (ch == '!') || (ch == ' ') || (ch == 92) || (ch == 13) || (ch == '\n') || (ch == '(') || (ch == ')') || (ch == '-') || (ch == '*') || (ch == ':') )
  {
    return true;
  } else {
    // addLog(LOG_LEVEL_DEBUG, F("P1   : Error: invalid char read from P1"));
    if (serialdebug) {
      Serial.print(F("faulty char>"));
      Serial.print(ch);
      Serial.println(F("<"));
    }
    return false;
  }
}

int FindCharInArrayRev(char array[], char c, int len) {
  for (int i = len - 1; i >= 0; i--) {
    if (array[i] == c) {
      return i;
    }
  }
  return -1;
}

/*
   CRC16
      based on code written by Jan ten Hove
     https://github.com/jantenhove/P1-Meter-ESP8266
*/
unsigned int CRC16(unsigned int crc, unsigned char *buf, int len)
{
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }

  return crc;
}

void blinkLED() {
  digitalWrite(STATUS_LED, 1);
  delay(500);
  digitalWrite(STATUS_LED, 0);
}

/*  checkDatagram
      checks whether the checksum of the data received from P1 matches the checksum attached to the
      telegram
     based on code written by Jan ten Hove
     https://github.com/jantenhove/P1-Meter-ESP8266
*/
bool checkDatagram(int len) {
  int startChar = FindCharInArrayRev(Plugin_044_serial_buf, '/', len);
  int endChar = FindCharInArrayRev(Plugin_044_serial_buf, '!', len);
  bool validCRCFound = false;

  if (!CRCcheck) return true;

  if (serialdebug) {
    Serial.print(F("input length: "));
    Serial.println(len);
    Serial.print("Start char \\ : ");
    Serial.println(startChar);
    Serial.print(F("End char ! : "));
    Serial.println(endChar);
  }

  if (endChar >= 0)
  {
    currCRC = CRC16(0x0000, (unsigned char *) Plugin_044_serial_buf, endChar - startChar + 1);

    char messageCRC[5];
    strncpy(messageCRC, Plugin_044_serial_buf + endChar + 1, 4);
    messageCRC[4] = 0;
    if (serialdebug) {
      for (int cnt = 0; cnt < len; cnt++)
        Serial.print(Plugin_044_serial_buf[cnt]);
    }

    validCRCFound = (strtoul(messageCRC, NULL, 16) == currCRC);
    if (!validCRCFound) {
      // addLog(LOG_LEVEL_DEBUG, F("P1   : Error: invalid CRC found"));
    }
    currCRC = 0;
  }
  return validCRCFound;
}

void setupHandler() {
  WiFiServer *P1GatewayServer;
  WiFiClient P1GatewayClient;

  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, 0);

  if (!Plugin_044_serial_buf)
    Plugin_044_serial_buf = (char *)malloc(P044_BUFFER_SIZE);

  if (P1GatewayClient.connected()) {
    int RXWait = 1;

    int timeOut = RXWait;
    while (timeOut > 0) {
      while (Serial.available() && state != DONE) {
        if (bytes_read < P044_BUFFER_SIZE - 5) {
          char  ch = Serial.read();
          digitalWrite(STATUS_LED, 1);
          switch (state) {
            case DISABLED: //ignore incoming data
              break;
            case WAITING:
              if (ch == '/')  {
                Plugin_044_serial_buf[0] = ch;
                bytes_read=1;
                state = READING;
              } // else ignore data
              break;
            case READING:
              if (ch == '!') {
                if (CRCcheck) {
                  state = CHECKSUM;
                } else {
                  state = DONE;
                }
              }
              if (validP1char(ch)) {
                Plugin_044_serial_buf[bytes_read] = ch;
                bytes_read++;
              } else if (ch=='/') {
                // addLog(LOG_LEVEL_DEBUG, F("P1   : Error: Start detected, discarded input."));
                Plugin_044_serial_buf[0] = ch;
                bytes_read = 1;
              } else {              // input is non-ascii
                // addLog(LOG_LEVEL_DEBUG, F("P1   : Error: DATA corrupt, discarded input."));
                Serial.flush();
                bytes_read = 0;
                state = WAITING;
              }
              break;
            case CHECKSUM:
              checkI ++;
              if (checkI == 4) {
                checkI = 0;
                state = DONE;
              }
              Plugin_044_serial_buf[bytes_read] = ch;
              bytes_read++;
              break;
            case DONE:
            // Plugin_044_serial_buf[bytes_read]= '\n';
            // bytes_read++;
            // Plugin_044_serial_buf[bytes_read] = 0;
              break;
            }
          }
          else {
            Serial.read();      // when the buffer is full, just read remaining input, but do not store...
            bytes_read = 0;
            state = WAITING;    // reset
          }
          digitalWrite(STATUS_LED, 0);
          timeOut = RXWait; // if serial received, reset timeout counter
        }
        delay(1);
        timeOut--;
      }

      if (state == DONE) {
        if (checkDatagram(bytes_read)) {
          Plugin_044_serial_buf[bytes_read] = '\r';
          bytes_read++;
          Plugin_044_serial_buf[bytes_read] = '\n';
          bytes_read++;
          Plugin_044_serial_buf[bytes_read] = 0;
          P1GatewayClient.write((const uint8_t*)Plugin_044_serial_buf, bytes_read);
          P1GatewayClient.flush();
          // addLog(LOG_LEVEL_DEBUG, F("P1   : data send!"));
          blinkLED();
          } else {
            // addLog(LOG_LEVEL_DEBUG, F("P1   : Error: Invalid CRC, dropped data"));
          }
          bytes_read = 0;
          state = WAITING;
        }   // state == DONE
      }
      success = true;
    }
}

void loopHandler() {
  Serial << "loopHandler" << endl;
  delay(2000);

}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-P1-Meter", "1.0.0");

  //electricityNode.advertise("text").settable(textHandler);

  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  Homie.setup();

}

void loop() {
  Homie.loop();
}
