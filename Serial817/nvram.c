/*
 * nvram.c
 * 
 * Maintains a non-volatile memory area which
 * is stored in the EEPROM.
 *
 * Created: 07/08/2019
 * Author : Richard Tomlinson G4TGJ
 */ 
 
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "eeprom.h"
#include "nvram.h"

// Magic number used to help verify the data is correct
#define MAGIC 0x8402

// Cached version of the NVRAM - read from the EEPROM at boot time
static struct
{
    uint16_t magic;                         // Magic number to check data is valid
    uint8_t baudIndex;                      // Index into the baud rate table
    uint16_t crc;                           // CRC to check that the data is valid
} nvram_cache;

// A simple checksum. Much smaller than a proper CRC.
static uint16_t checksum( const uint8_t *input_str, size_t num_bytes )
{
    uint16_t sum = 0;
    for( int i = 0 ; i < num_bytes ; i++ )
    {
        sum += input_str[i];
    }
    return sum;
}

// Calculate the CRC for the NVRAM cache - do not include the CRC itself
static uint16_t calc_crc()
{
    // Use a checksum instead of a proper CRC to save program and data space
    return checksum( (const uint8_t *) &nvram_cache, sizeof(nvram_cache) - sizeof(nvram_cache.crc) );
}

// Update the eeprom from the cache. Calculate the CRC and only
// write any changed bytes
static void nvramUpdate()
{
    nvram_cache.crc = calc_crc();
    
    // Only write back changed bytes to minimise EEPROM wear
    for( int i = 0 ; i < sizeof( nvram_cache ) ; i++ )
    {
        if( ((uint8_t *) &nvram_cache)[i] != eepromRead(i) )
        {
            eepromWrite( i, ((uint8_t *) &nvram_cache)[i]);
        }
    }
}

// Initialise the NVRAM - read it in and check valid.
// Must be called before any operations
void nvramInit()
{
    // Read from the EEPROM into the NVRAM cache
    for( int i = 0 ; i < sizeof( nvram_cache ) ; i++ )
    {
        ((uint8_t *) &nvram_cache)[i] = eepromRead(i);
    }
    
    // Check the CRC16 and magic numbers are correct
    if( calc_crc() != nvram_cache.crc ||
        nvram_cache.magic != MAGIC)
    {
        // CRC doesn't match so set the default values
        nvram_cache.baudIndex = DEFAULT_BAUD_INDEX;
        nvram_cache.magic = MAGIC;
        
        // Calculate the CRC and write to the EEPROM
        nvramUpdate();
    }
}

// Functions to read and write parameters in the NVRAM
// Read is done directly from the cache
// Writing updates the CRC and writes back changed bytes.
uint8_t nvramReadBaudIndex()
{
    return nvram_cache.baudIndex;
}

void nvramWriteBaudIndex( uint8_t index )
{
    nvram_cache.baudIndex = index;
    nvramUpdate();
}
