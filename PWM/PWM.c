/*
 * PWM.c
 *
 *  Created on: Oct 25, 2015
 *      Author:
 */

#include "PWM.h"
#include "..\include.h"

//#define		PB2_T3CCP0
//#define		PB0_T2CCP0
#define		PB4_T1CCP0




void ConfigBoostCircuit()
{
	SysCtlPeripheralEnable(BOOST_ENABLE_PREIPHERAL);
	GPIOPinTypeGPIOOutput(BOOST_ENABLE_PORT, BOOST_ENABLE_PIN);
	GPIOPadConfigSet(BOOST_ENABLE_PORT, BOOST_ENABLE_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
	enableBoostCircuit(false);	// TODO: false to disable	boost circuit
}

void ConfigDRV_Enable()
{
	SysCtlPeripheralEnable(DRV_ENABLE_LEFT_CHN_PERIPHERAL);
	SysCtlPeripheralEnable(DRV_ENABLE_RIGHT_CHN_PERIPHERAL);
	GPIOPinTypeGPIOOutput(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN);
	GPIOPinTypeGPIOOutput(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN);
//	GPIOPinTypeGPIOOutputOD(DRV_ENABLE_RIGHT_CHN_PORT,DRV_ENABLE_LEFT_CHN_PIN  | DRV_ENABLE_RIGHT_CHN_PIN);
	//	Turn off DRV when start
	GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN, 0);
	GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN, 0);
}
void Config_PWM(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

#ifdef	PB2_T3CCP0
	// Configure timer
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	GPIOPinConfigure(GPIO_PB2_T3CCP0);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
	TimerLoadSet(TIMER3_BASE, TIMER_A, DEFAULT_FREQ);
	TimerMatchSet(TIMER3_BASE, TIMER_A, DEFAULT_FREQ); // PWM
	TimerControlLevel(TIMER3_BASE, TIMER_A, false);
	TimerEnable(TIMER3_BASE, TIMER_A);
#endif

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	GPIOPinConfigure(GPIO_PB6_T0CCP0);		//Right
	GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
	TimerLoadSet(TIMER0_BASE, TIMER_A, DEFAULT_FREQ);
	TimerMatchSet(TIMER0_BASE, TIMER_A, DEFAULT_FREQ); // PWM
	TimerControlLevel(TIMER0_BASE, TIMER_A, false);
	TimerEnable(TIMER0_BASE, TIMER_A);

#ifdef	PB4_T1CCP0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	GPIOPinConfigure(GPIO_PB4_T1CCP0);
	GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_4);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
	TimerLoadSet(TIMER1_BASE, TIMER_A, DEFAULT_FREQ);
	TimerMatchSet(TIMER1_BASE, TIMER_A, DEFAULT_FREQ); // PWM
	TimerControlLevel(TIMER1_BASE, TIMER_A, false);
	TimerEnable(TIMER1_BASE, TIMER_A);
#endif

	SetPWM(PWM_LEFT,DEFAULT_FREQ, 0);
	SetPWM(PWM_RIGHT,DEFAULT_FREQ, 0);
}
void SetPWM(uint32_t ulBaseAddr, uint32_t ulTimer, uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = ROM_SysCtlClockGet() / ulFrequency;
	TimerLoadSet(ulBaseAddr, ulTimer, ulPeriod);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	TimerMatchSet(ulBaseAddr, ulTimer, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}
void speed_Enable_Hbridge(bool Enable)
{
	if (Enable)
	{
		GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT,
		        DRV_ENABLE_LEFT_CHN_PIN | DRV_ENABLE_RIGHT_CHN_PIN , DRV_ENABLE_LEFT_CHN_PIN |DRV_ENABLE_RIGHT_CHN_PIN);

	}
	else
	{
		GPIOPinWrite(DRV_ENABLE_LEFT_CHN_PORT, DRV_ENABLE_LEFT_CHN_PIN, 0);
		GPIOPinWrite(DRV_ENABLE_RIGHT_CHN_PORT, DRV_ENABLE_RIGHT_CHN_PIN, 0);
	}
}
void enableBoostCircuit( bool enable)
{
	if (enable)
		GPIOPinWrite(BOOST_ENABLE_PORT, BOOST_ENABLE_PIN, BOOST_ENABLE_PIN);
	else
	{
		GPIOPinWrite(BOOST_ENABLE_PORT, BOOST_ENABLE_PIN, 0);
	}
}


