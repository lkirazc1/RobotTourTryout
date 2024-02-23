#ifndef MOVEMENTS_H
#define MOVEMENTS_H

#include <Wire.h>
#include <cstdint>
#include "MPU6050.h"


MPU6050 mpu;



bool buttonPressed() {
    bool buttonPressed = false;
    // button is active low
    if (digitalRead(13) == LOW) {
        buttonPressed = true;
    } else {
        buttonPressed = false;
    }
    return buttonPressed;
}
// bool buttonPressed()
// {
//     //Serial.println("buttonpressed_test");
//     return true;
// }


void setup_sensor()
{
    mpu.initialize();
    mpu.setXGyroOffset(-20);
    mpu.setYGyroOffset(-27);
    mpu.setZGyroOffset(34);
    mpu.setXAccelOffset(-264);
    mpu.setYAccelOffset(2902);
    mpu.setZAccelOffset(521);

}

// int getHackedAzimuth(int readDelayMs = 1) {
//     compass.read();
//     delay(readDelayMs);
//     int azimuth = compass.getAzimuth() + 180;
//     if (azimuth <= 90) {
//         return azimuth;
//     } else if (azimuth <= 180) {
//         return azimuth;
//     } else if (azimuth <= 270) {
//         return azimuth;
//     } else {
//         return azimuth;
//     }
// }

double directionDiff(double azimuth1, double azimuth2) {
    double diff = abs(azimuth1 - azimuth2);
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
        {15, 14},
        // Right
        {17, 16}},
    // Back
    {
        // Left
        {0, 1},
        // Right
        {2, 3}}};

int MotorSpeedPins[NUM_MOTORS] = {5, 11, 22, 23};
int buttonPin = 13;


// Corresponds to Motor values.
int MotorDetectors[NUM_MOTORS] = {10, 6, 20, 4};

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

void motor_run(Motor motor, Movement movement, double speed_perc, bool is_raw)
{
    Serial.println("Motor running");
    FrontBack front_back = get_front_back(motor);
    LeftRight left_right = get_left_right(motor);

    auto *motor_array = MotorDrivers[front_back][left_right];
    double kStartingSpeed;
    int scaled_speed_perc;
    if (is_raw)
    {
        scaled_speed_perc = (int)(speed_perc * 255 / 100.0);
    }
    else
    {
        kStartingSpeed = 255 * 0.4;
        scaled_speed_perc = (int)(kStartingSpeed + speed_perc * (255 - kStartingSpeed) /100.0);
    }
    // Serial.print("Speed percent: ");
    // Serial.println(speed_perc);
    // Serial.print("Scaled speed percent: ");
    // Serial.println(scaled_speed_perc);
    

    analogWrite(MotorSpeedPins[motor], scaled_speed_perc);

    if (motor == MOTOR_LEFT_FRONT)
    {
        switch (movement)
        {
        case MOTOR_FORWARD:
            digitalWrite(motor_array[0], LOW);
            digitalWrite(motor_array[1], HIGH);
            break;
        case MOTOR_BACKWARD:
            digitalWrite(motor_array[0], HIGH);
            digitalWrite(motor_array[1], LOW);
            break;
        case MOTOR_STOP:
            digitalWrite(motor_array[0], LOW);
            digitalWrite(motor_array[1], LOW);
            break;
        }
    }
    else if (motor == MOTOR_RIGHT_FRONT)
    {
        switch (movement)
        {
        case MOTOR_FORWARD:
            digitalWrite(motor_array[0], LOW);
            digitalWrite(motor_array[1], HIGH);
            break;
        case MOTOR_BACKWARD:
            digitalWrite(motor_array[0], HIGH);
            digitalWrite(motor_array[1], LOW);
            break;
        case MOTOR_STOP:
            digitalWrite(motor_array[0], LOW);
            digitalWrite(motor_array[1], LOW);
            break;
        }
    }
    else
    {
        switch (movement)
        {
        case MOTOR_FORWARD:
            digitalWrite(motor_array[0], LOW);
            digitalWrite(motor_array[1], HIGH);
            break;
        case MOTOR_BACKWARD:
            digitalWrite(motor_array[0], HIGH);
            digitalWrite(motor_array[1], LOW);
            break;
        case MOTOR_STOP:
            digitalWrite(motor_array[0], LOW);
            digitalWrite(motor_array[1], LOW);
            break;
        }
    }
}
int CMtoSteps(int cm)
{
    if (cm == 25)
    {
        return 310;
    }
    else if (cm == 50)
    {
        return 410;
    }
}

// void setupInstructions(Instruction[] instructions, int* instruction_length)
// {
//     for ()
// }



float getStraightness(int speed_perc)
{   
    float straightness;
    float scaled_speed_perc = (speed_perc * 255 / 100.0);
    
    switch (speed_perc)
    {
        case 63: straightness = .423; break;
        case 64: straightness = .43; break;
        case 65: straightness = .43; break;
        case 66: straightness = .432; break;
        case 67: straightness = .434; break;
        case 68: straightness = .441; break;
        case 69: straightness = 0.436; break;
        case 70: straightness = 0.441; break;
        case 71: straightness = 0.436; break;
        case 72: straightness = .441; break;
        case 73: straightness = .446; break;
        case 74: straightness = .462; break;
        case 75: straightness = .458; break;
        default:
            straightness = .405;
            break;
    }
    return straightness;
}

#endif