 /******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.c
 *
 * Description: Source file for Timer0 driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/

#include"timer0.h"
#include"avr/io.h"
#include <avr/interrupt.h>
#include "common_macros.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/


/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;



/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_OVF_vect)
{
 	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description:
 * Functional responsible for Initialize Timer0 by:
 * 1. prescaler
 * 2. initial value
 * 3. compare value
 * 4. timer mode
 * 5. enable interrupt
 */
void Timer0_init(const Timer0_ConfigType * Config_Ptr)
{
	SET_BIT(TCCR0,FOC0); 					// Set when WGM00 bit is non PWM mode

	if((Config_Ptr->mode)==Normal)
	{

		CLEAR_BIT(TCCR0,WGM00);
		CLEAR_BIT(TCCR0,WGM01);
										//Enable Timer0 Overflow Interrupt
		SET_BIT(TIMSK,TOIE0);
	}
	else if((Config_Ptr->mode)==CMP)
	{

		CLEAR_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
										//Enable Timer0 Compare Interrupt
		SET_BIT(TIMSK,OCIE0);
										// Set Compare Value
		OCR0 = Config_Ptr->CMP_value;
	}
										//Normal port operation, OC0 disconnected
	CLEAR_BIT(TCCR0,COM00);
	CLEAR_BIT(TCCR0,COM01);

										//Set Timer Prescaler
	TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->prescaler);

										// Set Initial Value
	TCNT0 = Config_Ptr->Start_value;


}

/*
 * Description:
 * Functional responsible for De-Initialize Timer0 by:
 * 1. Putting TCCR0 register equal zero
 * 2. initial value equal zero
 * 3. compare value equal zero
 */
void Timer0_DeInit(void)
{
					//clear all the timer registers
	TCCR0=0;
	TCNT0=0;
	OCR0=0;
    				//Disable all Interrupts
	CLEAR_BIT(TIMSK,OCIE0);
	CLEAR_BIT(TIMSK,TOIE0);
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
