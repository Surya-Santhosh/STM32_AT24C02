//*********************************** stm32 ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : stm32.c
// Summary : Read and write data from ESP32 to AT24C02.
// Note    : None
// Author  : Surya Santhosh
// Day     : 10/SEP/2025
//*****************************************************************************

//******************************* Include Files *******************************
#include "stm32.h"

//******************************* Local Types *********************************
I2C_HandleTypeDef hi2c2 = {0};
I2C_HandleTypeDef hi2c1 = {0};

//***************************** Local Constants *******************************
static osSemaphoreId_t semMasterHandle;
static osSemaphoreId_t semSlaveHandle;
static osMessageQueueId_t mqSlaveHandle;
static osMessageQueueId_t mqMasterHandle;

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************

//********************************.stm32Slave.*********************************
// Purpose : Act as STM32 I2C Slave, Receive commands from ESP32 send to
//         : master via message queue and send back Ack to ESP32.
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
bool stm32Slave()
{
	uint8 ucRxData[TX_MESSAGE_SIZE] = {0};
	uint8 ucTxData[RX_MESSAGE_SIZE] = {0};
	TX_MESSAGE stTxMessage = {0};
	RX_MESSAGE stRecievedRxMessage ={0};

	if (true != osLayerGetSemHandler(&semSlaveHandle, &semMasterHandle))
	{
		printf("osLayerGetSemHandler failed\r\n");
	}

	if (HAL_OK == HAL_I2C_Slave_Receive(&hi2c2, ucRxData, TX_MESSAGE_SIZE,
			                            TIMEOUT))
	{
		memcpy(&stTxMessage, ucRxData, sizeof(ucRxData));

		if (osOK == osMessageQueuePut((osMessageQueueId_t) mqSlaveHandle,
											   &stTxMessage, 0, TIMEOUT))
		{
			printf("Message Received from ESP32\r\n");
		}
	}

	osSemaphoreRelease(semSlaveHandle);

	// Wait from Ack.
	osSemaphoreAcquire(semMasterHandle, osWaitForever);

	if (osOK == osMessageQueueGet((osMessageQueueId_t) mqMasterHandle,
	    		                       &stRecievedRxMessage, 0, TIMEOUT))
	{
		printf("Message Received from EEPROM\r\n");

		memcpy(ucTxData, &stRecievedRxMessage, sizeof(RX_MESSAGE));

		if (HAL_OK == HAL_I2C_Slave_Transmit(&hi2c2, ucTxData, RX_MESSAGE_SIZE,
				                             TIMEOUT))
		{
		  printf("Data Tx Success \r\n");
		}
		else
		{
		  printf("Tx Failed\r\n");
		}
	}

	osSemaphoreRelease(semSlaveHandle);

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
bool stm32Master()
{
	TX_MESSAGE stRecievedTxMessage = {0};
	RX_MESSAGE stRxMessage ={0};

	osSemaphoreAcquire(semSlaveHandle, osWaitForever);

    if (osOK == osMessageQueueGet((osMessageQueueId_t) mqSlaveHandle,
    		                       &stRecievedTxMessage, 0, TIMEOUT))
    {
    	printf("Message Received from Slave.\r\n");

    	if (WRITE_CMD == stRecievedTxMessage.ucCmd)
    	{
    		if (true == i2cWrite(&hi2c1, EEPROM_DEVICE_ADDRESS,
    				             stRecievedTxMessage.ucAddress,
    		    			     (uint16)I2C_MEMADD_SIZE_8BIT,
    		    				 &stRecievedTxMessage.ucValue,
							     sizeof(stRecievedTxMessage.ucValue), TIMEOUT))
			{
    			stRxMessage.ucAck = WRITE_ACK;
			}
    	}
    	else
    	{
    		if (true == i2cRead(&hi2c1, EEPROM_DEVICE_ADDRESS,
    				            stRecievedTxMessage.ucAddress,
    				            (uint16)I2C_MEMADD_SIZE_8BIT,
								&stRxMessage.ucValue,
								sizeof(stRxMessage.ucValue), TIMEOUT))
			{
				stRxMessage.ucAck = READ_ACK;
			}
    	}

		if (osOK == osMessageQueuePut((osMessageQueueId_t) mqMasterHandle,
									&stRxMessage, 0, TIMEOUT))
		{
			printf("Message send to slave\r\n");
		}
    }

	osSemaphoreRelease(semMasterHandle);
	osSemaphoreAcquire(semSlaveHandle, osWaitForever);

	return true;
}
//***************************.stm32SlaveI2C2Init.******************************
// Purpose : Initialize I2C2.
// Inputs  : None
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool stm32SlaveI2C2Init()
{
	bool blResult = false;
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 100000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 24;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_OK == HAL_I2C_Init(&hi2c2))
	{
		blResult = true;
	}

	return blResult;
}

//**************************.stm32SlaveI2C1Init.*******************************
// Purpose : Initialize I2C1.
// Inputs  : None
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool stm32SlaveI2C1Init()
{
	bool blResult = false;
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_OK == HAL_I2C_Init(&hi2c1))
	{
		blResult = true;
	}

	return blResult;
}

//*****************************.StartTaskSlave.********************************
// Purpose : Thread function for STM32 Slave operation.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
void StartTaskSlave()
{
	if (true != osLayerMQueueCreation(&mqSlaveHandle, sizeof(TX_MESSAGE)))
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

//*****************************.StartTaskMaster.*******************************
// Purpose : Thread function for STM32 Master operation.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : None
//*****************************************************************************
void StartTaskMaster()
{
	if (true != osLayerMQueueCreation(&mqMasterHandle, sizeof(RX_MESSAGE)))
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

