#ifndef MOVEMENTS_H
#define MOVEMENTS_H

#include <Wire.h>
#include "QMC5883LCompass.h"

QMC5883LCompass compass;


const float WHEEL_DIAMETER = 6; // CM
const int STEP_COUNT = 20;      // Ticks on Wheel

int getHackedAzimuth(int readDelayMs = 1) {
    compass.read();
    delay(readDelayMs);
    int azimuth = compass.getAzimuth() + 180;
    if (azimuth <= 90) {
        return azimuth;
    } else if (azimuth <= 180) {
        return azimuth;
    } else if (azimuth <= 270) {
        return azimuth;
    } else {
        return azimuth;
    }
}

int directionDiff(int azimuth1, int azimuth2) {
    int diff = abs(azimuth1 - azimuth2);
    if (diff > 180) {
        return 360 - diff;
    } else {
        return diff;
    }
}

enum Movement
{
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_STOP,
};

enum Motor
{
    MOTOR_LEFT_FRONT = 0,
    MOTOR_RIGHT_FRONT = 1,
    MOTOR_LEFT_BACK = 2,
    MOTOR_RIGHT_BACK = 3,
    NUM_MOTORS = 4,
};

enum FrontBack
{
    FRONT = 0,
    BACK = 1,
};

enum LeftRight
{
    LEFT = 0,
    RIGHT = 1,
};

int MotorDrivers[2][2][2] = {
    // Front
    {
        // Left
        {33, 34},
        // Right
        {35, 36}},
    // Back
    {
        // Left
        {0, 1},
        // Right
        {2, 3}}};

int MotorSpeedPins[NUM_MOTORS] = {24, 25, 22, 23};

// Corresponds to Motor values.
int MotorDetectors[NUM_MOTORS] = {20, 15, 17, 16};

void setup_motors()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                pinMode(MotorDrivers[i][j][k], OUTPUT);
            }
        }
    }

    for (int i = 0; i < NUM_MOTORS; i++)
    {
        pinMode(MotorSpeedPins[i], OUTPUT);
        analogWrite(MotorSpeedPins[i], 0);
    }

    for (int i = 0; i < NUM_MOTORS; i++)
    {
        pinMode(MotorDetectors[i], INPUT);
    }
}

FrontBack get_front_back(Motor motor)
{
    return motor >> 1 ? BACK : FRONT;
}

LeftRight get_left_right(Motor motor)
{
    return motor & 1 ? RIGHT : LEFT;
}

void motor_run(Motor motor, Movement movement, int speed_perc)
{
    FrontBack front_back = get_front_back(motor);
    LeftRight left_right = get_left_right(motor);

    auto *motor_array = MotorDrivers[front_back][left_right];

    // Scale speed_perc to 0..255, starting at a minimum.
    static const int kStartSpeed = 150;
    speed_perc = kStartSpeed + speed_perc * (255 - kStartSpeed) / 100;
    if (speed_perc <= kStartSpeed)
    {
        speed_perc = 0;
    }

    Serial.print("Writing motor speed ");
    Serial.print(motor);
    Serial.print(" ");
    Serial.println(speed_perc);
    analogWrite(MotorSpeedPins[motor], speed_perc);

    switch (movement)
    {
    case MOTOR_FORWARD:
        digitalWrite(motor_array[0], HIGH);
        digitalWrite(motor_array[1], LOW);
        break;
    case MOTOR_BACKWARD:
        digitalWrite(motor_array[0], LOW);
        digitalWrite(motor_array[1], HIGH);
        break;
    case MOTOR_STOP:
        digitalWrite(motor_array[0], LOW);
        digitalWrite(motor_array[1], LOW);
        break;
    }
}

int CMtoSteps(float cm)
{

    int result;                                  // Final calculation result
    float circumference = WHEEL_DIAMETER * 3.14; // Calculate wheel circumference in cm
    float cm_step = circumference / STEP_COUNT;  // CM per Step

    float f_result = cm / cm_step; // Calculate result as a float
    result = (int)f_result;        // Convert to an integer (note this is NOT rounded)

    return result - ((cm / 25) * 2); // End and return result
}


#endif