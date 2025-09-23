//******************************* osLayer *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : Read and write operation using I2C Communication.
// Note    : None
//
//*****************************************************************************
#ifndef OSLAYER_H
#define OSLAYER_H

//**************************** Include Files **********************************
#include "common.h"

//*************************** Global Types ************************************
typedef struct THREAD
{
	osThreadId_t stThreadID;
	const osThreadAttr_t stThreadAttributes;
	void (*thread)(void *);

}THREAD;

//************************* Global Constants **********************************

//************************* Global Variables **********************************

//************************* Forward Declarations ******************************
bool osLayerCreation();
bool osLayerGetSemHandler(osSemaphoreId_t *ppsemSlaveHandle,
		                  osSemaphoreId_t *ppsemMasterHandle);
bool osLayerMQueueCreation(osMessageQueueId_t *ppmqSlaveHandle, uint16 unSize);

//********************** Inline Method Implementations ************************

#endif

// OSLAYER_H
// EOF
