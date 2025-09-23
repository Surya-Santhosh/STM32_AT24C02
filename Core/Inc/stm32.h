//********************************* stm32 *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : Read and write data from ESP32 to AT24C02.
// Note    : None
//
//*****************************************************************************
#ifndef STM32_H
#define STM32_H

//**************************** Include Files **********************************
#include <i2c/i2c.h>
#include "main.h"
#include "common.h"
#include "osLayer.h"

//*************************** Global Types ************************************
typedef struct TX_MESSAGE
{
	uint8 ucCmd;
	uint8 ucAddress;
	uint8 ucValue;
}TX_MESSAGE;

typedef struct RX_MESSAGE
{
	uint8 ucAck;
	uint8 ucValue;
}RX_MESSAGE;

//************************* Global Constants **********************************

//************************* Global Variables **********************************

//************************* Forward Declarations ******************************
bool stm32Slave();
bool stm32Master();
bool stm32SlaveI2C2Init();
bool stm32SlaveI2C1Init();
void StartTaskMaster();
void StartTaskSlave();

//********************** Inline Method Implementations ************************

#endif

// STM32_H
// EOF
