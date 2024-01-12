/*
 * MCP23008.c
 *
 *  Created on: Jan 9, 2024
 *      Author: sbapk
 */
#include "MCP23008.h"

HAL_StatusTypeDef MCP23008_Setup(MCP23008_InitTypeDef* MCP23008_dev,uint8_t IODIR, uint32_t timeout){

  uint8_t data[2] = {0};

  data[0] = 0x00; // IODIR register
  data[1] = IODIR;
  HAL_StatusTypeDef retval = HAL_OK;
  retval = HAL_I2C_Master_Transmit(MCP23008_dev->hi2c, MCP23008_dev->address, data, 2, 1000);
  if (retval != HAL_OK) {
	  return retval;
  }

  data[0] = 0x09; // GPIO register
  data[1] = 0x00; // all outputs low
  retval = HAL_I2C_Master_Transmit(MCP23008_dev->hi2c,MCP23008_dev->address, data, 2, 1000);
  if (retval != HAL_OK) {
	  return retval;
  }
  MCP23008_dev->currentStatus = 0b00000000;
  return HAL_OK;

}


HAL_StatusTypeDef MCP23008_ReadPin(MCP23008_InitTypeDef* MCP23008_dev, uint8_t channel_id, uint8_t* presult, uint32_t timeout){
	uint8_t GPIO_Status = 0;

	if(HAL_I2C_Mem_Read(MCP23008_dev->hi2c,MCP23008_dev->address,0x09,1,&GPIO_Status,1,100) != HAL_OK) return HAL_ERROR;
	if( GPIO_Status| (1<<channel_id > 0 ) ){
		*presult = 1;
	}
	else{
		*presult = 0;
	}
	MCP23008_dev->currentStatus = GPIO_Status;


	return HAL_OK;
}
HAL_StatusTypeDef MCP23008_ReadAllPin(MCP23008_InitTypeDef* MCP23008_dev, uint32_t timeout){

	if(HAL_I2C_Mem_Read(MCP23008_dev->hi2c,MCP23008_dev->address,0x09,1,&MCP23008_dev->currentStatus,1,100) != HAL_OK) return HAL_ERROR;
	return HAL_OK;
}
HAL_StatusTypeDef MCP23008_WritePin(MCP23008_InitTypeDef* MCP23008_dev, uint8_t channel_id, uint8_t data, uint32_t timeout){
	uint8_t GPIO_Status = 0;
	if(HAL_I2C_Mem_Read(MCP23008_dev->hi2c,MCP23008_dev->address,0x09,1,&GPIO_Status,1,100) != HAL_OK) return HAL_ERROR;
	if(data == 0) GPIO_Status &= ~(1 << channel_id);
	else GPIO_Status |= 1<<channel_id;
	if(HAL_I2C_Mem_Write(MCP23008_dev->hi2c,MCP23008_dev->address,0x09,1,&GPIO_Status,1,100) != HAL_OK) return HAL_ERROR;
	MCP23008_dev->currentStatus = GPIO_Status;
	return HAL_OK;
}
HAL_StatusTypeDef MCP23008_WritePort(MCP23008_InitTypeDef* MCP23008_dev, uint8_t data, uint32_t timeout){
	if(HAL_I2C_Mem_Write(MCP23008_dev->hi2c,MCP23008_dev->address,0x09,1,&data,1,100) != HAL_OK) return HAL_ERROR;

	MCP23008_dev->currentStatus = data;
	return HAL_OK;
}
