/*
 * MCP23008.h
 *
 *  Created on: Jan 9, 2024
 *      Author: sbapk
 */

#ifndef INC_MCP23008_H_
#define INC_MCP23008_H_
#include "stm32f4xx_hal.h"

typedef enum
{
	GPIO_OK = 0x00U,
	GPIO_CHANN_ID_OOB = 0x04U
}GPIO_StatusTypeDef;

typedef struct{
	I2C_HandleTypeDef* hi2c;
	uint8_t address;
	uint8_t currentStatus;

}MCP23008_InitTypeDef;


HAL_StatusTypeDef MCP23008_Setup(MCP23008_InitTypeDef* MCP23008_dev,uint8_t IODIR, uint32_t timeout);
HAL_StatusTypeDef MCP23008_ReadPin(MCP23008_InitTypeDef* MCP23008_dev, uint8_t channel_id, uint8_t* presult, uint32_t timeout);
HAL_StatusTypeDef MCP23008_ReadAllPin(MCP23008_InitTypeDef* MCP23008_dev,  uint32_t timeout);
HAL_StatusTypeDef MCP23008_WritePin(MCP23008_InitTypeDef* MCP23008_dev, uint8_t channel_id, uint8_t data, uint32_t timeout);
HAL_StatusTypeDef MCP23008_WritePort(MCP23008_InitTypeDef* MCP23008_dev, uint8_t data, uint32_t timeout);

#endif /* INC_MCP23008_H_ */
