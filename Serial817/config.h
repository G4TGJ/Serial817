/*
 * config.h
 * 
 * Configuration settings for the whole project
 *
 * Created: 29/03/20
 * Author : Richard Tomlinson G4TGJ
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>

// General definitions
typedef uint8_t bool;
#define true 1
#define false 0

#define ULONG_MAX 0xFFFFFFFF

#ifdef VPORTC

// ATtiny 1-series

// Processor definitions
// CPU clock speed
#define F_CPU 3333333UL

#define CLOCK_DIV 1

// I/O definitions

#define LED_DIR_REG     VPORTC.DIR
#define LED_OUT_REG     VPORTC.OUT
#define LED_PIN         0
#define LED_TOGGLE_REG  PORTC.OUTTGL

#define SW_DIR_REG      VPORTC.DIR
#define SW_IN_REG       VPORTC.IN
#ifdef __AVR_ATtiny3216__ 
#define SW_PIN          1
#else
#define SW_PIN          3
#endif
#define SW_PIN_CTRL     PORTC.PIN5CTRL

// Serial port definitions

// Define to use the alternative pins (PA1, PA2) instead
// of the standard pins (PB2,PB3)
//#define SERIAL_ALTERNATIVE_PINS
#ifdef SERIAL_ALTERNATIVE_PINS
#define SERIAL_DIR_REG  VPORTA.DIR
#define SERIAL_TXD_PIN  1
#define SERIAL_RXD_PIN  2
#else
#define SERIAL_DIR_REG  VPORTB.DIR
#define SERIAL_TXD_PIN  2
#define SERIAL_RXD_PIN  3
#endif

#else

// ATtiny85

// Processor definitions
// CPU clock speed
// The CKDIV8 fuse must be unprogrammed for 8MHz.
#define F_CPU 8000000UL

// I/O definitions

#endif

// Dimensions of the LCD screen
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// Space for each frequency on the top line
// Must be able to get all 3 within the LCD_WIDTH
// (plus spaces in between)
#define SHORT_WIDTH 4

// Address of the LCD display
#define LCD_I2C_ADDRESS 0x27

// Time for debouncing a switch (ms)
#define DEBOUNCE_TIME   100

// Time for a key press to be a long press (ms)
#define LONG_PRESS_TIME 250

#define I2C_CLOCK_RATE 100000

// Serial port buffer lengths
// Lengths should be a power of 2 for efficiency
#define SERIAL_RX_BUF_LEN 16
#define SERIAL_TX_BUF_LEN 32

// Default entry into the baud rate table
#define DEFAULT_BAUD_INDEX 3 // 9600

#endif /* CONFIG_H_ */
