/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
#include "container.h"

//#include "container.c"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*define to Test the program with and without semaphores   */
//#	define	NOT_CRITICALLY_SAFE

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for producer1Task */
osThreadId_t producer1TaskHandle;
uint32_t producer1TaskBuffer[ 128 ];
osStaticThreadDef_t producer1TaskControlBlock;
const osThreadAttr_t producer1Task_attributes = {
  .name = "producer1Task",
  .cb_mem = &producer1TaskControlBlock,
  .cb_size = sizeof(producer1TaskControlBlock),
  .stack_mem = &producer1TaskBuffer[0],
  .stack_size = sizeof(producer1TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for producer2Task */
osThreadId_t producer2TaskHandle;
uint32_t producer2TaskBuffer[ 128 ];
osStaticThreadDef_t producer2TaskControlBlock;
const osThreadAttr_t producer2Task_attributes = {
  .name = "producer2Task",
  .cb_mem = &producer2TaskControlBlock,
  .cb_size = sizeof(producer2TaskControlBlock),
  .stack_mem = &producer2TaskBuffer[0],
  .stack_size = sizeof(producer2TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for consumerTask */
osThreadId_t consumerTaskHandle;
uint32_t consumerTaskBuffer[ 128 ];
osStaticThreadDef_t consumerTaskControlBlock;
const osThreadAttr_t consumerTask_attributes = {
  .name = "consumerTask",
  .cb_mem = &consumerTaskControlBlock,
  .cb_size = sizeof(consumerTaskControlBlock),
  .stack_mem = &consumerTaskBuffer[0],
  .stack_size = sizeof(consumerTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for outputTask */
osThreadId_t outputTaskHandle;
uint32_t outputTaskBuffer[ 128 ];
osStaticThreadDef_t outputTaskControlBlock;
const osThreadAttr_t outputTask_attributes = {
  .name = "outputTask",
  .cb_mem = &outputTaskControlBlock,
  .cb_size = sizeof(outputTaskControlBlock),
  .stack_mem = &outputTaskBuffer[0],
  .stack_size = sizeof(outputTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void producer1Function(void *argument);
void producer2Function(void *argument);
void consumerFunction(void *argument);
void outputFunction(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* print out the signs with UART */
int _write (int file, char *ptr, int len)
{
	while(len-- > 0)
	{
		while(  HAL_UART_GetState(&huart2) != HAL_UART_STATE_READY );

			if(*ptr == '\n')
			{
				static unsigned char crlf[2]={'\r','\n'};
				HAL_UART_Transmit_IT(&huart2, crlf, 2);
			}
			else
			{
				HAL_UART_Transmit_IT(&huart2, ptr, 1);
			}
			ptr++;
	}

	return len;
}

/* countUp the valuepair(T1/T2) with or without semaphores */
void produce_countUpValuePair_safety(enum variablepair t){
	int res=0;
#ifndef NOT_CRITICALLY_SAFE
	res = countUpValuePair_safety(t);
#else
	countUpValuePair(t);
#endif

	/* print the osStatus */
	if(res>0)
		printf("T1-countUp-blocked\n");
	else if(res<0)
		printf("T2-countUp-blocked\n");
}

/* halv the valuepair(T1/T2) with or without semaphores */
void produce_halveValuePair_safety(enum variablepair t){
	int res=0;
#ifndef NOT_CRITICALLY_SAFE
	res = halveValuePair_safety(t);
#else
	halveValuePair(t);
#endif

	/* print the osStatus */
	if(res>0)
		printf("T1-halve-blocked\n");
	else if(res<0)
		printf("T2-halve-blocked\n");
}

/* print the valuepair(T1/T2) with or without semaphores */
void consume_printData(){

#ifndef NOT_CRITICALLY_SAFE
	critical_data_t data = getcritical_data_safety();
#else
	critical_data_t data = getcritical_data();
#endif

	/* print the struct-memory variables */
	if( data.t1val != NULL && *data.t1str != NULL && data.t2val != NULL && *data.t2str!=NULL){
		printf("t1val %d t1str %s ",data.t1val ,data.t1str);
		printf("%s   ", ( data.t1val > atoi(data.t1str)  ) ? "#" : "");
		printf("t2val %d t2str %s ",data.t2val ,data.t2str);
		printf("%s   \n",( data.t2val > atoi(data.t2str)) ? "#" : "");
	}else{
		printf("STRUCT-blocked\n");
	}

}

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  Container_Init();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of producer1Task */
  producer1TaskHandle = osThreadNew(producer1Function, NULL, &producer1Task_attributes);

  /* creation of producer2Task */
  producer2TaskHandle = osThreadNew(producer2Function, NULL, &producer2Task_attributes);

  /* creation of consumerTask */
  consumerTaskHandle = osThreadNew(consumerFunction, NULL, &consumerTask_attributes);

  /* creation of outputTask */
  outputTaskHandle = osThreadNew(outputFunction, NULL, &outputTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_producer1Function */
/**
  * @brief  Function implementing the producer1Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_producer1Function */
void producer1Function(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	  /* countUp the valuepair (T1) */
	  produce_countUpValuePair_safety(T1);
    osDelay(200);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_producer2Function */
/**
* @brief Function implementing the producer2Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_producer2Function */
void producer2Function(void *argument)
{
  /* USER CODE BEGIN producer2Function */
  /* Infinite loop */
  for(;;)
  {
	  /* countUp the valuepair (T2) */
	  produce_countUpValuePair_safety(T2);
    osDelay(200);
  }
  /* USER CODE END producer2Function */
}

/* USER CODE BEGIN Header_consumerFunction */
/**
* @brief Function implementing the consumerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_consumerFunction */
void consumerFunction(void *argument)
{
  /* USER CODE BEGIN consumerFunction */
  /* Infinite loop */
  for(;;)
  {
	  /* halv the valuepair (T1 and T2) */
	  produce_halveValuePair_safety(T1);
	  osDelay(50);
	  produce_halveValuePair_safety(T2);
    osDelay(400);
  }
  /* USER CODE END consumerFunction */
}

/* USER CODE BEGIN Header_outputFunction */
/**
* @brief Function implementing the outputTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_outputFunction */
void outputFunction(void *argument)
{
  /* USER CODE BEGIN outputFunction */
  /* Infinite loop */
  for(;;)
  {
	  /* print the valuepair(T1/T2) */
	  consume_printData();
    osDelay(200);
  }
  /* USER CODE END outputFunction */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
