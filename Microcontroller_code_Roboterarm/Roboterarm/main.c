/*
 * Roboterarm.c
 */
#include "main.h"
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


#include "uart.h"

volatile uint16_t gu16Ticks = 0;
volatile uint8_t gu8ServoPulses[4];
volatile uint8_t gu8ServoPulses_buf[4];
volatile uint8_t gu8Servo1Puls = 0;
volatile uint8_t gu8Servo2Puls = 0;
volatile uint8_t gu8Servo3Puls = 0;
volatile uint8_t gu8Servo4Puls = 0;
volatile uint8_t gu8Servo1Puls_buf = 0;
volatile uint8_t gu8Servo2Puls_buf = 0;
volatile uint8_t gu8Servo3Puls_buf = 0;
volatile uint8_t gu8Servo4Puls_buf = 0;

ISR(TIMER1_COMPA_vect)	//10µs
{
	gu16Ticks++;

	if (gu16Ticks >= MS20)
	{
		gu16Ticks = 0;
		PORT_SERVO |= 0xF;
		gu8Servo1Puls_buf = gu8Servo1Puls;
		gu8Servo2Puls_buf = gu8Servo2Puls;
		gu8Servo3Puls_buf = gu8Servo3Puls;
		gu8Servo4Puls_buf = gu8Servo4Puls;
		/*for (uint8_t i=0; i<4; i++)
		{
			gu8ServoPulses_buf[i] = gu8ServoPulses[i];
		}*/
	}
	else
	{
		if (gu16Ticks == gu8Servo1Puls_buf)
		{
			CLR(PORT_SERVO, 0);
		}
		if (gu16Ticks == gu8Servo2Puls_buf)
		{
			CLR(PORT_SERVO, 1);
		}
		if (gu16Ticks == gu8Servo3Puls_buf)
		{
			CLR(PORT_SERVO, 2);
		}
		if (gu16Ticks == gu8Servo4Puls_buf)
		{
			CLR(PORT_SERVO, 3);
		}
		/*for (uint8_t i=0; i<4; i++)
		{
			//CLR(PORT_SERVO, 0);
			
			if (gu16Ticks >= gu8ServoPulses_buf[i])
			{
				CLR(PORT_SERVO, i);
			}
		}*/
	}
}

int main(void)
{
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	TIMER1_INIT();
	DDR_SERVO = 0xF;

	gu8Servo1Puls = 122;
	gu8Servo2Puls = 148;
	gu8Servo3Puls = 209;
	gu8Servo4Puls = 101;
	
	sei();   
	uint16_t resC = 0;
	uint8_t string[10];
	uint8_t error = 0;
    while (1) 
    {
		resC = uart_getc();
		if (HIGH(resC) == 0 && LOW(resC) == 'S')
		{
			_delay_ms(2);
			uint8_t j = 0;
			for(uint8_t i=0; i<10;i++)
			{
				resC = uart_getc();
				if (LOW(resC) == 0xA)
				{
					string[i] = 0;
					uart_getc();
					break;
				}
				if (i==1) 
				{
					if (LOW(resC) == 'P') continue;
					else
					{
						string[i] = 0;
						error = 1;
						break;
					}
				}
				string[j] = LOW(resC);
				string[j+1] = 0;
				j++;
				//uart_putc(i+48);
			}
			if (!error)
			{
				uint8_t puls = 0;
				uint8_t i = 1;
				while (string[i] != 0)
				{
					//uart_putc(string[i]);
					puls = puls * 10;
					puls = puls + string[i] - 48;
					i++;
				}
				
				switch (string[0])
				{
					case '1':
						gu8Servo1Puls = puls;
						break;
					case '2':
						gu8Servo2Puls = puls;
						break;
					case '3':
						gu8Servo3Puls = puls;
						break;
					case '4':
						gu8Servo4Puls = puls;
						break;
					default: break;
				}
			}
		}
    }
}

