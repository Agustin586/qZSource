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

/**********************************************************************
                             INCLUDES
**********************************************************************/
#include "I2C.h"

/**********************************************************************
                   EXTERNAL FUNCTIONS DEFINITION
**********************************************************************/

/**********************************************************************
* Function: InitI2C()
*
* Description: Initializes the I2C module on the F2833x
**********************************************************************/
extern void InitI2C(void)
{
    //---------------------------------------------------------------------
    //--- Disable I2C module clock before configuration and disable it
    //---------------------------------------------------------------------
    asm(" EALLOW");						// Enable EALLOW protected register access
    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;
    I2caRegs.I2CMDR.bit.IRS = 0;

    // Direcci√≥n propia si funciona como slave
    I2caRegs.I2COAR = 0x0;

    // Seteo del clock del m√≥dulo (10Mhz) y de SCL (100kHz)
    I2caRegs.I2CPSC.all = 14;   // Divido 150Mhz por (14+1) --> 10MHz
    I2caRegs.I2CCLKL = 45;      // Divido 10 MHz por 45+5 (tiempo en bajo de SCL)
    I2caRegs.I2CCLKH = 45;      // Divido 10 MHz por 45+5 (tiempo en alto de SCL)

    // Interrupt enables
    I2caRegs.I2CIER.bit.AAS = 0;  // Addressed as slave (reconozco mi direcci√≥n en SDA)
    I2caRegs.I2CIER.bit.SCD = 0;  // Stop condition detected
    I2caRegs.I2CIER.bit.XRDY = 0; // Transmit-data ready
    I2caRegs.I2CIER.bit.RRDY = 0; // Receive-data ready
    I2caRegs.I2CIER.bit.ARDY = 0; // Register-access ready
    I2caRegs.I2CIER.bit.NACK = 0; // No-ack
    I2caRegs.I2CIER.bit.ARBL = 0; // Arbitration lost

    // Status register
    I2caRegs.I2CSTR.bit.SDIR = 0;       // Slave direction - 0: receiver
    I2caRegs.I2CSTR.bit.NACKSNT = 1;    // Nack sent flag - W1C
    I2caRegs.I2CSTR.bit.BB = 0;         // Bus busy flag - Read only
    I2caRegs.I2CSTR.bit.RSFULL = 0;     // Overrun flag - Read only
    I2caRegs.I2CSTR.bit.XSMT = 0;       // Tx shift register empty flag - Read only (1: empty)
    I2caRegs.I2CSTR.bit.AAS = 0;        // Addressed as a slave flag - Read only
    I2caRegs.I2CSTR.bit.AD0 = 0;        // Address of all 0's detected flag - Read only
    I2caRegs.I2CSTR.bit.SCD = 0;        // Se setea cuando el modulo recive o env√≠a una stop condition - W1C
    I2caRegs.I2CSTR.bit.XRDY = 0;       // Transmit-data-ready flag - Read  Only
    I2caRegs.I2CSTR.bit.RRDY = 1;       // Receive-data-ready flag - W1C (tambi√©n clear autom√°tico al leer I2CDRR)
    I2caRegs.I2CSTR.bit.ARDY = 1;       // Register-access-ready flag - W1C
    I2caRegs.I2CSTR.bit.NACK = 1;       // No-acknowledgement flag - W1C
    I2caRegs.I2CSTR.bit.ARBL = 1;       // Arbitration lost flag - W1C

    // Count register -- se indica la cantidad de datos a transmitir o recibir -- CONFIGURAR EN TRANSACCION
    I2caRegs.I2CCNT = 0;

    // Data received register -- se guarda el dato recibido (8 bit) -- LEER EN TRANSACCION
    I2caRegs.I2CDRR = 0;

    // Slave address register -- direcci√≥n del slave en transacci√≥n -- ESCRIBIR EN TRANSACCION
    I2caRegs.I2CSAR = 0;

    // Data to transmit register -- dato a transmitir (8 bit) -- ESCRIBIR EN TRANSACCION
    I2caRegs.I2CDXR = 0;

    // Configuraci√≥n del modo de trabajo del I2C
    I2caRegs.I2CMDR.bit.NACKMOD = 0; // Enviar acknowledge cuando se recibe data
    I2caRegs.I2CMDR.bit.FREE = 1;    // El m√≥dulo I2C sigue funcionando aunque haya un breakpoint
    I2caRegs.I2CMDR.bit.RM = 0;      // Repeat mode desactivado, largo de transacciones determinado por registro I2CCNT
    I2caRegs.I2CMDR.bit.STT = 0;     // Si se pone en 1, se genera condici√≥n de start en el bus
    I2caRegs.I2CMDR.bit.STP = 0;     // Si se pone en 1, se genera condici√≥n de stop en el bus cuando se hayan transmitido I2CCNT datos
    I2caRegs.I2CMDR.bit.MST = 1;     // I2C en modo master
    I2caRegs.I2CMDR.bit.TRX = 1;     // I2C en modo transmisor
    I2caRegs.I2CMDR.bit.XA = 0;      // 7-bit addessing
    I2caRegs.I2CMDR.bit.DLB = 0;     // Digital loop-back disabled
    I2caRegs.I2CMDR.bit.STB = 0;     // Start byte disabled
    I2caRegs.I2CMDR.bit.FDF = 0;     // Free data format disabled
    I2caRegs.I2CMDR.bit.BC = 0;      // 8-bits por data byte

    // Interrupt source -- indica por qu√© se dio la interrupci√≥n -- LEER SI SE TRABAJA CON IRQ
    I2caRegs.I2CISRC.all = 0;

    // Configuraci√≥n de FIFO transmit
    I2caRegs.I2CFFTX.bit.I2CFFEN = 0;   // Modo FIFO deshabilitado
    I2caRegs.I2CFFTX.bit.TXFFRST = 0;   // Enable the transmit FIFO operation
    I2caRegs.I2CFFTX.bit.TXFFIENA = 0;  // Transmit FIFO interrupt disabled
    I2caRegs.I2CFFTX.bit.TXFFIL = 0;    // Level that sets the interrupt flag (if enabled)

    // Configuraci√≥n de FIFO receive
    I2caRegs.I2CFFRX.bit.RXFFRST = 0;   // Enable the receive FIFO operation
    I2caRegs.I2CFFRX.bit.RXFFIENA = 0;  // Receive FIFO interrupt disabled
    I2caRegs.I2CFFRX.bit.RXFFIL = 0;    // Level that sets the interrupt flag (if enabled)

    //---------------------------------------------------------------------
    //--- Enable I2C interrupts
    //---------------------------------------------------------------------
    //PieCtrlRegs.PIEIER8.bit.INTx1 = 1;  // Enable I2CINT1A in PIE group 8
    //PieCtrlRegs.PIEIER8.bit.INTx2 = 1;  // Enable I2CINT2A in PIE group 8
    //IER |= M_INT8;                      // Enable INT8 in IER to enable PIE group

    //---------------------------------------------------------------------
    //--- Enable the clocks to the I2C module and enable it.
    //---------------------------------------------------------------------
    I2caRegs.I2CMDR.bit.IRS = 1;
    asm(" EDIS");                           // Disable EALLOW protected register access
}


/**********************************************************************
* Function: i2c_write()
* Description: Writes a slave register - blocking function.
*
* Usage: i2c_write(slaveAddress, regAddr, data, dataLength);
* Input Parameters: slaveAddress = I2C address of the slave.
*                   regAddr = address of the register to write.
*                   data = pointer to array where data to write is
*                   stored.
*                   dataLength = length of the data in bytes.
**********************************************************************/
extern void i2c_write(unsigned char slaveAddress, unsigned char regAddr, unsigned char *data, int dataLength){
    int i = 0;

    while(I2caRegs.I2CSTR.bit.BB);                  // Esperar a que el bus este disponible

    I2caRegs.I2CMDR.bit.MST = 1;                    // Master mode
    I2caRegs.I2CMDR.bit.TRX = 1;                    // Modo transmisor
    I2caRegs.I2CSAR  = (slaveAddress & 0x7F);       // Cargo la direcciÛn del slave
    I2caRegs.I2CCNT = ((dataLength+1) & 0xFFFF);    // Cargo la cantidad de datos a transmitir

    I2caRegs.I2CMDR.bit.STP = 1;                    // Cuando termine la transacciÛn se dar· bit de stop
    I2caRegs.I2CMDR.bit.STT = 1;                    // Inicio transacciÛn con start

    while(!I2caRegs.I2CSTR.bit.XRDY);               // Esperar a que estÈ vacÌo el registro de datos a Tx
    I2caRegs.I2CDXR = regAddr;                      // Cargo el primer dato

    // EnvÌo el resto de los datos
    for(i=0; i<dataLength; i++){
        while(!I2caRegs.I2CSTR.bit.XRDY);
        I2caRegs.I2CDXR = data[i];
    }

    // Delay (si no esta se tilda el I2C)
    for(i=0; i<800; i++){
        __asm(" RPT #255 || NOP");
    }

    while(I2caRegs.I2CSTR.bit.BB);                  // Esperar a que el bus este disponible
}

/**********************************************************************
* Function: i2c_read()
* Description: Reads a slave register - blocking function.
*
* Usage: i2c_read(slaveAddress, regAddr, data, dataLength);
* Input Parameters: slaveAddress = I2C address of the slave.
*                   regAddr = address of the register to write.
*                   data = pointer to array where data to read would be
*                   stored.
*                   dataLength = length of the data in bytes.
**********************************************************************/
extern void i2c_read(unsigned char slaveAddress, unsigned char regAddr, unsigned char *data, int dataLength){
    int i = 0;

    while(I2caRegs.I2CSTR.bit.BB);                  // Esperar a que el bus este disponible

    I2caRegs.I2CMDR.bit.MST = 1;                    // Master mode
    I2caRegs.I2CMDR.bit.TRX = 1;                    // Modo transmisor
    I2caRegs.I2CSAR  = (slaveAddress & 0x7F);       // Cargo la direcciÛn del slave
    I2caRegs.I2CCNT = 1;                            // Cargo la cantidad de datos a transmitir

    I2caRegs.I2CMDR.bit.STP = 0;                    // Cuando termine la transaccion no se dara bit de stop
    I2caRegs.I2CMDR.bit.STT = 1;                    // Inicio transaccion con start

    while(!I2caRegs.I2CSTR.bit.XRDY);               // Esperar a que estÈ vacÌo el registro de datos a Tx
    I2caRegs.I2CDXR = regAddr;                      // Transmito la direcciÛn del registro del esclavo que quiero leer

    while(!I2caRegs.I2CSTR.bit.XRDY);               // Esperar a que estÈ vacÌo el registro de datos a Tx

    I2caRegs.I2CMDR.bit.TRX = 0;                    // Modo receptor
    I2caRegs.I2CCNT = (dataLength & 0xFFFF);        // Cargo la cantidad de datos a recibir

    I2caRegs.I2CMDR.bit.STP = 1;                    // Cuando termine la transaccion se dara bit de stop
    I2caRegs.I2CMDR.bit.STT = 1;                    // EnvÌo Restart

    // Recibo los datos
    for(i=0; i<dataLength; i++){
        while(!I2caRegs.I2CSTR.bit.RRDY);
        data[i] = I2caRegs.I2CDRR;
    }

    // Delay (si no esta se tilda el I2C)
    for(i=0; i<800; i++){
        __asm(" RPT #255 || NOP");
    }

    while(I2caRegs.I2CSTR.bit.BB);                  // Esperar a que el bus este disponible
}
