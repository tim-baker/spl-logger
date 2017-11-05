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

void GPIO_initOutputs(void)
{
	// Outputs
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                   // P1.0 output (LED1)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);                   // P4.0 output (LED3)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);                   // P4.1 output (LED4)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);                   // P4.7 output (LED2)
}

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

uint8_t GPIO_getSampleInterval(void)
{
	if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	    !GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
		GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x04;
	}
	else if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	         !GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
		GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x03;
	}
	else if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	         GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);
		GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x02;
	}
	else if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) &&
	         GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2))
	{
		GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
		GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);

		return 0x01;
	}
	else
		return 0x01;
}
