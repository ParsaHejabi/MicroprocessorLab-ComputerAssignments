/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
struct User
{
  char username[20];
  char password[20];
};
struct User users[20];
uint8_t menu[] = "1- Sign up\r\n2- Sign in\r\n";
uint8_t input;
char temp[20];
uint8_t tempIndex = 0;

int i, j, k;
int usersIndex = 0;
// 0 -> get menu; 1 -> SU username; 2 -> SU password; 3 -> SI username; 4 -> SI password;
int state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
int stringComp(char[], char[]);
void resetTemp(void);
/* USER CODE BEGIN PFP */
int stringComp(char c1[], char c2[])
{
  if (sizeof(c1) != sizeof(c2))
  {
    return 0;
  }

  for (j = 0; j < 20; j++)
    if (c1[j] != c2[j])
      return 0;

  return 1;
}

void resetTemp()
{
  for (i = 0; i < 20; i++)
    temp[i] = NULL;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (input == '\n')
  {
    if (state == 0)
    {
      if (tempIndex == 2)
      {
        if (temp[0] == '1')
        {
          state = 1;
          HAL_UART_Transmit(&huart1, "\nEnter username:\r\n", 18, HAL_MAX_DELAY);
        }
        else if (temp[0] == '2')
        {
          state = 3;
          HAL_UART_Transmit(&huart1, "\nEnter username:\r\n", 18, HAL_MAX_DELAY);
        }
        else
          HAL_UART_Transmit(&huart1, "\nChoose 1 or 2:\r\n", 17, HAL_MAX_DELAY);
      }
      else
        HAL_UART_Transmit(&huart1, "\nInvalid input! Try again!\r\n", 27, HAL_MAX_DELAY);
      tempIndex = 0;
    }
    else if (state == 1)
    {
      for (i = 0; i < usersIndex; i++)
      {
        if (stringComp(users[i].username, temp) == 1)
        {
          HAL_UART_Transmit(&huart1, "\nUsername taken!\r\nPlease choose another one:\r\n", 46, HAL_MAX_DELAY);
          break;
        }
      }
      if (i == usersIndex)
      {
        for (i = 0; i < tempIndex; i++)
        {
          users[usersIndex].username[i] = temp[i];
        }
        HAL_UART_Transmit(&huart1, "\nEnter password:\r\n", 18, HAL_MAX_DELAY);
        state = 2;
      }
      tempIndex = 0;
    }
    else if (state == 2)
    {
      for (i = 0; i < tempIndex; i++)
      {
        users[usersIndex].password[i] = temp[i];
      }
      HAL_UART_Transmit(&huart1, "\nUser registered!\r\n", 19, HAL_MAX_DELAY);
      HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
      tempIndex = 0;
      state = 0;
      usersIndex++;
    }
    else if (state == 3)
    {
      //HAL_UART_Transmit(&huart1, "\nstate 3", 8, HAL_MAX_DELAY);
			
			int flag =0;
			for (i = 0; i < usersIndex; i++)
      {
        if (stringComp(users[i].username, temp) == 1){
					HAL_UART_Transmit(&huart1, "\nExists!\r\n", 8, HAL_MAX_DELAY);
					k = i;
					flag =1;
				}
      }
			if (flag){
				HAL_UART_Transmit(&huart1, "\nEnter password:\r\n", 18, HAL_MAX_DELAY);
				state = 4;
			}
			else{
				HAL_UART_Transmit(&huart1, "\nUser not exists, try again later!\r\n", 36, HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
				state =0;
			}
			tempIndex = 0;
      
    }
    else if (state == 4)
    {
      //HAL_UART_Transmit(&huart1, "\nstate 4", 8, HAL_MAX_DELAY);
			
				if (stringComp(users[k].password, temp) == 1)
					HAL_UART_Transmit(&huart1, "\nLogged in!\r\n", 13, HAL_MAX_DELAY);
				else if (stringComp(users[k].password, temp) != 1)
					HAL_UART_Transmit(&huart1, "\nIncorrect Pass, try again later!\r\n", 35, HAL_MAX_DELAY);
      HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
      tempIndex = 0;
      state = 0;
    }
    resetTemp();
  }
  else
  {
    temp[tempIndex++] = input;
    HAL_UART_Transmit(&huart1, &input, sizeof(input), 0);
  }

  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);

  HAL_UART_Receive_IT(&huart1, &input, sizeof(input));
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
  HAL_UART_Receive_IT(&huart1, &input, sizeof(input));
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DEBUG_LED_Pin */
  GPIO_InitStruct.Pin = DEBUG_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DEBUG_LED_GPIO_Port, &GPIO_InitStruct);
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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
