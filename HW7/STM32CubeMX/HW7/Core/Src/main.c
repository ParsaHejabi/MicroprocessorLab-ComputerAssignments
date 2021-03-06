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
#define RS 0x20 /* PB5 mask for reg select */
#define RW 0x40 /* PB6 mask for read/write */
#define EN 0x80 /* PB7 mask for enable */

#define ROW_COUNT 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t TC72_CONTROL_REGISTER_ADD = 0x80;
uint8_t TC72_CONTROL_REGISTER_VALUE = 0x04;
uint8_t TC72_LSB_TEMP_ADD = 0x01;
uint8_t TC72_MSB_TEMP_ADD = 0x02;

int displayOn = 1;
int cursorOn = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
int lcdInit(int row);
void lcdHome(void);
void lcdClear(void);
void lcdDisplay(void);
void lcdCursor(void);
void lcdCursorBlink(int state);
void lcdSendCommand(unsigned char command);
void lcdPosition(int x, int y);
void lcdPutchar(unsigned char data);
void lcdPuts(const char *string);
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
  char uart_buf[100];
  int uart_buf_len;
  uint8_t spi_buf[100];
  char textToWrite[100];
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  lcdInit(ROW_COUNT);

  uart_buf_len = sprintf(uart_buf, "SPI Temperature Sensor:\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t *)uart_buf, uart_buf_len, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    // HAL_Delay(1000);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    // HAL_Delay(1000);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi1, &TC72_CONTROL_REGISTER_ADD, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, &TC72_CONTROL_REGISTER_VALUE, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

    HAL_Delay(200);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi1, &TC72_MSB_TEMP_ADD, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, spi_buf, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

    int temp = spi_buf[0];
    temp = temp <= 125 ? temp : temp - 256;

    uart_buf_len = sprintf(uart_buf, "Received: %d\r\n", temp);
    HAL_UART_Transmit(&huart1, (uint8_t *)uart_buf, uart_buf_len, HAL_MAX_DELAY);

    char degreeSign[] = {96, 'C'};
    int textToWriteLen = sprintf(textToWrite, "Temp: %d %s", temp, degreeSign);

    lcdPuts(textToWrite);
    HAL_Delay(500);

    lcdClear();
    lcdHome();
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin | CE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RS_Pin | RW_Pin | EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D0_Pin D1_Pin D2_Pin D3_Pin
                           D4_Pin D5_Pin D6_Pin D7_Pin
                           CE_Pin */
  GPIO_InitStruct.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin | CE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RS_Pin RW_Pin EN_Pin */
  GPIO_InitStruct.Pin = RS_Pin | RW_Pin | EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
int lcdInit(int row)
{
  HAL_Delay(30); /* initialization sequence */
  if (row == 1)
  {
    lcdSendCommand(0x30); /* set 8-bit data, 1-line, 5x7 font */
  }
  else
  {
    lcdSendCommand(0x38); /* set 8-bit data, 2-line, 5x7 font */
  }

  lcdSendCommand(0x0F); /* turn on display, cursor blinking */

  lcdSendCommand(0x01); /* clear screen, move cursor to home */

  lcdSendCommand(0x06); /* move cursor right after each char */
  return 0;
}

void lcdHome(void)
{
  lcdSendCommand(0x02);
  return;
}

void lcdClear(void)
{
  lcdSendCommand(0x01);
  return;
}

void lcdDisplay(void)
{
  if (displayOn)
  {
    displayOn = 0;
    lcdSendCommand(0x08); /* display off, cursor off, blink off */
  }
  else
  {
    displayOn = 1;
    lcdSendCommand(0x0F);
  }
  return;
}

void lcdCursor(void)
{
  if (cursorOn)
  {
    cursorOn = 0;
    lcdSendCommand(0x0C);
  }
  else
  {
    cursorOn = 1;
    lcdSendCommand(0x0F);
  }
  return;
}

void lcdCursorBlink(int state)
{
  if (state)
  {
    lcdSendCommand(0x0F);
  }
  else
  {
    lcdSendCommand(0x0E);
  }
  return;
}

void lcdSendCommand(unsigned char command)
{
  GPIOB->BSRR = (RS | RW) << 16; /* RS = 0, R/W = 0 */
  GPIOC->ODR = command;          /* put command on data bus */
  GPIOB->BSRR = EN;              /* pulse E high */
  HAL_Delay(0);
  GPIOB->BSRR = EN << 16; /* clear E */

  if (command < 4)
    HAL_Delay(2); /* command 1 and 2 needs up to 1.64ms */
  else
    HAL_Delay(1); /* all others 40 us */
  return;
}

void lcdPosition(int x, int y)
{
  int i = 0;
  switch (y)
  {
  case 1:
  {
    lcdSendCommand(0x80);
    break;
  }
  case 2:
  {
    lcdSendCommand(0xC0);
    break;
  }
  case 3:
  {
    lcdSendCommand(0x90);
    break;
  }
  case 4:
  {
    lcdSendCommand(0xD0);
    break;
  }
  }

  for (i = 0; i < x; i++)
  {
    lcdSendCommand(0x14);
  }
  return;
}

void lcdPutchar(unsigned char data)
{
  GPIOB->BSRR = RS;       /* RS = 1 */
  GPIOB->BSRR = RW << 16; /* R/W = 0 */
  GPIOC->ODR = data;      /* put data on data bus */
  GPIOB->BSRR = EN;       /* pulse E high */
  HAL_Delay(0);
  GPIOB->BSRR = EN << 16; /* clear E */

  HAL_Delay(1);
  return;
}

void lcdPuts(const char *string)
{
  int i = 0;
  for (i = 0; i < strlen(string); i++)
  {
    lcdPutchar(string[i]);
    HAL_Delay(10);
  }
  return;
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
