#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdbool.h>
//include the Correct HAL version here according to the Cortex core.
//#include "stm32f1xx_hal.h"
#include "stm32f3xx_hal.h"

// Define the I2C
extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C &hi2c1

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// Define the Page Size and number of pages
#define PAGE_SIZE 64     // in Bytes
#define PAGE_NUM  512    // number of pages

//################################################################################################################
uint16_t bytestowrite (uint16_t size, uint16_t offset);
void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable);
float Bytes2float(uint8_t * ftoa_bytes_temp);
void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float data);
float EEPROM_Read_NUM (uint16_t page, uint16_t offset);
void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_PageErase (uint16_t page);
//################################################################################################################

#endif
