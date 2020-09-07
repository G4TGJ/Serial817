/*
 * io.c
 *
 * Created: 11/09/2019
 * Author : Richard Tomlinson G4TGJ
 */ 

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "io.h"

// Functions to read and write inputs and outputs
// This isolates the main logic from the I/O functions making it
// easy to change the hardware e.g. we have the option to
// connect several inputs to a single ADC pin
// Also can support different AVR versions e.g. ATtiny85 and the 1-series

#ifdef VPORTC

// ATtiny 1-series

// Configure all the I/O we need
void ioInit()
{
    /* Set all pins to low power mode */

    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }

    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }

    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTC + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }

    // Set the LED pin to an output and turn on the LED
    LED_DIR_REG |= (1 << LED_PIN);
    LED_OUT_REG |= (1 << LED_PIN);

    // Set the switch pin to an input with no pull up
    SW_DIR_REG &= ~(1 << SW_PIN);
    SW_PIN_CTRL &= ~(1 << PORT_PULLUPEN_bp);

    /* Insert nop for synchronization*/
    _NOP();
}

void toggleLED()
{
    LED_TOGGLE_REG = (1 << LED_PIN);
}

bool readSW()
{
    return !(SW_IN_REG & (1 << SW_PIN));
}

#else

// ATtiny85

// Configure all the I/O we need
void ioInit()
{
    /* Insert nop for synchronization*/
    _NOP();
}
#endif
