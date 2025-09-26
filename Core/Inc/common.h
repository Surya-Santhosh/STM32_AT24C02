//******************************* common ***************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//******************************************************************************
//
// Summary : Typedefs are included.
// Note    : None
//
//******************************************************************************
#ifndef COMMON_H
#define COMMON_H

//**************************** Include Files ***********************************
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"

//**************************** Global Types ************************************
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef char int8;
typedef signed short int16;

typedef struct _I2C_PACKET_
{
	I2C_HandleTypeDef stI2C;
	uint16 unDeviceAddress;
	uint16 unMemoryAddress;
	uint16 unMemoryAddressSize;
	uint8 ucData;
	uint16 unSize;
	uint32 ulTimeout;
}_I2C_PACKET_;

//************************* Global Constants ***********************************
#define EEPROM_DEVICE_ADDRESS       (0x50 << 1)
#define STACK_SIZE                  (128 * 4)
#define WRITE_CMD                   (0x01)
#define READ_CMD                    (0x02)
#define WRITE_ACK                   (0x01)
#define READ_ACK                    (0x02)
#define TX_MESSAGE_SIZE             (3)
#define RX_MESSAGE_SIZE             (2)
#define TIMEOUT                     (100)
#define SIZE_DATA_TO_SEND           (1)
#define SIZE_DATA_TO_RECEIVE        (1)
#define NUM_SEM                     (2)

//************************* Global Variables ***********************************

//************************* Forward Declarations *******************************

//************************ Inline Method Implementations ***********************

#endif

// COMMON_H
// EOF
