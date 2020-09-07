/*
 * Serial817.c
 *
 * Created: 04/09/2020 22:05:38
 * Author : Richard
 */ 

#include <stdio.h>
#include <ctype.h>
#include <avr/io.h>
#include "config.h"
#include "io.h"
#include "millis.h"
#include "serial.h"
#include "display.h"
#include "nvram.h"
#include "pushbutton.h"

#define NUM_BAUD_RATES 8
static const uint32_t baudRate[NUM_BAUD_RATES] =
{
    1200,
    2400,
    4800,
    9600,
    19200,
    38400,
    57600,
    115200,
};

static uint8_t baudIndex;


// Display the baud rate on the LCD and print to the serial
void displayBaudRate()
{
    char buf[20];
    sprintf(buf,"%lu baud",baudRate[baudIndex]);
    displayText( 0, buf, true );

    serialTXString("\r\n");
    serialTXString(buf);
    serialTXString("\r\n");
}

int main(void)
{
    // Keep track of pushbutton debounce
    static struct sDebounceState debounceState;

    uint32_t previousMillis = 0;

    bool shortPress, longPress;

    // Receive string for the char plus null
    char s[2];

    // Set up the timer
    millisInit();

    // Initialise the inputs and outputs
    ioInit();

    // Initialise the NVRAM driver and get the baud rate index
    nvramInit();
    baudIndex = nvramReadBaudIndex();

    // Set up the serial with a message
    serialInit(baudRate[baudIndex]);
    serialTXString("Welcome to the ATtiny817\n\r");

    // Set up the display with a brief welcome message
    displayInit();
    displayText( 0, "ATtiny817", true );
    delay(1000);

    displayBaudRate();

    // Null terminate the received char string
    s[1] = '\0';

    while (1) 
    {
        uint32_t currentMillis = millis();

        // Have we received a character?
        s[0] = serialReceive();
        if( s[0] )
        {
            // Echo the character
            serialTransmit( s[0] );

            // Display and scroll on the bottom line of the screen if a printable char
            if( isprint(s[0]) )
            {
                displayText( 1, s, false );
            }

            // If it was a return then need to line feed too
            if( s[0] == '\r' )
            {
                serialTransmit( '\n' );

                // Clear the bottom LCD line
                displayText(1, "", true);
            }
        }

        // Read and debounce the pushbutton
        debouncePushbutton( readSW(), &shortPress, &longPress, 200, 1000, &debounceState);

        // If a long press off the button then display a message
        if( longPress )
        {
            displayText(0, "Long press", true);
        }

        // If the button is pressed then change the baud rate
        if( shortPress )
        {
            baudIndex = (baudIndex+1) % NUM_BAUD_RATES;

            // Display the baud rate before it changes
            displayBaudRate();

            // Wait for the new baud rate to be transmitted over
            // the serial before changing the rate.
            delay(200);

            // Set the new baud rate
            serialInit( baudRate[baudIndex] );

            // Store the new baud rate
            nvramWriteBaudIndex(baudIndex);
        }

        // Toggle LED state every second
        if( (currentMillis - previousMillis) > 1000 )
        {
            toggleLED();
            previousMillis = currentMillis;
        }
    }
}

