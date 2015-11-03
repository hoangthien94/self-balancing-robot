/*
 * motorControl.c
 *
 *  Created on: Oct 26, 2015
 *      Author:
 */
#include "motorControl.h"
#include "../include.h"

static	int32_t	dutycyclesmotor1 = 0, dutycyclesmotor2 = 0;
static bool changemotor1 = false, changemotor2 = false;

void setSpeedMotor(MOTOR typeMotor, int32_t speed)
{
	int dutycycles	= speed;		//	speed max 630.
	if( typeMotor == MOTOR1)
	{
		changemotor1 = true;
		dutycyclesmotor1 = dutycycles;
	}
	else if( typeMotor == MOTOR2)
	{
		changemotor2 = true;
		dutycyclesmotor2 = dutycycles;
	}
}

void ProcessSpeedControl()
{
	if( changemotor1)
	{
		changemotor1 = false;
		SetPWM(PWM_1, DEFAULT_FREQ, dutycyclesmotor1);
	}
	if(changemotor2 )
	{
		changemotor2 = false;
		SetPWM(PWM_2, DEFAULT_FREQ, dutycyclesmotor2);
	}
}



