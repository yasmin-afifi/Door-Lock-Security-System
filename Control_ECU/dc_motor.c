/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.c
 *
 * Description: source file for the DC Motor driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/


#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "dc_motor.h"


/*
 * Description :
 * The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 */
void DcMotor_Init(void)
{

	GPIO_setupPinDirection(DC_Motor_PORT_ID,DC_Motor_PIN1_ID, PIN_OUTPUT);  //setup DC motor pin1 direction as output
	GPIO_setupPinDirection(DC_Motor_PORT_ID,DC_Motor_PIN2_ID, PIN_OUTPUT);  //setup DC motor pin2 direction as output

	//Stop at the DC-Motor at the beginning through the GPIO driver.
	GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN1_ID,LOGIC_LOW);
	GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN2_ID,LOGIC_LOW);


}


/*
 * Description :
 * The Function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the
 * required speed value.
 */
void DcMotor_Rotate(DcMotor_State state)
{




	switch(state)
	{
		case stop:
			GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN1_ID,LOGIC_LOW);
			GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN2_ID,LOGIC_LOW);
				break;

		case CW:
			GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN1_ID,LOGIC_HIGH);
			GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN2_ID,LOGIC_LOW);
				break;

		case ACW:
			GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN1_ID,LOGIC_LOW);
			GPIO_writePin(DC_Motor_PORT_ID,DC_Motor_PIN2_ID,LOGIC_HIGH);
				break;
	}

}


