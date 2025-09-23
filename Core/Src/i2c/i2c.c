//************************************ i2c ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : i2c.c
// Summary : Read and write operation for EEPROM.
// Note    : None
// Author  : Surya Santhosh
// Day     : 10/SEP/2025
//*****************************************************************************

//******************************* Include Files *******************************
#include "i2c.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************
//
//*********************************.i2cWrite.**********************************
// Purpose : Write data into EEPROM memory address.
// Inputs  : pshi2c - Pointer to hi2c1 handle.
//         : unDeviceAddress - Device address.
// 		   : unMemoryAddress - Memory address
//		   : unMemoryAddressSize - Size of memory address.
//		   : pucWriteData - Pointer to data.
//		   : unSize - Amount of data to be write.
//		   : ulTimeout - Timeout duration.
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
bool i2cWrite(I2C_HandleTypeDef *pstI2C, uint16 unDeviceAddress,
		      uint16 unMemoryAddress, uint16 unMemoryAddressSize,
			  uint8 *pucWriteData, uint16 unSize, uint32 ulTimeout)
{
	bool blResult = false;

	if (HAL_OK == HAL_I2C_Mem_Write(pstI2C, (uint16_t)unDeviceAddress,
			                        (uint16_t)unMemoryAddress,
									(uint16_t)unMemoryAddressSize,
									(uint8_t *) pucWriteData,
									unSize, (uint32_t)ulTimeout))
	{
		blResult = true;
	}

    return blResult;
}

//********************************.i2cRead.************************************
// Purpose : Read data from EEPROM memory address.
// Inputs  : pshi2c - Pointer to hi2c1 handle.
//         : unDeviceAddress - Device address.
// 		   : unMemoryAddress - Memory address
//		   : unMemoryAddressSize - Size of memory address.
//		   : pucWriteData - Pointer to data.
//		   : unSize - Amount of data to be read.
//		   : ulTimeout - Timeout duration.
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
bool i2cRead(I2C_HandleTypeDef *pstI2C,uint16 unDeviceAddress,
             uint16 unMemoryAddress, uint16 unMemoryAddressSize,
		     uint8 *pucReadData, uint16 unSize, uint32 ulTimeout)
{
	bool blResult = false;

	if (HAL_OK == HAL_I2C_Mem_Read(pstI2C, (uint16_t)unDeviceAddress,
								  (uint16_t)unMemoryAddress,
								  (uint16_t)unMemoryAddressSize,
								  (uint8_t *) pucReadData,
								  unSize, (uint32_t)ulTimeout))
	{
		blResult = true;
	}

    return blResult;
}
// EOF




