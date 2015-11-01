#include "include.h"
#include "logo.h"

extern PIDType PIDVelocity, PIDPosition;
extern uint8_t SlaveID;
extern int32_t Velocity;
extern UARTType UART_Bluetooth;
extern uint8_t *p_UARTBuf, UARTBuf[];

void ButtonISR(void);
void LCD_Start();

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
	Config_LCD();
	Config_Button1();
	lcd_init();
	lcd_clear();

	LCD_Start();

	ConfigNetwork();
	ConfigEncoder();
	ConfigPWM();
	ConfigPIDTimer(20, 2);
	IntMasterEnable();
	InitPID();
	SetPWM(DEFAULT, 30);
	HBridgeEnable();
	PIDSpeedSet(10);
//	Clock = SysCtlClockGet();
	while(1)
	{
//		SysCtlDelay(SysCtlClockGet() / 3);
//		Velocity = ((int32_t)ROM_QEIVelocityGet(QEI0_BASE)) / 2;
	}
}
//==============================================================================================================//BTN1_ISR
void ButtonISR(void)
{
		lcd_clear();
		static int tick = 0;
		char c=0x30;
		c+=tick;
		GPIOIntClear(GPIO_PORTF_BASE, SW1);
		lcd_gotoxy(8,4);
		lcd_putc(c);
		tick+=1;
		if(tick>9)
			tick=0;
}
void LCD_Start()
{
	lcd_clear();
	lcd_gotoxy(8,150);
	lcd_puts("---loading---");
	SysCtlDelay(SysCtlClockGet() / 5);
	lcd_gotoxy(8,150);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_puts(".");
	SysCtlDelay(SysCtlClockGet() / 15);
	lcd_clear();
	int i;
		for( i = 0; i < 816; i++)
		{
		lcd_write(FLAG_DATA, teamlogo2[i]);
		}
	SysCtlDelay(SysCtlClockGet() / 3);
	lcd_clear();
	lcd_puts(".");
}
