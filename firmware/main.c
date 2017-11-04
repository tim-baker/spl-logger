// -----------------------------------------------------------------------------
// Filename:
//   main.c
// Description:
//   SPL logger
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#include "main.h"

#include "Analog.h"
#include "RTC.h"

const uint16_t gNO_SOUND_REFERENCE = 0x6F0; // Experimental value of no ambient noise
volatile Calendar gNEW_TIME;

// -----------------------------------------------------------------------------
// main()
// 
// Fucntion description here...
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void main(void)
{
	Calendar currentTime;
   uint16_t data[32] = {0};
   int16_t i = 0;

   int16_t soundTemp = 0;
   uint16_t soundLevel = 0;

	// Stop watchdog timer
	WDT_A_hold(WDT_A_BASE);

	// Inputs
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN2);  // P2.2 input (S1)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  // P1.1 input (S2)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3); // P1.3 input (S3.1)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2); // P1.2 input (S3.2)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN0); // P2.0 input (J1)
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN1); // P2.1 input (J2)

	// Outputs
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);                   // P1.0 output (LED1)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);                   // P4.0 output (LED3)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);                   // P4.1 output (LED4)
	GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);                   // P4.7 output (LED2)

	// GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);             // P1.0 off (LED1)
	// GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);             // P4.0 off (LED3)
	// GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);             // P4.1 off (LED4)
	// GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);             // P4.7 off (LED2)

	// Select XT1
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 + GPIO_PIN5);

	// Initialize LFXT1
	UCS_turnOnLFXT1(UCS_XT1_DRIVE_3, UCS_XCAP_3);

	// Setup currentTime for Calendar
	RTC_SetupCalendar(&currentTime);

	// Specify an interrupt to assert every day at midnight.
	RTC_A_setCalendarEvent(RTC_A_BASE, RTC_A_CALENDAREVENT_MIDNIGHT);

	// Enable interrupt for RTC Ready Status, which asserts when the RTC
	// Calendar registers are ready to read.
	// Also, enable interrupts for the Calendar alarm and Calendar event.
	RTC_A_clearInterrupt(RTC_A_BASE, RTCRDYIFG + RTCTEVIFG + RTCAIFG);
	RTC_A_enableInterrupt(RTC_A_BASE, RTCRDYIE + RTCTEVIE + RTCAIE);

	// Start RTC Clock
	RTC_A_startClock(RTC_A_BASE);

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
	ADC12_A_configureMemoryParam param = {0};
	param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
	param.inputSourceSelect = ADC12_A_INPUT_A0;
	param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_AVCC;
	param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	param.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
	ADC12_A_configureMemory(ADC12_A_BASE,&param);

	//Enable memory buffer 0 interrupt
	ADC12_A_clearInterrupt(ADC12_A_BASE,
									ADC12IFG0);
	ADC12_A_enableInterrupt(ADC12_A_BASE,
									ADC12IE0);

	// S3.1
	if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3)) // inverted logic
		GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
	else
		GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7); // LED2

	// S3.2
	if (!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN2)) // inverted logic
		GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
	else
		GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0); // LED3

	while(1)
	{
		// Enable/Start first sampling and conversion cycle
		/*
		 * Base address of ADC12_A Module
		 * Start the conversion into memory buffer 0
		 * Use the single-channel, single-conversion mode
		 */
		ADC12_A_startConversion(ADC12_A_BASE,
										ADC12_A_MEMORY_0,
										ADC12_A_SINGLECHANNEL);

		// S1
		if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2)) // inverted logic
			GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
		else
			GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // LED1

		// //Poll for interrupt on memory buffer 0
		while(!ADC12_A_getInterruptStatus(ADC12_A_BASE,
													 ADC12IFG0))
		{
			// null
		}



		// Capture next sample into array
		soundTemp = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_0);
		if (soundTemp >= 0x704)
			P4OUT |= BIT1; // P4.1 = 1 (LED4)
		else
			P4OUT &= ~BIT1; // P4.1 = 0 (LED4)
		
		if ((gNO_SOUND_REFERENCE - soundTemp) >= 0)
		{
			soundTemp = gNO_SOUND_REFERENCE - soundTemp;

			if (soundTemp <= 2)
				soundLevel = 70;
			else if (soundTemp <= 3)
				soundLevel = 75;
			else if (soundTemp <= 6)
				soundLevel = 80;
			else if (soundTemp <= 20)
				soundLevel = 90;
			else if (soundTemp <= 27)
				soundLevel = 93;
			else if (soundTemp <= 37)
				soundLevel = 95;
			else if (soundTemp <= 49)
				soundLevel = 98;
			else if (soundTemp <= 66)
				soundLevel = 100;
			else if (soundTemp <= 88)
				soundLevel = 103;
			else if (soundTemp <= 117)
				soundLevel = 105;
			else if (soundTemp <= 156)
				soundLevel = 108;
			else if (soundTemp <= 208)
				soundLevel = 110;
			else if (soundTemp <= 278)
				soundLevel = 113;
			else if (soundTemp <= 371)
				soundLevel = 115;
			else if (soundTemp <= 495)
				soundLevel = 118;
			else if (soundTemp <= 660)
				soundLevel = 120;
			else
				soundLevel = 130;

			// Shuffle samples along 1 element
			for (i = 30; i >= 0; --i)
			{
				data[i + 1] = data[i];
			}
			data[0] = soundLevel;
		}
		


		//SET BREAKPOINT HERE
		__no_operation();
	}
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch(__even_in_range(ADC12IV,34))
	{
		case  0: break;                           // Vector  0: No interrupt
		case  2: break;                           // Vector  2: ADC overflow
		case  4: break;                           // Vector  4: ADC timing overflow
		case  6:                                  // Vector  6: ADC12IFG0
			// // ADC12MEM = A0 > 0.45AVcc?
			// if (ADC12MEM0 >= 0x745)
			// 	P1OUT |= BIT0;  // P4.7 on (LED2)
			// else
			// 	P1OUT &= ~BIT0; // P4.7 on (LED2)
			
			// ADC12MEM = A0 > 0.40AVcc?

			
			// // Exit active CPU
			// __bic_SR_register_on_exit(LPM3_bits);
		case  8: break;                           // Vector  8: ADC12IFG1
		case 10: break;                           // Vector 10: ADC12IFG2
		case 12: break;                           // Vector 12: ADC12IFG3
		case 14: break;                           // Vector 14: ADC12IFG4
		case 16: break;                           // Vector 16: ADC12IFG5
		case 18: break;                           // Vector 18: ADC12IFG6
		case 20: break;                           // Vector 20: ADC12IFG7
		case 22: break;                           // Vector 22: ADC12IFG8
		case 24: break;                           // Vector 24: ADC12IFG9
		case 26: break;                           // Vector 26: ADC12IFG10
		case 28: break;                           // Vector 28: ADC12IFG11
		case 30: break;                           // Vector 30: ADC12IFG12
		case 32: break;                           // Vector 32: ADC12IFG13
		case 34: break;                           // Vector 34: ADC12IFG14
		default: break;
}
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_A_ISR(void)
{
	 switch(__even_in_range(RTCIV,16))
	 {
	 case 0: break;      //No interrupts
	 case 2:             //RTCRDYIFG
		  //Toggle P1.0 every second
		  GPIO_toggleOutputOnPin(
				GPIO_PORT_P1,
				GPIO_PIN0); // P1.0 LED1 (orange)
		  break;
	 case 4:             // RTCEVIFG
		  //Interrupts every minute
		  __no_operation();

		  // Read out New Time a Minute Later BREAKPOINT HERE
		  // gNEW_TIME = RTC_A_getCalendarTime(RTC_A_BASE);

		  //Toggle P4.0 every minute
		  // GPIO_toggleOutputOnPin(
		  //     GPIO_PORT_P4,
		  //     GPIO_PIN0); // P4.0 LED3 (green)

		  break;
	 case 6: break;
	 case 8: break;      // RT0PSIFG
	 case 10: break;     // RT1PSIFG
	 case 12: break;     // Reserved
	 case 14: break;     // Reserved
	 case 16: break;     // Reserved
	 default: break;
	 }
}
