 /******************************************************************************
 *
 * Module: Control ECU
 *
 * File Name: control.h
 *
 * Description: Source file for Contol ECU
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/

#include "uart.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "timer0.h"
#include "util/delay.h"
#include "twi.h"
#include "avr/io.h" /* To use the SREG Register */

/*******************************************************************************
 *                               Definitions                                  *
 *******************************************************************************/

#define Pass_Size				5
#define MC_READY 				0xFF
#define STORE_EEPROM 			0xF1
#define OPEN 					0xF2
#define CLOSE 					0xF3
#define WAIT 					0xF5
#define BUZZER_ON 				0xF6
#define BUZZER_OFF 				0xF7

/*global variable to count overflows*/
uint32 count2 = 0;
void timer0_count2(void) {
	count2++;
}

int main(void) {
	uint8 data;
	uint8 command;
	uint8 recieved_password[Pass_Size];
	uint8 pass_to_send[Pass_Size];

	DcMotor_Init(); 			// initialize the motor
	Buzzer_init(); 				// initialize the buzzer


	UART_ConfigType uart_config = { 9600, F_8_BIT, S_1_BIT, DISABLED};


	TWI_ConfigType TWI_config = { FCPU ,0b00000001,400000};

	/* Enable interrupts by setting I-bit */
	SREG |= (1 << 7);


	UART_init(&uart_config);


	TWI_init(&TWI_config);


	Timer0_ConfigType timer0_config = { Normal,F_CPU_1024, 0, 0};

	/* set the callback function of the timer */
	Timer0_setCallBack(timer0_count2);


	while (1) {

		command = UART_recieveByte(); 						// receive command from MC1 through the UART
		if (command == STORE_EEPROM) {
			for (int i = 0; i < Pass_Size; i++) {
				recieved_password[i] = UART_recieveByte(); 	// take the password from MC1
			}
			for (int i = 0; i < Pass_Size; i++) {
				EEPROM_writeByte(0x0311 + i, recieved_password[i]);// store this password in eeprom starting from address 0x0311
				_delay_ms(10);
			}
			_delay_ms(100);
			for (int i = 0; i < Pass_Size; i++) {
				EEPROM_readByte(0x0311 + i, &data); 	// read the password from eeprom
				_delay_ms(10);
				pass_to_send[i] = data; 				// store it in array to send it to MC1
			}
			while (UART_recieveByte() != MC_READY) { 	// wait until MC1 is ready to receive password
			};
			for (int i = 0; i < Pass_Size; i++) {
				UART_sendByte(pass_to_send[i]);
				_delay_ms(100);
			}
		} else if (command == BUZZER_ON) { 				// if received command from MC1 to turn on the buzzer
			Buzzer_ON();
			while (UART_recieveByte() != BUZZER_OFF) { // wait until MC1 send a command to turn off buzzer
			};
			Buzzer_OFF();
		} else if (command == OPEN) { 			// if received from MC1 to open the door
			DcMotor_Rotate(CW); 				// rotate the motor CW

		} else if (command == CLOSE) { 			// if received from MC1 to close the door
			DcMotor_Rotate(ACW); 				// rotate the motor ACW

			Timer0_init(&timer0_config);
			while(count2 != 458){}; //15 sec
			count2=0;
			Timer0_DeInit();

			DcMotor_Rotate(stop); 				// stop the motor
		} else if (command == WAIT) { 			// leave the door open
			DcMotor_Rotate(stop); 				// stop the motor
		}

	}
}
