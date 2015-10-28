/*
 * ConfigTimer.c
 *
 *  Created on: May 11, 2013
 *      Author: NHH
 */
#include "../include.h"
#include "Timer.h"

//#define 	 TEST_TIMER

#define 	USE_TIMER2

static bool ControlFlag;
void timerInit(uint32_t ui32Period)
{

#ifdef 	USE_TIMER2
	SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER2);
	//CONFIG TIMER2
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP );
	TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period-1);
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerIntRegister(TIMER2_BASE,TIMER_A,&Timer2AIntHandler);
	TimerEnable(TIMER2_BASE, TIMER_A);
#endif
}

void Timer2AIntHandler(void)
{
    TimerIntClear(TIMER2_BASE,TimerIntStatus(TIMER2_BASE,true));
    ControlFlag = true;
#ifdef	TEST_TIMER
    LED2_SWITCH();
#endif
    // read	eqi
    // read mpu
    // speed set
}

bool getControlFlag()
{
	return	ControlFlag;
}
void setControlFlag(bool flag)
{
	ControlFlag = flag;
}
