//********************************* stm32 *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : Implement STM32 as I2C slave and master, intermediate
//         : communication between ESP32 and AT24C02.
// Note    : None
//
//*****************************************************************************
#ifndef STM32_H
#define STM32_H

//**************************** Include Files **********************************
#include <i2c.h>
#include "main.h"
#include "common.h"
#include "osLayer.h"

//*************************** Global Types ************************************
typedef struct _TX_MESSAGE_
{
	uint8 ucCmd;
	uint8 ucAddress;
	uint8 ucData;
}_TX_MESSAGE_;

typedef struct _RX_MESSAGE_
{
	uint8 ucAck;
	uint8 ucData;
}_RX_MESSAGE_;

//************************* Global Constants **********************************

//************************* Global Variables **********************************

//************************* Forward Declarations ******************************
void stm32MasterTask();
void stm32SlaveTask();

//********************** Inline Method Implementations ************************

#endif

// STM32_H
// EOF
