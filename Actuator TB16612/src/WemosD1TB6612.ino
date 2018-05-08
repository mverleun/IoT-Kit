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


// Scanning (SDA : SCL) - D2 : D1 - I2C device found at address 0x30  !
//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)


Motor M1(I2C_ADDRESS, _MOTOR_A, 1000); // Motor A
Motor M2(I2C_ADDRESS, _MOTOR_B, 1000); // Motor B

float pwmMotor1 = 0;
uint8_t directionMotor1 = _STANDBY;
float pwmMotor2 = 0;
uint8_t directionMotor2 = _STANDBY;

HomieNode motor1Node("motor1", "motor");
HomieNode motor2Node("motor2", "motor");

void setupHandler() {
  Serial.println("Entering setupHandler");
}

bool pwm1Handler(HomieRange range, String value) {
  // devices/wemos-x/motor1/pwm/set
  Serial << "Received PWM: " << String(value) << endl;
  pwmMotor1 = value.toFloat();
  M1.setmotor( directionMotor1, pwmMotor1);
  return true;
}
bool pwm2Handler(HomieRange range, String value) {
  // devices/wemos-x/motor2/pwm/set
  Serial << "Received PWM: " << String(value) << endl;
  pwmMotor2 = value.toFloat();
  M2.setmotor( directionMotor2, pwmMotor2);
  return true;
}

bool motor1Handler(HomieRange range, String value) {
  // devices/wemos-x/motor1/direction/set
  Serial << "Received direction: " << String(value) << endl;
  if (value == "CW") {
    directionMotor1 = _CW;
  }
  if (value == "CCW") {
    directionMotor1 = _CCW;
  }
  if (value == "STOP") {
    directionMotor1 = _STOP;
  }
  if (value == "STANDBY") {
    directionMotor1 = _STANDBY;
  }
  M1.setmotor( directionMotor1, pwmMotor1);
  return true;
}

bool motor2Handler(HomieRange range, String value) {
  // devices/wemos-x/motor2/direction/set
  Serial << "Received direction: " << String(value) << endl;
  if (value == "CW") {
    directionMotor2 = _CW;
  }
  if (value == "CCW") {
    directionMotor2 = _CCW;
  }
  if (value == "STOP") {
    directionMotor2 = _STOP;
  }
  if (value == "STANDBY") {
    directionMotor2 = _STANDBY;
  }
  M1.setmotor( directionMotor2, pwmMotor2);
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

  M1.setmotor(_STOP, 0);
  M2.setmotor(_STOP, 0);
}

void loop() {
  Homie.loop();
}
