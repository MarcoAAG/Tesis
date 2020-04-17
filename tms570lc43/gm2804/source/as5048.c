#include "as5048.h"
#include "HL_sys_core.h"

const int AS5048A_CLEAR_ERROR_FLAG = 0x0001;
const int AS5048A_PROGRAMMING_CONTROL = 0x0003;
const int AS5048A_OTP_REGISTER_ZERO_POS_HIGH = 0x0016;
const int AS5048A_OTP_REGISTER_ZERO_POS_LOW = 0x0017;
const int AS5048A_DIAG_AGC = 0x3FFD;
const int AS5048A_MAGNITUDE = 0x3FFE;
uint16 AS5048A_ANGLE[1] = {0x3FFF};
// uint16_t value[0];

// spiDAT1_t SPI1_data_configCh0;

void as5048(uint8 arg_cs)
{
    cs = arg_cs; //pin cs
    errorFlag = FALSE;
    position = 0;
}

/**
 * Initialiser
 * Sets up the SPI interface
 */
// void as5048Init(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
// {
//     uint16 ComandoSPI[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
//     uint16 DatoSPI[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//     // CONFIGURACI�N DE CTRL0 #define CTRL0 0x1F00
//     ComandoSPI[0] = escribir | CTRL0_LSM303D | 0x0000; // ENCENDEMOS, byte alto registro byte bajo valor
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL1 #define CTRL1 0x2000
//     ComandoSPI[0] = escribir | CTRL1_LSM303D | 0x0067; //
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL2 #define CTRL2 0x2100
//     ComandoSPI[0] = escribir | CTRL2_LSM303D | 0x00D0; //
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL3 #define CTRL3 0x2200
//     ComandoSPI[0] = escribir | CTRL3_LSM303D | 0x0000; //
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL4 #define CTRL4 0x2300
//     ComandoSPI[0] = escribir | CTRL4_LSM303D | 0x0000; //
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL5 #define CTRL5 0x2400
//     ComandoSPI[0] = escribir | CTRL5_LSM303D | 0x00F4; // encendemos TPM, resoluci�n y rate del magnet�metro
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL6 #define CTRL6 0x2500
//     ComandoSPI[0] = escribir | CTRL6_LSM303D | 0x0060; //
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);
//     hacernada(1000);
//     // CONFIGURACI�N DE CTRL7 #define CTRL7 0x2600
//     ComandoSPI[0] = escribir | CTRL7_LSM303D | 0x0080; //
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, ComandoSPI, DatoSPI);

//     /*
//     IMPORTANTE !!!!!!!!!!!!!!!!!!1111
//     DESPUES DE ESTO ESTABLECER UN TIEMPO DE
//     1000
//     */
// }
// void as5048Init()
// {
//     spiInit();
// }

// void close()
// {
// }

/**
 * Utility function used to calculate even parity of word
 */
uint8 spiCalcEvenParity(uint16 value)
{
    uint8 cnt = 0;
    uint8 i;

    for (i = 0; i < 16; i++)
    {
        if (value & 0x1)
        {
            cnt++;
        }
        value >>= 1;
    }
    return cnt & 0x1;
}

// /**
//  * Get the rotation of the sensor relative to the zero position.
//  *
//  * @return {int} between -2^13 and 2^13
//  */
// int getRotation(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
// {
//     uint16 data;
//     int rotation;

//     data = getRawRotation(spiREGISTRO, SPIconfig);
//     rotation = (int)data - (int)position;
//     if (rotation > 8191)
//     {
//         rotation = -((0x3FFF) - rotation);
//     }
//     return rotation;
// }

// /**
//  * Returns the raw angle directly from the sensor
//  */
void getRawRotation(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig, uint16 out[])
{
    read(AS5048A_ANGLE, out, spiREGISTRO, SPIconfig);
}

// /**
//  * returns the value of the state register
//  * @return 16 bit word containing flags
//  */
// uint16 getState(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
// {
//     return read(AS5048A_DIAG_AGC, spiREGISTRO, SPIconfig);
// }

// /**
//  * Print the diagnostic register of the sensor
//  */
// void printState()
// {
// }

// /**
//  * Returns the value used for Automatic Gain Control (Part of diagnostic
//  * register)
//  */
// uint8 getGain(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
// {
//     uint16 data = getState(spiREGISTRO, SPIconfig);
//     return (uint8)data & 0xFF;
// }

// /*
//  * Get and clear the error register by reading it
//  */
// uint16 getErrors(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
// {
//     return read(AS5048A_CLEAR_ERROR_FLAG, spiREGISTRO, SPIconfig);
// }

// /*
//  * Set the zero position
//  */
// void setZeroPosition(uint16 arg_position)
// {
//     position = arg_position % 0x3FFF;
// }

// /*
//  * Returns the current zero position
//  */
// uint16 getZeroPosition()
// {
//     return position;
// }

// /*
//  * Check if an error has been encountered.
//  */
// bool error()
// {
//     return errorFlag;
// }

/*
 * Read a register from the sensor
 * Takes the address of the register as a 16 bit word
 * Returns the value of the register
 */
void read(uint16 registerAddress[], uint16 out[], spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
{
    uint16 command[1] = {0b0100000000000000}; // PAR=0 R/W=R
    command[0] = command[0] | registerAddress[0];

    // Add a parity bit on the the MSB
    // command[0] |= ((uint16)spiCalcEvenParity(command) << 15);

    // //Split the command into two bytes
    // uint8 right_byte[1] = command[0] & 0xFF;
    // uint8 left_byte = (command[0] >> 8) & 0xFF;
    command[0] = command[0] & 0xFFFF;

    //beginTransaction

    //Send the command
    //cs LOW
    // spiSendData(spiREGISTRO, SPIconfig, (uint32)1, left_byte);
    // spiSendData(spiREGISTRO, SPIconfig, (uint32)1, right_byte);
    //sc HIGH

    //Now read the response
    //cs LOW
    spiSendAndGetData(spiREGISTRO, SPIconfig, 1, registerAddress, out);
    // delaymio(20000);

    // spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, command, right_byte);
    // delaymio(20000);

    //cs_High

    // endTransaction

    //Check if the error bit is set
    //     if (left_byte & 0x40)
    //     {
    // #ifdef AS5048A_DEBUG
    //         Serial.println("Setting error bit");
    // #endif
    //         errorFlag = true;
    //     }
    //     else
    //     {
    //         errorFlag = false;
    //     }

    //Return the data, stripping the parity and error bits
    // return (((left_byte & 0xFF) << 8) | (right_byte & 0xFF)) & ~0xC000;
    // return value;
}

// uint16 write(uint16 registerAddress, uint16 data, spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig)
// {
//     uint16 command = 0b0000000000000000; // PAR=0 R/W=W
//     command |= registerAddress;

//     //Add a parity bit on the the MSB
//     command |= ((uint16)spiCalcEvenParity(command) << 15);

//     //Split the command into two bytes
//     uint8 right_byte = command & 0xFF;
//     uint8 left_byte = (command >> 8) & 0xFF;

//     //beginTransaction

//     //Start the write command with the target address
//     //cs LOW
//     spiSendData(spiREGISTRO, SPIconfig, (uint32)1, left_byte);
//     spiSendData(spiREGISTRO, SPIconfig, (uint32)1, right_byte);
//     //cs HIGH

//     uint16 dataToSend = 0b0000000000000000;
//     dataToSend |= data;

//     //Craft another packet including the data and parity
//     dataToSend |= ((uint16)spiCalcEvenParity(dataToSend) << 15);
//     right_byte = dataToSend & 0xFF;
//     left_byte = (dataToSend >> 8) & 0xFF;

//     //Now send the data packet
//     //csLOW
//     spiSendData(spiREGISTRO, SPIconfig, (uint32)1, left_byte);
//     spiSendData(spiREGISTRO, SPIconfig, (uint32)1, right_byte);
//     //csHIGH

//     //Send a NOP to get the new data in the register
//     //csLOW
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, 0x00, left_byte);
//     spiSendAndGetData(spiREGISTRO, SPIconfig, (uint32)1, 0x00, right_byte);
//     //cs_High

//     //endTransaction

//     //Return the data, stripping the parity and error bits
//     return (((left_byte & 0xFF) << 8) | (right_byte & 0xFF)) & ~0xC000;
// }
void delaymio(uint32_t DelayMax)
{
    uint32_t id = 0;
    for (id = 0; id < DelayMax; ++id)
    {
        asm(" nop");
    }
}
