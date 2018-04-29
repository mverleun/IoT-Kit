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
Motor M1(I2C_ADDRESS, _MOTOR_A, 1000); // Motor A
Motor M2(I2C_ADDRESS, _MOTOR_B, 1000); // Motor B

int pwm = 0;

HomieNode motor1Node("motor1", "motor");
HomieNode motor2Node("motor2", "motor");

void setupHandler() {
  Serial.println("Entering setupHandler");
}

bool pwm1Handler(HomieRange range, String value) {
  pwm = value.toInt();
  return true;
}
bool pwm2Handler(HomieRange range, String value) {
  pwm = value.toInt();
  return true;
}

bool motor1Handler(HomieRange range, String value) {
  if (value == "CW") {
    M1.setmotor( _CW, pwm);
  }
  if (value == "CCw") {
    M1.setmotor( _CCW, pwm);
  }
  if (value == "STANDBY") {
    M1.setmotor( _STANDBY);
  }
  return true;
}

bool motor2Handler(HomieRange range, String value) {
  if (value == "CW") {
    M1.setmotor( _CW, pwm);
  }
  if (value == "CCw") {
    M1.setmotor( _CCW, pwm);
  }
  if (value == "STANDBY") {
    M1.setmotor( _STANDBY);
  }
  return true;
}

void loopHandler() {
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("wemos-d1-tb6612", "1.0.0");


  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  motor1Node.advertise("pwm").settable(pwm1Handler);
  motor1Node.advertise("direction").settable(motor1Handler);
  motor2Node.advertise("pwm").settable(pwm2Handler);
  motor2Node.advertise("direction").settable(motor2Handler);

  Homie.setup();

}

void loop() {
  Homie.loop();
}
