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
void RTC_Setup(Calendar * time);

#endif // RTC_H
