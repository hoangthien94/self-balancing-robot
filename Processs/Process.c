/*
 * Process.c
 *
 *  Created on: Oct 27, 2015
 *      Author: HongDuc
 */

#include "Process.h"
#include "../include.h"

#define     test    0x32

//#define	TEST_MODE
#define 	_DEBUG_QEI
#define 	_DEBUG_SPEED
#define     _DEBUG_POS

static 	int32_t	speedleft,speedright;
int32_t speedset, enc;
void Balacing_Process()
{
	if (getControlFlag())
	{


//	   configPID();

#ifdef TEST_MODE
		LED1_SWITCH();
#endif

		qei_getVelocity(1, &speedleft);
		qei_getVelocity(0, &speedright);

		PIDPositionSet(speedleft);
		PIDPosCalc(5000, 8000);
        enc = (int32_t)(getPIDPos());
        speedset = enc ;
        setControlFlag(false);
        PIDSpeedSet(speedset);
#ifdef _DEBUG_QEI
		UARTCharPut(UART0_BASE,0x30);
		UARTCharPut(UART0_BASE,speedleft /100 +0x30);
		UARTCharPut(UART0_BASE,speedleft%100 /10 +0x30 );
		UARTCharPut(UART0_BASE,speedleft%10 +0x30);
#endif
		PIDVerCalc(speedleft,1000);
		int16_t speed = (int16_t)(getPIDVer());

#ifdef _DEBUG_POS
        UARTCharPut(UART0_BASE,enc /1000 + 0x30);
        UARTCharPut(UART0_BASE,enc %1000 /100 +0x30);
        UARTCharPut(UART0_BASE,enc%100 /10 +0x30 );
        UARTCharPut(UART0_BASE,enc%10 +0x30);
#endif
		setSpeedMotor(MOTOR1,speed);
		setSpeedMotor(MOTOR2,speed);
		// read IMU
		// cal PID
		// process
	}

}
