#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

/**
 * @defgroup    bsp_board_config    Board configuration
 * @ingroup     bsp_board
 * @brief       Board specific definitions
 *
 * @{
 * @file
 * @author Alexandre Abadie <alexandre.abadie@inria.fr>
 * @copyright Inria, 2023
 * @}
 */

#include "gpio.h"

#if defined(BOARD_NRF52840DK)
#include "conf/nrf52840dk_config.h"
#endif

///! LED1 pin
static const gpio_t db_led1 = { .port = DB_LED1_PORT, .pin = DB_LED1_PIN };

#if defined(DB_LED2_PORT)
///! LED2 pin
static const gpio_t db_led2 = { .port = DB_LED2_PORT, .pin = DB_LED2_PIN };
#endif

#if defined(DB_LED3_PORT)
///! LED3 pin
static const gpio_t db_led3 = { .port = DB_LED3_PORT, .pin = DB_LED3_PIN };
#endif

#if defined(DB_LED4_PORT)
///! LED4 pin
static const gpio_t db_led4 = { .port = DB_LED4_PORT, .pin = DB_LED4_PIN };
#endif

///! BTN1 pin
static const gpio_t db_btn1 = { .port = DB_BTN1_PORT, .pin = DB_BTN1_PIN };

#if defined(DB_BTN2_PORT)
///! BTN2 pin
static const gpio_t db_btn2 = { .port = DB_BTN2_PORT, .pin = DB_BTN2_PIN };
#endif

#if defined(DB_BTN3_PORT)
///! BTN3 pin
static const gpio_t db_btn3 = { .port = DB_BTN3_PORT, .pin = DB_BTN3_PIN };
#endif

#if defined(DB_BTN4_PORT)
///! BTN4 pin
static const gpio_t db_btn4 = { .port = DB_BTN4_PORT, .pin = DB_BTN4_PIN };
#endif

///! I2C SCL pin
static const gpio_t db_scl = { .port = DB_I2C_SCL_PORT, .pin = DB_I2C_SCL_PIN };

///! I2C SDA pin
static const gpio_t db_sda = { .port = DB_I2C_SDA_PORT, .pin = DB_I2C_SDA_PIN };

///! UART RX pin
static const gpio_t db_uart_rx = { .port = DB_UART_RX_PORT, .pin = DB_UART_RX_PIN };

///! UART TX pin
static const gpio_t db_uart_tx = { .port = DB_UART_TX_PORT, .pin = DB_UART_TX_PIN };

#endif
