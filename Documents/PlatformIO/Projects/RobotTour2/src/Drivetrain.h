#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <vector>
#include <math.h>
#include <Arduino.h>
//#include "Movements.h"
#include <cmath>


static const float kRawCalibrationSpeedAdj[NUM_MOTORS] = {
    1.00, // MOTOR_LEFT_FRONT
    0.99, // MOTOR_RIGHT_FRONT
    0.96, // MOTOR_LEFT_BACK
    0.95, // MOTOR_RIGHT_BACK
};

static const float kTurnAdj = 1.0;

static const int kStraightnessCalibratorReadingPin = A0;

class Drivetrain
{

public:
    enum Movement
    {
        FORWARD,
        BACKWARD,
        STOP,
        MOVE_LEFT,
        MOVE_RIGHT,
    };

    Drivetrain()
    {
        memset(last_read_, 0, sizeof(last_read_));
        memset(num_slits_left_, 0, sizeof(num_slits_left_));
        memset(is_moving_, 0, sizeof(is_moving_));
        memcpy(calibration_speed_adj_, kRawCalibrationSpeedAdj, sizeof(kRawCalibrationSpeedAdj));
        north_ = getHackedAzimuth(50);
        current_degrees_ = north_;
        Serial.print("North: ");
        Serial.println(north_);
    }

    bool is_moving() const {
        for (int i = 0; i < NUM_MOTORS; i++) {
            if (is_moving_[i]) {
                return true;
            }
        }
        return false;
    }

    void Calibrate() {
        pinMode(kStraightnessCalibratorReadingPin, INPUT);
        float straightness = (float)analogRead(A0) / 1023;
        Serial.print("Straightness: ");
        Serial.println(straightness);

        // Adjust by straightness.
        for (int i = 0; i < NUM_MOTORS; i++) {
            if (get_left_right((Motor)i) == LEFT) {
                calibration_speed_adj_[i] = kRawCalibrationSpeedAdj[i] * (straightness / 2);
            } else {
                calibration_speed_adj_[i] = kRawCalibrationSpeedAdj[i] * ((1 - straightness) / 2);
            }
        }

        // Create the normalized calibration speed adj.
        float max_adj = 0;
        for (int i = 0; i < NUM_MOTORS; i++) {
            if (max_adj < calibration_speed_adj_[i]) {
                max_adj = calibration_speed_adj_[i];
            }
        }
        // Scale everything by 1/max_adj.
        for (int i = 0; i < NUM_MOTORS; i++) {
            calibration_speed_adj_[i] /= max_adj;
        }
    }

    void Go(int num_slits, Movement movement, int speed_perc)
    {
        this->movement = movement;
        if (movement == FORWARD || movement == BACKWARD)
        {
            for (int i = 0; i < NUM_MOTORS; i++)
            {
                Motor m = (Motor)i;
                num_slits_left_[i] = num_slits;
                is_moving_[i] = true;
                motor_run(m, movement == FORWARD ? MOTOR_FORWARD : MOTOR_BACKWARD, (int)(speed_perc * calibration_speed_adj_[i]));
            }
        }

        else if (movement == MOVE_LEFT)
        {
            current_degrees_ = (current_degrees_ + 360 - 90) % 360;
            Serial.print("Current degrees: ");
            Serial.println(current_degrees_);

            for (int i = 0; i < NUM_MOTORS; i++)
            {
                Motor m = (Motor)i;
                if (get_left_right(m) == LEFT) {
                    num_slits_left_[i] = (int)(num_slits * kTurnAdj);
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_BACKWARD, (int)(speed_perc * calibration_speed_adj_[i] * kTurnAdj));
                } else {
                    num_slits_left_[i] = num_slits;
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_FORWARD, (int)(speed_perc * calibration_speed_adj_[i]));
                }
            }
        }

        else if (movement == MOVE_RIGHT)
        {
            current_degrees_ = (current_degrees_ + 90) % 360;
            Serial.print("Current degrees: ");
            Serial.println(current_degrees_);

            for (int i = 0; i < NUM_MOTORS; i++)
            {
                Motor m = (Motor)i;
                if (get_left_right(m) == LEFT) {
                    num_slits_left_[i] = num_slits;
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_FORWARD, (int)(speed_perc * calibration_speed_adj_[i]));
                } else {
                    num_slits_left_[i] = (int)(num_slits * kTurnAdj);
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_BACKWARD, (int)(speed_perc * calibration_speed_adj_[i] * kTurnAdj));
                }
            }
        }
        Serial.println(this->movement);
    }

    void loop()
    {
        int now = millis();
        int azimuth = getHackedAzimuth();
        bool target_direction_reached =
            (directionDiff(azimuth, current_degrees_) < 5);

        // Check slits left.
        for (int i = 0; i < NUM_MOTORS; i++) {
            int cur_slit = digitalRead(MotorDetectors[i]);
            bool slit_switch_detected = cur_slit == LOW && last_read_[i] == HIGH;
            switch (movement)
            {
                case FORWARD:
                    if(slit_switch_detected) num_slits_left_[i]--;
                    break;
                case BACKWARD:
                    if(slit_switch_detected) num_slits_left_[i]--;
                    break;
                case MOVE_LEFT:
                    num_slits_left_[i]--;
                case MOVE_RIGHT:
                    num_slits_left_[i]--;
                default:
                    break;
            }

            if (is_moving_[i] && (movement == MOVE_LEFT || movement == MOVE_RIGHT))
            {
                if (target_direction_reached)
                {
                    Serial.print("Stopping target: ");
                    Serial.print(current_degrees_);
                    Serial.print(" current: ");
                    Serial.println(azimuth);
                    is_moving_[i] = false;
                    motor_run((Motor)i, MOTOR_STOP, 0);
                }
            }

            last_read_[i] = cur_slit;
            if (num_slits_left_[i] <= 0 && is_moving_[i]) {
                is_moving_[i] = false;
                motor_run((Motor)i, MOTOR_STOP, 0);
            }
        }

        if (now > last_time_ + kCheckTime) {
            last_time_ = now;
            for (int i = 0; i < NUM_MOTORS; i++)
            {
                if (!is_moving_[i]) continue;

                // Serial.print("Motor ");
                // Serial.print(i);
                // Serial.print(": ");
                // Serial.println(num_slits_left_[i]);
            }
        }
    }

private:
    static const int kCheckTime = 100; 

    Movement movement;
    // int rotation_starting_direction;
    int last_time_ = 0;
    int last_read_[NUM_MOTORS];
    int num_slits_left_[NUM_MOTORS];
    bool is_moving_[NUM_MOTORS];
    float calibration_speed_adj_[NUM_MOTORS];
    int current_degrees_;
    int north_;
};



#endif