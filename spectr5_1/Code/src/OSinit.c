/******************************************************************************
* @file			OSinit.c
* @author		D_EL - Storozhenko Roman
* @version      V1.0
*/
#include "globalIncludes.h"

/******************************************************************************
* TaskHandle
*/
xTaskHandle         windowTskHandle;
xTaskHandle         spsTskHandle;

/******************************************************************************
* Mutex
*/
xSemaphoreHandle    oneWireBusyMutex;
xSemaphoreHandle    LCDMutex;

/******************************************************************************
* Semaphore
*/
xSemaphoreHandle    AdcSem;
xSemaphoreHandle    AdcEndConversionSem;
xSemaphoreHandle    uartSem;

/******************************************************************************
*
*/
void OSinit(void){
    BaseType_t Result;

    //Создаём семафор
    vSemaphoreCreateBinary(AdcSem);
    xSemaphoreTake(AdcSem, portMAX_DELAY);
    vSemaphoreCreateBinary(AdcEndConversionSem);
    xSemaphoreTake(AdcEndConversionSem, portMAX_DELAY);
    vSemaphoreCreateBinary(uartSem);
    xSemaphoreTake(uartSem, portMAX_DELAY);
    
    //Создаём мьютекс
    LCDMutex            = xSemaphoreCreateMutex();
    oneWireBusyMutex    = xSemaphoreCreateMutex();

    Result = pdTRUE;
    //Создаём задачи перед запуском ядра
    Result &= xTaskCreate(adcTSK,       "adcTSK",       ADC_TSK_SZ_STACK,       NULL, ADC_TSK_PRIO,     NULL);
    Result &= xTaskCreate(systemTSK,    "systemTSK",    SYSTEM_TSK_SZ_STACK,    NULL, SYSTEM_TSK_PRIO,  NULL);
    Result &= xTaskCreate(uartTSK,      "uartTSK",      UART_TSK_SZ_STACK,      NULL, UART_TSK_PRIO,    NULL);
    
    if(Result == pdTRUE){
        ps.state.bit.osInit = 1;
    }
    else{
        lcd_PutString8x12(5,65,"OS ERROR");
        ps.state.bit.osInit = 0;
    }
    
    selWindow(baseWindow);
    
    vTaskStartScheduler();
}

/****************** (C) COPYRIGHT **************** END OF FILE ***************/
