#include "include.h"


void main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	UARTType newUART0 = { UART0_BASE, 115200, 8, None, 1,&UART0_Handler};
	ConfigUART(&newUART0);
	ledInit();
	buttonInit();
	qei_init(Ts*1000);
	timerInit(SysCtlClockGet()/3*Ts);
	Config_PWM();
	ConfigDRV_Enable();
	ConfigBoostCircuit();
	speed_Enable_Hbridge(true);
	enableBoostCircuit(true);
    PIDPositionSet(0);
	while(1)
	{
		Balacing_Process();
		ProcessSpeedControl();
	}
}

