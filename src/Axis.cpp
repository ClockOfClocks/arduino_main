// Axis.cpp

#include "Arduino.h"
#include "AccelStepper.h"
#include "Axis.h"

Axis::Axis(AccelStepper& m, int s, bool cwDir, float axisShift, int hsp)
{
  state = Axis::STATE_INIT;
  motor = m;
  stepsPerRevolution = s;
  cwDirection = cwDir;
  axisShift = axisShift;
  hallSensorPin = hsp;
  pinMode(hallSensorPin, INPUT_PULLUP);
}

/**
 * Converts degree number to motor steps
 * @param  degree number of degrees
 * @return        number of steps
 */
int Axis::degreeToSteps(int degree){
  return round(degree * stepsPerRevolution / 360);
}

void Axis::setup(){
  motor.setAcceleration(ceil(stepsPerRevolution/4));
  motor.setMaxSpeed(ceil(stepsPerRevolution/4));
  motor.setSpeed(ceil(stepsPerRevolution/4));
}

void Axis::calibrate(){
  state = Axis::STATE_CALIBRATE;
  calibrationState = Axis::CALIBRATION_INIT;
  motor.enableOutputs();
}

void Axis::run(){
  state = Axis::STATE_RUN;
  motor.enableOutputs();
  // digitalWrite(hallPinPower, LOW); // Think about something like callback
}

void Axis::stanby(){
  state = Axis::STATE_STANDBY;
  motor.disableOutputs();
}

void Axis::loop(){
  switch(state){
    case Axis::STATE_INIT:
    case Axis::STATE_STANDBY:
      // Do nothing
    break;
    case Axis::STATE_CALIBRATE:

    //Serial.

    // Serial.print(hallSensorPin);
    // Serial.print(" ");
    // Serial.println(digitalRead(hallSensorPin));

      if (digitalRead(hallSensorPin) == LOW) { // LOW – magnet detected
        if(calibrationState == Axis::CALIBRATION_SEARCH){ // Do not track case when magnet can be detected in the beginning of calibration
          calibrationSavePosition = motor.currentPosition();
          calibrationState = Axis::CALIBRATION_DETECTED;
        }
      }else {
        if(calibrationState == Axis::CALIBRATION_INIT){
          calibrationState = Axis::CALIBRATION_SEARCH;
        }else if(calibrationState == Axis::CALIBRATION_DETECTED){

          motor.setCurrentPosition(ceil((motor.currentPosition() - calibrationSavePosition) / 2));
          // TODO set position relatively real home, use axis shift

          calibrationState = Axis::CALIBRATION_FINISHED;

          if(true){ // If move to home position after calibratin finish
            motor.moveTo(0);
            run();
            break;
          }
        }
      }
      motor.runSpeed();
    break;
    case Axis::STATE_RUN:
      motor.run();
      if (0 == motor.distanceToGo()){
        stanby();
      }
    break;
  }
}
