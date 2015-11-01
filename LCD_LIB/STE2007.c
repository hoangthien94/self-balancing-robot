//*****************************************************************************
// STE2007.c - This file provide some basic interface with STE2007 GLCD driver
// for MSP430 and ARM M4F from Texas Instrument. Software SPI is used instead
// of hardware SPI, so you can use this code for any microcontroller with just
// only a few adjustments.
//
// Copyright (c) Ho Tuan Vu, aka HTV, golden.spirit92@gmail.com
//
// To use these function, you must copy this file to your project and include
// the header file STE2007.h in the main.c file
// You must also configure some parameters in the USER CONFIGURATION SECTION
// in the STE2007.h file.
//
// This project contains ABSOLUTELY NO WARRANTY, meaning it does not even have
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  If there's a bug in this code that smokes your board, sorry, but
// you're on your own.
//*****************************************************************************
// IMPORT
//*****************************************************************************

#include "STE2007.h"
#include "ASCII.h"

#ifdef	PIN_CONFIG_MSP430
#include <msp430.h>
#endif

#ifdef	PIN_CONFIG_ARM
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "stdint.h"
#include "stdbool.h"
#endif
static const char * const g_pcHex = "0123456789abcdef";
//*****************************************************************************
// ASCII table
//*****************************************************************************

//*****************************************************************************
// PUBLIC FUNCTION:
// lcd_init(void)					: Initialize LCD
// lcd_write(char dc, char byte)			: Send data or command to LCD
//								dc = FLAG_CMD  : command
//								dc = FLAG_DATA : data
// lcd_clear(void)					: Clear all pixels on LCD
// lcd_gotoxy(unsigned char col, unsigned char row)	: Set pointer to coordinate(column, row)
// lcd_normal(void)					: Set LCD to normal state (display on, invert off,...)
// lcd_all(void)					: Display all pixels
// lcd_putc(char c)					: Display ASCII character on LCD
// lcd_puts(char *s)				        : Display character string on LCD
//*****************************************************************************
void lcd_write(char dc, char byte){
	char i;
	if(dc) LCD_SDA_HIGH;
	else LCD_SDA_LOW;
	LCD_CS_LOW;
	LCD_CLK_HIGH;
	LCD_CLK_LOW;

	// shift data out
	for(i = 0; i < 8; i++){
		if((byte<<i) & 0x80) LCD_SDA_HIGH;
		else LCD_SDA_LOW;
		LCD_CLK_HIGH;
		LCD_CLK_LOW;
	}
	LCD_CS_HIGH;
}

//******************************************************************************
void lcd_init(void){
	LCD_PIN_TYPE_OUTPUT;
	LCD_RESET_LOW;
	LCD_CS_HIGH;
	LCD_CLK_LOW;
	LCD_RESET_HIGH;

	SysCtlDelay(SysCtlClockGet()/3);

	lcd_write(FLAG_CMD, 0x20);						// Set voltage range
	lcd_write(FLAG_CMD, LCD_CONTRAST);

	lcd_write(FLAG_CMD, LCD_MODE | NORMAL);
	lcd_write(FLAG_CMD, LCD_CHARGE_PUMP_ON);		// Enable charge pump
	lcd_write(FLAG_CMD, LCD_MODE | ON);			// Display on
	lcd_write(FLAG_CMD, 0X9F);

	lcd_clear();
}

//*****************************************************************************
void lcd_clear(void){
	unsigned int index;
	lcd_gotoxy(0,0);
	for(index=0; index < 864; index++){
		lcd_write(FLAG_DATA,0x00);
	}
	lcd_gotoxy(0,0);
}

//*****************************************************************************
void lcd_gotoxy(unsigned char col, unsigned char row)
{
	lcd_write(FLAG_CMD,LCD_CHANGE_COL_LSB | ( col & 0x0F));
	lcd_write(FLAG_CMD,LCD_CHANGE_COL_MSB | ((col >> 4) & 0x07 ));
	lcd_write(FLAG_CMD,LCD_CHANGE_ROW | (row & 0x0F));
}

//*****************************************************************************
void lcd_normal(void){
	lcd_write(FLAG_CMD,LCD_MODE|ON);
  	lcd_write(FLAG_CMD,LCD_MODE|NON_REVERSE);
  	lcd_write(FLAG_CMD,LCD_MODE|NORMAL);
  	lcd_write(FLAG_CMD,LCD_RAM_ADDR_VER_MODE);
}

//*****************************************************************************
void lcd_all(void){
	lcd_write(FLAG_CMD,LCD_MODE|ON);
	lcd_write(FLAG_CMD,LCD_MODE|NON_REVERSE);
	lcd_write(FLAG_CMD,LCD_MODE|ALL);
}
//******************************************************************************
void lcd_mirror_x(void){
	lcd_write(FLAG_CMD,LCD_MIRROR_X);
}
//******************************************************************************
void lcd_mirror_y(void){
	lcd_write(FLAG_CMD,LCD_MIRROR_Y);
}
//******************************************************************************
void lcd_putc(char c)
{
	if((c >= 0x20) && (c <= 0x7f))
	{
		char i;
		c -= 0x20;
		for(i = 0; i < 6; i++)
		{
			lcd_write(FLAG_DATA, ASCII[c][i]);
		}
	}
}

//******************************************************************************
void lcd_puts(char *s)
{
	while(*s)
	{
		lcd_putc(*s++);
	}
}
//******************************************************************************
void LCDDataWrite(const char *pcBuf, uint32_t ulLen)
{
	uint32_t i;
	for(i = 0; i < ulLen; i++)
	{
		lcd_putc(*pcBuf);
		pcBuf++;
	}
}

//! A simple LCD based printf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! format string.
//!
//! This function is very similar to the C library <tt>fprintf()</tt> function.
//! All of its output will be sent to the LCD.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d or \%i to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments after \e pcString must match the requirements of
//! the format string.  For example, if an integer was passed where a string
//! was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
void LCDprintf(const char *pcString, ...)
{
	static unsigned int ulPos, ulCount, ulNeg;
	unsigned long ulValue, ulBase, ulIdx;
	char *pcStr, pcBuf[16], cFill;
	va_list vaArgP;

	//
	// Start the varargs processing.
	//
	va_start(vaArgP, pcString);

	//
	// Loop while there are more characters in the string.
	//
	while(*pcString)
	{
		//
		// Find the first non-% character, or the end of the string.
		//
		for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
				ulIdx++)
		{
		}

		//
		// Write this portion of the string.
		//
		LCDDataWrite(pcString, ulIdx);

		//
		// Skip the portion of the string that was written.
		//
		pcString += ulIdx;

		//
		// See if the next character is a %.
		//
		if(*pcString == '%')
		{
			//
			// Skip the %.
			//
			pcString++;

			//
			// Set the digit count to zero, and the fill character to space
			// (i.e. to the defaults).
			//
			ulCount = 0;
			cFill = ' ';

			//
			// It may be necessary to get back here to process more characters.
			// Goto's aren't pretty, but effective.  I feel extremely dirty for
			// using not one but two of the beasts.
			//
			again:

			//
			// Determine how to handle the next character.
			//
			switch(*pcString++)
			{
			//
			// Handle the digit characters.
			//
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				//
				// If this is a zero, and it is the first digit, then the
				// fill character is a zero instead of a space.
				//
				if((pcString[-1] == '0') && (ulCount == 0))
				{
					cFill = '0';
				}

				//
				// Update the digit count.
				//
				ulCount *= 10;
				ulCount += pcString[-1] - '0';

				//
				// Get the next character.
				//
				goto again;
			}

			//
			// Handle the %c command.
			//
			case 'c':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Print out the character.
				//
				LCDDataWrite((char *)&ulValue, 1);

				//
				// This command has been handled.
				//
				break;
			}

			//
			// Handle the %d and %i commands.
			//
			case 'd':
			case 'i':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Reset the buffer position.
				//
				ulPos = 0;

				//
				// If the value is negative, make it positive and indicate
				// that a minus sign is needed.
				//
				if((long)ulValue < 0)
				{
					//
					// Make the value positive.
					//
					ulValue = -(long)ulValue;

					//
					// Indicate that the value is negative.
					//
					ulNeg = 1;
				}
				else
				{
					//
					// Indicate that the value is positive so that a minus
					// sign isn't inserted.
					//
					ulNeg = 0;
				}

				//
				// Set the base to 10.
				//
				ulBase = 10;

				//
				// Convert the value to ASCII.
				//
				goto convert;
			}

			//
			// Handle the %s command.
			//
			case 's':
			{
				//
				// Get the string pointer from the varargs.
				//
				pcStr = va_arg(vaArgP, char *);

				//
				// Determine the length of the string.
				//
				for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
				{
				}

				//
				// Write the string.
				//
				LCDDataWrite(pcStr, ulIdx);

				//
				// Write any required padding spaces
				//
				if(ulCount > ulIdx)
				{
					ulCount -= ulIdx;
					while(ulCount--)
					{
						LCDDataWrite(" ", 1);
					}
				}
				//
				// This command has been handled.
				//
				break;
			}

			//
			// Handle the %u command.
			//
			case 'u':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Reset the buffer position.
				//
				ulPos = 0;

				//
				// Set the base to 10.
				//
				ulBase = 10;

				//
				// Indicate that the value is positive so that a minus sign
				// isn't inserted.
				//
				ulNeg = 0;

				//
				// Convert the value to ASCII.
				//
				goto convert;
			}

			//
			// Handle the %x and %X commands.  Note that they are treated
			// identically; i.e. %X will use lower case letters for a-f
			// instead of the upper case letters is should use.  We also
			// alias %p to %x.
			//
			case 'x':
			case 'X':
			case 'p':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Reset the buffer position.
				//
				ulPos = 0;

				//
				// Set the base to 16.
				//
				ulBase = 16;

				//
				// Indicate that the value is positive so that a minus sign
				// isn't inserted.
				//
				ulNeg = 0;

				//
				// Determine the number of digits in the string version of
				// the value.
				//
				convert:
				for(ulIdx = 1;
						(((ulIdx * ulBase) <= ulValue) &&
								(((ulIdx * ulBase) / ulBase) == ulIdx));
						ulIdx *= ulBase, ulCount--)
				{
				}

				//
				// If the value is negative, reduce the count of padding
				// characters needed.
				//
				if(ulNeg)
				{
					ulCount--;
				}

				//
				// If the value is negative and the value is padded with
				// zeros, then place the minus sign before the padding.
				//
				if(ulNeg && (cFill == '0'))
				{
					//
					// Place the minus sign in the output buffer.
					//
					pcBuf[ulPos++] = '-';

					//
					// The minus sign has been placed, so turn off the
					// negative flag.
					//
					ulNeg = 0;
				}

				//
				// Provide additional padding at the beginning of the
				// string conversion if needed.
				//
				if((ulCount > 1) && (ulCount < 16))
				{
					for(ulCount--; ulCount; ulCount--)
					{
						pcBuf[ulPos++] = cFill;
					}
				}

				//
				// If the value is negative, then place the minus sign
				// before the number.
				//
				if(ulNeg)
				{
					//
					// Place the minus sign in the output buffer.
					//
					pcBuf[ulPos++] = '-';
				}

				//
				// Convert the value into a string.
				//
				for(; ulIdx; ulIdx /= ulBase)
				{
					pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];
				}

				//
				// Write the string.
				//
				LCDDataWrite(pcBuf, ulPos);

				//
				// This command has been handled.
				//
				break;
			}

			//
			// Handle the %% command.
			//
			case '%':
			{
				//
				// Simply write a single %.
				//
				LCDDataWrite(pcString - 1, 1);

				//
				// This command has been handled.
				//
				break;
			}

			//
			// Handle all other commands.
			//
			default:
			{
				//
				// Indicate an error.
				//
				LCDDataWrite("ERROR", 5);

				//
				// This command has been handled.
				//
				break;
			}
			}
		}
	}

	//
	// End the varargs processing.
	//
	va_end(vaArgP);
}


