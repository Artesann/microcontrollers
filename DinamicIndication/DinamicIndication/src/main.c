#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define DIGIT PORTD 
#define RAZR PORTB
#define ONE   0b00000110
#define TWO   0b01011011
#define TREE  0b01001111
#define FORE  0b01100110
#define FIVE  0b01101101
#define SIX   0b01111101
#define SEVEN 0b00000111
#define EIGHT 0b01111111
#define NINE  0b01101111
#define ZERO  0b00111111
#define DEL 15

unsigned int digit [10] = {ZERO,ONE,TWO,TREE,FORE,FIVE,SIX,SEVEN,EIGHT,NINE};

unsigned int r1 = 0;
unsigned int r2 = 0;
unsigned int r3 = 0;
unsigned int r4 = 0;
unsigned int timer_counter = 0;

void set_digit(unsigned int d)
{
	r1 = d / 1000;
	r2 = d % 1000 / 100;
	r3 = d % 100 / 10;
	r4 = d % 10;

}
void print_digit()
{
	if(timer_counter == 0)
	{
		RAZR &= ~(1<<3);
		RAZR |= (1<<0);
		DIGIT = digit[r1];
		timer_counter++;
	}
	else if(timer_counter == 1)
	{
		RAZR &= ~(1<<0);
		RAZR |= (1<<1);
		DIGIT = digit[r2];
		timer_counter++;
	}
	else if(timer_counter == 2)
	{
		RAZR &= ~(1<<1);
		RAZR |= (1<<2);
		DIGIT = digit[r3];
		timer_counter++;
	}
	else 
	{
		RAZR &= ~(1<<2);
		RAZR |= (1<<3);
		DIGIT = digit[r4];
		timer_counter = 0;
	}
	
}

void sets()
{
	DDRD = 0xff;
	DDRB = 0xff;

	TCCR0 = 0b00000011;
	TIMSK = 1; 
	TCNT0 = 0;
	sei();
}

ISR(TIMER0_OVF_vect)
{
	print_digit();
}

int main (void)
{
	sets();

	while (1)
	{
		_delay_ms(100);
		count++;
		set_digit(count);
	}
} 