//******************************* osLayer *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : oslayer creation for STM32 communication framework(thread,
//         : semaphore, message queue).
// Note    : None
//
//*****************************************************************************
#ifndef OSLAYER_H
#define OSLAYER_H

//**************************** Include Files **********************************
#include "common.h"

//*************************** Global Types ************************************
typedef struct _THREAD_
{
	osThreadId_t stThreadID;
	uint32 stack_size;
	osPriority_t enpriority;
	void (*thread)(void *);

}_THREAD_;

//************************* Global Constants **********************************

//************************* Global Variables **********************************

//************************* Forward Declarations ******************************
bool osLayerCreation();
bool osLayerGetSemHandler(osSemaphoreId_t *ppSemSlaveHandle,
		                  osSemaphoreId_t *ppSemMasterHandle);
bool osLayerMQueueCreation(osMessageQueueId_t *ppMqSlaveHandle, uint16 unSize);

//********************** Inline Method Implementations ************************

#endif

// OSLAYER_H
// EOF
