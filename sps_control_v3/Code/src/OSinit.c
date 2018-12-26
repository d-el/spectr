/*
******************************************************************************
* @file			OSinit.c
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			
* @brief		Функции
******************************************************************************  
*/
#include "globalIncludes.h"

/*************************************************
 *TaskHandle
*************************************************/

/*************************************************
 *Mutex
*************************************************/
xSemaphoreHandle    oneWireBusyMutex;

/*************************************************
 *Semaphore
*************************************************/
xSemaphoreHandle    AdcSem;
xSemaphoreHandle    AdcEndConversionSem;
xSemaphoreHandle    uartSem;

void OSinit(void){
    BaseType_t Result;

    vSemaphoreCreateBinary(AdcEndConversionSem);
    xSemaphoreTake(AdcEndConversionSem, portMAX_DELAY);
    vSemaphoreCreateBinary(uartSem);
    xSemaphoreTake(uartSem, portMAX_DELAY);

    Result = pdTRUE;

    //Создаём задачи перед запуском ядра
    Result &= xTaskCreate(systemTSK,    "systemTSK",    SYSTEM_TSK_SZ_STACK,    NULL, SYSTEM_TSK_PRIO,  NULL);
    Result &= xTaskCreate(adcTSK,       "adcTSK",       ADC_TSK_SZ_STACK,       NULL, ADC_TSK_PRIO,     NULL);
    Result &= xTaskCreate(displayTSK,   "displayTSK",   DISPLAY_TSK_SZ_STACK,   NULL, DISPLAY_TSK_PRIO, NULL);
    Result &= xTaskCreate(uartTSK,      "uartTSK",      UART_TSK_SZ_STACK,      NULL, UART_TSK_PRIO,    NULL);	
 
    if(Result == pdTRUE){
        status.bit.osInit = 1;
    }
    else{
        status.bit.osInit = 0;
    }
    
    vTaskStartScheduler();
}

/******************* (C) COPYRIGHT ***************** END OF FILE ******************/
