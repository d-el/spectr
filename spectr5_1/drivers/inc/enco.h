/**
*******************************************************************************
* @file    enco.h
* @author  Storozhenko Roman - D_EL
* @version V2.3
* @date    13-05-2015 
* @brief   Средства работы с энкодером
*******************************************************************************
**/
#ifndef ENCO_H
#define ENCO_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define EnReg           TIM2->CNT
#define EnTIM           TIM2
#define NSHIFT          2

/******************************************************************************
* User typedef
*/
typedef enum{
    enNONCIRC,
    enCIRC
}enMode_type;

typedef enum{
    enU8,
    enU16,
    enU32
}enMemType_type;

typedef struct{
    void            *pval;
    uint16_t        bot;
    uint16_t        top;
    uint16_t        step;
    uint16_t        bigStep;
    uint16_t        ntic;
    enMode_type     mode;
    enMemType_type  memtype; 
}enco_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern enco_type enco;

/******************************************************************************
* Macro functions
*/
#define enGeReg()       (EnReg >> NSHIFT)
#define enSet(_enbot, _entop, _step, _bigStep, _enmode, _memtype, _pval)     \
    enco.bot        = _enbot;                               \
    enco.top        = _entop;                               \
    enco.step       = _step;                                \
    enco.bigStep    = _bigStep;                             \
    enco.mode       = _enmode;                              \
    enco.memtype    = _memtype;                             \
    enco.pval       = _pval;

/******************************************************************************
* Prototypes for the functions
*/
void Init_Encoder(void);
uint32_t enReadVal(void);
void enWriteVal(int32_t val);
void enAdd(int32_t val);
void enBigStepUp(void);
void enBigStepDown(void);
void enGet(void);

#endif //ENCO_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
