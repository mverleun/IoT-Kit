/*
  25-04-2018   1.2.0 Implemented boolean value to represent dusk status
  04-04-2018   1.1.0 Implemented JSON output
  03-04-2018   1.0.0 Initial version
*/

#include <Homie.h>

#include <ArduinoJson.h>

StaticJsonBuffer<100> jsonBuffer;

#include <Wire.h>
#include <BH1750.h>

// SCL = D1
// SDA = D2
#define MQTT_SERVER_FINGERPRINT {0xd6, 0x27, 0x18, 0xdd, 0x09, 0xce, 0x3d, 0x80, 0x1f, 0x59, 0x7a, 0x2b, 0x29, 0x78, 0x93, 0xe8, 0x0b, 0x82, 0x8a, 0x5d}

BH1750 lightMeter(0x23);
float current_lux;
float previous_lux = 0.0;

bool dusk = false;
unsigned long dusk_value = 45;        // Level from sensor
unsigned long minInterval = 1 * 60;   // Minimum interval between status changes dusk
unsigned long previousUpdate = 0L;

HomieNode lightNode("light", "light");

void setupHandler() {
  lightNode.setProperty("unit").send("lux");
}

void loopHandler() {
  JsonObject& JSONroot = jsonBuffer.createObject();
  String JSONmessageBuffer;
  JSONroot["name"] = String(Homie.getConfiguration().name);

  uint16_t current_lux = lightMeter.readLightLevel();
  unsigned long now = millis();

  // Only publish info if there is a change in Temperature
  if ( current_lux != previous_lux ) {
    Homie.getLogger() << "Light: " << current_lux << " lux" << endl;
    // Publish values as float.
    lightNode.setProperty("light").send(String(current_lux));
    // Publish data in JSON format
    JSONroot["name"] = String(Homie.getConfiguration().name);
    JSONroot["metric"] = "light";
    JSONroot["value"] = String(current_lux);

    //Convert JSON to string
    JSONmessageBuffer = "";
    JSONroot.printTo(JSONmessageBuffer);

    // Publish JSON information
    lightNode.setProperty("json").send(JSONmessageBuffer);

    // Clear JSON buffer
    jsonBuffer.clear();

    previous_lux = current_lux;
  }

  // Check if the previous update of the dusk status was some time
  // ago to avoid flipflopping.
  if (now - previousUpdate > minInterval * 1000L) {
    // Are we above dusk level but still in dusk states?
    if (current_lux > dusk_value and dusk == true) {
      Homie.getLogger() << "Dusk: Changing to false";
      lightNode.setProperty("dusk").send("0");
      previousUpdate = now;
    }
    // Are we below dusk level and not in dusk status?
    if (current_lux < dusk_value and dusk == false) {
      Homie.getLogger() << "Dusk: Changing to true";
      lightNode.setProperty("dusk").send("1");
      previousUpdate = now;
    }
  }
  // Delay between measurements.
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-bh1750", "1.0.2");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  lightNode.advertise("unit");
  lightNode.advertise("lux");

  Homie.setup();
  #if ASYNC_TCP_SSL_ENABLED
    Homie.getLogger() << endl << endl << F("ESP Homie Using TLS") << endl;
    Homie.getMqttClient().setSecure(true);
//    Homie.getMqttClient().addServerFingerprint((const uint8_t[])MQTT_SERVER_FINGERPRINT);
  #endif
  Wire.begin(D1, D2);

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop() {
  Homie.loop();
}
