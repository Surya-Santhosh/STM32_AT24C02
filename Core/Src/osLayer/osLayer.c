//********************************* osLayer ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : osLayer.c
// Summary : Read and write data from ESP32 to AT24C02.
// Note    : None
// Author  : Surya Santhosh
// Day     : 23/SEP/2025
//*****************************************************************************

//******************************* Include Files *******************************
#include "osLayer.h"
#include "stm32.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************
static THREAD stThread[] = {{0, {.priority = (osPriority_t) osPriorityNormal},
		                    StartTaskSlave},
		                   {0, {.priority = (osPriority_t) osPriorityNormal },
		                    StartTaskMaster}};

//***************************** Local Variables *******************************
static osSemaphoreId_t semMasterHandle;
static osSemaphoreId_t semSlaveHandle;

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

	for (ucIndex = 0; ucIndex < NUM_THREAD; ucIndex++)
	{
		stThread[ucIndex].stThreadID = osThreadNew(stThread[ucIndex].thread,
				               NULL, &stThread[ucIndex].stThreadAttributes);
	}

	return true;
}

//***************************.osLayerGetSemHandler.***************************
// Purpose : To provide semaphore handler for master and slave.
// Inputs  : None
// Outputs : None
// Return  : blResult
// Notes   : None
//*****************************************************************************
bool osLayerGetSemHandler(osSemaphoreId_t *ppsemSlaveHandle,
		                  osSemaphoreId_t *ppsemMasterHandle)
{
	bool blResult = false;

	if ((NULL != ppsemSlaveHandle) && (NULL != ppsemMasterHandle))
	{
		*ppsemSlaveHandle = semSlaveHandle;
		*ppsemMasterHandle = semMasterHandle;
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
	semSlaveHandle = osSemaphoreNew(1, 0, NULL);
	semMasterHandle = osSemaphoreNew(1, 0, NULL);

	return true;
}

//*************************.osLayerMQueueCreation.*****************************
// Purpose : To create message queue.
// Inputs  : None
// Outputs : None
// Return  : true
// Notes   : None
//*****************************************************************************
bool osLayerMQueueCreation(osMessageQueueId_t *ppmqSlaveHandle, uint16 unSize)
{
	bool blResult = false;

	if (NULL != ppmqSlaveHandle)
	{
		*ppmqSlaveHandle = osMessageQueueNew (1, unSize, NULL);
		blResult = true;
	}

	return blResult;
}

//****************************.osLayerCreation.********************************
// Purpose : Initialize thread and semaphore.
// Inputs  : None
// Outputs : None
// Return  : true
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

//EOF

