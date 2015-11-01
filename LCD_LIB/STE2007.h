#ifndef _STE2007_H_
#define _STE2007_H_
#include "stdint.h"
#include "stdbool.h"
//*****************************************************************************

//*****************************************************************************
//*****************************************************************************
// USER CONFIGURATION SECTION
//*****************************************************************************
// The following parameters need to be configured by the user.

#define PIN_CONFIG_ARM					// For using ARM M4, define PIN_CONFIG_ARM
//#define PIN_CONFIG_MSP430  					// For using MSP430, define PIN_CONFIG_MSP430

#define LCD_CONTRAST 		(0x82)			// Value range should be 0x80 -> 0x9F

//*****************************************************************************
// Define some basic commands.
//*****************************************************************************
// To change the hardware connection, simply replace the correspondent Port
// and Bit
#ifdef PIN_CONFIG_MSP430					// Pin configuration for msp430

#define LCD_PIN_TYPE_OUTPUT	P1DIR |= BIT0 + BIT1 + BIT2 + BIT3

#define LCD_CS_LOW		P1OUT &= ~BIT1
#define LCD_CS_HIGH		P1OUT |= BIT1

#define LCD_RESET_LOW		P1OUT &= ~BIT0
#define LCD_RESET_HIGH		P1OUT |= BIT0

#define LCD_SDA_HIGH		P1OUT |= BIT2
#define LCD_SDA_LOW		P1OUT &= ~BIT2

#define LCD_CLK_HIGH		P1OUT |= BIT3
#define LCD_CLK_LOW		P1OUT &= ~BIT3
#endif

//*****************************************************************************
#ifdef PIN_CONFIG_ARM					// Pin configuration for ARM M4


//LCD define
//*****************************************************************************
#define LCD_RESET_PIN				GPIO_PIN_2
#define LCD_RESET_PORT				GPIO_PORTA_BASE

#define LCD_RESET_CONFIG_OUTPUT   	GPIOPinTypeGPIOOutput(LCD_RESET_PORT,LCD_RESET_PIN)
#define LCD_RESET_LOW				GPIOPinWrite( LCD_RESET_PORT,  LCD_RESET_PIN, 0x00)
#define LCD_RESET_HIGH				GPIOPinWrite( LCD_RESET_PORT,  LCD_RESET_PIN, 0xFF)
//*****************************************************************************
#define LCD_CS_PIN					GPIO_PIN_3
#define LCD_CS_PORT					GPIO_PORTA_BASE

#define LCD_CS_CONFIG_OUTPUT   		GPIOPinTypeGPIOOutput(LCD_CS_PORT,LCD_CS_PIN)
#define LCD_CS_LOW					GPIOPinWrite(LCD_CS_PORT, GPIO_PIN_1, 0x00)
#define LCD_CS_HIGH					GPIOPinWrite(LCD_CS_PORT, GPIO_PIN_1, 0xFF)
//*****************************************************************************
#define LCD_SDA_PIN					GPIO_PIN_4
#define LCD_SDA_PORT				GPIO_PORTA_BASE

#define LCD_SDA_CONFIG_OUTPUT   	GPIOPinTypeGPIOOutput(LCD_SDA_PORT,LCD_SDA_PIN)
#define LCD_SDA_HIGH				GPIOPinWrite(LCD_SDA_PORT, LCD_SDA_PIN, 0xFF)
#define LCD_SDA_LOW					GPIOPinWrite(LCD_SDA_PORT, LCD_SDA_PIN, 0x00)
//*****************************************************************************
#define LCD_CLK_PIN					GPIO_PIN_5
#define LCD_CLK_PORT				GPIO_PORTA_BASE

#define LCD_CLK_CONFIG_OUTPUT   	GPIOPinTypeGPIOOutput(LCD_CLK_PORT,LCD_CLK_PIN)
#define LCD_CLK_HIGH				GPIOPinWrite(LCD_CLK_PORT, LCD_CLK_PIN, 0xFF)
#define LCD_CLK_LOW					GPIOPinWrite(LCD_CLK_PORT, LCD_CLK_PIN, 0x00)
//*****************************************************************************
#define LCD_PIN_TYPE_OUTPUT			{												\
										LCD_RESET_CONFIG_OUTPUT;					\
										LCD_CS_CONFIG_OUTPUT;						\
										LCD_SDA_CONFIG_OUTPUT;						\
										LCD_CLK_CONFIG_OUTPUT;						\
									}
#endif


//******************************************************************************
// END OF USER CONFIGURATION SECTION
//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Public function header
//******************************************************************************
void lcd_write(char dc, char byte);
void lcd_init(void);
void lcd_clear(void);
void lcd_gotoxy(unsigned char col, unsigned char row);
void lcd_normal(void);
void lcd_all(void);
void lcd_putc(char c);
void lcd_puts(char *s);
void LCDprintf(const char *pcString, ...);
void LCDDataWrite(const char *pcBuf, uint32_t ulLen);
void LCD_loadingScreen(void);
//*****************************************************************************
// COMMAND LIST
//*****************************************************************************
#define LCD_RESET_CYCLES		(CPU_CLOCK_MHZ*200)

#define FLAG_CMD				(0x00)
#define FLAG_DATA				(0x01)

// Display mode
#define ON						(0x0F)			// Display on
#define OFF						(0x0E)			// Display off
#define ALL						(0x05)			// All points ON
#define NORMAL					(0x04)			// Normal display
#define NON_REVERSE				(0x06)			// Non-reverse display
#define REVERSE					(0x07)			// Reverse display

#define LCD_MODE 				(0xA0)			// Display mode command identifier

#define LCD_NOP 				(0xE3)
#define LCD_CHARGE_PUMP_ON		(0x2F)
#define LCD_RAM_ADDR_VER_MODE	(0xAB)
#define LCD_RAM_ADDR_HOR_MODE	(0xAA)
#define LCD_CHANGE_COL_LSB 		(0x00)
#define LCD_CHANGE_COL_MSB		(0x10)
#define LCD_CHANGE_ROW 			(0xB0)
#define LCD_MIRROR_Y			(0xC8)
#define LCD_MIRROR_X 			(0xA1)
#define LCD_EXT_OSC 			(0x3A)
#define LCD_SOFT_RESET 			(0xE2)

//alignment
#define CENTER	1
#define LEFT	2
#define RIGHT	3

#endif /* _STE2007_H_ */
