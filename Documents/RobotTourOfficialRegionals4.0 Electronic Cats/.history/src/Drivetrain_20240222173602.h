#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <vector>
#include <math.h>
#include <Arduino.h>
#include "Movements.h"
#include <cmath>


static const int kStraightnessCalibratorReadingPin = A0;


// static const float kRawCalibrationSpeedAdj[NUM_MOTORS] = {
//     .7357, // MOTOR_LEFT_FRONT
//     1, // MOTOR_RIGHT_FRONT
//     .7357, // MOTOR_LEFT_BACK
//     1, // MOTOR_RIGHT_BACK
// };
static const float kRawCalibrationSpeedAdj[NUM_MOTORS] = {
    1,
    1,
    1,
    1,
};


static const float kTurnCalibrationSpeedAdj[NUM_MOTORS] = {
    1,
    1,
    1,
    1,
};


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
        setup_sensor();
        angularPos = 0;
        velocity_over_time = 0;
        count = 0;
        current_degrees_ = 0;

    }

    bool is_moving() const {
        for (int i = 0; i < NUM_MOTORS; i++) {
            if (is_moving_[i]) {
                return true;
            }
        }
        return false;
    }

    bool stop_motor_turning(double direction_diff)
    {
        if (movement == MOVE_LEFT)
        {
            return direction_diff < left_buffer;
        }
        return direction_diff < right_buffer;
    }

    void Calibrate(int speed_perc, bool backwards = false) {
        // pinMode(kStraightnessCalibratorReadingPin, INPUT);
        // float straightness = (float)analogRead(A0) / 1023;
        float straightness = getStraightness(speed_perc);
        if (speed_perc > 75 || speed_perc < 60)
        {
            calibration_speed_adj_[0]
        }
        if (backwards)
        {
            straightness = 0.44;
        }
        


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
        Serial.println("Going");
        Calibrate(speed_perc, movement == BACKWARD);
        this->movement = movement;
        counter_start = 0;
        time_at_last_go = millis();
        max_speed = speed_perc;
        counter_millis_last_check = millis();
        if (movement == FORWARD || movement == BACKWARD)
        {
            time_since_start = millis();
            for (int i = 0; i < NUM_MOTORS; i++)
            {
                Motor m = (Motor)i;
                num_slits_left_[i] = num_slits;
                is_moving_[i] = true;
                motor_run(m, movement == FORWARD ? MOTOR_FORWARD : MOTOR_BACKWARD, (int)(speed_perc * calibration_speed_adj_[i]), true);
            }
            // Serial.println(movement);
            // Serial.println(speed_perc * calibration_speed_adj_[0]);
            // Serial.println(speed_perc * calibration_speed_adj_[1]);
            // Serial.println(speed_perc * calibration_speed_adj_[2]);
            // Serial.println(speed_perc * calibration_speed_adj_[3]);
        }

        else if (movement == MOVE_LEFT)
        {
            angularPos = 0;
            current_degrees_ = 270;
            // Serial.print("Current degrees: ");
            // Serial.println(current_degrees_);

            for (int i = 0; i < NUM_MOTORS; i++)
            {
                Motor m = (Motor)i;
                if (get_left_right(m) == LEFT) {
                    num_slits_left_[i] = (int)(num_slits);
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_BACKWARD, (int)(speed_perc), true);
                } else {
                    num_slits_left_[i] = num_slits;
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_FORWARD, (int)(speed_perc), true);
                }
            }
        }

        else if (movement == MOVE_RIGHT)
        {
            max_speed += 0;
            current_degrees_ = 90;
            angularPos = 0;
            // Serial.print("Current degrees: ");
            // Serial.println(current_degrees_);

            for (int i = 0; i < NUM_MOTORS; i++)
            {
                Motor m = (Motor)i;
                if (get_left_right(m) == LEFT) {
                    num_slits_left_[i] = num_slits;
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_FORWARD, (int)(speed_perc), true);
                } else {
                    num_slits_left_[i] = (int)(num_slits);
                    is_moving_[i] = true;
                    motor_run(m, MOTOR_BACKWARD, (int)(speed_perc), true);
                }
            }
        }
        // Serial.println(this->movement);
    }

    void loop()
    {
        int now = millis();
        counter_start += now - counter_millis_last_check;
        counter_millis_last_check = now;
        

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



            // if (is_moving_[i] && (movement == MOVE_LEFT || movement == MOVE_RIGHT))
            // {
                // if (target_direction_reached)
                // {
                    // Serial.print("Stopping target: ");
                    // Serial.print(current_degrees_);
                    // Serial.print(" current: ");
                    // Serial.println(azimuth);
                //     is_moving_[i] = false;
                //     motor_run((Motor)i, MOTOR_STOP, 0);
                // }
            // }
            double direction_diff = directionDiff(current_degrees_, angularPos);

            bool target_direction_reached = stop_motor_turning(direction_diff);

            // Serial.println(direction_diff);
            if (is_moving_[i] && (movement == MOVE_LEFT || movement == MOVE_RIGHT) && target_direction_reached)
            {
                Serial.print("Time since start: ");
                Serial.println(counter_start);
                motor_run((Motor)i, MOTOR_STOP, 0, false);
                is_moving_ [i] = false;

            }

            last_read_[i] = cur_slit;
            if (num_slits_left_[i] <= 0 && is_moving_[i] && !(movement == MOVE_LEFT || movement == MOVE_RIGHT)) {
                Serial.print("Time: ");
                Serial.println(counter_start);
                for (int j = 0; j < NUM_MOTORS; j++)
                {
                    is_moving_[j] = false;
                    motor_run((Motor)j, MOTOR_STOP, 0, false);
                    // // Serial.println(time_since_start);
                }
            }
        }
        // for (int i = 0; i < 4; i++)
        // {
            // Serial.print(i);
            // Serial.print("     ");
            // Serial.println(num_slits_left_[i]);
        // }

        if (now > last_time_p_ + kCheckTimeP && is_moving() && (movement == MOVE_RIGHT || movement == MOVE_LEFT))
        {
            last_time_p_ = now;
            int new_speed;
            if (movement == MOVE_LEFT)
            {
                new_speed = (int)((abs(current_degrees_ - angularPos) - left_buffer) * kp_left + (now - time_at_last_go) * ki_left);
            }
            else
            {
                new_speed = (int)((abs(current_degrees_ - angularPos) - right_buffer) * kp_right + (now - time_at_last_go) * ki_right);
            }
            if (new_speed > max_speed)
            {
                new_speed = max_speed;
            }
            for (int j = 0; j < NUM_MOTORS; j++)
            {
                if (movement == MOVE_RIGHT && get_left_right((Motor)j) == LEFT)
                {
                    motor_run((Motor)j, MOTOR_FORWARD, (int)(new_speed * kTurnCalibrationSpeedAdj[j]), true);
                    // motor_run((Motor)j, MOTOR_FORWARD, new_speed, false);
                }
                else if (movement == MOVE_LEFT && get_left_right((Motor)j) == RIGHT)
                {
                    motor_run((Motor)j, MOTOR_FORWARD, (int)(new_speed * kTurnCalibrationSpeedAdj[j]), true);
                    // motor_run((Motor)j, MOTOR_FORWARD, new_speed, false);
                }
                else if (movement == MOVE_LEFT && get_left_right((Motor)j) == LEFT)
                {
                    motor_run((Motor)j, MOTOR_BACKWARD, (int)(new_speed * kTurnCalibrationSpeedAdj[j]), true);
                    // motor_run((Motor)j, MOTOR_BACKWARD, new_speed, false);
                }
                else if (movement == MOVE_RIGHT && get_left_right((Motor)j) == RIGHT)
                {
                    motor_run((Motor)j, MOTOR_BACKWARD, (int)(new_speed * kTurnCalibrationSpeedAdj[j]), true);
                    // motor_run((Motor)j, MOTOR_BACKWARD, new_speed, false);
                }
            }
        }

        if (now > last_time_ + kCheckTime && is_moving() && (movement == MOVE_LEFT || movement == MOVE_RIGHT)) {
            last_time_ = now;
            //for (int i = 0; i < NUM_MOTORS; i++)
            //{
                // if (!is_moving_[i]) continue;

                // Serial.print("Motor ");
                // Serial.print(i);
                // Serial.print(": ");
                // Serial.println(num_slits_left_[i]);
            //}

            float current_z = mpu.getRotationZ()/131.0;
            
            angularPos -= current_z*kCheckTime*.001; //integrates angular velocity with respect to time
            if (angularPos < 0) {
                angularPos += 360;
            }
            if (angularPos > 360) {
                angularPos -= 360;
            }
            // Serial.print("Current Z: ");
            // Serial.println(current_z);
            // Serial.print("Angular Pos: ");
            // Serial.println(angularPos, 5);
        }
    }

private:
    static const int kCheckTime = 30; 
    static const int kCheckTimeP = 1;
    const double kp_left = 6;
    const double ki_left = 0.08;
    const double kp_right = 7;
    const double ki_right = 0.1;
    const double left_buffer = 3.5;
    const double right_buffer = 2.5;

    Movement movement;

    int time_at_last_go;
    double angularPos;
    double count;
    double velocity_over_time;
    int last_time_p_ = 0;
    int last_time_ = 0;
    int last_read_[NUM_MOTORS];
    int num_slits_left_[NUM_MOTORS];
    bool is_moving_[NUM_MOTORS];
    float calibration_speed_adj_[NUM_MOTORS];
    int current_degrees_;
    int north_;
    int time_since_start;
    double max_speed;
    int counter_start;
    int counter_millis_last_check;

};



#endif