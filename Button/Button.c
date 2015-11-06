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
	GPIOPadConfigSet( GPIO_PORTF_BASE, GPIO_PIN_4 ,  GPIO_STRENGTH_4MA , GPIO_PIN_TYPE_STD_WPU );
	GPIOIntTypeSet( GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);
	GPIOIntRegister(GPIO_PORTF_BASE, &buttonISR);
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);
	IntEnable(INT_GPIOF);
}
void buttonISR()
{
	GPIOIntClear(GPIO_PORTF_BASE, GPIOIntStatus( GPIO_PORTF_BASE, true));
	if( getMode() == SLEEP_MODE)
	{
	    setMode( BALANCING_MODE);
	    enableBoostCircuit( 1);
	    SysCtlDelay(10000);
	    speed_Enable_Hbridge(1);
	    setSpeedMotor( MOTORLEFT , 0);
	    setSpeedMotor( MOTORRIGHT , 0);
	}
	else
	{
	    setMode(SLEEP_MODE);
	    LED_OFF(LED_BLUE_PIN );
        enableBoostCircuit(0);
        speed_Enable_Hbridge(0);
	}

}


