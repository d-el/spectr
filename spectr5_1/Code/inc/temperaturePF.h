/******************************************************************************
* @file    temperaturePF.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    14-09-2015
* @brief
*/
#ifndef temperaturePF_H
#define temperaturePF_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define temOsCriticalEnter()    vTaskSuspendAll()
#define temOsCriticalExit()     xTaskResumeAll()
#define TATOMIC(x)          \
    temOsCriticalEnter();   \
    x;                      \
    temOsCriticalExit();
    
/******************************************************************************
* User typedef
*/
typedef enum{
    temp_Ok,
    temp_ErrSensor
}temperatureState_type;
    
typedef struct{
    temperatureState_type   state;
    uint16_t                temperature;    //[X_X °C]
}temperature_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern temperature_type   temperature;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void temperaturePF(void);

#endif //temperaturePF_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
