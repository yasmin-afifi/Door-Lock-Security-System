 /******************************************************************************
 *
 * Module: HMI ECU
 *
 * File Name: hmi.h
 *
 * Description: Source file for HMI ECU
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/

#include"lcd.h"
#include"keypad.h"
#include "uart.h"
#include"timer0.h"
#include <util/delay.h>
#include "avr/io.h"
#include "gpio.h"

/*******************************************************************************
 *                              Definitions                                  *
 *******************************************************************************/

#define Pass_Size				5
#define MC_READY 				0xFF
#define STORE_EEPROM 			0xF1
#define OPEN 					0xF2
#define CLOSE 					0xF3
#define WAIT 					0xF5
#define BUZZER_ON 				0xF6
#define BUZZER_OFF 				0xF7

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*global variable to count overflows*/
uint32 count1 = 0;

void timer0_count1(void) {
	count1++;
}

void Password(uint8 *new_password) {
	uint8 key_num;

	for (int i = 0; i < Pass_Size; i++) {
		key_num = KEYPAD_getPressedKey();
		if ((key_num >= 0) && (key_num <= 9)) {
			new_password[i] = key_num;
			LCD_displayCharacter('*');
			while((GPIO_readPin(PORTB_ID, PIN0_ID)==0)||(GPIO_readPin(PORTB_ID, PIN1_ID)==0)||(GPIO_readPin(PORTB_ID, PIN2_ID)==0)||(GPIO_readPin(PORTB_ID, PIN3_ID)==0));	//waiting till the key is released
		}

	}
}

void send_password(uint8 *new_password)
{
	for (int i = 0; i < Pass_Size; i++) {
		UART_sendByte(new_password[i]);
		_delay_ms(100);
	}
}

void newPassword(uint8 match_flag, uint8 *password1, uint8 *password2) {

	uint8 correct_flag = 0; // to check if the two passwords are the same
	while (correct_flag == 0) {
		LCD_clearScreen();
		LCD_displayString("New Password:");
		LCD_moveCursor(1, 0);
		UART_sendByte(STORE_EEPROM);
		Password(password1);

		LCD_clearScreen();
		send_password(password1);


		LCD_clearScreen();
		LCD_displayString("Re-enter pass:");
		LCD_moveCursor(1, 0);
		Password(password2);


		LCD_clearScreen();
		for (int i = 0; i < Pass_Size; i++) {
			if (password1[i] == password2[i]) {
				match_flag = 0;
			} else {
				match_flag = 1;
				break;
			}
		}
		if (match_flag == 0) {
			LCD_displayString("Saved");
			_delay_ms(500);
			LCD_clearScreen();
			correct_flag = 1;
		} else {
			LCD_clearScreen();
			LCD_displayString("Wrong pass");
			_delay_ms(500);
			LCD_clearScreen();
		}


	}
}

void MainMenu(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+:Open door");
	LCD_displayStringRowColumn(1, 0, "-:change pass");

}

int main(void) {

	uint8 new_password[Pass_Size]; 					//new entered password
	uint8 new_password_repeated[Pass_Size]; 		// enter the password again
	uint8 eeprom_password[Pass_Size]; 				//password sent by the MC2 from eeprom
	uint8 old_password[Pass_Size]; 					// old password entered by the user
	uint8 matched_flag = 0; 						// set when the two new passwords don't match
	uint8 matched_flag2 = 0; 						// set when the two old passwords don't match
	uint8 error_flag = 0; 							// to count the number of wrongly entered passwords
	uint8 correct_flag = 0; 						// while it is 0 we repeat the loop again until entering correct password

	// Enable interrupts by setting I-bit
	SREG |= (1 << 7);

	// Initialize LCD driver
	LCD_init();


	UART_ConfigType uart_config = { 9600, F_8_BIT, S_1_BIT, DISABLED};


	UART_init(&uart_config);		//Initialize UART Driver


	Timer0_ConfigType timer0_config = { Normal,F_CPU_1024, 0, 0};
	Timer0_setCallBack(timer0_count1);

	// create new password
	newPassword(matched_flag, new_password, new_password_repeated);
	matched_flag = 0;

	// recieve password from eeprom interfaced with MC2
	UART_sendByte(MC_READY);
	for (int i = 0; i < 5; i++) {
		eeprom_password[i] = UART_recieveByte();
	}

	while (1) {

		MainMenu();

		if (KEYPAD_getPressedKey() == '-') {
			while ((error_flag < 3) && (correct_flag == 0)) /* we can enter wrong password 3 times only */
			{
				LCD_clearScreen();
				LCD_displayString("Old Password:");
				LCD_moveCursor(1, 0);
				Password(old_password);

				for (int i = 0; i < Pass_Size; i++) {
					if (old_password[i] == eeprom_password[i]) {
						matched_flag2 = 0;
					} else {
						matched_flag2 = 1;
						break;
					}
				}
				if (matched_flag2 == 0) {
					LCD_clearScreen();
					LCD_displayString("Correct Pass");
					_delay_ms(400);
					LCD_clearScreen();
					newPassword(matched_flag, new_password,new_password_repeated);
					matched_flag = 0;
					UART_sendByte(MC_READY);
					for (int i = 0; i < Pass_Size; i++) {
						eeprom_password[i] = UART_recieveByte();
					}
					correct_flag = 1;
				} else {
					LCD_clearScreen();
					LCD_displayString("Wrong pass");
					_delay_ms(400);
					error_flag++;
					if (error_flag == 3) {
						UART_sendByte(BUZZER_ON);
						LCD_clearScreen();
						LCD_displayString("!!!LOCKED!!!");

						Timer0_init(&timer0_config);
						while(count1 != 1832){}; //1min
						count1=0;
						Timer0_DeInit();

						LCD_clearScreen();
						UART_sendByte(BUZZER_OFF);
					}

				}


			}
			matched_flag2 = 0;
			error_flag = 0;
			correct_flag = 0;

		} else if (KEYPAD_getPressedKey() == '+') {

			while ((error_flag < 3) && (correct_flag == 0)) {
				LCD_clearScreen();
				LCD_displayString("Old Password:");
				LCD_moveCursor(1, 0);
				Password(old_password);

				for (int i = 0; i < Pass_Size; i++) {
					if (old_password[i] == eeprom_password[i]) {
						matched_flag2 = 0;
					} else {
						matched_flag2 = 1;
						break;
					}
				}
				if (matched_flag2 == 0) {
					LCD_clearScreen();
					LCD_displayString("Opening...");
					UART_sendByte(OPEN);

					Timer0_init(&timer0_config);
					while(count1 != 458){};  //15 sec
					count1=0;
					Timer0_DeInit();

					LCD_clearScreen();
					LCD_displayString("Opened");
					UART_sendByte(WAIT);

					Timer0_init(&timer0_config);
					while(count1 != 92){};  // 3 sec
					count1=0;
					Timer0_DeInit();

					LCD_clearScreen();
					LCD_displayString("Closing...");
					UART_sendByte(CLOSE);

					Timer0_init(&timer0_config);
					while(count1 != 458){};   //15 sec
					count1=0;
					Timer0_DeInit();

					LCD_clearScreen();
					correct_flag = 1;
				} else {
					LCD_clearScreen();
					LCD_displayString("Wrong Password");
					_delay_ms(400);
					error_flag++;
					if (error_flag == 3) {
						UART_sendByte(BUZZER_ON);
						LCD_clearScreen();
						LCD_displayString("!!!LOCKED!!!");

						Timer0_init(&timer0_config);
						while(count1 != 1832){}; //1min
						count1=0;
						Timer0_DeInit();

						LCD_clearScreen();
						UART_sendByte(BUZZER_OFF);

					}

				}

			}
			matched_flag2 = 0;
			error_flag = 0;
			correct_flag = 0;
		}
	}
}
