#include "../include.h"
#include "driverlib/pwm.h"
#include "driverlib/qei.h"
#include "driverlib/rom.h"

extern PIDType PIDVelocity, PIDPosition;
extern uint32_t PIDVerLoop;

int32_t Position = 0;
int32_t Speed = 0, Speedtemp = 0;
tCmdLineEntry CommandTable[1];

int32_t set = 0;
uint8_t *p_UARTBuf, UARTBuf[50];
UARTType UART_Bluetooth;
int16_t MaxSpeed = 100;

void HBridgeEnable(void)
{
	GPIOPinWrite(ENABLE_PORT, ENABLE_PIN, 0xFF);
}

void HBridgeDisable(void)
{
	GPIOPinWrite(ENABLE_PORT, ENABLE_PIN, 0);
}

void Timer5ISR(void)
{
	static uint8_t NumSpdSet = 0;
	ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
#ifdef USE_QEI
	//Get Velocity
	Speed = ROM_QEIVelocityGet(QEI0_BASE) * ROM_QEIDirectionGet(QEI0_BASE);
	//Get Position
	Position = ((int32_t)ROM_QEIPositionGet(QEI0_BASE)) / 2;
#else
	Speed = Speedtemp;
	Speedtemp = 0;
#endif

	if (PIDVelocity.Enable)
	{
#ifdef PID_SPEED
	UARTPutn(UART_Bluetooth.PortName, (int32_t)Speed);
	UARTCharPut(UART_Bluetooth.PortName, '\n');
#endif
		PIDVerCalc(Speed, 90);
		SetPWM(DEFAULT, (long)PIDVelocity.PIDResult);
	}

	NumSpdSet++;

	if (NumSpdSet == PIDVerLoop)	//PID position
	{
		NumSpdSet = 0;
		if (PIDPosition.Enable)
		{
#ifdef PID_POSITION
	UARTPutn(UART_Bluetooth.PortName, (int32_t)Position);
	UARTCharPut(UART_Bluetooth.PortName, '\n');
#endif
			PIDPosCalc(Position, MaxSpeed);
			PIDSpeedSet((long)PIDPosition.PIDResult);
		}
	}
}


void BluetoothIntHandler(void)
{
	uint32_t ulStatus;
//	static uint8_t count = 0;

	ulStatus = UARTIntStatus(UART_Bluetooth.PortName, true);
	ROM_UARTIntClear(UART_Bluetooth.PortName, ulStatus);
	while(UARTCharsAvail(UART_Bluetooth.PortName))
	{
#ifdef SET_PID
//		ControlFlag = 1;
		if (p_UARTBuf < &UARTBuf[50])
			*p_UARTBuf++ = (uint8_t)(UARTCharGetNonBlocking(UART_Bluetooth.PortName));
		if (p_UARTBuf[-1] == '\n')
		{
			p_UARTBuf = &UARTBuf[0];
			set = 0;
			count = 0;
#ifdef PID_SPEED
			PIDVelocity.Enable = 1;
#endif
#ifdef PID_POSITION
			PIDPosition.Enable = 1;
#endif

		}
		else if (p_UARTBuf[-1] == '\r')
		{
			switch (count)
			{
#ifdef PID_SPEED
				case 0:
					MaxSpeed = set / 10000;
					PIDVelocity.SetPoint = MaxSpeed;
					break;
				case 1:
					PIDVelocity.Kp = (float)set / 10000;
					break;
				case 2:
					PIDVelocity.Ki = (float)set / 10000;
					break;
				case 3:
					PIDVelocity.Kd = (float)set / 10000;
					break;
#endif
#ifdef PID_POSITION
				case 0:
					PIDPosition.SetPoint = set / 10000;
					break;
				case 1:
					PIDPosition.Kp = (float)set / 10000;
					break;
				case 2:
					PIDPosition.Ki = (float)set / 10000;
					break;
				case 3:
					PIDPosition.Kd = (float)set / 10000;
					break;
#endif
			}

			set = 0;
			count++;
		}
		else
		{
			set = set * 10 + (p_UARTBuf[-1] - 48);
		}
#else
		return;
#endif
	}
}

#ifndef USE_QEI
void EncoderISR(void)
{
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6);
	if (HWREG(GPIO_PORTD_BASE + 0x0200))
	{
		Speedtemp--;
		Position--;
	}
	else
	{
		Speedtemp++;
		Position++;
	}
}
#endif
