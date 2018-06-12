/*
  03-04-2018   1.0.0 Initial version
*/

#include <Homie.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//#define SCL D1
//#define SDA D2



HomieNode displayNode("display", "display");

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

bool textHandler(HomieRange range, String value) {
  Homie.getLogger() << "Received text " << value << endl;
  return true;
}

void setupHandler() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C
  display.display();

}

void loopHandler() {
  Serial << "loopHandler" << endl;
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  //display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello World");
  display.display();
  //display.startscrollleft(0x00, 0x0F); //make display scroll

}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-oled-shield", "1.0.0");

  displayNode.advertise("text").settable(textHandler);

  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  Homie.setup();

}

void loop() {
  Homie.loop();
}
