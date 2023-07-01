#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"


/*=========================================================================================*/

void Uart_Init(void);
char Uart_Read(void);
void Uart_Write(char ch);

/*=========================================================================================*/
/*=========================================================================================*/

void Uart_Write(char ch)
{
	//Make sure the transmit data register is empty
	while(!(USART1->SR & 0x0080)){}
	//write to transmit data register
	USART1->DR = (ch & 0XFF);

}
/*=========================================================================================*/

char Uart_Read(void)
{
	//Make sure the Read data register is not empty >RXNE
	// 0 : Data is Not Received
	// 1 : Received Data is Ready to be Read
	while(!(USART1->SR & 0x0020)){}
	return USART1->DR;

}
/*=========================================================================================*/
int main(void)
{
	char Data;
	Uart_Init();
	printf("Enter the Loop... \r\n");
	while(1)
	{
		Data = Uart_Read();
		if(Data == 'A')
		{
			GPIOA->ODR |=(1U<<5);
			Uart_Write(Data);
		}
		else
		{
			GPIOA->ODR &=~(1U<<5);
			Uart_Write(Data);
		}

	}

	return 0;

}

/*=========================================================================================*/

void Uart_Init(void)
{
	RCC->AHB1ENR |= (1U<<0); //Enable clock access to GPIOA
	RCC->AHB1ENR |= (1U<<1); //Enable clock access to GPIOB
	RCC->APB2ENR |= (1U<<4); //Enable clock access to USART1

	//SET PB6 MOODE TO Alternate function Mode >> PB6->TX & PB7->RX
	GPIOB->MODER |=(1U<<13);
	GPIOB->MODER &=~(1U<<12);
	GPIOB->MODER |=(1U<<15);
	GPIOB->MODER &=~(1U<<14);

	//Set PA5 is output connected to Relay
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);

	GPIOB->AFR[0] |=(1U<<24);
	GPIOB->AFR[0] |=(1U<<25);
	GPIOB->AFR[0] |=(1U<<26);
	GPIOB->AFR[0] &=~(1U<<27);

	GPIOB->AFR[0] |=(1U<<28);
	GPIOB->AFR[0] |=(1U<<29);
	GPIOB->AFR[0] |=(1U<<30);
	GPIOB->AFR[0] &=~(1U<<31);

	USART1->CR1 |=(1U<<13);
	USART1->CR1 |=(1U<<2);
	USART1->CR1 |=(1U<<3);

	USART1->BRR =0x0683; //9600 Baud Rate
}


int __io_putchar(int ch)
{
	Uart_Write(ch);
	return ch;

}
