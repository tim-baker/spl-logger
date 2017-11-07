// -----------------------------------------------------------------------------
// Filename:
//   GPIO.h
// Description:
//   Hardware abstraction layer for GPIO
// Author:
//   TB
// Last modified:
//   31-Oct-17
// Copyright (c) 2017 by Timothy Baker

#ifndef GPIO_H
#define GPIO_H

#include "main.h"

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
void GPIO_initOutputs(void);

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
void GPIO_initInputs(void);

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
uint8_t GPIO_getSampleInterval(void);

#endif // GPIO_H
