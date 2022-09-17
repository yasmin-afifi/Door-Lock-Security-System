/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: header file for the Buzzer driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define Buzzer_PORT_ID				PORTD_ID
#define Buzzer_PIN_ID				PIN5_ID



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the Buzzer device by:
 * 1. Setting the direction of Buzzer pin
 * 2. Initialize the pin by zero
 */
void Buzzer_init(void);

/*
 * Description :
 * Set Buzzer Pin by one to turn Buzzer ON
 */
void Buzzer_ON(void);

/*
 * Description :
 * Set Buzzer Pin by zero to turn Buzzer OFF
 */
void Buzzer_OFF(void);

#endif /* BUZZER_H_ */
