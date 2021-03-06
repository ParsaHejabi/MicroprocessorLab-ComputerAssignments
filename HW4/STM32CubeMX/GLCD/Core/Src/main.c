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
#include <string.h>
#include "FONT.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_DATA_PIN 8

#define GLCD_WIDTH 64
#define GLCD_HEIGHT 128
#define GLCD_DISPLAY_ON 0x3f
#define GLCD_DISPLAY_OFF 0x3e
#define GLCD_CMD_SET_X 0x40
#define GLCD_CMD_SET_LINE_NUMBER 0xB8
#define GLCD_CMD_START 0xC0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct GlcdDataStruct
{
  unsigned int rs;
  unsigned int rw;
  unsigned int e;
  unsigned int dataPins[LCD_DATA_PIN];
  unsigned int cs1;
  unsigned int cs2;
  unsigned int res;
};

struct GlcdDataStruct GlcdData;

unsigned char pixelValue_c1[8][64];
unsigned char pixelValue_c2[8][64];
unsigned char currPage;
unsigned char currPixel;
int currSel = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
int GlcdInit(const int rows, const int cols, const int rw, const int res,
             const int e, const int di, const int CS1, const int CS2,
             const int d0, const int d1, const int d2, const int d3,
             const int d4, const int d5, const int d6, const int d7);
void set_x_address(int, unsigned char);
void set_line_no(int, unsigned char);
void update(void);
void cls(void);
void clearPixel(int select, unsigned char page, unsigned char pixel);
void scrollUp(void);
void setGrCursor(int select, unsigned char page, unsigned char pixel);
int glcdPutchar(int select, const char c);
void glcdPuts(int select, const char *string);
void glcdWrite(int select, unsigned char data, int rs);

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
  /* USER CODE BEGIN 2 */
  GlcdInit(0, 0, GPIO_PIN_8, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_9, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  setGrCursor(1, 1, 0);
  glcdPuts(1, "Niki Nazaran 95243067 & Parsa Hejabi 95243019 & Alireza Mahdi Barzi 96243087");
  HAL_Delay(500);
  scrollUp();

  for (int i = 0; i < 6; i++)
  {
    clearPixel(1, 0, i);
  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
int GlcdInit(const int rows, const int cols, const int rw, const int res,
             const int e, const int di, const int CS1, const int CS2,
             const int d0, const int d1, const int d2, const int d3,
             const int d4, const int d5, const int d6, const int d7)
{
  int i;

  if ((rows < 0) || (rows > 63))
    return -1;

  if ((cols < 0) || (cols > 127))
    return -1;

  GlcdData.rw = rw;
  GlcdData.rs = di;
  GlcdData.res = res;
  GlcdData.e = e;
  GlcdData.cs1 = CS1;
  GlcdData.cs2 = CS2;

  GlcdData.dataPins[0] = d0;
  GlcdData.dataPins[1] = d1;
  GlcdData.dataPins[2] = d2;
  GlcdData.dataPins[3] = d3;
  GlcdData.dataPins[4] = d4;
  GlcdData.dataPins[5] = d5;
  GlcdData.dataPins[6] = d6;
  GlcdData.dataPins[7] = d7;

  for (i = 0; i < 8; i++)
  {
    HAL_GPIO_WritePin(GPIOA, GlcdData.dataPins[i], GPIO_PIN_RESET);
  }

  HAL_GPIO_WritePin(GPIOA, GlcdData.rs, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GlcdData.rw, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GlcdData.res, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GlcdData.e, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GlcdData.cs1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GlcdData.cs2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GlcdData.res, GPIO_PIN_SET);
  HAL_Delay(50);

  glcdWrite(0, GLCD_CMD_START, 0);
  glcdWrite(1, GLCD_CMD_START, 0);
  glcdWrite(0, GLCD_DISPLAY_ON, 0);
  glcdWrite(1, GLCD_DISPLAY_ON, 0);

  cls();

  return 1;
}

void set_x_address(int select, unsigned char pixel)
{
  unsigned char command = GLCD_CMD_SET_X + pixel;
  glcdWrite(select, command, 0);
  currPixel = pixel;
}

void set_line_no(int select, unsigned char page)
{
  unsigned char command = GLCD_CMD_SET_LINE_NUMBER + page;
  glcdWrite(select, command, 0);
  currPage = page;
}

void update()
{
  unsigned char x, y;

  for (y = 0; y < 8; y++)
  {
    set_line_no(0, y);
    set_line_no(1, y);
    set_x_address(0, 0);
    set_x_address(1, 0);
    for (x = 0; x < 64; x++)
    {
      glcdWrite(0, pixelValue_c1[y][x], 1);
      glcdWrite(1, pixelValue_c2[y][x], 1);
    }
  }

  return;
}

void cls(void)
{
  unsigned char x, y;

  for (y = 0; y < 8; y++)
  {
    set_line_no(0, y);
    set_line_no(1, y);
    set_x_address(0, 0);
    set_x_address(1, 0);
    for (x = 0; x < 64; x++)
    {
      glcdWrite(0, 0, 1);
      glcdWrite(1, 0, 1);
    }
  }

  return;
}

void clearPixel(int select, unsigned char page, unsigned char pixel)
{
  setGrCursor(select, page, pixel);
  glcdWrite(select, 0, 1);
  return;
}

void scrollUp(void)
{
  int i, j;
  for (i = 1; i < 8; i++)
  {
    for (j = 0; j < 64; j++)
    {
      pixelValue_c1[i - 1][j] = pixelValue_c1[i][j];
      pixelValue_c2[i - 1][j] = pixelValue_c2[i][j];
    }
  }
  for (i = 0; i < 64; i++)
  {
    pixelValue_c1[7][i] = 0x00;
    pixelValue_c2[7][i] = 0x00;
  }

  update();
  return;
}

void setGrCursor(int select, unsigned char page, unsigned char pixel)
{
  set_line_no(select, page);
  set_x_address(select, pixel);
  return;
}

int glcdPutchar(int select, const char c)
{
  int index = (c - 32) * 5, i;
  for (i = 0; i < 5; i++)
  {
    glcdWrite(currSel, font[index + i], 1);
    if (currSel == 0)
      pixelValue_c1[currPage][currPixel] = font[index + i];
    else
      pixelValue_c2[currPage][currPixel] = font[index + i];

    currPixel++;
    if (currPixel == 64)
    {
      currPixel = 0;
      if (currSel == 1)
        currSel = 0;

      else
      {
        currSel = 1;
        currPage++;
      }
      setGrCursor(currSel, currPage, currPixel);
    }
  }
  HAL_Delay(1);
  return 0;
}

void glcdPuts(int select, const char *string)
{
  int i = 0;
  currSel = select;
  for (i = 0; i < strlen(string); i++)
  {
    glcdPutchar(currSel, string[i]);
  }
  return;
}

void glcdWrite(int select, unsigned char data, int rs)
{
  int i;
  HAL_GPIO_WritePin(GPIOB, GlcdData.e, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GlcdData.e, GPIO_PIN_RESET);

  if (select == 0)
  {
    HAL_GPIO_WritePin(GPIOB, GlcdData.cs1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GlcdData.cs2, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(GPIOB, GlcdData.cs2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GlcdData.cs1, GPIO_PIN_SET);
  }

  HAL_GPIO_WritePin(GPIOA, GlcdData.rw, GPIO_PIN_RESET);

  if (rs == 1)
    HAL_GPIO_WritePin(GPIOA, GlcdData.rs, GPIO_PIN_SET);
  else
    HAL_GPIO_WritePin(GPIOA, GlcdData.rs, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, GlcdData.e, GPIO_PIN_SET);

  for (i = 0; i < 8; i++)
  {
    HAL_GPIO_WritePin(GPIOA, GlcdData.dataPins[i], data & 0x01);
    data = data >> 1;
  }

  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOB, GlcdData.e, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GlcdData.rw, GPIO_PIN_SET);

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
