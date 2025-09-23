//********************************** i2c **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : Read and write operation using I2C Communication.
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
bool i2cWrite(I2C_HandleTypeDef *pstI2C,uint16 unDeviceAddress,
		               uint16 unMemoryAddress, uint16 unMemoryAddressSize,
					   uint8 *pucWriteData, uint16 unSize, uint32 ulTimeout);
bool i2cRead(I2C_HandleTypeDef *pstI2C,uint16 unDeviceAddress,
                      uint16 unMemoryAddress, uint16 unMemoryAddressSize,
		              uint8 *pucReadData, uint16 unSize, uint32 ulTimeout);

//********************** Inline Method Implementations ************************

#endif

// I2C_H
// EOF
