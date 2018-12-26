/******************************************************************************
* @file			OSinit.h
* @author		D_EL - Storozhenko Roman
* @version      V1.0
*/
#include "globalIncludes.h"

/******************************************************************************
*-------NAME--------------------size [2byte Word] */
#define SYSTEM_TSK_SZ_STACK     96
#define SCOPE_TSK_SZ_STACK      64
#define BASE_TSK_SZ_STACK       128
#define CHARG_TSK_SZ_STACK      128
#define ADC_TSK_SZ_STACK        64
#define CUBE_TSK_SZ_STACK       96
#define BUBLE_TSK_SZ_STACK      64
#define UART_TSK_SZ_STACK       64
/******************************************************************************
*-------NAME--------------------size [2byte Word] */
#define SYSTEM_TSK_PRIO         4
#define SCOPE_TSK_PRIO          1
#define BASE_TSK_PRIO           1
#define CHARG_TSK_PRIO          1
#define ADC_TSK_PRIO            3
#define CUBE_TSK_PRIO           1
#define BUBLE_TSK_PRIO          1
#define UART_TSK_PRIO           2

/******************************************************************************
* TaskHandle
*/
extern xTaskHandle         windowTskHandle;
extern xTaskHandle         spsTskHandle;

/******************************************************************************
* Mutex
*/
extern xSemaphoreHandle    oneWireBusyMutex;
extern xSemaphoreHandle    LCDMutex;

/******************************************************************************
* Semaphore
*/
extern xSemaphoreHandle    AdcSem;
extern xSemaphoreHandle    AdcEndConversionSem;
extern xSemaphoreHandle    uartSem;

/******************************************************************************
* Prototypes for the functions in OSinit.c
*/
void OSinit(void);

/****************** (C) COPYRIGHT **************** END OF FILE ***************/
