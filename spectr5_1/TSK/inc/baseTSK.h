/*
*******************************************************************************
* @file			base.h
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			
* @brief		
*******************************************************************************  
*/
#ifndef BASE_H
#define BASE_H
#include "globalIncludes.h"
#include "systemTSK.h"

/******************************************************************************
* User define
*/
#define NPRESET             5
#define IDLE_TIME           (180  / 0.03)       //Время до заставки [секунд]
#define BASE_TSK_PERIOD     30                  //[ms]
#define T_BEEP_CC_CV        10                  //[ms]
#define T_BEEP_ERROR        1000                //[ms]
#define T_BEEP_PRESS_KEY    30                  //[ms]

/******************************************************************************
* User typedef
*/
typedef enum{
    baseImax,
    baseILimitation,
    baseUnprotected,
}baseMode_type;

typedef struct{
    uint16_t        u;
    uint16_t        i;
    baseMode_type   Mode;
}set_type;

typedef struct{
    set_type    set[NPRESET];
    uint8_t     crst;
    uint8_t     reserv1;
}base_type;

/******************************************************************************
* User enum
*/
enum{  
    VAR_VOLT, 
    VAR_CURR, 
    VAR_MODE,
};


/******************************************************************************
* Extern viriables
*/
extern base_type bs;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void baseTSK(void *pPrm);
void printStatusBar(void);

#endif //BASE_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
