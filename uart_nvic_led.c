#include <stdint.h>
#include <stdio.h>

/* UART Receive Interrupt Demo----------------------------------------- */
/* -LED PIN 5 >> GPIO5
 * -USART 2
 * - NVIC  INT
 * When enter 'a' LED is  ON others will be LED will be OFF
 *
 * */

#define GPIOAEN        (1U<<0)
#define UART2EN        (1U<<17)

#define UART2_IRQN0     38

#define SYS_FREQ       16000000
#define APB1_CLK       SYS_FREQ
#define UART_BAUDRATE  115200

//initialize the pointers for the registers
uint32_t *pAHB1ENR     = (uint32_t*)0x40023830; //RCC registers
uint32_t *APB1ENR      = (uint32_t*)0x40023840; //APB bus peripherals
uint32_t *GPIOA_MODER  = (uint32_t*)0x40020000; //configure the gpio a mode for the alternate function
uint32_t *GPIOA_ODR    = (uint32_t*)0x40020014;
uint32_t *GPIOA_AFRL   = (uint32_t*)0x40020020;

uint32_t *USART2_SR    = (uint32_t*)0x40004400;
uint32_t *USART2_DR    = (uint32_t*)0x40004404;
uint32_t *USART2_BRR   = (uint32_t*)0x40004408;
uint32_t *USART2_CR1   = (uint32_t*)0x4000440C;

uint32_t *pNVIC_ISER1  = (uint32_t*)0xe000e104; //Interrupt Set-Enable Register
uint32_t *pNVIC_ISPR1  = (uint32_t*)0xe000e204; //Interrupt Set-Pending Register
int __io_putchar(int ch);

void Uart_Init(void);
void Uart_Write(int ch);
char Uart_Read(void);

int main(void)
{
	Uart_Init();
	printf("Enter the Loop \r\n");
	while(1)
	{

	}

	return 0;

}
void USART2_IRQHandler(void)
{
	char ch;
	*pNVIC_ISPR1 |= (1<<(UART2_IRQN0 % 32));

	ch = Uart_Read();
	if(ch == 'a')
	{
		*GPIOA_ODR |= (1<<5);
	}
	else
	{
		*GPIOA_ODR &=~(1<<5);
	}
	printf("Data is %c \r\n", ch);
}
void Uart_Init(void)
{
	//Enable clock access to GPIOA
	*pAHB1ENR |= GPIOAEN;

	//Enable Clock Access for USART2
	*APB1ENR |= (1U<<17);

	//SET PA2 MOODE TO Alternate function Mode
	*GPIOA_MODER &=~ (1U<<4);
	*GPIOA_MODER |=  (1U<<5);


	//SET PA3 MOODE TO Alternate function Mode
	*GPIOA_MODER &=~ (1U<<6);
	*GPIOA_MODER |=  (1U<<7);

	//configure GPIO to O/P Function
	//SET PA5 MOODE TO Output Mode
	*GPIOA_MODER |=  (1U<<10);
	*GPIOA_MODER &=~ (1U<<11);



	//SET PA2 & PA3 Alternative function type to UART_TX (AF07)
	*GPIOA_AFRL |= (1U<<8);
	*GPIOA_AFRL |= (1U<<9);
	*GPIOA_AFRL |= (1U<<10);
	*GPIOA_AFRL &=~(1U<<11);

	*GPIOA_AFRL |= (1U<<12);
	*GPIOA_AFRL |= (1U<<13);
	*GPIOA_AFRL |= (1U<<14);
	*GPIOA_AFRL &=~(1U<<15);

	//CONFIGURE the BRR to 9600
	*USART2_BRR = 0x0683;

	//CONFIGURE the UART for TX & RX
	*USART2_CR1 |= 0X0008;    //Bit 3 TE: Transmitter enable
	*USART2_CR1 |= 0X0004;    //Bit 2 RE: Receiver enable
	*USART2_CR1 |= (1U<<5);   //Bit 5 RXNE interrupt enable
	*USART2_CR1 |= 0X2000;    //Bit 13 UE: USART enable

	*pNVIC_ISER1 |= (1<<(UART2_IRQN0 % 32));
}


void Uart_Write(int ch)
{
	//Make sure the transmit data register is empty
	while(!(*USART2_SR & 0x0080)){}
	//write to transmit data register
	*USART2_DR = (ch & 0XFF);

}

char Uart_Read(void)
{
	while(!(*USART2_SR & 0x0020)){}
	return *USART2_DR ;

}

int __io_putchar(int ch)
{
	Uart_Write(ch);
	return ch;

}
