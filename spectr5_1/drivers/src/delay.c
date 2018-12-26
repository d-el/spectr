/**
******************************************************************************
* @file    delay.c
* @author  Storozhenko Roman - D_EL
* @version V2.1
* @date    17-01-2015
* @brief   Средства работы со временем, использует таймер
******************************************************************************
**/
#include "globalIncludes.h"

#if(USE_FUNC_DEL == 1)

void delay_us(uint16_t us){
    DELTIM->PSC     = SYSTEM_FREQUENCY / 1000000 - 1;               //Предделитель на 1us
    DELTIM->ARR     = us;
    DELTIM->EGR     = TIM_EGR_UG;                                   //Генерируем событие для перегрузки из в рабочие регистры
    DELTIM->SR      = ~TIM_SR_UIF;
    DELTIM->CR1     = TIM_CR1_OPM | TIM_CR1_CEN;                    //Counter enable
    while((DELTIM->SR & TIM_SR_UIF) == 0) {;}  
}

void delay_ms(uint16_t ms){
    DELTIM->PSC     = SYSTEM_FREQUENCY / 1000 - 1;                  //Предделитель на 1ms
    DELTIM->ARR     = ms;
    DELTIM->EGR     = TIM_EGR_UG;                                   //Генерируем событие для перегрузки из в рабочие регистры
    DELTIM->SR      = ~TIM_SR_UIF;
    DELTIM->CR1     = TIM_CR1_OPM | TIM_CR1_CEN;                    //Counter enable
    while((DELTIM->SR & TIM_SR_UIF) == 0) {;}  
}

#endif  //USE_FUNC_DEL == 1
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
