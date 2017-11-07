// -----------------------------------------------------------------------------
// Filename:
//   RTC.h
// Description:
//   * Hardware abstraction layer for the RTC_A module
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#include "RTC.h"

// -----------------------------------------------------------------------------
// RTC_Setup()
// 
// Sets up RTC_A and sets calendar to input argument
// 
// Input:
//   time is a Calendar type to be initialized with values
// Output:
//   none
// Conditions:
//   none
void RTC_Setup(Calendar * time)
{
	// Select XT1
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 + GPIO_PIN5);

	// Initialize LFXT1
	UCS_turnOnLFXT1(UCS_XT1_DRIVE_3, UCS_XCAP_3);

	//Setup Current Time for Calendar
	time->Seconds    = 0x50;
	time->Minutes    = 0x30;
	time->Hours      = 0x02;
	time->DayOfWeek  = 0x00;
	time->DayOfMonth = 0x06;
	time->Month      = 0x0B; // 11
	time->Year       = 0x2017;

	//Initialize Calendar Mode of RTC
	/*
	* Base Address of the RTC_A_A
	* Pass in current time, intialized above
	* Use BCD as Calendar Register Format
	*/
	RTC_A_initCalendar(RTC_A_BASE,
	                   time,
	                   RTC_A_FORMAT_BCD);

	//Setup Calendar Alarm for 5:00pm on the 5th day of the week.
	//Note: Does not specify day of the week.
	RTC_A_configureCalendarAlarmParam param1 = {0};
	param1.minutesAlarm = 0x00;
	param1.hoursAlarm = 0x03;
	param1.dayOfWeekAlarm = RTC_A_ALARMCONDITION_OFF;
	param1.dayOfMonthAlarm = 0x06;
	RTC_A_configureCalendarAlarm(RTC_A_BASE, &param1);
	
	//Specify an interrupt to assert every minute
	RTC_A_setCalendarEvent(RTC_A_BASE,
	                       RTC_A_CALENDAREVENT_MINUTECHANGE);

	// Enable interrupt for RTC Ready Status, which asserts when the RTC
	// Calendar registers are ready to read.
	// Also, enable interrupts for the Calendar alarm and Calendar event.
	RTC_A_clearInterrupt(RTC_A_BASE, RTCRDYIFG + RTCTEVIFG + RTCAIFG);
	RTC_A_enableInterrupt(RTC_A_BASE, RTCRDYIE + RTCTEVIE + RTCAIE);
}
