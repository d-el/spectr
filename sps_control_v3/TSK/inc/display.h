/******************************************************************************
* @file    display.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#ifndef display_H
#define display_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define DISPLAY_TSK_PERIOD  1       //[ms]

#define BLINK_UPDATE_PERIOD 1000    //[ms]
#define BLINK_TIME          100     //[ms]
#define ONE_BLINK           2
#define BLINK_LIGHT         300     //[X_X %]

#define LED_MAX_VAL         10000   //[X_XX %]
#define LED_MIN_VAL         0       //[X_XX %]

/******************************************************************************
* User typedef
*/

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void displayTSK(void *pPrm);

#endif //display_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
