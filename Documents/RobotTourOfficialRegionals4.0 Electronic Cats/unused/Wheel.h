#ifndef WHEEL_H
#define WHEEL_H
#include "Movements.h"

class Wheel {
public:
    
    Wheel(Motor motor, int spin_tracker_pin) : motor_(motor), spin_tracker_pin_(spin_tracker_pin) {}

    int get_local_spins() const
    {
        return local_spins_;
    }

    int get_and_reset_local_spins() {
        int local_spins = local_spins_;
        local_spins_ = 0;
        return local_spins;
    }

    void print() const {
        Serial.print(motor_);
        Serial.print(", ");
        Serial.print(local_ticks_);
        Serial.print(", ");
        Serial.println(local_spins_);
    }

    Motor get_motor_index() const
    {
        return motor_;
    }

    void loop(bool r, int speed)
    {
        if (r)
        {
            motor_run(motor_, FORWARD, speed);
        }
        else
        {
            motor_run(motor_, STOP, speed);
        }
        int rotation = get_rotation();

        if (last_read_ == 1 && rotation == 0)
        {
            local_spins_++;
        }
        local_ticks_++;
        last_read_ = rotation;

        int now = millis();
        interval_last_time_ = now - last_saved_time_;
        if (interval_last_time_ > kCheckTime_)
        {
            interval_last_time_ = now;
            local_ticks_ = 0;
        }
    }

private:
    int get_rotation()
    {
        return digitalRead(spin_tracker_pin_);
    }
    
    const Motor motor_;
    const int spin_tracker_pin_;
    const int kCheckTime_ = 1000;

    int last_saved_time_ = 0;
    int interval_last_time_ = 0;
    int last_read_ = 0;
    int local_spins_ = 0;
    int local_ticks_ = 0;
};

#endif