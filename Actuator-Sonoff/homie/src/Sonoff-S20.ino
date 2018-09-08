/*
 1.0.2    02-04-2018  Removed UI to reduce SPIFFS size
 1.0.1    02-04-2018  Updated Libraries, added code from example
 1.0.0    25-03-2018  Initial version
*/

#include <Homie.h>

/*
        Define where the internal components of the Sonoff device
        are connected to.
*/
const int PIN_RELAY = 12; // D6
const int PIN_LED = 13; // D7
const int PIN_BUTTON = 0; //D3

unsigned long buttonDownTime = 0;
byte lastButtonState = 1;
byte buttonPressHandled = 0;

/*
          Configure the node as a single device, even as it has
          serveral ways of operation.
*/
HomieNode switchNode("switch", "switch");

/*        Function called when something is published to topic
          devices/<device>/switch/switch/on/set
          Valid values are "true" and "false"
*/
bool switchOnHandler(HomieRange range, String value) {
  if (value != "true" && value != "false") return false;

  bool on = (value == "true");
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  switchNode.setProperty("on").send(value);
  return true;
}

/*
          Toggle the relay from on to off and vice versa and
          update the status of the topic devices/<device>/switch/switch/on
*/
void toggleRelay() {
  bool on = digitalRead(PIN_RELAY) == HIGH;
  digitalWrite(PIN_RELAY, on ? LOW : HIGH);
  switchNode.setProperty("on").send(on ? "false" : "true");
}


void setupHandler() {
}

void loopHandler() {
/*
            Check to see if button is pressed. If it is pressed start a counter
            to measure how long the button is pressed.
            If the button is held down between 90 and 900 ms then toggle the relay.
*/
  byte buttonState = digitalRead(PIN_BUTTON);
  if ( buttonState != lastButtonState ) {
    if (buttonState == LOW) {
      buttonDownTime     = millis();
      buttonPressHandled = 0;
    } // if
    else {
      unsigned long dt = millis() - buttonDownTime;
      if ( dt >= 90 && dt <= 900 && buttonPressHandled == 0 ) {
        toggleRelay();
        buttonPressHandled = 1;
      } 
    }
    lastButtonState = buttonState;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_RELAY, LOW);

  Homie_setFirmware("sonoff-s20", "1.0.2");

  Homie.setLedPin(PIN_LED, LOW).setResetTrigger(PIN_BUTTON, LOW, 5000);

  switchNode.advertise("on").settable(switchOnHandler);

  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
