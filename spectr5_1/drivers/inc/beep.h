/**
******************************************************************************
* @file    beep.h
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    19.12.2014
* @brief   
******************************************************************************
*/
#ifndef BEEP_H
#define BEEP_H
#include "globalIncludes.h"

#define SYSTEM_FREQUENCY    24000000
#define F_BEEP              4000                                //Частота пищалки (1 - 32767)
#define BEEP_IRQ_Priority   15
#define BEEP_TIM            TIM16
#define TIME_B_TIM          TIM6

#define	beepON()            BEEP_TIM->CR1 |= TIM_CR1_CEN        //Включить пищалку
#define	beepOFF()           BEEP_TIM->CR1 &= ~TIM_CR1_CEN       //Выключить пищалку

#define	beepSetFreq(freq)   BEEP_TIM->ARR = SYSTEM_FREQUENCY / (freq) / 2;

//-----------------------------------------------------------
//Prototypes for the functions in beep.c
//-----------------------------------------------------------
void BeepTime(uint16_t timems);
void Init_Beep(void);
void Dht (void *pvParameters);

#endif //BEEP_H
