#include "include.h"

extern PIDType PIDVelocity, PIDPosition;
extern uint8_t SlaveID;
extern int32_t Velocity;
extern UARTType UART_Bluetooth;
extern uint8_t *p_UARTBuf, UARTBuf[];

void InitPID(void)
{
	PIDVelocity.Kp = 0.02;
	PIDVelocity.Kd = 0.02;

	PIDPosition.Kp = 0.035;//350/10000 ko tai
	PIDPosition.Kd = 0;

	PIDVelocity.Enable = 0;
	PIDPosition.Enable = 0;
}

void ConfigNetwork(void)
{
	p_UARTBuf = &UARTBuf[0];

	UART_Bluetooth.PortName = UART7;
	UART_Bluetooth.BaudRate = 115200;
	UART_Bluetooth.DataBits = 8;
	UART_Bluetooth.Parity = None;
	UART_Bluetooth.StopBits = 1;
	UART_Bluetooth.ISR = &BluetoothIntHandler;
	ConfigUART(&UART_Bluetooth);
}

void main(void)
{
	ConfigSystem();
//	ConfigNetwork();
	ConfigEncoder();
	ConfigPWM();
	ConfigPIDTimer(20, 2);
	IntMasterEnable();
	InitPID();
	SetPWM(DEFAULT, 30);
	HBridgeEnable();
	
//	PIDSpeedSet(10);
//	Clock = SysCtlClockGet();
	while(1)
	{
//		SysCtlDelay(SysCtlClockGet() / 3);
//		Velocity = ((int32_t)ROM_QEIVelocityGet(QEI0_BASE)) / 2;
	}
}
