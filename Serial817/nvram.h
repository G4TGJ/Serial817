/*
 * nvram.h
 *
 * Created: 06/09/2020
 * Author : Richard Tomlinson G4TGJ
 */ 
 

#ifndef NVRAM_H
#define NVRAM_H

void nvramInit();

uint8_t nvramReadBaudIndex();
void nvramWriteBaudIndex( uint8_t index );

#endif //NVRAM_H
