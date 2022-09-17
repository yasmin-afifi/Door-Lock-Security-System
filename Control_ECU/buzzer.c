/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: source file for the Buzzer driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/

#include "gpio.h"
#include "buzzer.h"

/*
 * Description :
 * Functional responsible for Initialize the Buzzer device by:
 * 1. Setting the direction of Buzzer pin
 * 2. Initialize the pin by zero
 */
void Buzzer_init(void)
{

	GPIO_setupPinDirection(Buzzer_PORT_ID, Buzzer_PIN_ID, PIN_OUTPUT);  // Set Buzzer pin direction as output pin
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);            // Initialize Buzzer pin by zero

}

/*
 * Description :
 * Set Buzzer Pin by one to turn Buzzer ON
 */
void Buzzer_ON(void)
{

	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_HIGH);            // Output one on Buzzer pin

}

/*
 * Description :
 * Set Buzzer Pin by zero to turn Buzzer OFF
 */
void Buzzer_OFF(void)
{

	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);            // Output zero on Buzzer pin
}
