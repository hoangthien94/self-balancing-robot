/*
 * motorControl.c
 *
 *  Created on: Oct 26, 2015
 *      Author:
 */
#include "motorControl.h"
#include "../include.h"

static	int32_t	dutycyclesmotorrleft = 0, dutycyclesmotorright = 0;
static bool changemotorleft = false, changemotorright = false;

void setSpeedMotor(MOTOR typeMotor, int32_t speed)
{
	int32_t dutycycles	= speed;		//	speed max 630.
	if( typeMotor == MOTORLEFT)
	{
		changemotorleft = true;
		dutycyclesmotorrleft = dutycycles;
	}
	else if( typeMotor == MOTORRIGHT)
	{
		changemotorright = true;
		dutycyclesmotorright = -dutycycles;         //inverted
	}
}

void ProcessSpeedControl()
{
	if( changemotorleft)
	{
		changemotorleft = false;
		SetPWM(PWM_LEFT, DEFAULT_FREQ, dutycyclesmotorrleft);
	}
	if(changemotorright )
	{
		changemotorright = false;
		SetPWM(PWM_RIGHT, DEFAULT_FREQ, dutycyclesmotorright);
	}
}



