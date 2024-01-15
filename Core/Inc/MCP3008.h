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

#define ADC1_FAVG_ROUTE 0
#define ADC1_FAVG_CROISEMENT 1
#define ADC1_FAVG_CLIGNO 2
#define ADC1_FAVG_VEILLE 3

#define ADC1_FAVD_ROUTE 4
#define ADC1_FAVD_CROISEMENT 5
#define ADC1_FAVD_CLIGNO 6
#define ADC1_FAVD_VEILLE 7


#define ADC2_FARG_VEILLE 0
#define ADC2_FARG_STOP 1
#define ADC2_FARG_CLIGNO 2
#define ADC2_FARG_RECUL 3

#define ADC2_FARD_VEILLE 4
#define ADC2_FARD_STOP 5
#define ADC2_FARD_CLIGNO 6
#define ADC2_FARD_RECUL 7

#define ADC3_R_M1 0
#define ADC3_R_M2 1
#define ADC3_S_M1 2
#define ADC3_S_M2 3

#define ADC3_L_M1 4
#define ADC3_FARG_BROUILL 5
#define ADC3_FARD_BROUILL 6


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
	uint8_t ID;
}MCP3008_InitTypeDef;


HAL_StatusTypeDef MCP3008_ReadChannel(MCP3008_InitTypeDef* MCP3008_dev, uint8_t channel_id, uint32_t timeout);
HAL_StatusTypeDef MCP3008_ReadAllChannels(MCP3008_InitTypeDef* MCP3008_dev, uint32_t timeout);
HAL_StatusTypeDef MCP3008_Polling_Benchmark(MCP3008_InitTypeDef* MCP3008_dev, uint8_t channel_id, float* ksps, uint32_t timeout);


#endif /* INC_MCP3008_H_ */
