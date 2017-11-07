// -----------------------------------------------------------------------------
// Filename:
//   GPIO.c
// Description:
//   Hardware abstraction layer for GPIO
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#include "GPIO.h"

// -----------------------------------------------------------------------------
// GPIO_initOutputs()
// 
// Initializes outputs on SPL logger board
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void GPIO_initOutputs(void)
{
	// Outputs
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                   // P1.0 output (LED1)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);                   // P4.0 output (LED3)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);                   // P4.1 output (LED4)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);                   // P4.7 output (LED2)

	GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0); // P4.0 off (LED3)
	GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1); // P4.1 off (LED4)
}

// -----------------------------------------------------------------------------
// GPIO_initInputs()
// 
// Initializes inputs on SPL logger board
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void GPIO_initInputs(void)
{
	// Inputs
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN2);  // P2.2 input (S1)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  // P1.1 input (S2)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3); // P1.3 input (S3.1)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2); // P1.2 input (S3.2)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN0); // P2.0 input (J1)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN1); // P2.1 input (J2)
}

// -----------------------------------------------------------------------------
// GPIO_getSampleInterval()
// 
// Gets position of DIP switch and returns value
// 
// Input:
//   none
// Output:
//   returns number of minutes between samples
// Conditions:
//   none
uint8_t GPIO_getSampleInterval(void)
{
	if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	    !GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		//GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
		//GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x0A; // 10 minutes interval
	}
	else if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	         !GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		//GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
		//GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x05; // 5 minutes interval
	}
	else if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	         GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		//GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
		//GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x02; // 2 minutes interval
	}
	else if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	         GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		//GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
		//GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x01; // 1 minutes interval
	}
	else
		return 0x01;
}
