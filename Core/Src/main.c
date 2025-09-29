//********************************** main **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
// File    : main.c
// Summary : Initialize UART, I2C, and OSLayer for STM32 communication.
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
static UART_HandleTypeDef stHuart2 = {0};

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
  i2c2Init();
  i2c1Init();
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

  if (HAL_OK != HAL_RCC_OscConfig(&RCC_OscInitStruct))
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_OK != HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    Error_Handler();
  }
}

//**********************************._write.***********************************
// Purpose : Retargets printf to UART2.
// Inputs  : file - file descriptor.
//         : ptr - pointer to data to transmit.
//         : len - No. of bytes to transmit
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&stHuart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);

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
	stHuart2.Instance = USART2;
	stHuart2.Init.BaudRate = 115200;
	stHuart2.Init.WordLength = UART_WORDLENGTH_8B;
	stHuart2.Init.StopBits = UART_STOPBITS_1;
	stHuart2.Init.Parity = UART_PARITY_NONE;
	stHuart2.Init.Mode = UART_MODE_TX_RX;
	stHuart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	stHuart2.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_OK != HAL_UART_Init(&stHuart2))
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
