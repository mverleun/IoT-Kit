/*
1.1.0  04-04-2018 Added JSON output
1.0.2  02-04-2018 Simplified code
1.0.1  02-04-2018 Updated Libraries, added ui in /data
1.0.0  25-03-2018 Initial version
*/
#include <Homie.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN            D4 // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

uint32_t delayMS;  // Store the minimum wait time between readings

float current_temperature;
float previous_temperature = 0.0;
float current_humidity;
float previous_humidity = 0.0;

HomieNode temperatureNode("temperature", "temperature");
HomieNode humidityNode("humidity", "humidity");

void setupHandler() {
  temperatureNode.setProperty("unit").send("c");
  humidityNode.setProperty("unit").send("%");
}

bool broadcastHandler(const String& level, const String& value) {
  Serial << "Received broadcast level " << level << ": " << value << endl;
  return true;
}

void loopHandler() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and publish its value.

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    current_temperature = temperature;
    // Only publish info if there is a change in Temperature
    if ( current_temperature != previous_temperature ) {
      Homie.getLogger() << "Temperature: " << current_temperature << " °C" << endl;
      temperatureNode.setProperty("temperature").send(String(current_temperature));
      temperatureNode.setProperty("json").send("{ \"name\": \"" + String(Homie.getConfiguration().name) + "\""+
                                              ", \"metric\": \"temperature\"" +
                                              ", \"value\": " + String(current_temperature) +
                                              " }");
      previous_temperature = current_temperature;
    }
  }

  if (isnan(humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    current_humidity = humidity;
    // Only publish info if there is a change in Humidity
    if ( current_humidity != previous_humidity ) {
      Homie.getLogger() << "Humidity: " << current_humidity << " %" << endl;
      humidityNode.setProperty("humidity").send(String(current_humidity));
      temperatureNode.setProperty("json").send("{ \"name\": \"" + String(Homie.getConfiguration().name) + "\""+
                                              ", \"metric\": \"humidity\"" +
                                              ", \"value\": " + String(current_humidity) +
                                              " }");
      previous_humidity = current_humidity;
    }
  }
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
    case HomieEventType::OTA_PROGRESS:
      Serial << "OTA progress, " << event.sizeDone << "/" << event.sizeTotal << endl;
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
  Homie_setFirmware("wemos-d1-dht11", "1.0.2");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);
  Homie.setBroadcastHandler(broadcastHandler); // before Homie.setup()
  temperatureNode.advertise("temperature_unit");
  humidityNode.advertise("humidity_unit");

  Homie.setup();

  // Initialize device.
  dht.begin();

  delayMS = 5000;
}

void loop() {
  Homie.loop();
}
