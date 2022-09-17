 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	FCPU ,FCPU_4, FCPU_16, FCPU_64
}TWI_Prescaler;


typedef struct
{
	TWI_Prescaler prescaler;
	uint8 myaddress;
	uint32 SCL_freq;
}TWI_ConfigType;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Calculating the value stored in TWBR register
 */
uint8 TWBR_Calculation(uint32 scl_freq, uint8 prescaler);

/*
 * Description :
 * Functional responsible for Initialize the TWI device by:
 * 1. Bit Rate
 * 2. Prescaler
 * 3. Giving the device an address
 * 4. Enable TWI
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*
 * Description :
 * 1. Clear the TWINT flag before sending the start bit TWINT=1
 * 2. send the start bit by TWSTA=1
 * 3. Enable TWI Module TWEN=1
 */
void TWI_start(void);

/*
 * Description :
 * 1. Clear the TWINT flag before sending the start bit TWINT=1
 * 2. send the stop bit by TWSTO=1
 * 3. Enable TWI Module TWEN=1
 */
void TWI_stop(void);

/*
 * Description :
 * Put data On TWI data Register
 */
void TWI_writeByte(uint8 data);

/*
 * Description :
 * Read Data and sending ACK
 */
uint8 TWI_readByteWithACK(void);

/*
 * Description :
 * Read Data and sending NACK
 */
uint8 TWI_readByteWithNACK(void);

/*
 * Description :
 * Getting status of TWI
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
