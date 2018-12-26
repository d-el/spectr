/******************************************************************************
* @file    systemTSK.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#ifndef systemTSK_H
#define systemTSK_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define SYSTEM_TSK_PERIOD       20      //[ms]
#define VTASK_FILTER_K          8
#define DacVref                 3.293   //[V]
#define vTaskDef                25000   //[mV]
#define vTaskMin                11000   //[mV]
#define vTaskMax                33000   //[mV]
#define TMax                    1000    //[X_X °C]

/******************************************************************************
* User typedef
*/
typedef union{
    struct{
        uint16_t    osInit                      :1;
    }bit;
    uint16_t        all;
}status_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern status_type        status;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void systemTSK(void *pPrm);

#endif //systemTSK_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
