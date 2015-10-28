/*
 * motorControl.c
 *
 *  Created on: Oct 26, 2015
 *      Author:
 */
#include "motorControl.h"
#include "../include.h"

//#define 	SPEED_CONTROL
static	int32_t	dutycyclesmotor1 = 00, dutycyclesmotor2 = 00;
static bool changemotor1 = false, changemotor2 = false;

void setSpeedMotor(MOTOR typeMotor, int32_t speed)
{
	int dutycycles	= speed/7;		//	speed max 630.
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
//void enableSpeedMotor()
//{
//	if (!( dutycyclesmotor1 || dutycyclesmotor2 ))
//	{
//		speed_Enable_Hbridge(false);
//		return;
//	}
//	else
//	{
//		SetPWM(PWM_1, DEFAULT_FREQ,dutycyclesmotor1);
//		SetPWM(PWM_2, DEFAULT_FREQ,dutycyclesmotor2);
//	}
//}
void ProcessSpeedControl()
{
#ifdef	SPEED_CONTROL
	LED2_SWITCH();
#endif
	if( changemotor1)
	{
		changemotor1 = false;
		SetPWM(PWM_1, DEFAULT_FREQ,dutycyclesmotor1);
	}
	if(changemotor2 )
	{
		changemotor2 = false;
		SetPWM(PWM_2, DEFAULT_FREQ,dutycyclesmotor2);
	}
}



