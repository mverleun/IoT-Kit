/*
  25-04-2018   1.0.0 Initial version
*/

#include <Homie.h>

/*
A scanner to detect I2C devices. Can be used to determine the address
of an attached I2C devices
*/


// SCL = D1
// SDA = D2
#include <Wire.h>


HomieNode diagNode("diag", "diag");

void setupHandler() {
  // Initialize the I2C bus
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  Wire.begin(D1, D2);
  diagNode.setProperty("address").send("");
}

void loopHandler() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");
      diagNode.setProperty("address").send(String(address));
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("Done.\n");
  }

  delay(2000);
}

void onHomieEvent(const HomieEvent& event) {
  switch (event.type) {
    case HomieEventType::STANDALONE_MODE:
      Serial << "Standalone mode started" << endl;
      break;
    case HomieEventType::CONFIGURATION_MODE:
      Serial << "Configuration mode started" << endl;
      break;
    case HomieEventType::NORMAL_MODE:
      Serial << "Normal mode started" << endl;
      break;
    case HomieEventType::OTA_STARTED:
      Serial << "OTA started" << endl;
      break;
    case HomieEventType::OTA_FAILED:
      Serial << "OTA failed" << endl;
      break;
    case HomieEventType::OTA_SUCCESSFUL:
      Serial << "OTA successful" << endl;
      break;
    case HomieEventType::ABOUT_TO_RESET:
      Serial << "About to reset" << endl;
      break;
    case HomieEventType::WIFI_CONNECTED:
      Serial << "Wi-Fi connected, IP: " << event.ip << ", gateway: " << event.gateway << ", mask: " << event.mask << endl;
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      Serial << "Wi-Fi disconnected, reason: " << (int8_t)event.wifiReason << endl;
      break;
    case HomieEventType::MQTT_READY:
      Serial << "MQTT connected" << endl;
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      Serial << "MQTT disconnected, reason: " << (int8_t)event.mqttReason << endl;
      break;
    case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
      Serial << "MQTT packet acknowledged, packetId: " << event.packetId << endl;
      break;
    case HomieEventType::READY_TO_SLEEP:
      Serial << "Ready to sleep" << endl;
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-i2cscanner", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);


  Homie.setup();


}

void loop() {
  Homie.loop();
}
