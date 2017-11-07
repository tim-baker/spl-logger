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
// Analog_Setup()
// 
// Sets up ADC12_A
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void Analog_Setup(void)
{
	//P6.0 ADC option select
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN0);

	//Initialize the ADC12_A_A Module
	/*
	 * Base address of ADC12_A_A Module
	 * Use internal ADC12_A_A bit as sample/hold signal to start conversion
	 * USE MODOSC 5MHZ Digital Oscillator as clock source
	 * Use default clock divider of 1
	 */
	ADC12_A_init(ADC12_A_BASE,
					 ADC12_A_SAMPLEHOLDSOURCE_SC,
					 ADC12_A_CLOCKSOURCE_ADC12OSC,
					 ADC12_A_CLOCKDIVIDER_1);

	ADC12_A_enable(ADC12_A_BASE);

	/*
	 * Base address of ADC12_A_A Module
	 * For memory buffers 0-7 sample/hold for 64 clock cycles
	 * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
	 * Disable Multiple Sampling
	 */
	ADC12_A_setupSamplingTimer(ADC12_A_BASE,
										ADC12_A_CYCLEHOLD_64_CYCLES,
										ADC12_A_CYCLEHOLD_4_CYCLES,
										ADC12_A_MULTIPLESAMPLESDISABLE);

	// Configure Memory Buffer
	/*
	 * Base address of the ADC12_A_A Module
	 * Configure memory buffer 0
	 * Map input A0 to memory buffer 0
	 * Vref+ = AVcc
	 * Vr- = AVss
	 * Memory buffer 0 is not the end of a sequence
	 */
	ADC12_A_configureMemoryParam param2 = {0};
	param2.memoryBufferControlIndex = ADC12_A_MEMORY_0;
	param2.inputSourceSelect = ADC12_A_INPUT_A0;
	param2.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
	param2.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	param2.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
	ADC12_A_configureMemory(ADC12_A_BASE,&param2);

	//Enable memory buffer 0 interrupt
	ADC12_A_clearInterrupt(ADC12_A_BASE,
	                       ADC12IFG0);
	ADC12_A_enableInterrupt(ADC12_A_BASE,
	                        ADC12IE0);
}

// -----------------------------------------------------------------------------
// Analog_ConvertADCtoSPLdB()
// 
// Converts 12-bit ADC value to SPL approximation
// 
// Input:
//   none
// Output:
//   return value is SPL in decibels
// Conditions:
//   none
uint8_t Analog_ConvertADCtoSPLdB(uint16_t value)
{
	if (value <= 2)
		return 70;
	else if (value <= 3)
		return 75;
	else if (value <= 6)
		return 80;
	else if (value <= 20)
		return 90;
	else if (value <= 27)
		return 93;
	else if (value <= 37)
		return 95;
	else if (value <= 49)
		return 98;
	else if (value <= 66)
		return 100;
	else if (value <= 88)
		return 103;
	else if (value <= 117)
		return 105;
	else if (value <= 156)
		return 108;
	else if (value <= 208)
		return 110;
	else if (value <= 278)
		return 113;
	else if (value <= 371)
		return 115;
	else if (value <= 495)
		return 118;
	else if (value <= 660)
		return 120;
	else
		return 130;
}
