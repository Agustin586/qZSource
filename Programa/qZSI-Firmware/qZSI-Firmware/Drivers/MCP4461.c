/*
 * Proyecto Final de Ing. Electronica - TM
 *
 * Copyright 2024, Guido Cicconi - Gustavo Garcia - Juan Tinazzo
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 http://ww1.microchip.com/downloads/en/DeviceDoc/22265a.pdf
 ADDR is 0101 aaax
 
 COMMAND byte: AAAACCDD
 AAAA        : Address in memory to do things
 CC          : What to do at AAAA (R, W, Inc, Decr)
     00      : Write
     01      : increment (NV only)
     10      : Decrement (NV only)
     11      : Read
 DD  : D9 and D8 (MSB of the value we are sending)
 
 
 Data unsigned char     : DDDDDDDD ( 0-255 : 00-FF ), this gets used with D8 to give a full range of 00-257 : 0x00-0x100
 8 bit data (8 LSB of the 10 bit data).
 Only read and write commands have data.

 The 9th bit is obtained from the "Command Byte" where D8 is taken to form the 9 bits of data. D9 is unused.
*/

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "MCP4461.h"
#include "../Inits/I2C.h"
#include "../board.h"

/**********************************************************************
                             TYPEDEFS
**********************************************************************/
typedef enum{
    VWIPER0_ADDR = 0x0,         //Volatile Wiper 0
    VWIPER1_ADDR = 0x1,         //Volatile Wiper 1
    NVWIPER0_ADDR = 0x2,        //NV Wiper 0
    NVWIPER1_ADDR = 0x3,        //NV Wiper 1
    TCON0_ADDR = 0x4,           //Volatile TCON 0 Register
    STATUS_ADDR = 0x5,          //Status Register
    VWIPER2_ADDR = 0x6,         //Volatile Wiper 2
    VWIPER3_ADDR = 0x7,         //Volatile Wiper 3
    NVWIPER2_ADDR = 0x8,        //NV Wiper 2
    NVWIPER3_ADDR = 0x9,        //NV Wiper 3
    TCON1_ADDR = 0xA,           //Volatile TCON 1 Register
    DATA0_ADDR = 0xB,           //Data0 EEPROM
    DATA1_ADDR = 0xC,           //Data1 EEPROM
    DATA2_ADDR = 0xD,           //Data2 EEPROM
    DATA3_ADDR = 0xE,           //Data3 EEPROM
    DATA4_ADDR = 0xF            //Data4 EEPROM
}MCP4461Register_t;

typedef enum{
    WRITE_CMD = 0x0,
    INCREMENT_CMD = 0x1,
    DECREMENT_CMD = 0x2,
    READ_CMD = 0x3
}MCP4461Command_t;


/**********************************************************************
                   INTERNAL FUNCTIONS DECLARATION
**********************************************************************/
unsigned char isInWritingCycle(MCP4461Address_t pot);

/**********************************************************************
                   INTERNAL FUNCTIONS DEFINITION
**********************************************************************/
unsigned char isInWritingCycle(MCP4461Address_t pot)
{
    uint16_t status = MCP4461_getStatus(pot);
    return (status & 1<<4) ? 1 : 0;
}

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: MCP4461_init()
* Description: MCP4461 driver init function.
*
**********************************************************************/
extern void MCP4461_init(void)
{
    GpioDataRegs.GPASET.bit.GPIO_DPOT_RESET = 1;    // Saca a los potenciómetros del estado de reset
    InitI2C();
}

/**********************************************************************
* Function: MCP4461_setVolatileWiper()
* Description: function to set a volatile value for a specific wiper
*              of a digital potentiometer.
*
* Usage: MCP4461_setVolatileWiper(DIGPOTx, WIPERx, value);
*
* Input Parameters: DIGPOTx = digital potentiometer ID.
*                   WIPERx = wiper ID
*                   value = value to set
*
**********************************************************************/
extern void MCP4461_setVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper, uint16_t wiper_value)
{
    unsigned char data_byte = 0;
    unsigned char cmd_byte = 0;

    if (wiper_value > 0xFF){
        wiper_value = 0x100;
        cmd_byte = 0x1;
    }

    data_byte = (unsigned char)wiper_value;

    switch (wiper)
    {
    case WIPER0:
        cmd_byte |= VWIPER0_ADDR << 4;
        break;
    case WIPER1:
        cmd_byte |= VWIPER1_ADDR << 4;
        break;
    case WIPER2:
        cmd_byte |= VWIPER2_ADDR << 4;
        break;
    case WIPER3:
        cmd_byte |= VWIPER3_ADDR << 4;
        break;
    default:
        break;
    }

    cmd_byte |= WRITE_CMD << 2;

    i2c_write((unsigned char)pot, cmd_byte, &data_byte, 1);
}

/**********************************************************************
* Function: MCP4461_setNonVolatileWiper()
* Description: function to set a non volatile value for a specific
*              wiper of a digital potentiometer.
*
* Usage: MCP4461_setNonVolatileWiper(DIGPOTx, WIPERx, value);
*
* Input Parameters: DIGPOTx = digital potentiometer ID.
*                   WIPERx = wiper ID
*                   value = value to set
*
**********************************************************************/
extern void MCP4461_setNonVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper, uint16_t wiper_value)
{
    unsigned char data_byte = 0;
    unsigned char cmd_byte = 0;

    if (wiper_value > 0xFF){
        wiper_value = 0x100;
        cmd_byte = 0x1;
    }
    data_byte = (unsigned char)wiper_value;

    switch (wiper)
    {
    case WIPER0:
        cmd_byte |= NVWIPER0_ADDR << 4;
        break;
    case WIPER1:
        cmd_byte |= NVWIPER1_ADDR << 4;
        break;
    case WIPER2:
        cmd_byte |= NVWIPER2_ADDR << 4;
        break;
    case WIPER3:
        cmd_byte |= NVWIPER3_ADDR << 4;
        break;
    default:
        break;
    }

    cmd_byte |= WRITE_CMD << 2;

    while(isInWritingCycle(pot));

    i2c_write((unsigned char)pot, cmd_byte, &data_byte, 1);
}

/**********************************************************************
* Function: MCP4461_getVolatileWiper()
* Description: function to get the volatile value of a specific wiper
*              of a digital potentiometer.
*
* Usage: MCP4461_getVolatileWiper(DIGPOTx, WIPERx);
*
* Input Parameters: DIGPOTx = digital potentiometer ID.
*                   WIPERx = wiper ID
*
* Output value: WIPERx of DIGPOTx volatile value.
**********************************************************************/
extern uint16_t MCP4461_getVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper)
{
    unsigned char cmd_byte = 0;
    unsigned char wiperValueArray[2];
    uint16_t wiperValue = 0;

    switch (wiper)
    {
    case WIPER0:
        cmd_byte |= VWIPER0_ADDR << 4;
        break;
    case WIPER1:
        cmd_byte |= VWIPER1_ADDR << 4;
        break;
    case WIPER2:
        cmd_byte |= VWIPER2_ADDR << 4;
        break;
    case WIPER3:
        cmd_byte |= VWIPER3_ADDR << 4;
        break;
    default:
        break;
    }

    cmd_byte |= READ_CMD << 2;

    i2c_read((unsigned char)pot, cmd_byte, wiperValueArray, 2);

    wiperValue = wiperValueArray[0]<<8 | wiperValueArray[1];

    return wiperValue;
}

/**********************************************************************
* Function: MCP4461_getNonVolatileWiper()
* Description: function to get the non volatile value of a specific
*              wiper of a digital potentiometer.
*
* Usage: MCP4461_getNonVolatileWiper(DIGPOTx, WIPERx);
*
* Input Parameters: DIGPOTx = digital potentiometer ID.
*                   WIPERx = wiper ID
*
* Output value: WIPERx of DIGPOTx non volatile value.
**********************************************************************/
extern uint16_t MCP4461_getNonVolatileWiper(MCP4461Address_t pot, MCP4461Wiper_t wiper)
{
    unsigned char cmd_byte = 0;
    unsigned char wiperValueArray[2];
    uint16_t wiperValue = 0;

    switch (wiper)
    {
    case WIPER0:
        cmd_byte |= NVWIPER0_ADDR << 4;
        break;
    case WIPER1:
        cmd_byte |= NVWIPER1_ADDR << 4;
        break;
    case WIPER2:
        cmd_byte |= NVWIPER2_ADDR << 4;
        break;
    case WIPER3:
        cmd_byte |= NVWIPER3_ADDR << 4;
        break;
    default:
        break;
    }

    cmd_byte |= READ_CMD << 2;

    while(isInWritingCycle(pot));

    i2c_read((unsigned char)pot, cmd_byte, wiperValueArray, 2);

    wiperValue = wiperValueArray[0]<<8 | wiperValueArray[1];

    return wiperValue;
}

/**********************************************************************
* Function: MCP4461_getNonVolatileWiper()
* Description: function to get the status register of a digital
*              potentiometer.
*
* Usage: MCP4461_getStatus(DIGPOTx);
*
* Input Parameters: DIGPOTx = digital potentiometer ID.
*
* Output value: DIGPOTx status register.
**********************************************************************/
extern uint16_t MCP4461_getStatus(MCP4461Address_t pot)
{
    unsigned char cmd_byte = (STATUS_ADDR << 4 | READ_CMD << 2);
    unsigned char statusArray[2];
    uint16_t status = 0;

    i2c_read((unsigned char)pot, cmd_byte, statusArray, 2);

    status = statusArray[0]<<8 | statusArray[1];

    return status;
}
