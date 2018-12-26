/*
******************************************************************************
* @file			TemTSK.c
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			10.01.2015
* @brief		
******************************************************************************  
*/
#ifndef TEM_TSK_H
#define TEM_TSK_H
#include "globalIncludes.h"

#define temOsCriticalEnter()    vTaskSuspendAll()
#define temOsCriticalExit()     xTaskResumeAll()

#define TATOMIC(x)          \
    temOsCriticalEnter();   \
    x;                      \
    temOsCriticalExit();

//-----------------------------------------------------------
//Prototypes for the functions in temTSK.c
//-----------------------------------------------------------
void temTSK(void *pParm);

#endif  //TEM_TSK_H

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
