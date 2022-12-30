#include "wheel.h"

void Wheel::onChange()
{
    resetSpeedTicker.start();
    int previousSensorA = sensorA;
    int previousSensorB = sensorA;
    sensorA = digitalRead(pinA);
    sensorB = digitalRead(pinB);

    if (
        previousSensorA == LOW && previousSensorB == LOW && sensorA == HIGH && sensorB == LOW ||
        previousSensorA == HIGH && previousSensorB == LOW && sensorA == HIGH && sensorB == HIGH ||
        previousSensorA == HIGH && previousSensorB == HIGH && sensorA == LOW && sensorB == HIGH ||
        previousSensorA == LOW && previousSensorB == HIGH && sensorA == LOW && sensorB == LOW)
    {
        if (direction == Direction::Reverse)
            resetSpeed();
        direction = Direction::Forward;
    }
    else if (
        previousSensorA == LOW && previousSensorB == LOW && sensorA == LOW && sensorB == HIGH ||
        previousSensorA == LOW && previousSensorB == HIGH && sensorA == HIGH && sensorB == HIGH ||
        previousSensorA == HIGH && previousSensorB == HIGH && sensorA == HIGH && sensorB == LOW ||
        previousSensorA == HIGH && previousSensorB == LOW && sensorA == LOW && sensorB == LOW)
    {
        if (direction == Direction::Forward)
            resetSpeed();
        direction = Direction::Reverse;
    }

    // if (previousSensorA == LOW && sensorA == HIGH)
    // {
    //   speed = &speedAR;
    //   speed->registerEvent();
    // }
    // else if (previousSensorA == HIGH && sensorA == LOW)
    // {
    //   speed = &speedAF;
    // }
    // else if (previousSensorB == LOW && sensorB == HIGH)
    // {
    //   speed = &speedBR;
    //   speed->registerEvent();
    // }
    // else if (previousSensorB == HIGH && sensorB == LOW)
    // {
    //   speed = &speedBF;
    // }
}