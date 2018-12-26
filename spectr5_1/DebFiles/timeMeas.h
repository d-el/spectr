/******************************************************************************
* @file    timeMeas
* @author  d_el
* @version V1.0
* @date    04.10.2015, by d_el
* @brief   debug
*/
#ifndef timeMeas_H
#define timeMeas_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define NUMTIMERS       4
#define MEASTIM         TIM1
#define MEASTIM_FREQ    24000000

/******************************************************************************
* User typedef
*/

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern unsigned long long    measTime[NUMTIMERS];

/******************************************************************************
* Macro functions
*/
#define measTimeStart(){                                \
    MEASTIM->CR1 |= TIM_CR1_CEN;                        \
    MEASTIM->CNT = 0;                                   \
}
#define measTimeStop(n){                                \
    measTime[n] = MEASTIM->CNT;                         \
    MEASTIM->CR1 &= ~TIM_CR1_CEN;                       \
    measTime[n] = (measTime[n] * 1000000000) / MEASTIM_FREQ; \
}

/******************************************************************************
* Prototypes for the functions
*/
void timMeasInit(void);

#endif //timeMeas_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
