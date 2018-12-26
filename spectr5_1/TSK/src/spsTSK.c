/******************************************************************************
* @file			spsTSK.c
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			10.01.2015
* @brief		
*/
#include "globalIncludes.h"
/*
uint8_t vreg(uint8_t tsk){
    static uint8_t  out = SPS_MAX_VOLTAGE;
    static uint8_t  cnt;
    int8_t          err;

    if(tsk >= SPS_MAX_VOLTAGE){
        out = SPS_MAX_VOLTAGE;
        return  out;
    }
    
    err = tsk - out;
    if(err > 0){
        out = tsk;
    }
    
    if(err < 0){
        vTaskDelay(1500);
        if((out >= SPS_VOLT_BAND1L) && (out <= SPS_VOLT_BAND1H)){
             cnt++;
            if(cnt >= SPS_CALL_PER_S / SPS_VOLT_PER_S_BAND1){
                cnt = 0;
                out--;
            }
        }
        
        if((out >= SPS_VOLT_BAND2L) && (out <= SPS_VOLT_BAND2H)){
            cnt++;
            if(cnt >= SPS_CALL_PER_S / SPS_VOLT_PER_S_BAND2){
                cnt = 0;
                out--;
            }
        }
    }
    
    return out;
}*/

void spsTSK(void *pPrm){
    uint8_t         errCnt;     //Колличество ошибок связи
    uint8_t         vneed;      //Какое хотим выходное напряжение
    spsSt_type      spsSt;      //Статус текущей операции
    
    while(1){
        if(xSemaphoreTake(oneWireBusyMutex, portMAX_DELAY ) == pdTRUE){
            spsSt = sps_verification();
            if(spsSt == spsOk){
                //Возвращаем мютекс занятости 1Wire
                xSemaphoreGive(oneWireBusyMutex);
                break;
            }
            //Ошибка связи SPS
            psStatus.bit.ErrorSps = 1;
            //Возвращаем мютекс занятости 1Wire
            xSemaphoreGive(oneWireBusyMutex);
            vTaskDelay(250);
        }
    }

    while(1){
        if(xSemaphoreTake(oneWireBusyMutex, portMAX_DELAY ) == pdTRUE){

            spsSt = spsRedMem();

            if(spsSt == spsOk){
                errCnt = 0;
                sps_scratchpad.stateToSps.ledState ^= 1;
                
                if(sps.tmpr > MAX_TEMP_SWS){
                    switchOFF();
                    //Превышение температуры импульсника
                    psStatus.bit.OvfSpsTemper = 1;
                }
                
                //Регулируем выходное напряжение импульсного источника питания
                vneed = ((((_DACU * 10000) / 1600) + 1000/2) / 1000) + ADDITIV_VOLTAGE;
                //sps_scratchpad.vTask = vreg(vneed) * 1000;
                
                //Записываем в память SPS
                spsSt = spsWriteMem();
            }
            
            if((spsSt != spsOk)&&(errCnt < SPS_MAX_ERR)){
                errCnt++;
            }

            if(errCnt >= SPS_MAX_ERR){
                //Ошибка связи SPS
                psStatus.bit.ErrorSps = 1;
            }
            
            //Возвращаем мютекс занятости 1Wire
            xSemaphoreGive(oneWireBusyMutex);
        }
        vTaskDelay(250);
    }
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/