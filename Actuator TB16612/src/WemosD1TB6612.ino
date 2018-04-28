/*
  12-04-2018   1.0.0 Initial version
*/

/*
This shield is a tricky one to get working.

The shield is reflashed with firmware from: https://github.com/pbugalski/wemos_motor_shield/files/1434679/motor_shield_XX_bin.zip
Depending on the file flashed the I2C address changes, don't use the pads on the PCB.

Instructions how to flash the shield are found here: https://hackaday.io/project/18439/log/57855-reprogramming-without-soldering

The wiki for this shield is found here: https://wiki.wemos.cc/products:d1_mini_shields:motor_shield
*/
#include <Homie.h>
#include <Wire.h>
#include "WEMOS_Motor.h"

#define I2C_ADDRESS 0x30


// SDA, SCL: D4, D5
//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(I2C_ADDRESS, _MOTOR_A, 1000); //Motor A

HomieNode motorNode("motor", "motor");

void setupHandler() {
  Serial.println("Entering setupHandler");
}

void loopHandler() {
  int pwm;

  for (pwm = 40; pwm <= 100; pwm++)
    {
      M1.setmotor( _CW, pwm);
      Serial.printf("A:%d%, B:%d%, DIR:CW\r\n", pwm, 100 - pwm);
      delay(500);
      M1.setmotor(_STANDBY);
      Serial.println("Motor A STANDBY");
      delay(100);
    }
  delay(2000);
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
