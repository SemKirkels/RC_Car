#ifndef RC_CONFIG_H
#define RC_CONFIG_H

/* GPIO */
#define GPIO_ENABLE true

/* MOTOR PWM */
#define MOTOR_PWM_ENABLE true
#define MOTOR_PWM_PERIOD 65535          /* Max val 65535 */

/* STEERING PWM */
#define STEERING_PWM_ENABLE true
#define STEERING_PWM_PERIOD 4294967295  /* Max val 4294967295 */

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