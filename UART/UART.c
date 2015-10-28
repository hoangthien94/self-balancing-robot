/*
 * UART.c
 *
 *  Created on: Apr 18, 2013
 *      Author: Admin
 */

#include "UART.h"
#include "../include.h"

#define MAX_RX_BUF              1500




static volatile uint8_t rxBuffer[MAX_RX_BUF];
static volatile uint16_t rxHead;
static uint16_t rxTail;
static uint16_t ui16_rxSize = 0;

unsigned char ConfigUART(UARTType *UART)

{
	unsigned long ulSet = 0;
	//Configure GPIO for UART Mode
	switch ((*UART).PortName)
	{
		case UART0_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
			GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			GPIOPinConfigure(GPIO_PA0_U0RX);
			GPIOPinConfigure(GPIO_PA1_U0TX);
			break;
		case UART1_BASE:
			break;
		case UART2_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
			GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinConfigure(GPIO_PD6_U2RX);
			GPIOPinConfigure(GPIO_PD7_U2TX);
			break;
		case UART3_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
			GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinConfigure(GPIO_PC6_U3RX);
			GPIOPinConfigure(GPIO_PC7_U3TX);
			break;
		case UART4_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
			GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
			GPIOPinConfigure(GPIO_PC4_U4RX);
			GPIOPinConfigure(GPIO_PC5_U4TX);
			break;
		case UART5_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
			GPIOPinConfigure(GPIO_PE4_U5RX);
			GPIOPinConfigure(GPIO_PE5_U5TX);
			break;
		case UART6_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
			GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
			GPIOPinConfigure(GPIO_PD4_U6RX);
			GPIOPinConfigure(GPIO_PD5_U6TX);
			break;
		case UART7_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			GPIOPinConfigure(GPIO_PE0_U7RX);
			GPIOPinConfigure(GPIO_PE1_U7TX);
			break;
		default:
			return(INVALID_UART_NAME);
	}

	switch ((*UART).DataBits)
	{
		case 5:
			ulSet |= UART_CONFIG_WLEN_5;
			break;
		case 6:
			ulSet |= UART_CONFIG_WLEN_6;
			break;
		case 7:
			ulSet |= UART_CONFIG_WLEN_7;
			break;
		case 8:
			ulSet |= UART_CONFIG_WLEN_8;
			break;
		default:
			return(INVALID_DATA_BITS);
	}

	switch ((*UART).Parity)
	{
		case None:
			ulSet |= UART_CONFIG_PAR_NONE;
			break;
		case Odd:
			ulSet |= UART_CONFIG_PAR_ODD;
			break;
		case Even:
			ulSet |= UART_CONFIG_PAR_EVEN;
			break;
		case One:
			ulSet |= UART_CONFIG_PAR_ONE;
			break;
		case Zero:
			ulSet |= UART_CONFIG_PAR_ZERO;
			break;
		default:
			return(INVALID_PARITY_BIT);
	}

	switch ((*UART).StopBits)
	{
		case 1:
			ulSet |= UART_CONFIG_STOP_ONE;
			break;
		case 2:
			ulSet |= UART_CONFIG_STOP_TWO;
			break;
		default:
			return(INVALID_STOP_BIT);
	}
	//Configure UART
	UARTConfigSetExpClk((*UART).PortName, SysCtlClockGet(), (*UART).BaudRate, ulSet);

	UARTIntRegister((*UART).PortName, (*UART).ISR);
	UARTIntEnable((*UART).PortName, UART_INT_RX);
	UARTEnable((*UART).PortName);
  	IntMasterEnable();
  	return(ERROR_NONE);
}
void UART0_Handler()
{
	uint32_t IntStatus;
	IntStatus = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, IntStatus);
	while (UARTCharsAvail(UART0_BASE))
	{
		if (rxHead + 1 < MAX_RX_BUF)
		{
			if ((rxHead + 1) != rxTail)
			{
				rxBuffer[rxHead++] = UARTCharGet(UART0_BASE);
				ui16_rxSize++;
			}
		}
		else
		{
			if (0 != rxTail)
			{
				rxBuffer[rxHead] = UARTCharGet(UART0_BASE);
				ui16_rxSize++;
				rxHead = 0;
			}
		}
	}
}
uint16_t recv_UART0(uint8_t* rxBuf, uint16_t numToRead, bool block)
{
	if (block)
	{
		// Blocking mode, so don't return until we read all the bytes requested
		uint16_t bytesRead;
		// Keep getting data if we have a number of bytes to fetch
		while (numToRead)
		{
			bytesRead = ReadRxBuffer_recv_UART0(rxBuf, numToRead);
			if (bytesRead)
			{
				rxBuf += bytesRead;
				numToRead -= bytesRead;
			}
		}
		return bytesRead;
	}
	else
	{
		// Non-blocking mode, just read what is available in buffer
		return ReadRxBuffer_recv_UART0(rxBuf, numToRead);
	}
}
void UARTPutNum(uint32_t UART_Base, long Num)
{
	unsigned long temp = 1;
	long NumTemp;
	NumTemp = Num;
	if (Num == 0)
	{
		UARTCharPutNonBlocking(UART_Base, 48);
	}
	else
	{
		if (Num < 0)
		{
			UARTCharPutNonBlocking(UART_Base, '-');
			Num *= -1;
		}
		while (NumTemp)
		{
			NumTemp /= 10;
			temp *= 10;
		}
		temp /= 10;
		while (temp)
		{
			UARTCharPutNonBlocking(UART_Base,(Num / temp) % 10 + 48);
			temp /= 10;
		}
	}

}
void UARTStringPut(uint32_t ui32Base,const uint8_t* s)
{
	while(*s != '\0')
	{
		UARTCharPutNonBlocking(ui32Base,*s);
		s++;
	}
}
uint16_t ReadRxBuffer_recv_UART0(uint8_t* rxBuf, uint16_t numToRead)
{
	uint16_t bytesRead = 0;
	while (rxTail != rxHead && bytesRead < numToRead)
	{
		if (rxTail + 1 < MAX_RX_BUF)
		{
			*rxBuf++ = rxBuffer[rxTail++];
			bytesRead++;
		}
		else
		{
			*rxBuf++ = rxBuffer[rxTail];
			rxTail = 0;
			bytesRead++;
		}
	}
	return bytesRead;
}

uint16_t GetRxData(uint8_t *data, uint16_t len)
{
    volatile uint16_t ui16_size = 0;

    ui16_size = ReadRxBuffer_recv_UART0(data, len);
    if (ui16_rxSize >= len)
        ui16_rxSize -= len;
    else
        ui16_rxSize = 0;
    return ui16_size;
}
