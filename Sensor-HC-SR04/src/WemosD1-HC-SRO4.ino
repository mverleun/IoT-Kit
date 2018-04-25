
/*
  04-04-2018   1.1.0 Implemented JSON output
  03-04-2018   1.0.0 Initial version
*/
#include <Homie.h>



#define TRIGGER D6
#define ECHO D7


float current_distance;
float previous_distance = 0.0;
long duration;

int  measurement_counter = 0;

HomieNode distanceNode("distance", "distance");

void setupHandler() {
  distanceNode.setProperty("unit").send("cm");
}

void loopHandler() {
  /* The following TRIGGER/ECHO cycle is used to determine the
distance of the nearest object by bouncing soundwaves off of it. */
noInterrupts(); //Avoid interrupts

digitalWrite(TRIGGER, LOW);
delayMicroseconds(2);
digitalWrite(TRIGGER, HIGH);
delayMicroseconds(10);
digitalWrite(TRIGGER, LOW);
duration = pulseIn(ECHO, HIGH);
interrupts(); // Enable interrupts

//Calculate the distance (in cm) based on the speed of sound.
current_distance = duration/58.2;
  // Only publish info if there is a change in distance and if a couple of measurements are close
  if ( abs(current_distance - previous_distance) < 5 ) {
    measurement_counter += 1;
  } else {
    measurement_counter = 0;
  }
  previous_distance = current_distance;

  if (measurement_counter > 3 ) {
    Homie.getLogger() << "Distance: " << current_distance << " cm" << endl;
    distanceNode.setProperty("distance").send(String(current_distance));
    distanceNode.setProperty("json").send("{ \"name\": \"" + String(Homie.getConfiguration().name) + "\""+
                                            ", \"metric\": \"distance\"" +
                                            ", \"value\": " + String(current_distance) +
                                            " }");

    }
    // Delay between measurements.
    delay(1000);
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
  Homie_setFirmware("wemos-d1-hc-sr04", "1.1.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  distanceNode.advertise("unit");
  distanceNode.advertise("cm");

  Homie.setup();

  // Set the GPIO pins correctly
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  Homie.loop();
}
