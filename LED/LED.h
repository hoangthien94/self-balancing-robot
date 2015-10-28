#ifndef _LED_H_
#define _LED_H_



#define	LED_PORT		GPIO_PORTF_BASE
#define	LED_1_PIN		GPIO_PIN_1
#define	LED_2_PIN		GPIO_PIN_2
#define	LED_3_PIN		GPIO_PIN_3

#define LED1_ON()	GPIOPinWrite(LED_PORT,LED_1_PIN,LED_1_PIN )
#define LED2_ON()	GPIOPinWrite(LED_PORT,LED_2_PIN,LED_2_PIN )
#define LED3_ON()	GPIOPinWrite(LED_PORT,LED_3_PIN,LED_3_PIN )

#define LED1_SWITCH()	GPIOPinWrite(LED_PORT,LED_1_PIN, ~GPIOPinRead(LED_PORT,LED_1_PIN))
#define LED2_SWITCH()	GPIOPinWrite(LED_PORT,LED_2_PIN, ~GPIOPinRead(LED_PORT,LED_2_PIN))
#define LED3_SWITCH()	GPIOPinWrite(LED_PORT,LED_3_PIN, ~GPIOPinRead(LED_PORT,LED_3_PIN))



extern void ledInit();

#endif
