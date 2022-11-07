/*   ----------------------------------------------------------------------------------
 *  Mini Project 2: Stop Watch
 *  Author: Mohamed Khaled
 *  Created on: Sep 13, 2022
 *  ----------------------------------------------------------------------------------
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define no_7_segments 6

unsigned char count[no_7_segments] = {0}; //Counter to display time on 7 segments

//Interrupt Service Routine Of INT0
ISR(INT0_vect)
{
	TCNT1 = 0; //Initiate TIMER1 to ZERO again
	OCR1A = 15625; //Set Compare value for Match A
	count[5] = count[4] = count[3] = count[2] = count[1] = count[0] = 0;
}

//Interrupt Service Routine Of INT1
ISR(INT1_vect)
{
	/* Pause the timer by removing the clock source
	 * CS10 = 0 & CS11 = 0 & CS12 = 0 */
	TCCR1B &= ~(1<<CS10) & ~(1<<CS11);
}

//Interrupt Service Routine Of INT2
ISR(INT2_vect)
{
	/* Resume the timer by returning the clock source
	 * CS10 = 1 & CS11 = 1 & CS12 = 0 */
	TCCR1B |= (1<<CS10) | (1<<CS11);
}

//Interrupt Service Routine Of TIMER1
ISR(TIMER1_COMPA_vect)
{
	count[0]++;
	if(count[0] > 9)
	{
		count[0] = 0;
		count[1]++;
	}
	if(count[1] == 6)
	{
		count[1] = 0;
		count[2]++;
	}
	if(count[2] > 9)
	{
		count[2] = 0;
		count[3]++;
	}
	if(count[3] == 6)
	{
		count[3] = 0;
		count[4]++;
	}
	if(count[4] > 9)
	{
		count[4] = 0;
		count[5]++;
	}
}

//Configure External Interrupt INT0 with falling edge
void INTO_Init(void)
{
	//Set ISC01 & clear ISC00 -> The falling edge of INT0 generates an interrupt request.
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);

	GICR |= (1<<INT0); //Enable External Interrupt INT0

	DDRD &= ~(1<<PD2); //Configure PD2 as I/P
	PORTD |= (1<<PD2); //Activate Internal Pull-Up resistor
}

//Configure External Interrupt INT1 with raising edge
void INT1_Init(void)
{
	//Setting ISC11 & ISC10 -> The rising edge of INT1 generates an interrupt request
	MCUCR |= (1<<ISC10) | (1<<ISC11);

	GICR |= (1<<INT1); //Enable External Interrupt INT1

	DDRD &= ~(1<<PD3); //Configure PD3 as I/P
}

//Configure External Interrupt INT2 with falling edge
void INT2_Init(void)
{
	//clear ISC2 -> The falling edge on INT2 activates the interrupt
	MCUCSR &= ~(1<<ISC2);

	GICR |= (1<<INT2); //Enable External Interrupt INT2

	DDRB &= ~(1<<PB2); //Configure PB2 as I/P
	PORTB |= (1<<PB2); //Activate Internal Pull-Up resistor
}

//Configure TIMER1 to be in CTC mode Compare Match A
void TIMER0_COMPA_Init(void)
{
	/* Configure TIMER1 Control Register A
	 * 1. Disconnect OC1A/OC1B (i.e. clear COM1A1 & COM1B1 & COM1A0 & COM1B0)
	 * 2. Set FOC1A since I use Compare Match A & Clear FOC1B as I won't use it
	 * 3. Clear WGM10 & WGM11 -> Set Timer Mode to CTC
	 */
	TCCR1A = (1<<FOC1A);

	/* Configure TIMER1 Control Register B
	 * 1. Clear WGM13 & Set WGM12 -> Set Timer Mode to CTC
	 * 2. Set CS10 & CS11 but Clear CS12 -> pre-scaled clk/64
	 */
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS11);

	TCNT1 = 0; //Initiate TIMER1 to ZERO

	OCR1A = 15625; //Set Compare value for Match A

	TIMSK |= (1<<OCIE1A); //Output Compare A Match Interrupt Enable
}

//Function to display 7-segment
void display_7_segment(unsigned char pin_pos, unsigned char value)
{
	if((PORTA<<2) == 0xFC)
	{
		PORTA &= 0xC0; //TURN OFF all the 7-segments
	}
	PORTC = (PORTC & 0xF0) | (value & 0x0F); //Set number of selected 7 segment
	PORTA |= (1<<pin_pos);        //Activate Certain 7-segment
	_delay_ms(2);
	PORTA &= ~(1<<pin_pos);       //De-activate the 7-segment
}

//Main Function
int main(void)
{
	DDRA  |= 0x3F;       //Configure first 6-pins in PORTA as O/P for enabling six 7-segments
	PORTA |= 0x3F;		 //Turn All The 7-Segments in the Beginning

	DDRC  |= 0x0F;		 //Configure first 4-pins in PORTC as O/P for decoder 4-pins
	PORTC &= 0xF0;		 //Print ZERO on all the 7-segments in the Beginning

	INTO_Init();		 //Initiate INT0

	INT1_Init();		 //Initiate INT1

	INT2_Init();		 //Initiate INT2

	TIMER0_COMPA_Init(); //Initiate TIMER1

	SREG |= (1<<7); 	 //Enable Global Interrupt

	for(;;)
	{
		display_7_segment(PA0, count[0]);
		display_7_segment(PA1, count[1]);
		display_7_segment(PA2, count[2]);
		display_7_segment(PA3, count[3]);
		display_7_segment(PA4, count[4]);
		display_7_segment(PA5, count[5]);
	}
}
