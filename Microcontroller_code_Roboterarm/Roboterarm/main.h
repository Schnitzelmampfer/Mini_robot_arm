/*
 * main.h
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#define	F_CPU	16000000UL

// Makros
#ifndef SET
#define	SET(reg, bit)	(reg) |= (1<<(bit))
#define CLR(reg, bit)	(reg) &= ~(1<<(bit))
#endif

#ifndef HIGH
#define HIGH(reg)		(reg)/256
#define LOW(reg)		(reg)%256
#endif

// Timer1
#define TIMER1_MSK_TCCR1B	(0b001<<CS10) | (1<<WGM12)	//Prescale 1
#define TIMER1_VAL_OCR1A	16e6 * 10e-6 - 1	//10 µs
#define TIMER1_MSK_TIMSK1	(1<<OCIE1A)
#define TIMER1_INIT()		TCCR1B |= TIMER1_MSK_TCCR1B; OCR1A = TIMER1_VAL_OCR1A; TIMSK1 |= TIMER1_MSK_TIMSK1;
#define MS20				2000	// 20 ms
#define LOWERBOUND			55		// 0.55 ms
#define UPPERBOUND			240		// 2.4 ms

// Ports
#define DDR_SERVO	DDRB
#define PORT_SERVO	PORTB

#endif /* MAIN_H_ */