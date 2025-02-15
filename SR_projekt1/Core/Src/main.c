/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

volatile uint16_t pwm_m1;
volatile uint16_t pwm_m2;

volatile uint16_t dir1_m1;
volatile uint16_t dir2_m1;

volatile uint16_t dir1_m2;
volatile uint16_t dir2_m2;

volatile uint16_t czuj1;
volatile uint16_t czuj2;
volatile uint16_t czuj3;
volatile uint16_t czuj4;
volatile uint16_t czuj5;
volatile uint16_t czuj6;

volatile uint16_t batt;

//////////////////////////////////////////////////////////////////////////////
// Funkcje ustawiające kierunek ruchu pierwszego silnika

void setDIR1_M1(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, SET);
}
void resetDIR1_M1(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
}
void setDIR2_M1(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
}
void resetDIR2_M1(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);
}

void setM1_Forward(){
	resetDIR1_M1();
	setDIR2_M1();
}

void setM1_Backward(){
	setDIR1_M1();
	resetDIR2_M1();
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Funkcje ustawiające kierunek ruchu drugiego silnika

void setDIR1_M2(){
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, SET);
}
void resetDIR1_M2(){
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, RESET);
}
void setDIR2_M2(){
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, SET);
}
void resetDIR2_M2(){
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, RESET);
}

void setM2_Forward(){
	resetDIR1_M2();
	setDIR2_M2();
}

void setM2_Backward(){
	setDIR1_M2();
	resetDIR2_M2();
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Funkcje ustawiające wypełnienie PWM'ów obu silników

void setPWM_Motor1(int wypelnienie){
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, wypelnienie);
}

void setPWM_Motor2(int wypelnienie){
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, wypelnienie);
}

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Dokonywanie pomiarów z przetworników analogowo cyfrowych

void HAL_ADC_ConvCptlCallback(ADC_HandleTypeDef * hadc){
	  HAL_ADC_Start_IT(&hadc1);

	  if(hadc == &hadc1){
		adc_value = HAL_ADC_GetValue(&hadc1);
		adc_flag = 1;
	  }
	  if(hadc == &hadc2){
		adc_value = HAL_ADC_GetValue(&hadc2);
		adc_flag = 1;
	  }
	  HAL_ADC_Start_IT(&hadc1);

}



//////////////////////////////////////////////////////////////////////////////
// Funkcje obsługujące wymianę danych poprzez SPI

uint8_t mcp_read_reg(uint8_t addr){
	uint8_t tx_buf[] = { 0x41, addr, 0xFF };
	uint8_t rx_buf[3];

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	return rx_buf[2];
}

void mcp_write_reg(uint8_t addr, uint8_t value){
	uint8_t tx_buf[] = { 0x40, addr, value };

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, tx_buf, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

//////////////////////////////////////////////////////////////////////////////



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
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_ADC2_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
  HAL_ADC_Start_IT(&hadc1);
  HAL_ADC_Start_IT(&hadc2);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {





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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
