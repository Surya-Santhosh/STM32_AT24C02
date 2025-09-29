//************************************ i2c ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : i2c.c
// Summary : I2C initialization, read and write operations in stm32.
// Note    : None
// Author  : Surya Santhosh
// Day     : 10/SEP/2025
//*****************************************************************************

//******************************* Include Files *******************************
#include "i2c.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************
static I2C_HandleTypeDef stHi2c2 = {0};
static I2C_HandleTypeDef stHi2c1 = {0};

//****************************** Local Functions ******************************
//
//*********************************.i2cWrite.**********************************
// Purpose : Write data into EEPROM memory address.
// Inputs  : pstI2cPacket - struct contain parameters required to perform write
//         : operation.
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
bool i2cWrite(_I2C_PACKET_ *pstI2cPacket)
{
	bool blResult = false;

	if (HAL_OK == HAL_I2C_Mem_Write(&pstI2cPacket->stI2C,
			                        pstI2cPacket->unDeviceAddress,
			                        pstI2cPacket->unMemoryAddress,
									pstI2cPacket->unMemoryAddressSize,
									&pstI2cPacket->ucData,
									pstI2cPacket->unSize,
									pstI2cPacket->ulTimeout))
	{
		blResult = true;
	}

    return blResult;
}

//********************************.i2cRead.************************************
// Purpose : Read data from EEPROM memory address.
// Inputs  : pstI2cPacket - struct contain parameters required to perform read
//         : operation.
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
bool i2cRead(_I2C_PACKET_ *pstI2cPacket)
{
	bool blResult = false;

	if (HAL_OK == HAL_I2C_Mem_Read(&pstI2cPacket->stI2C,
								   pstI2cPacket->unDeviceAddress,
								   pstI2cPacket->unMemoryAddress,
								   pstI2cPacket->unMemoryAddressSize,
								   &pstI2cPacket->ucData,
								   pstI2cPacket->unSize,
								   pstI2cPacket->ulTimeout))
	{
		blResult = true;
	}

    return blResult;
}

//*******************************.i2c2Init.************************************
// Purpose : Initialize I2C2.
// Inputs  : None
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool i2c2Init()
{
	bool blResult = false;
	stHi2c2.Instance = I2C2;
	stHi2c2.Init.ClockSpeed = 100000;
	stHi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	stHi2c2.Init.OwnAddress1 = 24;
	stHi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	stHi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	stHi2c2.Init.OwnAddress2 = 0;
	stHi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	stHi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_OK == HAL_I2C_Init(&stHi2c2))
	{
		blResult = true;
	}

	return blResult;
}

//*******************************.i2c1Init.************************************
// Purpose : Initialize I2C1.
// Inputs  : None
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool i2c1Init()
{
	bool blResult = false;
	stHi2c1.Instance = I2C1;
	stHi2c1.Init.ClockSpeed = 100000;
	stHi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	stHi2c1.Init.OwnAddress1 = 0;
	stHi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	stHi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	stHi2c1.Init.OwnAddress2 = 0;
	stHi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	stHi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_OK == HAL_I2C_Init(&stHi2c1))
	{
		blResult = true;
	}

	return blResult;
}

//*******************************.i2c1GetHandler.******************************
// Purpose : To provide I2C handler for master.
// Inputs  : pstHi2c1 - pointer to receive I2C1 Handler.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool i2c1GetHandler(I2C_HandleTypeDef *pstHi2c1)
{
	bool blResult = false;

	if ((NULL != pstHi2c1))
	{
		*pstHi2c1 = stHi2c1;
		blResult = true;
	}

	return blResult;
}

//*******************************.i2c2GetHandler.******************************
// Purpose : To provide I2C handler for slave.
// Inputs  : pstHi2c2 - pointer to receive I2C2 Handler.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool i2c2GetHandler(I2C_HandleTypeDef *pstHi2c2)
{
	bool blResult = false;

	if ((NULL != pstHi2c2))
	{
		*pstHi2c2 = stHi2c2;
		blResult = true;
	}

	return blResult;
}

//*******************************.i2cSlaveReceive.*****************************
// Purpose : To receive data from master.
// Inputs  : pstHi2c - pointer to I2C Handler.
//         : ucData - Pointer to data.
//         : unSize - Amount of data to receive.
//         : ulTimeout - Timeout duration.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool i2cSlaveReceive(I2C_HandleTypeDef *pstHi2c, uint8 *pucData, uint16 unSize,
		             uint32 ulTimeout )
{
	bool blResult = false;

	if (HAL_OK == HAL_I2C_Slave_Receive(pstHi2c, pucData, unSize, ulTimeout))
	{
		blResult = true;
	}

	return blResult;
}

//******************************.i2cSlaveTransmit.*****************************
// Purpose : To transmit data to master.
// Inputs  : pstHi2c - pointer to I2C Handler.
//         : ucData - Pointer to data.
//         : unSize - Amount of data to be transmit.
//         : ulTimeout - Timeout duration.
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool i2cSlaveTransmit(I2C_HandleTypeDef *pstHi2c, uint8 *pucData, uint16 unSize,
		              uint32 ulTimeout )
{
	bool blResult = false;

	if (HAL_OK == HAL_I2C_Slave_Transmit(pstHi2c, pucData, unSize, ulTimeout))
	{
		blResult = true;
	}

	return blResult;
}

// EOF




