/*
  12-04-2018   1.0.0 Initial version
*/

#include <Homie.h>
#include "Grove_I2C_Motor_Driver.h"
#include <Wire.h>

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

HomieNode motorNode("motor", "motor");

void setupHandler() {
  Serial.println("Entering setupHandler");
  Motor.begin(I2C_ADDRESS); // join i2c bus (address optional for master)
}

void loopHandler() {
  Serial.println("Entering loopHandler");
  // Set speed of MOTOR1, Clockwise, speed: -100~100
  Motor.speed(MOTOR1, 50);
  // Set speed of MOTOR2, Anticlockwise
  Motor.speed(MOTOR2, -70);
  delay(2000);
  // Change speed and direction of MOTOR1
  Motor.speed(MOTOR1, -100);
  // Change speed and direction of MOTOR2
  Motor.speed(MOTOR2, 100);
  delay(2000);
  // Stop MOTOR1 and MOTOR2
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
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
  Homie_setFirmware("wemos-d1-tb6612", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);


  Homie.setup();

}

void loop() {
  Homie.loop();
}
