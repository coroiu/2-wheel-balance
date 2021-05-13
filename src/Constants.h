#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define FREQ_CONTROL_HZ 50
#define FREQ_LOOP_HZ 200
#define FREQ_LOG_HZ 20

#define INTERVAL_CONTROL_MICROS (1000000 / FREQ_CONTROL_HZ)
#define INTERVAL_LOOP_MICROS (1000000 / FREQ_LOOP_HZ)
#define INTERVAL_LOG_MICROS (1000000 / FREQ_LOG_HZ)

#define WHEEL_SPEED_RESET_INTERVAL_MICROS 1000 * 200
#define WHEEL_LEFT 100
#define WHEEL_LEFT_PIN_A 16
#define WHEEL_LEFT_PIN_B 19
#define WHEEL_RIGHT 200
#define WHEEL_RIGHT_PIN_A 0
#define WHEEL_RIGHT_PIN_B 0
#define MOTOR_LEFT 300
#define MOTOR_LEFT_1 12
#define MOTOR_LEFT_2 27
#define MOTOR_LEFT_PWM_CHANNEL 0
#define MOTOR_LEFT_PWM_PIN 0

#endif