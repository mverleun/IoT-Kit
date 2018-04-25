/*
  25-04-2018   1.2.0 Implemented boolean value to represent dusk status
  04-04-2018   1.1.0 Implemented JSON output
  03-04-2018   1.0.0 Initial version
*/

#include <Homie.h>

/*
  Advanced BH1750 library usage example
  This example has some comments about advanced usage features.
  Connection:
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND
  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).
*/


// SCL = D1
// SDA = D2
#include <Wire.h>
#include <BH1750.h>

/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)
  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.
*/
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

  // Only publish info if there is a change in Temperature
  if ( current_lux != previous_lux ) {
    Homie.getLogger() << "Light: " << current_lux << " lux" << endl;
    lightNode.setProperty("light").send(String(current_lux));
    lightNode.setProperty("json").send("{ \"name\": \"" + String(Homie.getConfiguration().name) + "\""+
                                            ", \"metric\": \"light\"" +
                                            ", \"value\": " + String(current_lux) +
                                            " }");
    previous_lux = current_lux;
  }

  // Check if the previous update of the dusk status was some Time
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
  Homie_setFirmware("wemos-d1-bh1750", "1.0.2");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  lightNode.advertise("unit");
  lightNode.advertise("lux");

  Homie.setup();

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  Wire.begin(D1, D2);
    /*
    BH1750 has six different measurement modes. They are divided in two groups;
    continuous and one-time measurements. In continuous mode, sensor continuously
    measures lightness value. In one-time mode the sensor makes only one
    measurement and then goes into Power Down mode.
    Each mode, has three different precisions:
      - Low Resolution Mode - (4 lx precision, 16ms measurement time)
      - High Resolution Mode - (1 lx precision, 120ms measurement time)
      - High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)
    By default, the library uses Continuous High Resolution Mode, but you can
    set any other mode, by passing it to BH1750.begin() or BH1750.configure()
    functions.
    [!] Remember, if you use One-Time mode, your sensor will go to Power Down
    mode each time, when it completes a measurement and you've read it.
    Full mode list:
      BH1750_CONTINUOUS_LOW_RES_MODE
      BH1750_CONTINUOUS_HIGH_RES_MODE (default)
      BH1750_CONTINUOUS_HIGH_RES_MODE_2
      BH1750_ONE_TIME_LOW_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE_2
  */

  // begin returns a boolean that can be used to detect setup problems.
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
