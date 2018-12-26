/**
******************************************************************************
* @file    drivers.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    05-07-2013
******************************************************************************
**/

#ifndef DRIVERS_H
#define DRIVERS_H
#include "globalIncludes.h"
#include "IQmathLib.h"

/******************************************************************************
* User define
*/
//-----------------------------------------------------------------------------
//ÏÐÈÎÐÈÒÅÒÛ ÏÐÅÐÛÂÀÍÈÉ, ÏÎ ÓÁÛÂÀÍÈÞ
//-----------------------------------------------------------------------------
#define PVD_IRQ_Priority            0
#define EXTI12_IRQn_Priority        2
#define TIM15_IRQn_Priority         1
#define DMA1_Channel1_IRQn_Priority 14

/******************************************************************************
* User typedef
*/
typedef struct{
    _iq         qf;
    int32_t     x;
}pointCalibr_type;

typedef struct{    
    pointCalibr_type  pointsU[6];
    pointCalibr_type  pointsI[6];
    pointCalibr_type  pointsL[6];
}syssetting_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern syssetting_type      sysset;

/******************************************************************************
* Macro functions
*/
#define LED_ON()            GPIOA->BSRR = GPIO_BSRR_BR1             //Âêëþ÷èòü LED
#define LED_OFF()           GPIOA->BRR  = GPIO_BRR_BR1               //Âûêëþ÷èòü LED
#define LED_TOGGLE()        GPIOA->ODR  ^= GPIO_ODR_ODR1            //Èíâåðòèðîâàòü LED
#define ledLightGreen(x)    TIM2->CCR2  = x
#define ledLightRed(x)      TIM2->CCR3  = x
#define setDacU(u16val)     DAC->DHR12R1 = (u16val)

/******************************************************************************
* Prototypes for the functions
*/
void        Init_Hard(void);
void        prmInitDef(void);

#endif //DRIVERS_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
