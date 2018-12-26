/******************************************************************************
* @file    spsControl
* @author  d_el
* @version V2.0
* @date    20.09.2015, by d_el
* @brief   ”Ô‡‚ÎÂÌËÂ »»œ
*/
#ifndef spsControl_H
#define spsControl_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/

/**************************************
* One wire commands
*/
#define OWCOM_SKIP_ROM                  0xCC
#define OWCOM_READ_ROM                  0x33
#define OWCOM_MATCH_ROM                 0x55
#define OWCOM_SEARCH_ROM                0xF0
#define OWCOM_WRITE_SCRATCHPAD          0x4E
#define OWCOM_READ_SCRATCHPAD           0xBE

/******************************************************************************
* User typedef
*/
typedef enum{
    spsOk,
    spsOwNotFound,
    spsOwShortCircle,
    spsIdError,
    spsCrcError,
    spsError
}spsSt_type;

typedef struct{
    //---------------------------------------Readed
    uint16_t    temperature;        //[X_X ∞C]
    uint16_t    voltage;            //[mV]
    
    uint8_t     hvoltage;           //[V]
    struct{
        uint8_t     mhvPresent          :1;
    }stateSps;
    uint8_t     crcRd;
    //---------------------------------------Writable
    struct{
        uint8_t     temperatureLimOn    :1;
    }bitTask;
    
    uint16_t    vTask;              //[mV]
    uint8_t     crcWr;
    
    //--------------------------------------Counts
    uint16_t    errCnt;
    uint16_t    rxCnt;
}scratchpad_type;

typedef struct{
    spsSt_type          state;
    scratchpad_type     scratchpad;
}sps_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern sps_type    sps;

/******************************************************************************
* Macro functions
*/
#define spsOsCriticalEnter()    vTaskSuspendAll()
#define spsOsCriticalExit()     xTaskResumeAll()
#define spsOsInit               ps.state.bit.osInit

#define SATOMIC(x)                          \
    if(spsOsInit)   spsOsCriticalEnter();   \
    x;                                      \
    if(spsOsInit)   spsOsCriticalExit();
    
#define spsDelayOS_ms(ms)       vTaskDelay(msToSt(ms))
#define spsDelay_ms(ms)         delay_ms(ms)
#define spsCodeToHV(code, k)    (code * k) / 1000 + 1;

/******************************************************************************
* Prototypes for the functions
*/
spsSt_type spsRedMem(void);
spsSt_type spsWriteMem(void);

#endif //spsControl_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
