 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Yasmin Khaled
 *
 *******************************************************************************/
 
#include "twi.h"

#include "common_macros.h"
#include <avr/io.h>


/*
 * Description :
 * Calculating the value stored in TWBR register
 */
uint8 TWBR_Calculation(uint32 scl_freq, uint8 prescaler)
{
	uint8 TWBR_value;
	TWBR_value = (((F_CPU*8)/(scl_freq))-16)/(2*(prescaler));
	return TWBR_value;
}

/*
 * Description :
 * Functional responsible for Initialize the TWI device by:
 * 1. Bit Rate
 * 2. Prescaler
 * 3. Giving the device an address
 * 4. Enable TWI
 */
void TWI_init(const TWI_ConfigType * Config_Ptr)
{
    /* Passing the values of bit rate, prescaler to TWBR_Calculation function
     * to calculate the value of TWBR register */
	TWBR = TWBR_Calculation(Config_Ptr->SCL_freq,Config_Ptr->prescaler);


	TWSR = Config_Ptr->prescaler;
	
    /* Two Wire Bus address my address if any master device want to call me:  (used in case this MC is a slave device)
       General Call Recognition: Off */
    TWAR = ((Config_Ptr-> myaddress)<<1);
	
    TWCR = (1<<TWEN); /* enable TWI */
}

/*
 * Description :
 * 1. Clear the TWINT flag before sending the start bit TWINT=1
 * 2. send the start bit by TWSTA=1
 * 3. Enable TWI Module TWEN=1
 */
void TWI_start(void)
{

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 * 1. Clear the TWINT flag before sending the start bit TWINT=1
 * 2. send the stop bit by TWSTO=1
 * 3. Enable TWI Module TWEN=1
 */
void TWI_stop(void)
{

    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*
 * Description :
 * Put data On TWI data Register
 */
void TWI_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 * Read Data and sending ACK
 */
uint8 TWI_readByteWithACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 * Description :
 * Read Data and sending NACK
 */
uint8 TWI_readByteWithNACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 * Description :
 * Getting status of TWI
 */
uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
