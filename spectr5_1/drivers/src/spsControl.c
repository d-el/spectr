/******************************************************************************
* @file    spsControl
* @author  d_el
* @version V2.0
* @date    20.09.2015, by d_el
* @brief   Управление ИИП
*/
#include "globalIncludes.h"

/******************************************************************************
* Memory
*/
sps_type        sps = {
    spsOk,
    {
        0,      //temperature
        0,      //voltage
        0,      //hvoltage
        {
            0,      //mhvPresent
        },
        0,      //crcRd
        {
            1,      //temperatureLimOn
        },
        25000,  //vTask
        0       //crcWr
    }
};
const uint8_t   sps_owid[8] = {'S', 'P', 'S', '_', 'V', '2', '0', 0x64};

/******************************************************************************
*
*/
spsSt_type spsRedMem(void){
    uint8_t         *pnt, *pntEnd;
    uint8_t         mas[7], crc;
    owSt_type       st;
    
    pnt = &mas[0];
    pntEnd = pnt + sizeof(mas);
    
    owch2_init();
    SATOMIC(st = ow_init());                        //Инициализация шины 1-wire
    if(st != owOk){
        return (spsSt_type)st;                      //Если были ошибки то возвращаем код ошибки
    }
    SATOMIC(ow_write(OWCOM_READ_SCRATCHPAD));
    while(pnt < pntEnd){
        SATOMIC(*pnt = ow_read());
        pnt++;
    }
    crc = crc8(&mas[0], sizeof(mas)-1);
    if(crc == mas[sizeof(mas)-1]){
        memcpy(&sps.scratchpad, &mas[0], sizeof(mas));
        sps.scratchpad.rxCnt++;
        return spsOk;
    }
    else{
        sps.scratchpad.errCnt++;
        return spsCrcError;
    }
    
}

/******************************************************************************
*
*/
spsSt_type spsWriteMem(void){
    owSt_type       st;
    uint8_t         *pnt, *pntEnd;
    uint8_t         wrSz = (((uint32_t)&(sps.scratchpad.crcWr) -  (uint32_t)&(sps.scratchpad.bitTask)) + sizeof(sps.scratchpad.crcWr));
    
    sps.scratchpad.crcWr = crc8((uint8_t*)&sps.scratchpad.bitTask, wrSz - 1);
    pnt = (uint8_t*)&sps.scratchpad.bitTask;
    pntEnd = pnt + wrSz; 
    
    owch2_init();
    SATOMIC(st = ow_init());                                    //Инициализация шины 1-wire
    if(st != owOk)  return (spsSt_type)st;                      //Если были ошибки то возвращаем код ошибки
    SATOMIC(ow_write(OWCOM_WRITE_SCRATCHPAD));                  //Write scratchpad

    while(pnt < pntEnd){
        SATOMIC(ow_write(*pnt));
        pnt++;
    }
    return spsOk;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
