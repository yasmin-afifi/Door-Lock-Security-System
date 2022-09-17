/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the DC Motor driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_Motor_PORT_ID				PORTB_ID
#define DC_Motor_PIN1_ID				PIN0_ID
#define DC_Motor_PIN2_ID				PIN1_ID

#define DC_Motor_E_PIN_ID				PIN3_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	stop,CW,ACW
}DcMotor_State;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * The Function responsible for setup the direction for the two
 * motor pins through the GPIO driver.
 */
void DcMotor_Init(void);


/*
 * Description :
 * The Function responsible for rotate the DC Motor CW/ or A-CW or
 * stop the motor based on the state input state value.
 * Send the required duty cycle to the PWM driver based on the
 * required speed value.
 */
void DcMotor_Rotate(DcMotor_State state);


#endif /* DC_MOTOR_H_ */
