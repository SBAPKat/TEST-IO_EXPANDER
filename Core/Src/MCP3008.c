/*
 * MCP3008_dev.c
 *
 *  Created on: Dec 5, 2023
 *      Author: sbapk
 */
#include "MCP3008.h"

HAL_StatusTypeDef MCP3008_ReadChannel(MCP3008_InitTypeDef* MCP3008_dev, uint8_t channel_id, float* result, uint32_t timeout){
	uint8_t TxData[3] = {0};
	uint8_t RxData[3] = {0};
	uint16_t RxData_16b = 0;
	MCP3008_dev->status = ADC_OK;

	if(channel_id > 7){
		/* Channel id out of bounds */
		MCP3008_dev->status |= ADC_CHANN_ID_OOB;
		return HAL_ERROR;
	}

	/* Building data to be sent, start bit + measure mode + channel ID */
	TxData[0] |= ADC_START | ADC_SINGLE_ENDED_MEASURE_MODE | ((channel_id& 0x07)<<3);

	/* Select chip */
	HAL_GPIO_WritePin(MCP3008_dev->pgpio_cs, MCP3008_dev->pin_nbr, GPIO_PIN_RESET);


	/* Start transfer */
	if( HAL_SPI_TransmitReceive(MCP3008_dev->phspi, TxData, RxData, 3, timeout) != HAL_OK){
		MCP3008_dev->status |= ADC_XFER_ERROR;

	}

	/* Un-select Chip */
	HAL_GPIO_WritePin(MCP3008_dev->pgpio_cs, MCP3008_dev->pin_nbr, GPIO_PIN_SET);


	/* Translate the data to a 16bit value */
	RxData_16b = 0x3FF & ((RxData[0] & 0x01) << 9 | (RxData[1] & 0xFF) << 1 | (RxData[2] & 0x80) >> 7);

	/* Get the real value */
	*result = RxData_16b * MCP3008_dev->vref /1024;


	if(MCP3008_dev->status != ADC_OK) return HAL_ERROR;
	return HAL_OK;


}


HAL_StatusTypeDef MCP3008_ReadAllChannels(MCP3008_InitTypeDef* MCP3008_dev, float* result, uint32_t timeout){
	uint8_t i =0;
	for( uint8_t channel = 0 ; channel < 8 ; channel++ ){
		if( MCP3008_ReadChannel(MCP3008_dev, channel, result + i, timeout) != HAL_OK ) return HAL_ERROR;
		HAL_Delay(1);
		i++;
	}
	return HAL_OK;
}


HAL_StatusTypeDef MCP3008_Polling_Benchmark(MCP3008_InitTypeDef* MCP3008_dev, uint8_t channel_id, float* ksps, uint32_t timeout){
	uint32_t conversion_number = 0;
	uint32_t start = HAL_GetTick();
	while( HAL_GetTick() - start < 1000){
		if( MCP3008_ReadChannel(MCP3008_dev, channel_id, NULL, timeout) != HAL_OK) return HAL_ERROR;
		conversion_number++;
	}
	*ksps = ( conversion_number / 1000.0 ) / ( timeout / 1000.0 );
	return HAL_OK;
}
