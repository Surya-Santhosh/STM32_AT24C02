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
static I2C_HandleTypeDef stHi2c2;
static I2C_HandleTypeDef stHi2c1;

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************
static bool stm32Slave();
static bool stm32Master();

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
	uint8 ucRxData[TX_MESSAGE_SIZE] = {0};
	uint8 ucTxData[RX_MESSAGE_SIZE] = {0};
	_TX_MESSAGE_ stTxMessage = {0};
	_RX_MESSAGE_ stRecievedRxMessage ={0};

	if (true != osLayerGetSemHandler(&pSemSlaveHandle, &pSemMasterHandle))
	{
		printf("osLayerGetSemHandler failed\r\n");
	}

	if (true != i2cGetHandler(&stHi2c1, &stHi2c2))
	{
		printf("osLayerGetI2CHandler failed\r\n");
	}

	if (HAL_OK == HAL_I2C_Slave_Receive(&stHi2c2, ucRxData, TX_MESSAGE_SIZE,
			                            TIMEOUT))
	{
		memcpy(&stTxMessage, ucRxData, sizeof(ucRxData));

		if (osOK == osMessageQueuePut((osMessageQueueId_t) pMqSlaveHandle,
											   &stTxMessage, 0, TIMEOUT))
		{
			printf("Message Received from ESP32\r\n");
		}
	}

	osSemaphoreRelease(pSemSlaveHandle);
	osSemaphoreAcquire(pSemMasterHandle, osWaitForever);

	if (osOK == osMessageQueueGet((osMessageQueueId_t) pMqMasterHandle,
	    		                       &stRecievedRxMessage, 0, TIMEOUT))
	{
		printf("Message Received from EEPROM\r\n");
		memcpy(ucTxData, &stRecievedRxMessage, sizeof(_RX_MESSAGE_));

		if (HAL_OK == HAL_I2C_Slave_Transmit(&stHi2c2, ucTxData, RX_MESSAGE_SIZE,
				                             TIMEOUT))
		{
		  printf("Data Tx Success \r\n");
		}
		else
		{
		  printf("Tx Failed\r\n");
		}
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

	if (true != osLayerGetSemHandler(&pSemSlaveHandle, &pSemMasterHandle))
	{
		printf("osLayerGetSemHandler failed\r\n");
	}

	osSemaphoreAcquire(pSemSlaveHandle, osWaitForever);

    if (osOK == osMessageQueueGet((osMessageQueueId_t) pMqSlaveHandle,
    		                       &stRecievedTxMessage, 0, TIMEOUT))
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

		if (osOK == osMessageQueuePut((osMessageQueueId_t) pMqMasterHandle,
									&stRxMessage, 0, TIMEOUT))
		{
			printf("Message send to slave\r\n");
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

//EOF

