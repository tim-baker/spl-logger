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

void GPIO_initInputs(void);
void GPIO_initOutputs(void);
uint8_t GPIO_getSampleInterval(void);

#endif // GPIO_H
