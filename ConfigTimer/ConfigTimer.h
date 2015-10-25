/*
 * ConfigTimer.h
 *
 *  Created on: May 11, 2013
 *      Author: NHH
 */

#ifndef CONFIGTIMER_H_
#define CONFIGTIMER_H_

//define Timer Name
#define TIMER0			0x01
#define TIMER1			0x02
#define TIMER2			0x03
#define TIMER3			0x04
#define TIMER4			0x05
#define TIMER5			0x06
#define WIDE_TIMER0		0x07
#define WIDE_TIMER1		0x08
#define WIDE_TIMER2		0x09
#define WIDE_TIMER3		0x0a
#define WIDE_TIMER4		0x0b
#define WIDE_TIMER5		0x0c
//define Timer Type
#define ONE_FULL_WIDTH	0x01
#define TWO_HALF_WIDTH	0x02
//define Timer Config Error
#define INTERVAL_TIME_ZERO			0x08
#define INTERVAL_TIME_TOO_HIGH		0x04
#define WRONG_TIMER_NAME			0x02
#define WRONG_PIN_NAME				0x04
#define FREQUENCY_TOO_HIGH			0x08
#define FREQUENCY_TOO_LOW			0x10
#define WRONG_DUTY_CYCLE			0x20
#define TIMER_IS_RUNNING			0x40
//define Pin Name for PWM
#define PB6		GPIO_PB6_T0CCP0
#define PF0		GPIO_PF0_T0CCP0
#define PB7		GPIO_PB7_T0CCP1
#define PF1		GPIO_PF1_T0CCP1
#define PF2		GPIO_PF2_T1CCP0
#define PB4		GPIO_PB4_T1CCP0
#define PF3		GPIO_PF3_T1CCP1
#define PB5		GPIO_PB5_T1CCP1
#define PF4		GPIO_PF4_T2CCP0
#define PB0		GPIO_PB0_T2CCP0
#define PB1		GPIO_PB1_T2CCP1
#define PB2		GPIO_PB2_T3CCP0
#define PB3		GPIO_PB3_T3CCP1
#define PC0		GPIO_PC0_T4CCP0
#define PC1		GPIO_PC1_T4CCP1
#define PC2		GPIO_PC2_T5CCP0
#define PC3		GPIO_PC3_T5CCP1
#define PC4		GPIO_PC4_WT0CCP0
#define PC5		GPIO_PC5_WT0CCP1
#define PC6		GPIO_PC6_WT1CCP0
#define PC7		GPIO_PC7_WT1CCP1
#define PD0		GPIO_PD0_WT2CCP0
#define PD1		GPIO_PD1_WT2CCP1
#define PD2		GPIO_PD2_WT2CCP0
#define PD3		GPIO_PD3_WT3CCP1
#define PD4		GPIO_PD4_WT4CCP0
#define PD5		GPIO_PD5_WT4CCP1
#define PD6		GPIO_PD6_WT5CCP0
#define PD7		GPIO_PD7_WT5CCP1

typedef struct
{
		uint8_t 	TimerName;
		uint32_t	TimerPeripheral;
		uint32_t	TimerBase;
		uint8_t		InterruptAssign;
} TimerReg;

typedef struct
{
		uint8_t 	TimerName;
		uint8_t 	TimerType;
		uint16_t	TimerAInterval;
		uint16_t	TimerBInterval;
		void 		(*TimerAISR)(void);
		void 		(*TimerBISR)(void);
} TimerStruct;
/*
typedef struct
{
		unsigned char 	TimerName;
		unsigned char 	TimerType;
		unsigned char 	CCP0Pin;
		unsigned char 	CCP1Pin;
};
*/
unsigned char ConfigSysTick(uint16_t msInterval, void (*SysTickISR)(void));
void SysTickSubISR(void);
unsigned char ConfigTimerTick(TimerStruct *TimerInit);
unsigned char TimerTickDisable(unsigned char TimerName);
uint32_t GetPortBase (uint32_t Arg);
unsigned char ConfigTimerPWM(unsigned char TimerName, uint32_t Pin1Name, uint32_t Pin2Name);
unsigned char PWMSet(unsigned char TimerName, uint32_t PinName, unsigned int Freq, unsigned char DutyCycle);

#endif /* CONFIGTIMER_H_ */
