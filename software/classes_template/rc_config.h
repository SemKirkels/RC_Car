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

/* MOTOR PWM */
#define MOTOR_PWM_ENABLE true
#define MOTOR_PWM_PERIOD 7999 /* 10 kHz *//* Max val 65535 */

/* STEERING PWM */
#define STEERING_PWM_ENABLE true
#define STEERING_PWM_PERIOD 19999  /* 50 Hz *//* Max val 4294967295 */
#define STEERING_PWM_PRESCALER 79

/* UART4 (ESP COMMUNICATION) */
#define ESP_COMM_ENABLE true
#define ESP_COMM_BAUD_RATE 115200

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