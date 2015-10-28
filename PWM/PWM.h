#ifndef	_PWM_H_
#define	_PWM_H_

#include <stdint.h>
#include <stdbool.h>

#define	DEFAULT_FREQ					20000

#define PWM_1			TIMER1_BASE, TIMER_A	//PB4
#define PWM_2			TIMER0_BASE, TIMER_A	//PB6

#define DRV_ENABLE_LEFT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_RIGHT_CHN_PERIPHERAL		SYSCTL_PERIPH_GPIOB
#define DRV_ENABLE_LEFT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_RIGHT_CHN_PORT			GPIO_PORTB_BASE
#define DRV_ENABLE_LEFT_CHN_PIN				GPIO_PIN_5
#define DRV_ENABLE_RIGHT_CHN_PIN			GPIO_PIN_7

#define BOOST_ENABLE_PREIPHERAL				SYSCTL_PERIPH_GPIOB
#define BOOST_ENABLE_PORT					GPIO_PORTB_BASE
#define BOOST_ENABLE_PIN					GPIO_PIN_2

extern void ConfigBoostCircuit();		//TODO: enable boost circuit to run
extern void Config_PWM(void);
extern void ConfigDRV_Enable();
extern void SetPWM(uint32_t ulBaseAddr, uint32_t ulTimer, uint32_t ulFrequency, int32_t ucDutyCycle);
extern void enableBoostCircuit( bool enable);
extern void speed_Enable_Hbridge(bool Enable);

#endif
