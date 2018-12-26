/******************************************************************************
* @file			charge.h
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			
*/
#ifndef CHARGE_H
#define CHARGE_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define CH_TSK_PERIOD   30      //[ms]

/******************************************************************************
* User typedef
*/
typedef struct{
    uint16_t    Udac;
    uint16_t    Idac;
    uint16_t    Time;
    uint16_t    Mode;
}charge_type;

/******************************************************************************
* User enum
*/
enum{
    C_MODE = 0,
    C_TIME = 1,
    C_VOLT = 2,
    C_CURR = 3,
};

enum{
    Mode_Time       = 0,
    Mode_Current    = 1,
};

/******************************************************************************
* Extern viriables
*/
extern charge_type ch;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void chargeTSK(void *pPrm);

#endif //CHARGE_H
/******************* (C) COPYRIGHT ********** END OF FILE ********* D_EL *****/
