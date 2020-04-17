#ifndef __AS5048_H__
#define __AS5048_H__

#define LIBRARY_VERSION 1.0.0

#include "HL_spi.h"

bool errorFlag;
uint8 _cs;
uint8 cs;
uint8 dout;
uint8 din;
uint8 clk;
uint16 position;
uint16 transaction(uint16 data);

void as5048(uint8 arg_cs);

/*
* Initialiser
* Sets up the SPI interface
*/
void as5048Init();

/**
	 * Closes the SPI connection
	 */
void close();

/*
	 * Read a register from the sensor
	 * Takes the address of the register as a 16 bit word
	 * Returns the value of the register
	 */
void read(uint16 registerAddress[], uint16 out[], spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig);

/*
	 * Write to a register
	 * Takes the 16-bit  address of the target register and the 16 bit word of data
	 * to be written to that register
	 * Returns the value of the register after the write has been performed. This
	 * is read back from the sensor to ensure a sucessful write.
	 */
uint16 write(uint16 registerAddress, uint16 data, spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig);

/**
	 * Get the rotation of the sensor relative to the zero position.
	 *
	 * @return {int} between -2^13 and 2^13
	 */
int getRotation(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig);

/**
	 * Returns the raw angle directly from the sensor
	 */
void getRawRotation(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig, uint16 out[]);

/**
	 * returns the value of the state register
	 * @return 16 bit word containing flags
	 */
uint16 getState(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig);

/**
	 * Print the diagnostic register of the sensor
	 */
void printState();

/**
	 * Returns the value used for Automatic Gain Control (Part of diagnostic
	 * register)
	 */
uint8 getGain(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig);

/*
	 * Get and clear the error register by reading it
	 */
uint16 getErrors(spiBASE_t *spiREGISTRO, spiDAT1_t *SPIconfig);

/*
	 * Set the zero position
	 */
void setZeroPosition(uint16 arg_position);

/*
	 * Returns the current zero position
	 */
uint16 getZeroPosition();

/*
	 * Check if an error has been encountered.
	 */
bool error();

uint8 spiCalcEvenParity(uint16);
void delaymio(uint32_t DelayMax);

#endif
