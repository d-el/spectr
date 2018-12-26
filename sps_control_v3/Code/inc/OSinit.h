/******************************************************************************
* @file    example.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#ifndef example_H
#define example_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
//----------------------------------------------------
//      NAME                    size [2byte Word]
#define ADC_TSK_SZ_STACK        64
#define UART_TSK_SZ_STACK       64
#define DISPLAY_TSK_SZ_STACK    64
#define SYSTEM_TSK_SZ_STACK     64
//----------------------------------------------------
//      NAME                    Prioriti
#define ADC_TSK_PRIO            4
#define UART_TSK_PRIO           3
#define DISPLAY_TSK_PRIO        1
#define SYSTEM_TSK_PRIO         2

/******************************************************************************
* User typedef
*/

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern xSemaphoreHandle    AdcSem;
extern xSemaphoreHandle    AdcEndConversionSem;
extern xSemaphoreHandle    uartSem;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void OSinit(void);

#endif //example_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
