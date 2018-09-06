/*
  Axis.h - Library for controlling of clock axises.
  Created by Mikhail Razumovskiy, 2018.
  https://github.com/ClockOfClocks
*/
#ifndef Axis_h
#define Axis_h

#include "Arduino.h"
#include "AccelStepper.h"

class Axis
{
  public:

    typedef enum
    {
    	STATE_INIT  = 0,
    	STATE_CALIBRATE = 1,
      STATE_RUN = 2,
      STATE_STANDBY = 3
    } AxisState;

    typedef enum
    {
      CALIBRATION_INIT = 0,
      CALIBRATION_SEARCH = 1,
      CALIBRATION_DETECTED = 2,
      CALIBRATION_FINISHED = 3
    } AxisCalibrationState;

    Axis(AccelStepper& m, int s, bool cwDir, float axisShift, int hallSensorPin);
    void setup();
    void calibrate();
    void run();
    void stanby();
    void loop();
  private:
    AccelStepper motor;
    int stepsPerRevolution; // Steps per ull circle
    bool cwDirection; // Reverse direction
    float axisShift;
    int hallSensorPin;
    int state; // Global axis state
    int calibrationState; // Axis calibratiob state
    int calibrationSavePosition; // Save motor position when hall sensor begins to detect axis
    int degreeToSteps(int degree);
};

#endif
