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
#define NONE  0b00000000
#define RESET 6
#define START 5
#define MINUS 4
#define PLUS  3
#define MINSEC   2
#define DEL 300

unsigned int digit [11] = {ZERO,ONE,TWO,TREE,FORE,FIVE,SIX,SEVEN,EIGHT,NINE,NONE};

unsigned int r1 = 0;
unsigned int r2 = 0;
unsigned int r3 = 0;
unsigned int r4 = 0;
unsigned int timer_counter = 0;
int sec = 0;
int min = 0;

#define ONSEC 0
#define ONMIN 1
#define COUNTING 2
#define STOP 3
unsigned int tack = 1;
unsigned int state = ONSEC;


void set_time(unsigned int min, unsigned int sec)
{
	r1 = min % 100 / 10;
	r2 = min % 10;
	r3 = sec % 100 / 10;
	r4 = sec % 10;

}
void print_digit()
{
	if (state == COUNTING)
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
			PORTD |= (1<<7);
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
	else if (state == STOP)
	{
		if(timer_counter == 0)
		{
			RAZR &= ~(1<<3);
			RAZR |= (1<<0);
			if (tack)	DIGIT = digit[r1];	else DIGIT = NONE;
			timer_counter++;
		}
		else if(timer_counter == 1)
		{
			RAZR &= ~(1<<0);
			RAZR |= (1<<1);
			if (tack)	DIGIT = digit[r2];	else DIGIT = NONE;
			PORTD |= (1<<7);
			timer_counter++;
		}
		else if(timer_counter == 2)
		{
			RAZR &= ~(1<<1);
			RAZR |= (1<<2);
			if (tack)	DIGIT = digit[r3];	else DIGIT = NONE;
			timer_counter++;
		}
		else
		{
			RAZR &= ~(1<<2);
			RAZR |= (1<<3);
			if (tack)	DIGIT = digit[r4];	else DIGIT = NONE;
			timer_counter = 0;
		}
	}
	else
	{
		if (state == ONSEC)
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
				PORTD |= (1<<7);
				timer_counter++;
			}
			else if(timer_counter == 2)
			{
				RAZR &= ~(1<<1);
				RAZR |= (1<<2);
				if (tack)	DIGIT = digit[r3];	else DIGIT = NONE;			
				timer_counter++;
			}
			else
			{
				RAZR &= ~(1<<2);
				RAZR |= (1<<3);
				if (tack)	DIGIT = digit[r4];	else DIGIT = NONE;			
				timer_counter = 0;
			}
			
		}
		else
		{
			
			if(timer_counter == 0)
			{
				RAZR &= ~(1<<3);
				RAZR |= (1<<0);
				if (tack)	DIGIT = digit[r1];	else DIGIT = NONE;
				timer_counter++;
			}
			else if(timer_counter == 1)
			{
				RAZR &= ~(1<<0);
				RAZR |= (1<<1);
				if (tack)	DIGIT = digit[r2];	else DIGIT = NONE;
				PORTD |= (1<<7);
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
	}
}

void alarm()
{
	TCCR1B = 0;
	PORTC |= (1<<1);
}

void set_timer()
{
	TCCR0 = 0b00000011;
	TIMSK = 1;
	TCNT0 = 0;
	sei();

	TIMSK |= (1<<OCIE1A);
	OCR1A = 15625;
	TCNT1 = 0;
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<CS12);
	TCCR1B |= (1<<CS11) | (1<<CS10);
}

void sets()
{
	DDRD = 0xff;
	DDRB = 0x0f;
	DDRC |= (1<<1);
	DDRC &= ~((1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6));
	PORTC |= (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);
}

ISR(TIMER0_OVF_vect)
{
	print_digit();
}
ISR(TIMER1_COMPA_vect)
{
	if (state == COUNTING)
	{
		sec--;
		if (sec < 1)
		{
			if(min == 0)
			{
				alarm();
			}
			else
			{
				min--;
				sec = 59;
			}
		}
	}
	else
	{
		if(tack == 1)  tack = 0; else  tack = 1; 
	}
}


int main (void)
{

	sets();
	set_timer();

	while (1)
	{
		if(~PINC & (1<<MINSEC))
		{
			if(state == ONSEC)  state = ONMIN; else if (state == ONMIN) state = ONSEC;
			_delay_ms(DEL);
		}
		if (~PINC & (1<<PLUS))
		{
			if (state == ONSEC)
			{
				if(sec == 59) sec = 0;
				else sec++;
				_delay_ms(DEL);
			} 
			else if(state == ONMIN)
			{
				if(min == 99) min = 0;
				else min++;
				_delay_ms(DEL);
			}
		}
		if (~PINC & (1<<MINUS))
		{
			if (state == ONSEC)
			{
				if(sec == 0) sec = 59;
				else sec--;
				_delay_ms(DEL);
			}
			else if(state == ONMIN)
			{
				if(min == 00) min = 99;
				else min--;
				_delay_ms(DEL);
			}
		}
		if (~PINC & (1<<START))
		{
			if (state != COUNTING)	state = COUNTING;	
			else	state = STOP;
			_delay_ms(DEL);
		}
		set_time(min, sec);
	}
}