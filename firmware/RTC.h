// -----------------------------------------------------------------------------
// Filename:
//   RTC.h
// Description:
//   * Hardware abstraction layer for the RTC_A module
//   * Ability to timestamp SPL samples
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#ifndef RTC_H
#define RTC_H

#include "main.h"

void RTC_Setup(Calendar * time);

// -----------------------------------------------------------------------------
// RTC_SetupCalendar()
// 
// Sets up RTC_A with calendar as 2017-11-31T12:00:00+00:00
// 
// Input:
//   time is a Calendar type to be initialized with values
// Output:
//   none
// Conditions:
//   none
void RTC_SetupCalendar(Calendar * const time);

#endif // RTC_H
