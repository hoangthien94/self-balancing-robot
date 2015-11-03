#ifndef _LED_H_
#define _LED_H_

#define     PORT(x)                  GPIO_PORT##x##_BASE
#define     PORTF                    PORT(F)

#define     LED_PORT                 PORTF
#define     LED_RED_PIN              GPIO_PIN_1
#define     LED_BLUE_PIN             GPIO_PIN_2
#define     LED_GREEN_PIN            GPIO_PIN_3

#define     LedInit()                ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);\
                                     ROM_GPIOPinTypeGPIOOutput(LED_PORT, LED_RED_PIN | LED_BLUE_PIN |  LED_GREEN_PIN)

#define     LED_ON(x)                ROM_GPIOPinWrite(LED_PORT, x, x)
#define     LED_OFF(x)               ROM_GPIOPinWrite(LED_PORT, x, ~x)
#define     LED_TOGGLE(x)            ROM_GPIOPinWrite(LED_PORT, x, ~ROM_GPIOPinRead(LED_PORT, x ))

#endif
