#include "../include.h"
#include "inc/hw_gpio.h"
#include "driverlib/qei.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"

uint32_t PIDVerLoop = 0;

extern void Timer5ISR(void);			//Timer 5 - used for PID control

#ifndef USE_QEI
extern void EncoderISR(void);
#endif

void ConfigSystem(void)
{
	//Enable FPU
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	// Configure clock
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	//H-Bridges Control
	ROM_SysCtlPeripheralEnable(ENAPORT_PERIPHERAL);
	ROM_GPIOPinTypeGPIOOutput(ENABLE_PORT, ENABLE_PIN);
	ROM_GPIOPinWrite(ENABLE_PORT, ENABLE_PIN, 0);
}

void ConfigPIDTimer(uint32_t TimerIntervalms, uint32_t PIDVerlocityLoop)
{
	PIDVerLoop = PIDVerlocityLoop;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
	ROM_TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER5_BASE, TIMER_A, (ROM_SysCtlClockGet() / 1000) * TimerIntervalms);	//Interval: //1:150
	TimerIntRegister(TIMER5_BASE, TIMER_A, &Timer5ISR);
	ROM_IntEnable(INT_TIMER5A);
	ROM_TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerEnable(TIMER5_BASE, TIMER_A);
}

void ConfigPWM(void)
{
	//Configures the rate of the clock provided to the PWM module
	//= System Clock / 1
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	//Enable PWM0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	//Enable GPIO B
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	//Configures PB6, PB7 for use by the PWM peripheral.
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7 | GPIO_PIN_6);
	ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
	//Set the mode of operation for a PWM generator 0
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
	//Sets the period of the specified PWM generator block
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ROM_SysCtlClockGet() / DEFAULT);
	//Sets the pulse width for the specified PWM output
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
    		(ROM_SysCtlClockGet() / DEFAULT) / 2);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,
    		(ROM_SysCtlClockGet() / DEFAULT) / 2);
	//Enable specified PWM output
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
	//Select the inversion mode for the selected PWM outputs
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_0_BIT, false);
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_1_BIT, true);
	//Enable PWM generator 0
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

void SetPWM(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = ROM_SysCtlClockGet() / ulFrequency;
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ulPeriod - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void ConfigEncoder(void)
{
	//Unlock PD7 for GPIO functions
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~0x80;

#ifdef USE_QEI
    //Enable QEI0 Peripheral & GPIO D
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    //Configure specified pins for use by the QEI0 module
    ROM_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinConfigure(GPIO_PD6_PHA0);
    ROM_GPIOPinConfigure(GPIO_PD7_PHB0);
    //Configure QEI0 operation
    ROM_QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B|QEI_CONFIG_NO_RESET|QEI_CONFIG_QUADRATURE|QEI_CONFIG_SWAP, 0xFFFFFFFF);
    //Update Velocity every 10ms (ROM_ROM_SysCtlClockGet() / 100 = 10ms)
    ROM_QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_4, ROM_SysCtlClockGet() / 50);
    //Enable QEI0 module
    ROM_QEIVelocityEnable(QEI0_BASE);
    ROM_QEIEnable(QEI0_BASE);
#else
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_GPIODirModeSet (GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_DIR_MODE_IN);
	ROM_GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_FALLING_EDGE);
	ROM_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOIntRegister(GPIO_PORTD_BASE, &EncoderISR);
	IntEnable(INT_GPIOD);
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_6);
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6);
#endif
}
