/**
******************************************************************************
* @file    key.c 
* @author  Storozhenko Roman - D_EL
* @version V1.2
* @date    22-11-2014
******************************************************************************
*/
#include "globalIncludes.h"

volatile ks_type    ks;
kc_type             kc;

uint8_t BtScan(void){
static uint8_t busyKey, rewindRun;
static uint16_t cntScanningToFirstRewind, cntScanningToRewind;

    //Обрабатываем событие виртуальной клавиатуры
    if(kc.fVirtualKey != 0){
        kc.fVirtualKey = 0;
        ks.ANY = 1;
        return ks.ANY;
    }

    //Обновляем по физическому состоянию входов
    ks.ANY = 0;

    if(gppin_get(GP_B_UP) == 0)     ks.UP       = 1;
    if(gppin_get(GP_B_DOWN) == 0)   ks.DOWN     = 1;
    if(gppin_get(GP_B_ZERO) == 0)   ks.ZERO     = 1;
    if(gppin_get(GP_B_MODE) == 0)   ks.BMDVW    = 1;
    if(gppin_get(GP_B_SET) == 0)    ks.SET      = 1;
    if(gppin_get(GP_B_SWITCH) == 0) ks.SWITCH   = 1;
    if(gppin_get(GP_B_NEXT) == 0)   ks.NEXT     = 1;
    
    //Обрабатываем автонажатие
    if(kc.AutoPress != 0){
    
        if(b_rewind != 0){
            
            cntScanningToFirstRewind++;
            
            if(cntScanningToFirstRewind >= kc.scanningToFirstRewind){
                rewindRun = 1;
            }
     
        }else{
            cntScanningToFirstRewind = 0;
            rewindRun = 0;
        }
        
        if(rewindRun != 0){
            cntScanningToRewind++;
        }
        if(cntScanningToRewind >= kc.scanningToRewind){
            cntScanningToRewind = 0;
            return ks.ANY;
        }
        
    }
    
    //Обрабатываем обычный режим
    if(ks.ANY){ //Если нажата хоть одна кнопка
        if(busyKey == 0){
            busyKey = 1;
            return ks.ANY;
        }
    }else{
        busyKey = 0;
    }

    ks.ANY = 0;
    return 0;
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
