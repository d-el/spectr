/**
******************************************************************************
* @file    enco.c
* @author  Storozhenko Roman - D_EL
* @version V2.2
* @date    28-12-2015 
* @brief   Средства работы с энкодером
******************************************************************************
**/
#include "globalIncludes.h"

enco_type   enco;

/*****************************************************************************
* @brief  Настройка периферии МК для работы с инкрементальным энкодером
* @param  Указатель на переменную приемник (приемник должен быть uint16_t)
* @retval None
*****************************************************************************/
void Init_Encoder(void){
    //Вывод GPIOA.0
    GPIOA->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);           //Очистить разряды MODE и CNF
    GPIOA->CRL  |=  GPIO_CRL_CNF0_1;                            //Дискретный вход
    GPIOA->BSRR =  GPIO_BSRR_BS0;                               //Подтяжка к плюсу
    //Вывод GPIOA.1
    GPIOA->CRL  &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);           //Очистить разряды MODE и CNF
    GPIOA->CRL  |=  GPIO_CRL_CNF1_1;                            //Дискретный вход
    GPIOA->BSRR =  GPIO_BSRR_BS1;                               //Подтяжка к плюсу

    //Настройка таймера
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;                         //Тактирование таймера
    TIM2->CCMR1  = TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC1S_0;         //Полярность сигнала для каждого входа
    TIM2->CCER   = TIM_CCER_CC1P | TIM_CCER_CC2P;
    
    TIM2->CCMR1 |= (TIM_CCMR1_IC2PSC_1 | TIM_CCMR1_IC1PSC_1);   //Capture is done once every 4 events

    TIM2->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_IC2F;             //Input capture 1 filter fSAMPLING=fDTS/32, N=8

    TIM2->SMCR  = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;              //Режим энкодера
    TIM2->ARR = 0xFFFF;                                         //Значение, до которого считает
    TIM2->SMCR  |=  TIM_SMCR_ETPS_1;
    TIM2->CR1   = TIM_CR1_CEN;                                  //Включаем таймер
}

/*****************************************************************************
* @brief  Прочитать значние из экодера
* @param  Указатель на переменную приемник (приемник должен быть uint16_t)
* @retval None
*****************************************************************************/
void enGet(void *dst){
    static uint16_t     prevVal;
    
    if(*(uint16_t*)dst != prevVal){ //Кто-то изменил значение?
        enSet(*(uint16_t*)dst);
        prevVal = *(uint16_t*)dst;
    }
    
    if(enco.circ == CIRC){
        if(__EnGet() > 15000)       enSet(enco.top);
        if(__EnGet() > enco.top)    enSet(0);
        if(__EnGet() < enco.bot)    enSet(enco.top);
    }else{
        if(__EnGet() > 15000)       enSet(0);
        if(__EnGet() > enco.top)    enSet(enco.top);            //Ограничеваем сверху до значения EnTop
        if(__EnGet() < enco.bot)    enSet(enco.bot);
    }
    *(uint16_t*)dst = __EnGet(); 
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
