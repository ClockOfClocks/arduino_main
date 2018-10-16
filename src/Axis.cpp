// Axis.cpp

#include "Arduino.h"
#include "AccelStepper.h"
#include "Axis.h"

Axis::Axis(AccelStepper& m, float s, bool cwDir, float aShift, int hsp)
{
        state = Axis::STATE_INIT;
        motor = m;
        stepsPerRevolution = s;
        cwDirection = cwDir;
        axisShift = aShift;
        hallSensorPin = hsp;
        pinMode(hallSensorPin, INPUT_PULLUP);
}

/**
 * Converts degree number to motor steps
 * @param  degree number of degrees
 * @return        number of steps
 */
int Axis::degreeToSteps(int degree){
        return ceil(degree * stepsPerRevolution / 360);
}

void Axis::setup(){
        motor.setAcceleration(ceil(stepsPerRevolution/8));
        motor.setMaxSpeed(ceil(stepsPerRevolution/8));
        motor.setSpeed(ceil(stepsPerRevolution/8));
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

void Axis::standby(){
        state = Axis::STATE_STANDBY;
        motor.disableOutputs();
}

void Axis::loop(){
        switch(state) {
        case Axis::STATE_INIT:
        case Axis::STATE_STANDBY:
                // Do nothing
                break;
        case Axis::STATE_CALIBRATE:
                if (digitalRead(hallSensorPin) == LOW) { // LOW – magnet detected
                        if(calibrationState == Axis::CALIBRATION_SEARCH) {
                                calibrationSavePosition = motor.currentPosition();
                                calibrationState = Axis::CALIBRATION_DETECTED;
                        }
                }else {
                        if(calibrationState == Axis::CALIBRATION_INIT) {
                                // Do not track case when magnet detected in the beginning of calibration
                                calibrationState = Axis::CALIBRATION_SEARCH;
                        }else if(calibrationState == Axis::CALIBRATION_DETECTED) {

                                motor.setCurrentPosition(
                                        // position of magnet center
                                        ceil((motor.currentPosition() - calibrationSavePosition) / (float)2)
                                        // steps to move from magnet center to home
                                        + degreeToSteps(ceil(90-axisShift))
                                        );

                                calibrationState = Axis::CALIBRATION_FINISHED;

                                if(true) { // If move to home position after calibratin finish
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
                if (0 == motor.distanceToGo()) {
                        standby();
                }
                break;
        }
}
