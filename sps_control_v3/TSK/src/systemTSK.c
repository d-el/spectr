/******************************************************************************
* @file    systemTSK.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#include "globalIncludes.h"

/******************************************************************************
*
*/
status_type        status;

/******************************************************************************
*
*/
void systemTSK(void *pPrm){
    portTickType        xLastWakeTime;      //Вемя ОС
    uint32_t            code;
    uint32_t            vDacCumul;
    
    xLastWakeTime   = xTaskGetTickCount();                          //Инициализируем xLastWakeTime текущим временем
    spStr.vTask     = vTaskDef;
    vDacCumul = (((uint32_t)4095 * spStr.vTask) / vTaskMax) << VTASK_FILTER_K;
    
    while(1){
        spStr.voltage     = IQtoInt(adcStct.voltage, 1000);
        spStr.temperature = IQtoInt(adcStct.temperature, 10);
        
        if(spStr.vTask > vTaskMax){
            spStr.vTask = vTaskMax;
        }else if(spStr.vTask < vTaskMin){
            spStr.vTask = vTaskMin;
        }
        if(spStr.bitTask.temperatureLimOn == 1){
            if(spStr.temperature > TMax){
                spStr.vTask = vTaskMin;
            }
        }
        code = (4095 * spStr.vTask) / vTaskMax;
        setDacU(iq_filtr(vDacCumul, code, VTASK_FILTER_K));

        /*************************************/
        vTaskDelayUntil(&xLastWakeTime, msToSt(DISPLAY_TSK_PERIOD));
    }
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
