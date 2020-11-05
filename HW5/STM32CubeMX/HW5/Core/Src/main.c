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
#include <stdio.h>
#include <string.h>
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
uint8_t menu[] = "1- Sign Up\r\n2- Sign In\r\n";
uint8_t signedInMenu[] = "1- Sign Out\r\n2- Account Information\r\n3- Change Password\r\n";
uint8_t input;
char temp[20];
uint8_t tempIndex = 0;
int signedInUserIndex = -1;

int i, j, k;
int usersIndex = 0;
/*  0 -> get menu; 1 -> SU username; 2 -> SU password; 3 -> SI username; 4 -> SI password;
    5 -> signed in menu; 6 -> Change Password;
*/
int state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void resetTemp(void);
char *ltrim(char *str);
char *rtrim(char *str);
char *trim(char *str);
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
      {
        HAL_UART_Transmit(&huart1, "\nInvalid input! Try again!\r\n", 28, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
      }
      tempIndex = 0;
    }
    else if (state == 1)
    {
      for (i = 0; i < usersIndex; i++)
      {
        if (strcmp(trim(users[i].username), trim(temp)) == 0)
        {
          char message[100];
          int n = sprintf(message, "\nUsername %s is taken!\r\nPlease choose another one:\r\n", trim(temp));
          HAL_UART_Transmit(&huart1, (uint8_t *)message, n, HAL_MAX_DELAY);
          break;
        }
      }
      if (i == usersIndex)
      {
        char *trimmedTemp = trim(temp);
        for (i = 0; i < strlen(trimmedTemp); i++)
        {
          users[usersIndex].username[i] = trimmedTemp[i];
        }
        HAL_UART_Transmit(&huart1, "\nEnter password:\r\n", 18, HAL_MAX_DELAY);
        state = 2;
      }
      tempIndex = 0;
    }
    else if (state == 2)
    {
      char *trimmedTemp = rtrim(temp);
      for (i = 0; i < strlen(trimmedTemp); i++)
      {
        users[usersIndex].password[i] = trimmedTemp[i];
      }
      char message[100];
      int n = sprintf(message, "\nUser %s is registered!\r\n", users[usersIndex].username);
      HAL_UART_Transmit(&huart1, (uint8_t *)message, n, HAL_MAX_DELAY);
      HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
      tempIndex = 0;
      state = 0;
      usersIndex++;
    }
    else if (state == 3)
    {
      for (i = 0; i < usersIndex; i++)
      {
        if (strcmp(trim(users[i].username), trim(temp)) == 0)
        {
          HAL_UART_Transmit(&huart1, "\nEnter password:\r\n", 18, HAL_MAX_DELAY);
          state = 4;
          k = i;
          break;
        }
      }
      if (i == usersIndex)
      {
        char message[100];
        int n = sprintf(message, "\nUser %s not found, try again later!\r\n", trim(temp));
        HAL_UART_Transmit(&huart1, (uint8_t *)message, n, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
        state = 0;
      }
      tempIndex = 0;
    }
    else if (state == 4)
    {
      char message[100];
      int n;
      if (strcmp(rtrim(users[k].password), rtrim(temp)) == 0)
      {
        n = sprintf(message, "\nHello %s, you are signed in!\r\n", users[k].username);
        HAL_UART_Transmit(&huart1, (uint8_t *)message, n, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, signedInMenu, sizeof(signedInMenu), HAL_MAX_DELAY);
        signedInUserIndex = k;
        state = 5;
      }
      else
      {
        HAL_UART_Transmit(&huart1, "\nBad credentials, try again later!\r\n", 35, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
        state = 0;
      }
      tempIndex = 0;
    }
    else if (state == 5)
    {
      if (signedInUserIndex != -1)
      {
        if (tempIndex == 2)
        {
          if (temp[0] == '1')
          {
            char message[100];
            int n = sprintf(message, "\nUser %s is signed out!\r\nRedirecting to the menu...\r\n", users[signedInUserIndex].username);
            HAL_UART_Transmit(&huart1, (uint8_t *)message, n, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
            signedInUserIndex = -1;
            state = 0;
          }
          else if (temp[0] == '2')
          {
            char message[100];
            int n = sprintf(message, "\nUsername: %s\r\nPassword: %s\r\n", users[signedInUserIndex].username, users[signedInUserIndex].password);
            HAL_UART_Transmit(&huart1, (uint8_t *)message, n, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart1, signedInMenu, sizeof(signedInMenu), HAL_MAX_DELAY);
            state = 5;
          }
          else if (temp[0] == '3')
          {
            HAL_UART_Transmit(&huart1, "\nEnter New Password:\r\n", 22, HAL_MAX_DELAY);
            state = 6;
          }
          else
            HAL_UART_Transmit(&huart1, "\nChoose 1, 2 or 3:\r\n", 20, HAL_MAX_DELAY);
        }
        else
        {
          HAL_UART_Transmit(&huart1, "\nInvalid input! Try again!\r\n", 28, HAL_MAX_DELAY);
          HAL_UART_Transmit(&huart1, menu, sizeof(menu), HAL_MAX_DELAY);
        }
      }
      else
      {
        HAL_UART_Transmit(&huart1, "\nThis should not be seen!\r\nRedirecting to the menu...\r\n", 53, HAL_MAX_DELAY);
        state = 0;
        signedInUserIndex = -1;
      }
      tempIndex = 0;
    }
    else if (state == 6)
    {
      char *trimmedTemp = rtrim(temp);
      for (i = 0; i < strlen(trimmedTemp); i++)
      {
        users[signedInUserIndex].password[i] = trimmedTemp[i];
      }
      HAL_UART_Transmit(&huart1, "\nPassword successfully updated!\r\n", 33, HAL_MAX_DELAY);
      HAL_UART_Transmit(&huart1, signedInMenu, sizeof(signedInMenu), HAL_MAX_DELAY);
      state = 5;
      tempIndex = 0;
    }
    resetTemp();
  }
  else
  {
    temp[tempIndex++] = input;
    HAL_UART_Transmit(&huart1, &input, sizeof(input), 0);
  }

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */
void resetTemp()
{
  for (int i = 0; i < 20; i++)
  {
    temp[i] = NULL;
  }
}

char *ltrim(char *str)
{
  size_t totrim;
  const char *seps = "\t\n\v\f\r ";
  totrim = strspn(str, seps);
  if (totrim > 0)
  {
    size_t len = strlen(str);
    if (totrim == len)
    {
      str[0] = '\0';
    }
    else
    {
      memmove(str, str + totrim, len + 1 - totrim);
    }
  }
  return str;
}

char *rtrim(char *str)
{
  int i;
  const char *seps = "\t\n\v\f\r ";
  i = strlen(str) - 1;
  while (i >= 0 && strchr(seps, str[i]) != NULL)
  {
    str[i] = '\0';
    i--;
  }
  return str;
}

char *trim(char *str)
{
  return ltrim(rtrim(str));
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
