/******************************************************************************
* @file    spsPF.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    14-09-2015
* @brief
*/
#include "globalIncludes.h"

/******************************************************************************
*
*/    
void spsPF(void){
    spsSt_type      spsSt;      //Статус текущей операции
    //uint16_t        vneed;      //Какое хотим выходное напряжение
    static uint8_t  errCnt;     //Колличество ошибок связи
    
    spsSt = spsRedMem();

    if(spsSt == spsOk){
        errCnt = 0;        
        //Регулируем выходное напряжение импульсного источника питания
        sps.scratchpad.vTask = vreg(IQtoInt(adcStct.voltage, 1000) + ADDITIV_VOLTAGE);
        //Записываем в память SPS
        spsSt = spsWriteMem();
    }
    
    if((spsSt != spsOk)&&(errCnt < SPS_MAX_ERR)){
        errCnt++;
    }
    if(errCnt < SPS_MAX_ERR){
        sps.state = spsOk;
    }else{
        sps.state = spsError;
    }
}

/******************************************************************************
*
*/
uint16_t vreg(uint16_t vtsk){
    static uint32_t     vCumul;
    uint16_t            out;
    
    if(vtsk >= (vCumul >> kFiltrSpsTsk)){
        out = vtsk;
        vCumul = (vtsk << kFiltrSpsTsk);
    }
    else{
        out = iq_filtr(vCumul, vtsk, kFiltrSpsTsk);
    }

    if(out > SPS_MAX_VOLTAGE){
        out = SPS_MAX_VOLTAGE;
    }
    else if(out < SPS_MIN_VOLTAGE){
        out = SPS_MIN_VOLTAGE;
    }
    return out;
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
