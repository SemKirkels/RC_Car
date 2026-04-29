#ifndef RC_CONFIG_H
#define RC_CONFIG_H

/* Temperature sensors */
#define TEMP_MOTOR_ADDR     0x60
#define TEMP_AMBIENT_ADDR   0x18

/* BMS Sensor */
#define TEMP_BATTERY_ADDR   0x08

/* TOF sensors */
#define TOF_DEFAULT_ADDR    0x29
/* Front */
#define TOF_FRONT_ADDR          0x30
#define XSHUT_FRONT_GPIO_PORT   GPIOC
#define XSHUT_FRONT_PIN         GPIO_PIN_5
/* Left */
#define TOF_LEFT_ADDR           0x31
#define XSHUT_LEFT_GPIO_PORT    GPIOC
#define XSHUT_LEFT_PIN          GPIO_PIN_4
/* Right */
#define TOF_RIGHT_ADDR          0x32
#define XSHUT_RIGHT_GPIO_PORT   GPIOB
#define XSHUT_RIGHT_PIN         GPIO_PIN_0

/* Safety limits */
#define STOP_DISTANCE_MM        300
#define STEER_BLOCK_LEFT_MM     200
#define STEER_BLOCK_RIGHT_MM    200

/* GPIO */
#define GPIO_ENABLE true

/* MOTOR */
#define MOTOR_PWM_ENABLE true
#define MOTOR_PWM_PERIOD 7999 /* 10 kHz *//* Max val 65535 */
#define MOTOR_TRIGGER_DEADZONE 1

/* STEERING */
#define STEERING_PWM_ENABLE true
#define STEERING_PWM_PERIOD 19999  /* 50 Hz *//* Max val 4294967295 */
#define STEERING_PWM_PRESCALER 79
#define STEERING_JOYSTICK_DEADZONE 1500

/* UART4 (ESP COMMUNICATION) */
#define ESP_COMM_ENABLE true
#define ESP_COMM_BAUD_RATE 115200
/* BUTTON VALUES */
#define CTRL_D_PAD_UP       0x0001
#define CTRL_D_PAD_DOWN     0x0002
#define CTRL_D_PAD_LEFT     0x0004
#define CTRL_D_PAD_RIGHT    0x0008
#define CTRL_START          0x0010
#define CTRL_BACK           0x0020
#define CTRL_JOY_LEFT_BTN   0x0040
#define CTRL_JOY_RIGHT_BTN  0x0080
#define CTRL_LB             0x0100
#define CTRL_RB             0x0200
#define CTRL_A              0x1000
#define CTRL_B              0x2000
#define CTRL_X              0x4000
#define CTRL_Y              0x8000

/* UART5 (UNUSED) */
#define UART5_COMM_ENABLE false
#define UART5_COMM_BAUD_RATE 115200

/* USART1 (DEBUG COMMUNICATION)*/
#define DEBUG_COMM_ENABLE true
#define DEBUG_COMM_BAUD_RATE 115200

/* I2C1 */
#define I2C1_ENABLE true

/* ADC1 */
#define ADC1_ENABLE false

/* ADC1 */
#define ADC2_ENABLE false

/* DAC1 */
#define DAC1_ENABLE false

/* SPI1 */
#define SPI1_ENABLE false

#endif