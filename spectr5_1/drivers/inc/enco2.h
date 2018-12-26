/**
******************************************************************************
* @file    enco.h
* @author  Storozhenko Roman - D_EL
* @version V2.2
* @date    28-12-2015 
* @brief   Средства работы с энкодером
******************************************************************************
**/
#ifndef ENCO_H
#define ENCO_H
#include "globalIncludes.h"

#define EnReg           TIM2->CNT
#define CIRC            1
#define NONCIRC         0
#define NSHIFT          2

#define __EnGet()       (EnReg >> NSHIFT)
#define enSet(inval)    (EnReg = (inval << NSHIFT))

typedef struct{
    void        *pval;
    uint16_t    bot;
    uint16_t    top;
    uint8_t     circ;
}enco_type;

extern enco_type enco;

/*****************************************************************************
* Macro functions in enco.h
*****************************************************************************/
#define enSetRange(_bot, _top, _circ)   \
    enco.bot  = _bot;                   \
    enco.top  = _top;                   \
    enco.circ = _circ;                  \

/*****************************************************************************
* Prototypes for the functions in enco.c
*****************************************************************************/
void Init_Encoder(void);
void enGet(void *dst);

#endif //ENCO_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
