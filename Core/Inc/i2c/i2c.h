//********************************** i2c **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : I2C initialization, read and write operations in stm32.
// Note    : None
//
//*****************************************************************************
#ifndef I2C_H
#define I2C_H

//**************************** Include Files **********************************
#include "common.h"

//*************************** Global Types ************************************

//************************* Global Constants **********************************

//************************* Global Variables **********************************

//************************* Forward Declarations ******************************
bool i2cRead(_I2C_PACKET_ *pstI2cPacket);
bool i2cRead(_I2C_PACKET_ *pstI2cPacket);
bool i2c2Init();
bool i2c1Init();
bool i2c1GetHandler(I2C_HandleTypeDef *pstHi2c1);
bool i2c2GetHandler(I2C_HandleTypeDef *pstHi2c2);
bool i2cSlaveReceive(I2C_HandleTypeDef *pstHi2c, uint8 *pucData, uint16 unSize,
		             uint32 ulTimeout );
bool i2cSlaveTransmit(I2C_HandleTypeDef *pstHi2c, uint8 *pucData, uint16 unSize,
		              uint32 ulTimeout );

//********************** Inline Method Implementations ************************

#endif

// I2C_H
// EOF
