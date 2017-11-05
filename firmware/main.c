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
volatile uint16_t gIN_VALUE;

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
   uint16_t buffer[256] = {0};
   uint8_t index = 0;
   uint8_t sampleInterval = 0;

   int16_t soundTemp = 0;
   unsigned long soundLevel = 0;
	
	// Stop watchdog timer
	WDT_A_hold(WDT_A_BASE);

	if (!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN2)) // S1 pressed
	{
		HandleSDCardReader();
	}
	else
	{
		GPIO_initInputs();
		GPIO_initOutputs();

		RTC_Setup(&currentTime);
		Analog_Setup();

		sampleInterval = GPIO_getSampleInterval();

		while(1)
		{
			// // Enable/Start first sampling and conversion cycle
			// /*
			//  * Base address of ADC12_A Module
			//  * Start the conversion into memory buffer 0
			//  * Use the single-channel, single-conversion mode
			//  */
			// ADC12_A_startConversion(ADC12_A_BASE,
			//                         ADC12_A_MEMORY_0,
			//                         ADC12_A_SINGLECHANNEL);

			// // Poll for interrupt on memory buffer 0
			while (ADC12_A_isBusy(ADC12_A_BASE));

			// Store current sample if frequency conditions are met
			if (RTCSEC % sampleInterval == 0)
			{
				soundTemp = gIN_VALUE;
				if (soundTemp >= 0x731)
					P4OUT |= BIT0; // P4.0 = 1 (LED3)
				else
					P4OUT &= ~BIT0; // P4.0 = 0 (LED3)
				
				soundTemp = abs(gNO_SOUND_REFERENCE - soundTemp);

				// Store in ring buffer of 256 items
				buffer[index] = Analog_ConvertADCtoSPLdB(soundTemp);
				index++; // 8-bit data type wraps by itself
			}

			//SET BREAKPOINT HERE
			//Enter LPM3 mode with interrupts enabled
			__bis_SR_register(LPM3_bits + GIE);
			__no_operation();
		}
	}
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	switch(__even_in_range(ADC12IV,34))
	{
		case  0: break; // Vector  0: No interrupt
		case  2: break; // Vector  2: ADC overflow
		case  4: break; // Vector  4: ADC timing overflow
		case  6:
			gIN_VALUE = ADC12MEM0;
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

/*
 * ======== TIMER0_A0_ISR ========
 */
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not found!
#endif
{
    //Set the flag that will trigger main() to detect the card
    bDetectCard = 0x01;

    //Wake from ISR, if sleeping
    __bic_SR_register_on_exit(LPM0_bits);
}

/*
 * ======== UNMI_ISR ========
 */
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not found!
#endif
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


/*
 * ======== setTimer_A_Parameters ========
 */
// This function sets the timer A parameters
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


void initTimer(void)
{

    setTimer_A_Parameters();

    //start timer
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

    Timer_A_initUpMode(TIMER_A0_BASE, &Timer_A_params);

    Timer_A_startCounter(TIMER_A0_BASE,
        TIMER_A_UP_MODE);
}

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
