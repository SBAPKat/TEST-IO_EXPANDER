/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MCP3008.h"
#include "MCP23008.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define OVERCURRENT_THRESHOLD 7.0 //in AMPS
#define SHUNT_R 0.010 // Ohms
#define OVERCURRENT_THRESHOLD_V OVERCURRENT_THRESHOLD/SHUNT_R
/* USER CODE END PD */


/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

DAC_HandleTypeDef hdac;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */

MCP3008_InitTypeDef ADC_1 = {0};
MCP3008_InitTypeDef ADC_2 = {0};
MCP3008_InitTypeDef ADC_3 = {0};
MCP3008_InitTypeDef *ADC_LIST[] = {&ADC_1, &ADC_2, &ADC_3};

MCP23008_InitTypeDef GPIO_0 = {0};
MCP23008_InitTypeDef GPIO_1 = {0};
MCP23008_InitTypeDef GPIO_2 = {0};
MCP23008_InitTypeDef GPIO_3 = {0};
MCP23008_InitTypeDef GPIO_4 = {0};
MCP23008_InitTypeDef GPIO_5 = {0};
MCP23008_InitTypeDef GPIO_6 = {0};
MCP23008_InitTypeDef GPIO_7 = {0};

uint8_t INT_FLAG =0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI3_Init(void);
static void MX_CAN1_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

void TO54_IOEXPANDERS_Init();

void TO54_UPDATE_ANALOG();

void TO54_OVERCURRENT_ACTION(MCP3008_InitTypeDef* adc, uint8_t pin_nbr);
void TO54_ADC1_OVERCURRENT_ACTION(uint8_t pin_nbr);
void TO54_ADC2_OVERCURRENT_ACTION(uint8_t pin_nbr);
void TO54_ADC3_OVERCURRENT_ACTION(uint8_t pin_nbr);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_DAC_Init();
	MX_SPI3_Init();
	MX_CAN1_Init();
	MX_TIM6_Init();
	/* USER CODE BEGIN 2 */
	TO54_IOEXPANDERS_Init();
	uint8_t TimerTestData = 0xff;

	HAL_TIM_Base_Start(&htim6);
	float result[8] = {0};
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		if(INT_FLAG != 0){
			if(MCP23008_WritePort(&GPIO_1, TimerTestData, 100)!= HAL_OK) Error_Handler();
			TimerTestData = ~TimerTestData;
			if(MCP3008_ReadAllChannels(&ADC_1,100) != HAL_OK) Error_Handler();
			INT_FLAG = 0;
		}
		  if (val_can>0) Receive_frame();

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief CAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN1_Init(void)
{

	/* USER CODE BEGIN CAN1_Init 0 */

	/* USER CODE END CAN1_Init 0 */

	/* USER CODE BEGIN CAN1_Init 1 */

	/* USER CODE END CAN1_Init 1 */
	hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 16;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN CAN1_Init 2 */

	/* USER CODE END CAN1_Init 2 */

}

/**
 * @brief DAC Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC_Init(void)
{

	/* USER CODE BEGIN DAC_Init 0 */

	/* USER CODE END DAC_Init 0 */

	DAC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN DAC_Init 1 */

	/* USER CODE END DAC_Init 1 */

	/** DAC Initialization
	 */
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT2 config
	 */
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN DAC_Init 2 */

	/* USER CODE END DAC_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */
	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI3_Init(void)
{

	/* USER CODE BEGIN SPI3_Init 0 */

	/* USER CODE END SPI3_Init 0 */

	/* USER CODE BEGIN SPI3_Init 1 */

	/* USER CODE END SPI3_Init 1 */
	/* SPI3 parameter configuration*/
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI3_Init 2 */
	ADC_1.phspi = &hspi3;
	ADC_1.pgpio_cs = SPI_ADC1_CS_GPIO_Port;
	ADC_1.pin_nbr = SPI_ADC1_CS_Pin;
	ADC_1.vref = 5.206;
	ADC_1.ID = 0;

	ADC_2.phspi = &hspi3;
	ADC_2.pgpio_cs = SPI_ADC2_CS_GPIO_Port;
	ADC_2.pin_nbr = SPI_ADC2_CS_Pin;
	ADC_2.vref = 5.206;
	ADC_2.ID = 1;

	ADC_3.phspi = &hspi3;
	ADC_3.pgpio_cs = SPI_ADC3_CS_GPIO_Port;
	ADC_3.pin_nbr = SPI_ADC3_CS_Pin;
	ADC_3.vref = 5.206;
	ADC_3.ID = 2;
	/* USER CODE END SPI3_Init 2 */

}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void)
{

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 31;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 524;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM6_Init 2 */
	__HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);
	/* USER CODE END TIM6_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LIN_CS_GPIO_Port, LIN_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, SPI_ADC2_CS_Pin|SPI_ADC1_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI_ADC3_CS_GPIO_Port, SPI_ADC3_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LIN_CS_Pin */
	GPIO_InitStruct.Pin = LIN_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LIN_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI_ADC2_CS_Pin */
	GPIO_InitStruct.Pin = SPI_ADC2_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SPI_ADC2_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI_ADC1_CS_Pin */
	GPIO_InitStruct.Pin = SPI_ADC1_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPI_ADC1_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI_ADC3_CS_Pin */
	GPIO_InitStruct.Pin = SPI_ADC3_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SPI_ADC3_CS_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void TO54_IOEXPANDERS_Init(){


	/* adresses physiques */
	GPIO_0.address = ( 0b0100<<3 | 0b000) << 1;
	GPIO_1.address = ( 0b0100<<3 | 0b001) << 1;
	GPIO_2.address = ( 0b0100<<3 | 0b100) << 1;
	GPIO_3.address = ( 0b0100<<3 | 0b011) << 1;
	GPIO_4.address = ( 0b0100<<3 | 0b010) << 1;
	GPIO_5.address = ( 0b0100<<3 | 0b110) << 1;
	GPIO_6.address = ( 0b0100<<3 | 0b101) << 1;
	GPIO_7.address = ( 0b0100<<3 | 0b111) << 1;

	GPIO_0.hi2c = &hi2c1;
	GPIO_1.hi2c = &hi2c1;
	GPIO_2.hi2c = &hi2c1;
	GPIO_3.hi2c = &hi2c1;
	GPIO_4.hi2c = &hi2c1;
	GPIO_5.hi2c = &hi2c1;
	GPIO_6.hi2c = &hi2c1;
	GPIO_7.hi2c = &hi2c1;

	if(MCP23008_Setup(&GPIO_0, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_1, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_2, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_3, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_4, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_5, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_6, 0x00, 100) != HAL_OK) Error_Handler(); //Sorties
	if(MCP23008_Setup(&GPIO_7, 0xFF, 100) != HAL_OK) Error_Handler(); //Entrées

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	/* This timer is generating an interrupt every 5kHz, used for analog measures, and electric motor monitoring */
	if(INT_FLAG != 1) INT_FLAG = 1;
	else{
		//	Error_Handler();
	}
}

void TO54_UPDATE_ANALOG(){
	MCP3008_InitTypeDef* adc;
	for(uint8_t adc_nbr = 0 ; adc_nbr < 3; adc_nbr++ ){
		adc = ADC_LIST[adc_nbr];
		/* check if we need to skip the current ADC*/
		if(adc->update_request == 0x00) continue;

		//we check if we need to read the entire ADC
		if(adc->update_request == 0xFF){
			MCP3008_ReadAllChannels(adc, 100);
			continue;
		}

		/* If we get here, we need to read specific channels */
		for(uint8_t channel=0;channel<8;channel++){
			if((adc->update_request>>channel) & 0b00000001) MCP3008_ReadChannel(adc, channel, 100);
		}
	}
}
void TO54_OVERCURRENT_ACTION(MCP3008_InitTypeDef* adc, uint8_t pin_nbr){
	switch(adc->ID){
	case 1: //ADC0
		TO54_ADC1_OVERCURRENT_ACTION(pin_nbr);
		break;
	case 2: //ADC1
		TO54_ADC2_OVERCURRENT_ACTION(pin_nbr);
		break;
	case 3: //ADC2
		TO54_ADC3_OVERCURRENT_ACTION(pin_nbr);
		break;

	}

}

void TO54_ADC1_OVERCURRENT_ACTION(uint8_t pin_nbr){
	switch(pin_nbr){
		//we need to deactivate the corresponding output to avoid damage
	case(ADC1_FAVG_ROUTE):
			MCP23008_WritePin(&GPIO_3, 0,0, 100);
			break;
	case(ADC1_FAVG_CROISEMENT):
			MCP23008_WritePin(&GPIO_3, 1,0, 100);
			break;
	case(ADC1_FAVG_CLIGNO):
			MCP23008_WritePin(&GPIO_3, 2,0, 100);
			break;
	case(ADC1_FAVG_VEILLE):
			MCP23008_WritePin(&GPIO_3, 3,0, 100);
			break;
	case(ADC1_FAVD_ROUTE):
			MCP23008_WritePin(&GPIO_3, 4,0, 100);
			break;
	case(ADC1_FAVD_CROISEMENT):
			MCP23008_WritePin(&GPIO_3, 5,0, 100);
			break;
	case(ADC1_FAVD_CLIGNO):
			MCP23008_WritePin(&GPIO_3, 6,0, 100);
			break;
	case(ADC1_FAVD_VEILLE):
			MCP23008_WritePin(&GPIO_3, 7,0, 100);
			break;

	}
	ADC_1.update_request ^= 1 << pin_nbr;
	MCP3008_ReadChannel(&ADC_1, pin_nbr, 100);
}
void TO54_ADC2_OVERCURRENT_ACTION(uint8_t pin_nbr){
	switch(pin_nbr){
	case(ADC2_FARG_VEILLE):
			MCP23008_WritePin(&GPIO_2, 0,0, 100);

			break;
	case(ADC2_FARG_STOP):
			MCP23008_WritePin(&GPIO_2, 1,0, 100);
			break;
	case(ADC2_FARG_CLIGNO):
			MCP23008_WritePin(&GPIO_2, 2,0, 100);
			break;
	case(ADC2_FARG_RECUL):
			MCP23008_WritePin(&GPIO_2, 3,0, 100);
			break;
	case(ADC2_FARD_VEILLE):
			MCP23008_WritePin(&GPIO_2, 4,0, 100);
			break;
	case(ADC2_FARD_STOP):
			MCP23008_WritePin(&GPIO_2, 5,0, 100);
			break;
	case(ADC2_FARD_CLIGNO):
			MCP23008_WritePin(&GPIO_2, 6,0, 100);
			break;
	case(ADC2_FARD_RECUL):
			MCP23008_WritePin(&GPIO_2, 7,0, 100);
			break;
	}
	ADC_2.update_request ^= 1 << pin_nbr;
	MCP3008_ReadChannel(&ADC_2, pin_nbr, 100);


}
void TO54_ADC3_OVERCURRENT_ACTION(uint8_t pin_nbr){
	switch(pin_nbr){
	case(ADC3_R_M1):
			MCP23008_WritePin(&GPIO_1, 0, 0, 100);
			MCP23008_WritePin(&GPIO_1, 1, 0, 100);
			break;
	case(ADC3_R_M2):
			MCP23008_WritePin(&GPIO_1, 4, 0, 100);
			MCP23008_WritePin(&GPIO_1, 5, 0, 100);
			break;
	case(ADC3_S_M1):
			MCP23008_WritePin(&GPIO_0, 0, 0, 100);
			MCP23008_WritePin(&GPIO_0, 1, 0, 100);
			break;
	case(ADC3_S_M2):
			MCP23008_WritePin(&GPIO_0, 4, 0, 100);
			MCP23008_WritePin(&GPIO_0, 5, 0, 100);
			break;
	case(ADC3_L_M1):
			MCP23008_WritePin(&GPIO_4, 0, 0, 100);
			MCP23008_WritePin(&GPIO_4, 1, 0, 100);
			break;
	case(ADC3_FARG_BROUILL):
			MCP23008_WritePin(&GPIO_4, 4, 0, 100);
			break;
	case(ADC3_FARD_BROUILL):
			MCP23008_WritePin(&GPIO_4, 5, 0, 100);
			break;
	}
	ADC_3.update_request ^= 1 << pin_nbr;
	MCP3008_ReadChannel(&ADC_3, pin_nbr, 100);
}
void TO54_CHECK_OVERCURRENT(){
	MCP3008_InitTypeDef* adc;
	/* we check each ADC */
	for(uint8_t adc_nbr = 0 ; adc_nbr < 3; adc_nbr++ ){
		adc = ADC_LIST[adc_nbr];

		/* and each input */
		for(uint8_t i=0;i<8;i++){
			if(adc->result[i] > OVERCURRENT_THRESHOLD) TO54_OVERCURRENT_ACTION(adc, i);
		}
	}

}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
