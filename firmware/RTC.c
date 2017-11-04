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
// RTC_SetupCalendar()
// 
// Sets up RTC_A with calendar as 2017-11-31T12:00:00
// 
// Input:
//   time is a Calendar type to be initialized with values
// Output:
//   none
// Conditions:
//   none
void RTC_SetupCalendar(Calendar * const time)
{
	// 2017-01-01T00:00:00
	time->Seconds    = 0x55;   // Seconds of minute between 0-59
	time->Minutes    = 0x00;   // Minutes of hour between 0-59
	time->Hours      = 0x00;   // Hour of day between 0-23
	time->DayOfWeek  = 0x00;   // Day of week between 0-6
	time->DayOfMonth = 0x01;   // Day of month between 1-31
	time->Month      = 0x00;   // Month between 0-11
	time->Year       = 0x2017; // Year between 0-4095

	// Initialize Calendar Mode of RTC
	//  * Base Address of the RTC_A_A
	//  * Pass in current time, intialized above
	//  * Use BCD as Calendar Register Format
	RTC_A_initCalendar(RTC_A_BASE,
							 time,
							 RTC_A_FORMAT_BCD);
}
