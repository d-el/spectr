/******************************************************************************
* @file    systemTSK.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    14-09-2015
* @brief
*/
#ifndef systemTSK_H
#define systemTSK_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define SYSTEM_TSK_PERIOD   20      //[ms]
#define SPS_PROC_PERIOD     100     //[ms]
#define TEM_PROC_PERIOD     1000    //[ms]
#define LED_PROC_PERIOD     500     //[ms]
#define CUR_OFF_TIME        1500    //[ms]
#define COOLER_PWM_START    0.28    //[%]
#define MIN_TEMP            30.0    //[°C]
#define MAX_TEMP            60.0    //[°C]
#define TEMP_OFF            650     //[X_X °C]
#define MAX_TEMP_SWS        650     //[X_X °C]
#define VTASK_FILTER_K      1
#define I_SHORT_CIRCUIT     8000    //[X_XXX A]
#define MAX_VAL_U           25300   //[X_XXX V]
#define MAX_VAL_I           8000    //[X_XXX A]
#define U_BIG_STEP          1000    //[X_XXX V]
#define I_BIG_STEP          500     //[X_XXX A]
/******************************************************************************
* User typedef
*/
typedef enum{
	noneWindow,
	settingWindow,
	baseWindow,
	chargerWindow,
    scopeWindow,
	cube3dWindow
}selWindow_type;

typedef union{
    struct{
        uint16_t    ovfSpsTemper                :1;
        uint16_t    ovfLinearRegTemper          :1;
        uint16_t    errorLinearRegTemperSens    :1;
        uint16_t    errorSps                    :1;
        uint16_t    noCalibration               :1;
    }bit;
    uint16_t        all;
}psError_type;

typedef union{
    struct{
        uint16_t    osInit                      :1;
        uint16_t    ovfCurrent                  :1;
        uint16_t    scopeRun                    :1;
        uint16_t    switchIsON                  :1;
        uint16_t    mhvPresent                  :1;
        uint16_t    modeIlim                    :1;
    }bit;
    uint16_t        all;
}psState_type;

typedef enum{
    mode_overcurrentShutdown,
    mode_limitation,
    mode_timeShutdown,
    mode_lowCurrentShutdown
}psMode_type;

typedef enum{
    localControl,
    acsControl
}typeContr_type;

typedef enum{
    switchNoRequest,
    switchOnRequest,
    switchOffRequest
}switchRequest_type;

typedef struct{
    psState_type            state;
    psError_type            error;
    
    struct{
        uint32_t            capacity;       //[mA/h]
        uint16_t            temperatureLin; //[X_X °Ñ]
        uint16_t            temperatureSps; //[X_X °Ñ]
        uint16_t            current;        //[X_XXX A]
        uint16_t            voltage;        //[X_XXX V]
        uint16_t            hvoltage;       //[X_XXX V]
        uint32_t            power;          //[X_XXX Wt]
        uint32_t            resistens;      //[X_XXX Ohm]
        uint32_t            time;           //[s]
    }meas;
    struct{
        uint16_t            current;        //[X_XXX A]
        uint16_t            voltage;        //[X_XXX V]
        uint16_t            time;           //[s]
        switchRequest_type  switchRequest;
        typeContr_type      control;
        psMode_type         mode;
    }task;    
}ps_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern ps_type             ps;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void systemTSK(void *pPrm);
void selWindow(selWindow_type window);

#endif //systemTSK_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
