/*
    1.0.0  02-06-2018 Initial version
*/
#include <Homie.h>

int delayMS = 0;

const int powerMoisture = D5; // Allows us to switch power to the sensor on and off
const unsigned int readingCount = 10; // When measuring take 10 samples
unsigned int counter = 0;
unsigned int values_avg = 0;

float current_moisture;
float previous_moisture = 0.0;

HomieNode wateringNode("plant", "watering");
HomieNode moistureNode("plant", "moisture");
HomieNode probePowered("plant", "powered");

float readProbe() {
  unsigned int counter;
  int total = 0;
  digitalWrite(powerMoisture, HIGH);
  for( counter = 0; counter < readingCount; counter++){
    Serial.println("Reading probe value...:");
    total = total + analogRead(A0);
    delay(100);
  }
  digitalWrite(powerMoisture, LOW);
  return total / readingCount;
}

void setupHandler() {
  wateringNode.setProperty("unit").send("boolean");
  moistureNode.setProperty("unit").send("%");

  delayMS = 5000;
}

bool broadcastHandler(const String& level, const String& value) {
  Serial << "Received broadcast level " << level << ": " << value << endl;
  return true;
}

void loopHandler() {
  // Delay between measurements.
  delay(delayMS);

  // Read moisture value
  current_moisture = readProbe();


  if ( current_moisture != previous_moisture ) {
    Homie.getLogger() << "Moisture: " << current_moisture << " %" << endl;
    // Publish value as float
    moistureNode.setProperty("moisture").send(String(current_moisture));
    // Publish data in JSON format
    moistureNode.setProperty("json").send("{ \"name\": \"" + String(Homie.getConfiguration().name) + "\""+
                                              ", \"metric\": \"moisture\"" +
                                              ", \"value\": " + String(current_moisture) +
                                              " }");
    previous_moisture = current_moisture;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-plantwatering", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);
  Homie.setBroadcastHandler(broadcastHandler); // before Homie.setup()
  wateringNode.advertise("watering");
  moistureNode.advertise("moisture");

  Homie.setup();
}

void loop() {
  Homie.loop();
}
