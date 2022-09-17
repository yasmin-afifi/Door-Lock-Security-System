 /******************************************************************************
 *
 * Module: Timer0
 *
 * File Name: timer0.h
 *
 * Description: Header file for the Timer0 driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/

#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	Normal,PWM,CMP,FAST_PWM
}Timer0_Mode;

typedef enum
{
	NO_CLK,F_CPU_,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer0_Prescaler;


typedef struct
{
	Timer0_Mode mode;
	Timer0_Prescaler prescaler;
	uint16 Start_value;
	uint16 CMP_value;


}Timer0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
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
void Timer0_init(const Timer0_ConfigType * Config_Ptr);

/*
 * Description:
 * Functional responsible for De-Initialize Timer0 by:
 * 1. Putting TCCR0 register equal zero
 * 2. initial value equal zero
 * 3. compare value equal zero
 */
void Timer0_DeInit(void);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void));


#endif /* TIMER0_H_ */
