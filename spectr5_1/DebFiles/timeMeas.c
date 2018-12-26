/******************************************************************************
* @file    timeMeas
* @author  d_el
* @version V1.0
* @date    04.10.2015, by d_el
* @brief   debug
*/
#include "globalIncludes.h"

unsigned long long    measTime[NUMTIMERS];
//xFreeBytesRemaining

void timMeasInit(void){
    RCC->APB2ENR  |= RCC_APB2ENR_TIM1EN;                        //Включили тактирование Timer
    TIM1->PSC     = 1 - 1;                                      //претделитель на 1

}

/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
