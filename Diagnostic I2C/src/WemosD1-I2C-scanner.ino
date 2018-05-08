/*
  28-04-2018:  1.0.1 Improved with code from https://github.com/jainrk/i2c_port_address_scanner.git
  25-04-2018:   1.0.0 Initial version
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
uint8_t portArray[] = {16, 5, 4, 0, 2, 14, 12, 13};
String portMap[] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7"}; //for Wemos
//String portMap[] = {"GPIO16", "GPIO5", "GPIO4", "GPIO0", "GPIO2", "GPIO14", "GPIO12", "GPIO13"};

void setupHandler() {
  // Initialize the I2C bus
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  Wire.begin(D1, D2);
  diagNode.setProperty("address").send("");
}

void scanPorts() {
  for (uint8_t i = 0; i < sizeof(portArray); i++) {
    for (uint8_t j = 0; j < sizeof(portArray); j++) {
      if (i != j){
        Wire.begin(portArray[i], portArray[j]);
        check_if_exist_I2C(i, j);
      }
    }
  }
}

void check_if_exist_I2C(uint8_t i, uint8_t j) {
  byte error, address;
  int nDevices;
  nDevices = 0;
  for (address = 1; address < 127; address++ )  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0){
      Serial.print("Scanning (SDA : SCL) - " + portMap[i] + " : " + portMap[j] + " - ");
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  } //for loop

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("**********************************\n");

  //delay(1000);           // wait 1 seconds for next scan, did not find it necessary
}

void loopHandler() {

}



void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-i2cscanner", "1.0.0");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);


  Homie.setup();
  Serial.println("\n\nI2C Scanner to scan for devices on each port pair D0 to D7");
  scanPorts();
  delay(2000);

}

void loop() {
  Homie.loop();
}
