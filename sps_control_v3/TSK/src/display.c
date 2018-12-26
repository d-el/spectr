/******************************************************************************
* @file    display.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#include "globalIncludes.h"

/******************************************************************************
*
*/
void displayTSK(void *pPrm){
    portTickType    xLastWakeTime;      //Вемя ОС
    uint16_t        step;
    int16_t         val;
    uint16_t        sum_sub;
    uint16_t        uartRxCntPrev;
    uint16_t        spStrRxCntPrev;
    uint16_t        blinlUpDateTim;
    uint8_t         blinkTim, blinkOnOff, blinkCnt;
    
    xLastWakeTime   = xTaskGetTickCount();                          //Инициализируем xLastWakeTime текущим временем
    step            = 1;
    val             = LED_MAX_VAL;
    sum_sub         = 0;
    uartRxCntPrev   = uart1->rxCnt;
    spStrRxCntPrev  = spStr.rxCnt;
    blinlUpDateTim  = 0;
    blinkCnt        = 0;
    blinkTim        = 0;
    blinkOnOff      = 0;
    
    while(1){    
        
        /**************************************
        * Плавная мигалка
        */
        if(sum_sub != 0){
            val += step;
        }else{
            val -= step;
        }
        
        if(val >= LED_MAX_VAL){
            val = LED_MAX_VAL;
            sum_sub = 0;
        }
        if(val < LED_MIN_VAL){
            val = LED_MIN_VAL;
            sum_sub = 1;
        }
        
        step = (spStr.vTask - vTaskMin) / 500 + 1;
        ledLightGreen(val / 12);
        
        /**************************************
        * Индикация приема пакета
        */
        if(blinlUpDateTim >= (BLINK_UPDATE_PERIOD / DISPLAY_TSK_PERIOD)){
            if(uart1->rxCnt != uartRxCntPrev){
                blinkCnt += ONE_BLINK;
                uartRxCntPrev = uart1->rxCnt;
            }
            if(spStr.rxCnt != spStrRxCntPrev){
                blinkCnt += ONE_BLINK;
                spStrRxCntPrev = spStr.rxCnt;
            }
            blinlUpDateTim = 0;
        }
        blinlUpDateTim++;
        
        if(blinkCnt != 0){
            if(blinkTim >= (BLINK_TIME / DISPLAY_TSK_PERIOD)){
                blinkOnOff ^= 1;
                blinkCnt--;
                blinkTim = 0;
            }
            blinkTim++;
            
            if(blinkOnOff != 0){
                ledLightRed(BLINK_LIGHT);
            }else{
                ledLightRed(0);
            }
        }

        /*************************************/
        vTaskDelayUntil(&xLastWakeTime, msToSt(DISPLAY_TSK_PERIOD));
    }
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
