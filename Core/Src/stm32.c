//*********************************** stm32 ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : stm32.c
// Summary : Implement STM32 as I2C slave and master, intermediate
//         : communication between ESP32 and AT24C02.
// Note    : None
// Author  : Surya Santhosh
// Day     : 10/SEP/2025
//*****************************************************************************

//******************************* Include Files *******************************
#include "stm32.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************
static osSemaphoreId_t pSemMasterHandle;
static osSemaphoreId_t pSemSlaveHandle;
static osMessageQueueId_t pMqSlaveHandle;
static osMessageQueueId_t pMqMasterHandle;

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************
static bool stm32Slave();
static bool stm32Master();
static bool stm32slaveReceivedFromEsp32(I2C_HandleTypeDef *pstHi2c2);
static bool stm32slaveTransmitToEsp32(I2C_HandleTypeDef *pstHi2c2);

//*******************************.stm32Slave.**********************************
// Purpose : Act as STM32 I2C Slave, Receive commands from ESP32 send to
//         : master via message queue and send back Ack to ESP32.
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
static bool stm32Slave()
{
	I2C_HandleTypeDef stHi2c2 = {0};

	if (true != osLayerGetSemHandler(&pSemSlaveHandle, &pSemMasterHandle))
	{
		printf("osLayerGetSemHandler failed\r\n");
	}

	if (true != i2c2GetHandler(&stHi2c2))
	{
		printf("osLayerGetI2C2Handler failed\r\n");
	}

	if (true == stm32slaveReceivedFromEsp32(&stHi2c2))
	{
		// dummy case.
	}

	osSemaphoreRelease(pSemSlaveHandle);
	osSemaphoreAcquire(pSemMasterHandle, osWaitForever);

	if (true == stm32slaveTransmitToEsp32(&stHi2c2))
	{
		// dummy case.
	}

	osSemaphoreRelease(pSemSlaveHandle);

	return true;
}

//********************************.stm32Master.********************************
// Purpose : Act as STM32 I2C Master, Receive commands from Slave send to
//         : AT24C02 via message queue and send back Ack to Slave.
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
static bool stm32Master()
{
	_TX_MESSAGE_ stRecievedTxMessage = {0};
	_RX_MESSAGE_ stRxMessage ={0};
	_I2C_PACKET_ stI2cPacket = {0};
	I2C_HandleTypeDef stHi2c1 = {0};

	if (true != osLayerGetSemHandler(&pSemSlaveHandle, &pSemMasterHandle))
	{
		printf("osLayerGetSemHandler failed\r\n");
	}

	osSemaphoreAcquire(pSemSlaveHandle, osWaitForever);

	if (true != i2c1GetHandler(&stHi2c1))
	{
		printf("osLayerGetI2C1Handler failed\r\n");
	}

    if (true == osLayerMessageQueueGet(pMqSlaveHandle, &stRecievedTxMessage,
    		                           NULL, TIMEOUT))
    {
    	printf("Message Received from Slave.\r\n");

    	stI2cPacket.stI2C = stHi2c1;
    	stI2cPacket.unDeviceAddress = EEPROM_DEVICE_ADDRESS;
    	stI2cPacket.unMemoryAddress = stRecievedTxMessage.ucAddress;
    	stI2cPacket.unMemoryAddressSize = I2C_MEMADD_SIZE_8BIT;
    	stI2cPacket.ulTimeout = TIMEOUT;

    	if (WRITE_CMD == stRecievedTxMessage.ucCmd)
    	{
    		stI2cPacket.ucData = stRecievedTxMessage.ucData;
    		stI2cPacket.unSize = sizeof(stRecievedTxMessage.ucData);

    		if (true == i2cWrite(&stI2cPacket))
			{
    			stRxMessage.ucAck = WRITE_ACK;
			}
    	}
    	else
    	{
    		stI2cPacket.ucData = stRxMessage.ucData;
    		stI2cPacket.unSize = sizeof(stRxMessage.ucData);

    		if (true == i2cRead(&stI2cPacket))
			{
				stRxMessage.ucAck = READ_ACK;
				stRxMessage.ucData = stI2cPacket.ucData;
			}
    	}

		if (true == osLayerMessageQueuePut(pMqMasterHandle, &stRxMessage, 0,
				                           TIMEOUT))
		{
			printf("Message send to slave from EEPROM\r\n");
		}
    }

	osSemaphoreRelease(pSemMasterHandle);
	osSemaphoreAcquire(pSemSlaveHandle, osWaitForever);

	return true;
}

//*****************************.stm32SlaveTask.********************************
// Purpose : Thread function for STM32 Slave operation.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
void stm32SlaveTask()
{
	if (true != osLayerMQueueCreation(&pMqSlaveHandle, sizeof(_TX_MESSAGE_)))
	{
		printf ("Message Queue creation failed\r\n");
	}

	while (1)
	{
	  if (true != stm32Slave())
	  {
		  printf("Error in stm32Slave\r\n");
	  }
	}
}

//*****************************.stm32MasterTask.*******************************
// Purpose : Thread function for STM32 Master operation.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
void stm32MasterTask()
{
	if (true != osLayerMQueueCreation(&pMqMasterHandle, sizeof(_RX_MESSAGE_)))
	{
		printf ("Message Queue creation failed\r\n");
	}

	while (1)
	{
	  if (true != stm32Master())
	  {
		  printf("Error in stm32Master\r\n");
	  }
	}
}

//***************************.stm32slaveEsp32.*********************************
// Purpose : Act as STM32 I2C Slave, Receive commands from ESP32 send to
//         : master via message queue.
// Inputs  : pstHi2c2 - pointer to receive I2C2 Handler.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
static bool stm32slaveReceivedFromEsp32(I2C_HandleTypeDef *pstHi2c2)
{
	bool blResult = false;
	uint8 ucRxData[TX_MESSAGE_SIZE] = {0};
	_TX_MESSAGE_ stTxMessage = {0};

	if (true == i2cSlaveReceive(pstHi2c2, ucRxData, TX_MESSAGE_SIZE,
			                      TIMEOUT))
	{
		printf("Message Received from ESP32\r\n");

		memcpy(&stTxMessage, ucRxData, sizeof(ucRxData));

		if (true == osLayerMessageQueuePut(pMqSlaveHandle, &stTxMessage, 0,
				                           TIMEOUT))
		{
			printf("Message send to Master from ESP32.\r\n");

			blResult = true;
		}
	}

	return blResult;
}

//**************************.stm32slaveAt24c02.********************************
// Purpose : Act as STM32 I2C Slave, send back Ack to ESP32 from Master.
// Inputs  : pstHi2c2 - pointer to receive I2C2 Handler.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
static bool stm32slaveTransmitToEsp32(I2C_HandleTypeDef *pstHi2c2)
{
	bool blResult = false;
	uint8 ucTxData[RX_MESSAGE_SIZE] = {0};
	_RX_MESSAGE_ stRecievedRxMessage ={0};

	if (true == osLayerMessageQueueGet(pMqMasterHandle, &stRecievedRxMessage,
			                           NULL, TIMEOUT))
	{
		printf("Message Received from Master\r\n");

		memcpy(ucTxData, &stRecievedRxMessage, sizeof(_RX_MESSAGE_));

		if (true == i2cSlaveTransmit(pstHi2c2, ucTxData, RX_MESSAGE_SIZE,
				                       TIMEOUT))
		{
		  printf("Data Tx Success \r\n");

		  blResult = true;
		}
		else
		{
		  printf("Tx Failed\r\n");
		}
	}

	return blResult;
}

//EOF

