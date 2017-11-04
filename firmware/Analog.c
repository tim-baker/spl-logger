// -----------------------------------------------------------------------------
// Filename:
//   Analog.c
// Description:
//   * Hardware abstraction layer for the ADC12 module
//   * Sound pressure level conversion
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#include "Analog.h"

// -----------------------------------------------------------------------------
// Analog_SetupADC()
// 
// Sets up ADC12_A
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void Analog_SetupADC(void)
{
	ADC12CTL0 &= ~ADC12ENC;           // ADC12ENC = 0 to allow control registers to be modified
	ADC12CTL0 = ADC12SHT02 + ADC12ON; // Sampling time, ADC12 on
	ADC12CTL1 = ADC12SHP;             // Use sampling timer
	ADC12IE = 0x01;                   // Enable interrupt
	ADC12CTL0 |= ADC12ENC;            // ADC12ENC = 1 to allow conversion to start
	P6SEL |= BIT0;                    // Select P6.0 ADC option
}
