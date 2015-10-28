/*
 * Button.c
 *
 *  Created on: Oct 27, 2015
 *      Author: HongDuc
 */
#include "Button.h"
#include "../include.h"

void buttonInit()
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  		// PF2 button recieve, PF3 button send, set interrupt
	GPIODirModeSet( GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet( GPIO_PORTF_BASE, GPIO_PIN_4 ,  GPIO_STRENGTH_4MA , GPIO_PIN_TYPE_STD_WPD );
	GPIOIntTypeSet( GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);
	GPIOIntRegister(GPIO_PORTF_BASE, &buttonISR);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
	IntEnable(INT_GPIOF);
}
void buttonISR()
{
	GPIOIntClear(GPIO_PORTF_BASE, GPIOIntStatus( GPIO_PORTF_BASE, true));
	setSpeedMotor(MOTOR1, 0);
	setSpeedMotor(MOTOR2, 0);
}


