//********************************** main **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : main.c
// Summary : Read and write data from ESP32 to AT24C02.
// Note    : None
// Author  : Surya Santhosh
// Day     : 10/SEP/2025
//******************************************************************************

//******************************* Include Files ********************************
#include "i2c.h"
#include "common.h"
#include "stm32.h"
#include "osLayer.h"

//******************************* Local Types **********************************

//***************************** Local Constants ********************************

//***************************** Local Variables ********************************
static UART_HandleTypeDef huart2 = {0};

//****************************** Local Functions *******************************
void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);
int _write(int file, char *ptr, int len);

//******************************.mainFunction.**********************************
// Purpose : Initialize System Clock Configuration, UART.
// Inputs  : none
// Outputs : none
// Return  : 0
// Notes   : None
//******************************************************************************
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_USART2_UART_Init();
  stm32SlaveI2C2Init();
  stm32SlaveI2C1Init();
  osKernelInitialize();
  osLayerCreation();
  osKernelStart();

  return 0;
}

//***************************.SystemClock_Config.******************************
// Purpose : System Clock Configuration.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

//**********************************._write.***********************************
// Purpose : Retargets printf.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);

    return len;
}

//**************************.MX_USART2_UART_Init.******************************
// Purpose : initialize UART2.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

//*****************************.Error_Handler.*********************************
// Purpose : Function for error handler.
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

//EOF
