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

#include <string.h>

#include "main.h"
#include "Analog.h"
#include "RTC.h"
#include "GPIO.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_MSC_API/UsbMscScsi.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"
#include "USB_API/USB_MSC_API/UsbMscStateMachine.h"

#include "USB_app/msc.h"  // FatFs #includes

#include "hal.h"

void initTimer(void);
void setTimer_A_Parameters(void);
void HandleSDCardReader(void);

// Global flag by which the timer ISR will trigger main() to check the
// media status
extern uint8_t bDetectCard;
Timer_A_initUpModeParam Timer_A_params = {0};

const uint16_t gNO_SOUND_REFERENCE = 0x6F0; // Experimental value of no ambient noise

volatile Calendar gNEW_TIME;
volatile uint16_t gADC_VALUE;

// -----------------------------------------------------------------------------
// main()
// 
// Logs data at a specified interval or functions as an SD card reader
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
	uint16_t splBuffer[256] = {0};
	uint8_t index = 0;
	uint8_t sampleInterval = 0;

	int16_t soundTemp = 0;
	
	// Stop watchdog timer
	WDT_A_hold(WDT_A_BASE);

	GPIO_initInputs();
	GPIO_initOutputs();
	
	if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2)) // S1 pressed
	{
		HandleSDCardReader();
	}
	else
	{
		RTC_Setup(&currentTime);
		// Start RTC Clock
		RTC_A_startClock(RTC_A_BASE);
		Analog_Setup();

		sampleInterval = GPIO_getSampleInterval();

		while(1)
		{
			// After wake from LPM, wait for conversion to complete
			while (ADC12_A_isBusy(ADC12_A_BASE));

			// Store current sample if frequency conditions are met
			if (((RTCSEC == 0) && (RTCMIN % sampleInterval == 0)) ||
				 !GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1))
			{
				soundTemp = gADC_VALUE;
				if (soundTemp >= 0x720)
				    P4OUT |= BIT7; // P4.7 = 1 (LED2)
				else
				    P4OUT &= ~BIT7; // P4.7 = 0 (LED2)

				// Toggle P4.0 LED3 (green)
				GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN0);
				
				soundTemp = abs(gNO_SOUND_REFERENCE - soundTemp);

				// Store converted SPL value in ring buffer of 256 items
				splBuffer[index] = Analog_ConvertADCtoSPLdB(soundTemp);
				index++; // 8-bit data type wraps by itself
			}

			//Enter LPM3 mode with interrupts enabled
			__bis_SR_register(LPM3_bits + GIE);
		}
	}
}

// -----------------------------------------------------------------------------
// ADC12 Interrupt Service Routine
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	switch(__even_in_range(ADC12IV,34))
	{
		case  0: break; // Vector  0: No interrupt
		case  2: break; // Vector  2: ADC overflow
		case  4: break; // Vector  4: ADC timing overflow
		case  6:
			gADC_VALUE = ADC12MEM0;
			//Exit active CPU
			__bic_SR_register_on_exit(LPM3_bits);
			break; // Vector  6: ADC12IFG0
		case  8: break; // Vector  8: ADC12IFG1
		case 10: break; // Vector 10: ADC12IFG2
		case 12: break; // Vector 12: ADC12IFG3
		case 14: break; // Vector 14: ADC12IFG4
		case 16: break; // Vector 16: ADC12IFG5
		case 18: break; // Vector 18: ADC12IFG6
		case 20: break; // Vector 20: ADC12IFG7
		case 22: break; // Vector 22: ADC12IFG8
		case 24: break; // Vector 24: ADC12IFG9
		case 26: break; // Vector 26: ADC12IFG10
		case 28: break; // Vector 28: ADC12IFG11
		case 30: break; // Vector 30: ADC12IFG12
		case 32: break; // Vector 32: ADC12IFG13
		case 34: break; // Vector 34: ADC12IFG14
		default: break;
	}
}

// -----------------------------------------------------------------------------
// RTC_A Interrupt Service Routine
#pragma vector=RTC_VECTOR
__interrupt void RTC_A_ISR(void)
{
	switch(__even_in_range(RTCIV,16))
	{
		case 0: break;      // Vector  0: No interrupts
		case 2:             // Vector  2: RTCRDYIFG
			ADC12_A_startConversion(ADC12_A_BASE,
											ADC12_A_MEMORY_0,
											ADC12_A_SINGLECHANNEL);
			// Toggle P1.0 every second			
			GPIO_toggleOutputOnPin(
				GPIO_PORT_P1,
				GPIO_PIN0); // P1.0 LED1 (orange)
			break;
		case 4:             // Vector  4: RTCEVIFG
			// Interrupts every minute
			__no_operation();
			// Read out new time a minute later
			gNEW_TIME = RTC_A_getCalendarTime(RTC_A_BASE);
			break;
		case 6:             // Vector  6: RTCAIFG
			// Interrupts 5:00pm on 5th day of week
			__no_operation();
			break;
		case 8: break;      // Vector  8: RT0PSIFG
		case 10: break;     // Vector 10: RT1PSIFG
		case 12: break;     // Vector 12: Reserved
		case 14: break;     // Vector 14: Reserved
		case 16: break;     // Vector 16: Reserved
		default: break;
	}
}

// -----------------------------------------------------------------------------
// TIMER0_A Interrupt Service Routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
	//Set the flag that will trigger main() to detect the card
	bDetectCard = 0x01;
	
	//Wake from ISR, if sleeping
	__bic_SR_register_on_exit(LPM0_bits);
}

// -----------------------------------------------------------------------------
// UNMI Interrupt Service Routine
#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR (void)
{
	switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
	{
		case SYSUNIV_NONE:
			__no_operation();
			break;
		case SYSUNIV_NMIIFG:
			__no_operation();
			break;
		case SYSUNIV_OFIFG:
			UCS_clearFaultFlag(UCS_XT2OFFG);
			UCS_clearFaultFlag(UCS_DCOFFG);
			SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
			break;
		case SYSUNIV_ACCVIFG:
			__no_operation();
			break;
		case SYSUNIV_BUSIFG:
			// If the CPU accesses USB memory while the USB module is
			// suspended, a "bus error" can occur.  This generates an NMI.  If
			// USB is automatically disconnecting in your software, set a
			// breakpoint here and see if execution hits it.  See the
			// Programmer's Guide for more information.
			SYSBERRIV = 0; //clear bus error flag
			USB_disable(); //Disable
	}
}

// -----------------------------------------------------------------------------
// setTimer_A_Parameters()
// 
// This function sets the timer A parameters
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void setTimer_A_Parameters()
{
	Timer_A_params.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
	Timer_A_params.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
	Timer_A_params.timerPeriod = 32768;
	Timer_A_params.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
	Timer_A_params.captureCompareInterruptEnable_CCR0_CCIE =
	                                  TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
	Timer_A_params.timerClear = TIMER_A_DO_CLEAR;
	Timer_A_params.startTimer = false;
}

// -----------------------------------------------------------------------------
// initTimer()
// 
// This function initialises timer A
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   Timer A parameters must be set
void initTimer(void)
{

	 setTimer_A_Parameters();

	// Start timer
	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

	Timer_A_initUpMode(TIMER_A0_BASE, &Timer_A_params);

	Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

// -----------------------------------------------------------------------------
// HandleSDCardReader()
// 
// This function performs SD card reader functionality
// Copyright (c) 2016, Texas Instruments Incorporated
// 
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void HandleSDCardReader(void)
{
	// Minumum Vcore setting required for the USB API is PMM_CORE_LEVEL_2 .
	PMM_setVCore(PMM_CORE_LEVEL_2);
	USBHAL_initPorts();                // Config GPIOS for low-power (output low)
	USBHAL_initClocks(MCLK_FREQUENCY); // Config clocks. MCLK=SMCLK=FLL=MCLK_FREQUENCY; ACLK=REFO=32kHz
	USBMSC_initMSC();                  // Initialize MSC API, and report media to the host
	initTimer();
	USB_setup(TRUE, TRUE);      // Init USB & events; if a host is present, connect

	__enable_interrupt();       // Enable interrupts globally

	while (1)
	{
		switch (USB_getConnectionState())
		{
			case ST_ENUM_ACTIVE:

				USBMSC_processMSCBuffer(); // Handle READ/WRITE cmds from the host

				// Every second, the Timer_A ISR sets this flag.  The
				// checking can't be done from within the timer ISR, because it
				// enables interrupts, and this is not a recommended
				// practice due to the risk of nested interrupts.
				if (bDetectCard){
					USBMSC_checkMSCInsertionRemoval();

					// Clear the flag, until the next timer ISR
					bDetectCard = 0x00;
				}

				break;

			// These cases are executed while your device is disconnected from
			// the host (meaning, not enumerated); enumerated but suspended
			// by the host, or connected to a powered hub without a USB host
			// present.
			case ST_PHYS_DISCONNECTED:
			case ST_ENUM_SUSPENDED:
			case ST_PHYS_CONNECTED_NOENUM_SUSP:
				 __bis_SR_register(LPM3_bits + GIE);
				 _NOP();
				 break;

			// The default is executed for the momentary state
			// ST_ENUM_IN_PROGRESS.  Usually, this state only last a few
			// seconds.  Be sure not to enter LPM3 in this state; USB
			// communication is taking place here, and therefore the mode must
			// be LPM0 or active-CPU.
			case ST_ENUM_IN_PROGRESS:
			default:;
		}
	}
}
