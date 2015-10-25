/*
 * ConfigTimer.c
 *
 *  Created on: May 11, 2013
 *      Author: NHH
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "ConfigTimer.h"

//Define Timer register
const TimerReg TimerRegister[12] =
{
		{TIMER0, SYSCTL_PERIPH_TIMER0, TIMER0_BASE, INT_TIMER0A},
		{TIMER1, SYSCTL_PERIPH_TIMER1, TIMER1_BASE, INT_TIMER1A},
		{TIMER2, SYSCTL_PERIPH_TIMER2, TIMER2_BASE, INT_TIMER2A},
		{TIMER3, SYSCTL_PERIPH_TIMER3, TIMER3_BASE, INT_TIMER3A},
		{TIMER4, SYSCTL_PERIPH_TIMER4, TIMER4_BASE, INT_TIMER4A},
		{TIMER5, SYSCTL_PERIPH_TIMER5, TIMER5_BASE, INT_TIMER5A},
		{WIDE_TIMER0, SYSCTL_PERIPH_WTIMER0, WTIMER0_BASE, INT_WTIMER0A},
		{WIDE_TIMER1, SYSCTL_PERIPH_WTIMER1, WTIMER1_BASE, INT_WTIMER1A},
		{WIDE_TIMER2, SYSCTL_PERIPH_WTIMER2, WTIMER2_BASE, INT_WTIMER2A},
		{WIDE_TIMER3, SYSCTL_PERIPH_WTIMER3, WTIMER3_BASE, INT_WTIMER3A},
		{WIDE_TIMER4, SYSCTL_PERIPH_WTIMER4, WTIMER4_BASE, INT_WTIMER4A},
		{WIDE_TIMER5, SYSCTL_PERIPH_WTIMER5, WTIMER5_BASE, INT_WTIMER5A}
};

uint16_t SysTickCount = 0;

void (*SystemTickISR)(void);
uint16_t SysTickNumPeriod = 0;

unsigned char ConfigSysTick(uint16_t msInterval, void (*SysTickISR)(void))
{
	SystemTickISR = SysTickISR;
	if (msInterval > 10000)
		return (INTERVAL_TIME_TOO_HIGH);
	else if (msInterval == 0)
		return (INTERVAL_TIME_ZERO);
	while (SysCtlClockGet() / (1000 * (SysTickNumPeriod + 1))  * msInterval > 16777215)
		SysTickNumPeriod++;
	SysTickPeriodSet(SysCtlClockGet() / (1000 * (SysTickNumPeriod + 1))  * msInterval);
	SysTickIntRegister(&SysTickSubISR);
	SysTickIntEnable();
	SysTickEnable();
	return (1);
}

void SysTickSubISR(void)
{
	if (SysTickCount == SysTickNumPeriod)
	{
		SysTickCount = 0;
		(*SystemTickISR)();
		return;
	}
		SysTickCount++;
}

unsigned char ConfigTimerTick(TimerStruct *TimerInit)
{
	unsigned char temp = 0;
	uint64_t SetTimer;
	uint64_t SetTimer64;
	while ((TimerRegister[temp].TimerName != TimerInit->TimerName) && (temp < 12))
		temp++;
	if (temp == 12)
		return (WRONG_TIMER_NAME);
	SysCtlPeripheralEnable(TimerRegister[temp].TimerPeripheral);

	if (TimerInit->TimerType == ONE_FULL_WIDTH)		//Configure Timer A for full width
	{
		if (TimerInit->TimerAInterval == 0)
			return (INTERVAL_TIME_ZERO);

		TimerConfigure(TimerRegister[temp].TimerBase, TIMER_CFG_PERIODIC);

		if (TimerInit->TimerName > TIMER5)
		{
			SetTimer64 = SysCtlClockGet() / 1000 * TimerInit->TimerAInterval;
			TimerLoadSet64(TimerRegister[temp].TimerBase, SetTimer64);
		}
		else
		{
			SetTimer = SysCtlClockGet() / 1000 * TimerInit->TimerAInterval;
			if (SetTimer > (uint64_t)0xFFFFFFFF)
			{
				return (INTERVAL_TIME_TOO_HIGH);
			}
			TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_A, (uint32_t)SetTimer);
		}
		TimerIntRegister(TimerRegister[temp].TimerBase, TIMER_A, TimerInit->TimerAISR);
		TimerIntEnable(TimerRegister[temp].TimerBase, TIMER_TIMA_TIMEOUT);
		TimerEnable(TimerRegister[temp].TimerBase, TIMER_A);
	}
	else	//Two half-width Timers
	{
		if ((TimerInit->TimerAInterval == 0) || (TimerInit->TimerBInterval == 0))
			return (INTERVAL_TIME_ZERO);

		TimerConfigure(TimerRegister[temp].TimerBase, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);

		if (TimerInit->TimerName > TIMER5)	//Wide Timer: 32-bit/Timer
		{
			SetTimer = SysCtlClockGet() / 1000 * TimerInit->TimerAInterval;
			if (SetTimer > (uint64_t)0xFFFFFFFF)
			{
				return (INTERVAL_TIME_TOO_HIGH);
			}
			TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_A, (uint32_t)SetTimer);
			SetTimer = SysCtlClockGet() / 1000 * TimerInit->TimerBInterval;
			if (SetTimer > (uint64_t)0xFFFFFFFF)
			{
				return (INTERVAL_TIME_TOO_HIGH);
			}
			TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_B, (uint32_t)SetTimer);
		}
		else	//16-bit/Timer
		{
			SetTimer = SysCtlClockGet() / 1000 * TimerInit->TimerAInterval;
			if (SetTimer > (uint64_t)0xFFFF)
			{
				return (INTERVAL_TIME_TOO_HIGH);
			}
			TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_A, (uint32_t)SetTimer);
			SetTimer = SysCtlClockGet() / 1000 * TimerInit->TimerBInterval;
			if (SetTimer > (uint64_t)0xFFFF)
			{
				return (INTERVAL_TIME_TOO_HIGH);
			}
			TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_B, (uint32_t)SetTimer);
		}
		//TimerPrescaleSet(TimerRegister[temp].TimerBase, TIMER_A, 10);
		TimerIntRegister(TimerRegister[temp].TimerBase, TIMER_A, TimerInit->TimerAISR);
		TimerIntRegister(TimerRegister[temp].TimerBase, TIMER_B, TimerInit->TimerBISR);
		TimerIntEnable(TimerRegister[temp].TimerBase, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
		TimerEnable(TimerRegister[temp].TimerBase, TIMER_BOTH);
	}
	return (1);
}

unsigned char TimerTickDisable(unsigned char TimerName)
{
	unsigned char temp = 0;
	while ((TimerRegister[temp].TimerName != TimerName) && (temp < 12))
		temp++;
	if (temp == 12)
		return (WRONG_TIMER_NAME);
	TimerDisable(TimerRegister[temp].TimerBase, TIMER_BOTH);
	return (1);
}

uint32_t GetPortBase (uint32_t Arg)
{
	switch (Arg)
	{
		case 0:
			return (GPIO_PORTA_BASE);
		case 1:
			return (GPIO_PORTB_BASE);
		case 2:
			return (GPIO_PORTC_BASE);
		case 3:
			return (GPIO_PORTD_BASE);
		case 4:
			return (GPIO_PORTE_BASE);
		case 5:
			return (GPIO_PORTF_BASE);
		default:
			return (0);
	}
}

unsigned char ConfigTimerPWM(unsigned char TimerName, uint32_t Pin1Name, uint32_t Pin2Name)
{
	uint32_t ulBase, ulPin, ulPortBase;
	uint8_t temp = 0;

	while ((TimerRegister[temp].TimerName != TimerName) && (temp < 12))
		temp++;
	if (temp == 12)
		return (WRONG_TIMER_NAME);

	//ulBase = HWREG(TimerRegister[temp].TimerBase + TIMER_O_CTL);
	//if (ulBase & (TIMER_CTL_TAEN | TIMER_CTL_TBEN))
		//return (TIMER_IS_RUNNING);

	// Configure timer
	SysCtlPeripheralEnable(TimerRegister[temp].TimerPeripheral);

	if (Pin1Name != 0)
	{
		ulBase = (Pin1Name >> 16) & 0xff;
		ulPortBase = GetPortBase (ulBase);
		ulPin = (Pin1Name >> 10) & 0x0f;
		if (!ulPortBase)
			return (WRONG_PIN_NAME);
		SysCtlPeripheralEnable(0xf0000800 + ulBase);
		GPIOPinConfigure(Pin1Name);
		GPIOPinTypeTimer(ulPortBase, (uint32_t)0x01 << ulPin);
	}
	if (Pin2Name != 0)
	{
		ulBase = (Pin2Name >> 16) & 0xff;
		ulPortBase = GetPortBase (ulBase);
		ulPin = (Pin2Name >> 10) & 0x0f;
		if (!ulPortBase)
			return (WRONG_PIN_NAME);
		SysCtlPeripheralEnable(0xf0000800 + ulBase);
		GPIOPinConfigure(Pin2Name);
		GPIOPinTypeTimer(ulPortBase, (uint32_t)0x01 << ulPin);
	}

	TimerIntUnregister(TimerRegister[temp].TimerBase, TIMER_BOTH);
	TimerConfigure(TimerRegister[temp].TimerBase, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM | TIMER_CFG_A_PWM);
	TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_A, SysCtlClockGet() / 1000);
	//Default PWM frequency: 1kHz
	TimerMatchSet(TimerRegister[temp].TimerBase, TIMER_A, TimerLoadGet(TimerRegister[temp].TimerBase, TIMER_A) / 2);
	//Default Duty cycle: 50%
	TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_B, SysCtlClockGet() / 1000);
	//Default PWM frequency: 1kHz
	TimerMatchSet(TimerRegister[temp].TimerBase, TIMER_B, TimerLoadGet(TimerRegister[temp].TimerBase, TIMER_B) / 2);
	//Default Duty cycle: 50%
	TimerEnable(TimerRegister[temp].TimerBase, TIMER_BOTH);
	return (1);
}

unsigned char PWMSet(unsigned char TimerName, uint32_t PinName, unsigned int Freq, unsigned char DutyCycle)
{
	uint32_t ulPin;
	uint8_t temp = 0;

	ulPin = (PinName >> 10) & 0x0f;

	while ((TimerRegister[temp].TimerName != TimerName) && (temp < 12))
		temp++;
	if (temp == 12)
		return (WRONG_TIMER_NAME);

	if ((SysCtlClockGet() / Freq) < 100)
	{
		return (FREQUENCY_TOO_HIGH);
	}

	if (temp < 6)	//32-bit Timer -> PWM 16-bit
	{
		if ((SysCtlClockGet() / Freq) > 0xFFFF)
		{
			return (FREQUENCY_TOO_LOW);
		}
	}
	else			//64-bit Timer -> PWM 32-bit
	{
	}

	if (DutyCycle > 100)
	{
		return (WRONG_DUTY_CYCLE);
	}
	if (ulPin & 0x01)	//CCP1 -> TimerB
	{
		TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_B, SysCtlClockGet() / Freq - 1);
		TimerMatchSet(TimerRegister[temp].TimerBase, TIMER_B, TimerLoadGet(TimerRegister[temp].TimerBase, TIMER_B) * (100 - DutyCycle) / 100 - 1);
	}
	else	//CCP0 -> TimerA
	{
		TimerLoadSet(TimerRegister[temp].TimerBase, TIMER_A, SysCtlClockGet() / Freq - 1);
		TimerMatchSet(TimerRegister[temp].TimerBase, TIMER_A, TimerLoadGet(TimerRegister[temp].TimerBase, TIMER_A) * (100 - DutyCycle) / 100 - 1);
	}
	return (1);
}
