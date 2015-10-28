/*
 * LED.c
 *
 *  Created on: Oct 27, 2015
 *      Author: HongDuc
 */
#include "LED.h"
#include "../include.h"


void ledInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  		// PF2 button recieve, PF3 button send, set interrupt
	GPIOPinTypeGPIOOutput(LED_PORT, LED_1_PIN | LED_2_PIN |LED_3_PIN);
}

