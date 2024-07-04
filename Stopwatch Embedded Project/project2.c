/******************************************************************************
 *
 * Module: Ultra Sonic
 *
 * File Name: ultra_sonic.h
 *
 * Description: Source file for the Ultra Sonic HC-SR04 driver
 *
 * Author: Mostafa Osama
 *
 *******************************************************************************/
#include<avr/interrupt.h>
#include<avr/io.h>
#include<util/delay.h>

unsigned char seg[6]={0};

void Display(void);

void TIMER_1(unsigned int x);

void configure(void);

int main()
{
	TIMER_1(1000);
	configure();
	DDRA|= 0x3F;
	PORTA&=~(0x3f);
	DDRC |= 0x0f;
	PORTC &=~(0x0f);
	DDRD &=~(1<<2)&~(1<<3);
	DDRB &=~(1<<2);
	while(1)
	{
		Display();
        PORTA ^= (1<<0);
        PORTC &= ~(0x0F);
        PORTC |= seg[0];
        _delay_ms(3);
        PORTA ^= (1<<0)^(1<<1);
        PORTC &= ~(0x0F);
        PORTC |= seg[1];
        _delay_ms(3);
        PORTA ^= (1<<1)^(1<<2);
        PORTC &= ~(0x0F);
        PORTC |= seg[2];
        _delay_ms(3);
        PORTA ^= (1<<2)^(1<<3);
        PORTC &= ~(0x0F);
        PORTC |= seg[3];
        _delay_ms(3);
        PORTA ^= (1<<3)^(1<<4);
        PORTC &= ~(0x0F);
        PORTC |= seg[4];
        _delay_ms(3);
        PORTA ^= (1<<4)^(1<<5);
        PORTC &= ~(0x0F);
        PORTC |= seg[5];
        _delay_ms(3);
        PORTA ^= (1<<5);
	}
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
    seg[0]++;

}
ISR(INT0_vect)
{
    seg[0]=0;
    seg[1]=0;
    seg[2]=0;
    seg[3]=0;
    seg[4]=0;
    seg[5]=0;
}
ISR(INT1_vect)
{
    TCCR1B &=~ (1<<CS10) &~(1<<CS12);
}
ISR(INT2_vect)
{
    TCCR1B |= (1<<CS10) | (1<<CS12);
}

void Display(void)
{
    if(seg[0]>9)
    {
        seg[1]++;
        seg[0]=0;
    }
    if(seg[1]>5)
    {
        seg[2]++;
        seg[1]=0;
    }
    if(seg[2]>9)
    {
        seg[3]++;
        seg[2]=0;
    }
    if(seg[3]>5)
    {
        seg[4]++;
        seg[3]=0;
    }
    if(seg[4]>9)
    {
        seg[5]++;
        seg[4]=0;
    }
}
void TIMER_1(unsigned int x)
{
	TCCR1A=(1<<FOC1A);
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TCNT1=0;
	OCR1A=x;
	TIMSK|=(1<< OCIE1A);
	SREG|=(1<<7);
}
void configure(void)
{
    MCUCR |=(1<<ISC11)|(1<<ISC10)|(1<<ISC01);
    GICR|=(1<<INT0)|(1<<INT1)|(1<<INT2);
    SREG |= (1<<7);
}