#include "motion_sensor.h";

bool is_motion_detected() {
    return false;
}

void motion_thread(bool &is_verified, bool &buzzer_active, bool &kill_flag) {
    while (!kill_flag)
    {
        if (!is_verified && !buzzer_active && is_motion_detected()) {
            buzzer_active = true;
            setFalseIn(buzzer_active, 20000);
        }
    }
}
