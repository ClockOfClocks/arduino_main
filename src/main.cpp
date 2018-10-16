#include <Arduino.h>

const int hallPinPower = 52;     // the number of the hall effect sensor pin power
const int hallPin1 = 51;     // the number of the hall effect sensor pin
const int hallPin2 = 53;     // the number of the hall effect sensor pin
const int ledPin =  13;     // the number of the LED pin
// variables will change:
int hallState = 0;          // variable for reading the hall sensor status

int state = 0;
int calibrateState = 0;
int calibrateStepIn;
int calibrateStepOut;

#include <AccelStepper.h>
#include "Axis.h"

#define HALFSTEP 8

// Motor pin definitions
#define motor1Pin1  23     // IN1 on the ULN2003 driver 1
#define motor1Pin2  25     // IN2 on the ULN2003 driver 1
#define motor1Pin3  27     // IN3 on the ULN2003 driver 1
#define motor1Pin4  29     // IN4 on the ULN2003 driver 1

#define motor2Pin1  31     // IN1 on the ULN2003 driver 2
#define motor2Pin2  33     // IN2 on the ULN2003 driver 2
#define motor2Pin3  35     // IN3 on the ULN2003 driver 2
#define motor2Pin4  37     // IN4 on the ULN2003 driver 2


float fullRotationSteps = 8151.04; // 64 * 63.68 = 4075.52 // * 2 // https://youtu.be/B86nqDRskVU?t=18m41s

AccelStepper stepper1(HALFSTEP, motor1Pin1, motor1Pin3, motor1Pin2, motor1Pin4);
AccelStepper stepper2(HALFSTEP, motor2Pin1, motor2Pin3, motor2Pin2, motor2Pin4);

Axis ax1(stepper1, fullRotationSteps, false, 27.118, hallPin1);
Axis ax2(stepper2, fullRotationSteps, true, 28.118, hallPin2);

void setup() {
        Serial.begin(9600);

        ax1.setup();
        ax2.setup();

        ax1.calibrate();
        ax2.calibrate();

        pinMode(hallPinPower, OUTPUT);
}

void loop(){
        // Power on hall sensor
        digitalWrite(hallPinPower, HIGH);

        ax1.loop();
        ax2.loop();
}
