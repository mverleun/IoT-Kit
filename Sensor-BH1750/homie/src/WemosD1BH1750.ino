/*
  25-04-2018   1.2.0 Implemented boolean value to represent dusk status
  04-04-2018   1.1.0 Implemented JSON output
  03-04-2018   1.0.0 Initial version
*/

#include <Homie.h>
#include <Wire.h>
#include <BH1750.h>

// SCL = D1
// SDA = D2

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
  uint16_t current_lux = lightMeter.readLightLevel();
  unsigned long now = millis();

  // Only publish info if there is a change in light intensity
  if ( current_lux != previous_lux ) {
    Homie.getLogger() << "Light: " << current_lux << " lux" << endl;
    // Publish values as float.
    lightNode.setProperty("light").send(String(current_lux));
    // Publish data in JSON format
    lightNode.setProperty("json").send("{ \"name\": \"" + String(Homie.getConfiguration().name) + "\""+
                                            ", \"metric\": \"light\"" +
                                            ", \"value\": " + String(current_lux) +
                                            " }");
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
