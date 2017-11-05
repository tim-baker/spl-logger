// -----------------------------------------------------------------------------
// Filename:
//   Analog.h
// Description:
//   * Hardware abstraction layer for the ADC12 module
//   * Sound pressure level conversion
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#ifndef ANALOG_H
#define ANALOG_H

#include "main.h"

// -----------------------------------------------------------------------------
// Analog_Setup()
// 
// Sets up ADC12
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void Analog_Setup(void);

// -----------------------------------------------------------------------------
// Analog_ConvertADCtoSPLdB()
// 
// Converts 12-bit ADC value SPL approximation
// 
// Input:
//   none
// Output:
//   return value is SPL in decibels
// Conditions:
//   none
uint8_t Analog_ConvertADCtoSPLdB(uint16_t value);

#endif // ANALOG_H
