#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main (void)
{
	DDRB |= (1<<2) | (1<<1) | (1<<0);
	PORTB &= ~((1<<2) | (1<<1) | (1<<0));
	DDRC &= ~(1<<1);
	ADCSRA |= (1<<ADEN); //��������� ������ ���
	ADCSRA |= (1<<ADFR); //����������� ��������������
	ADCSRA &= ~(1<<ADPS2);
	ADCSRA |= (1<<ADPS1) | (1<<ADPS1); //��������� �������
	
	ADMUX |= (1<<REFS1) | (1<<REFS0); //����� ���
	ADMUX &= ~(1<<ADLAR); //������������
	ADMUX |= (1<<MUX0);
	ADMUX &= ~((1<<MUX1) | (1<<MUX2) | (1<<MUX3));//��������� ������ ���
	
	ADCSRA |= (1<<ADSC);//������ ���
	
	while (1)
	{
		if (ADCSRA & (1<<4))
		{
			if(ADC >= 600)
			{
				PORTB |= (1<<0);
				PORTB &= ~(1<<1);	
				PORTB &= ~(1<<2);
			}
			if (ADC < 600 && ADC >= 550)
			{
				PORTB |= (1<<1);
				PORTB &= ~(1<<0);
				PORTB &= ~(1<<2);
			} 
			if (ADC < 550)
			{
				PORTB |= (1<<2);
				PORTB &= ~(1<<0);
				PORTB &= ~(1<<1);
			}
		}
		ADCSRA |= (1<<4);
	}
}