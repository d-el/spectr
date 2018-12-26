/******************************************************************************
* @file    adcTSK.c 
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    07-01-2015
*/
#include "globalIncludes.h"

void adcTSK(void *pPrm){
    startSampling();
    
    while(1){
        xSemaphoreTake(AdcEndConversionSem, portMAX_DELAY);
        adcCalc();
        TIM3->ARR = adcStct.sampleRate;
        xSemaphoreGive(AdcSem);                     //Отдать семафор задаче-обработчику (осциллографу)
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** D_EL ****/
