//********************************* osLayer ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : osLayer.c
// Summary : oslayer creation for STM32 communication framework(thread,
//         : semaphore, message queue).
// Note    : None
// Author  : Surya Santhosh
// Day     : 23/SEP/2025
//*****************************************************************************

//******************************* Include Files *******************************
#include "osLayer.h"
#include "stm32.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************
static _THREAD_ stThread[] = {{NULL, STACK_SIZE, osPriorityNormal,
		                      stm32SlaveTask},
		                      {NULL, STACK_SIZE, osPriorityNormal,
		                      stm32MasterTask}};

//***************************** Local Variables *******************************
static osSemaphoreId_t pSemMasterHandle;
static osSemaphoreId_t pSemSlaveHandle;

//****************************** Local Functions ******************************
static bool osLayerSemCreation();
static bool osLayerThreadCreation();

//***************************.osLayerThreadCreation.***************************
// Purpose : To Create Thread.
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
static bool osLayerThreadCreation()
{
	uint8 ucIndex = 0;
	osThreadAttr_t stThreadAttributes = {0};
	uint8 ucNumThread = sizeof(stThread)/sizeof(_THREAD_);

	for (ucIndex = 0; ucIndex < ucNumThread; ucIndex++)
	{
		stThreadAttributes.stack_size = stThread[ucIndex].stack_size;
		stThreadAttributes.priority = stThread[ucIndex].enpriority;
		stThread[ucIndex].stThreadID = osThreadNew(stThread[ucIndex].thread,
				                                   NULL, &stThreadAttributes);
	}

	return true;
}

//***************************.osLayerGetSemHandler.***************************
// Purpose : To provide semaphore handler for master and slave.
// Inputs  : ppSemSlaveHandle - pointer to receive slave semaphore handler.
//         : ppSemMasterHandle - pointer to receive master semaphore handler.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool osLayerGetSemHandler(osSemaphoreId_t *ppSemSlaveHandle,
		                  osSemaphoreId_t *ppSemMasterHandle)
{
	bool blResult = false;

	if ((NULL != ppSemSlaveHandle) && (NULL != ppSemMasterHandle))
	{
		*ppSemSlaveHandle = pSemSlaveHandle;
		*ppSemMasterHandle = pSemMasterHandle;
		blResult = true;
	}

	return blResult;
}

//****************************.osLayerSemCreation.*****************************
// Purpose : To create Semaphore
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
static bool osLayerSemCreation()
{
	pSemSlaveHandle = osSemaphoreNew(1, 0, NULL);
	pSemMasterHandle = osSemaphoreNew(1, 0, NULL);

	return true;
}

//*************************.osLayerMQueueCreation.*****************************
// Purpose : To create message queue.
// Inputs  : ppMqSlaveHandle - pointer to store message queue handler.
//         : unSize - Size of each message.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool osLayerMQueueCreation(osMessageQueueId_t *ppMqSlaveHandle, uint16 unSize)
{
	bool blResult = false;

	if (NULL != ppMqSlaveHandle)
	{
		*ppMqSlaveHandle = osMessageQueueNew (1, unSize, NULL);
		blResult = true;
	}

	return blResult;
}

//****************************.osLayerCreation.********************************
// Purpose : Initialize thread and semaphore.
// Inputs  : None
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool osLayerCreation()
{
	bool blResult = false;

	do
	{
		if (true != osLayerThreadCreation())
		{
			printf("osLayerThreadCreation failed\r\n");
		}
		if (true != osLayerSemCreation())
		{
			printf("osLayerThreadCreation failed\r\n");
		}

		blResult = true;

	}while (true != blResult);

	return blResult;
}

//*************************.osLayerMessageQueuePut.****************************
// Purpose : Put a Message into Queue.
// Inputs  : pMqHandle - Message queue handler.
//         : pvMessage - pointer to message to put into a queue.
//         : ucPriority - Message priority.
//         : ulTimeout - Timeout duration.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool osLayerMessageQueuePut(osMessageQueueId_t pMqHandle, void *pvMessage,
		                    uint8 ucPriority, uint32 ulTimeout)
{
	bool blResult = false;

	if (osOK == osMessageQueuePut(pMqHandle, pvMessage, ucPriority, ulTimeout))
	{
		blResult = true;
	}

	return blResult;
}

//*************************.osLayerMessageQueueGet.****************************
// Purpose : Get a Message from a Queue.
// Inputs  : pMqHandle - Message queue handler.
//         : pvMessage - Pointer to message to put into a queue.
//         : pucPriority - Pointer to Message priority.
//         : ulTimeout - Timeout duration.
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool osLayerMessageQueueGet(osMessageQueueId_t pMqHandle, void *pvMessage,
		                    uint8 *pucPriority, uint32 ulTimeout)
{
	bool blResult = false;

	if (osOK == osMessageQueueGet(pMqHandle, pvMessage, pucPriority, ulTimeout))
	{
		blResult = true;
	}

	return blResult;
}

//EOF

