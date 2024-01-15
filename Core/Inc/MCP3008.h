/*
 * MCP3008.h
 *
 *  Created on: Dec 5, 2023
 *      Author: sbapk
 */

#ifndef INC_MCP3008_H_
#define INC_MCP3008_H_
#include "stm32f4xx_hal.h"

#define ADC_START 0b10000000
#define ADC_SINGLE_ENDED_MEASURE_MODE 0b01000000

typedef enum
{
  ADC_OK       = 0x00U,
  ADC_XFER_ERROR     = 0x01U,
  ADC_TIMEOUT  = 0x02U,
  ADC_CHANN_ID_OOB = 0x04U
} ADC_StatusTypeDef;

typedef struct{
	SPI_HandleTypeDef* phspi;
	GPIO_TypeDef* pgpio_cs;
	uint16_t pin_nbr;
	ADC_StatusTypeDef status;
	float vref;
	float result[8];
	uint8_t update_request;
}MCP3008_InitTypeDef;


HAL_StatusTypeDef MCP3008_ReadChannel(MCP3008_InitTypeDef* MCP3008_dev, uint8_t channel_id, uint32_t timeout);
HAL_StatusTypeDef MCP3008_ReadAllChannels(MCP3008_InitTypeDef* MCP3008_dev, uint32_t timeout);
HAL_StatusTypeDef MCP3008_Polling_Benchmark(MCP3008_InitTypeDef* MCP3008_dev, uint8_t channel_id, float* ksps, uint32_t timeout);


#endif /* INC_MCP3008_H_ */
