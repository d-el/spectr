/**
******************************************************************************
* @file    delay.h
* @author  Storozhenko Roman - D_EL
* @version V2.1
* @date    17-01-2015
* @brief   Средства работы со временем, использует таймер
******************************************************************************
**/
#ifndef DELAY_H
#define DELAY_H

#define DELTIM_FREQ     24000000	//Частота в Hz
#define DELTIM          TIM7
#define USE_FUNC_DEL    0           // 0 - использовать макросы
                                    // 1 - использовать функции

#define InitDelTim()                                    \
{                                                       \
    RCC->APB1ENR    |= RCC_APB1ENR_TIM7EN;             \
}

#if(USE_FUNC_DEL == 0)

#define delay_us(us)                                    \
{                                                       \
    DELTIM->PSC     = SYSTEM_FREQUENCY / 1000000 - 1;   \
    DELTIM->ARR     = us;                               \
    DELTIM->EGR     = TIM_EGR_UG;                       \
    DELTIM->SR      = ~TIM_SR_UIF;                      \
    DELTIM->CR1     = TIM_CR1_OPM | TIM_CR1_CEN;        \
    while((DELTIM->SR & TIM_SR_UIF) == 0) {;}           \
}

#define delay_ms(ms){                                   \
    DELTIM->PSC     = SYSTEM_FREQUENCY / 1000 - 1;      \
    DELTIM->ARR     = ms;                               \
    DELTIM->EGR     = TIM_EGR_UG;                       \
    DELTIM->SR      = ~TIM_SR_UIF;                      \
    DELTIM->CR1     = TIM_CR1_OPM | TIM_CR1_CEN;        \
    while((DELTIM->SR & TIM_SR_UIF) == 0) {;}           \
}

#endif  //USE_FUNC_DEL == 0

//-----------------------------------------------------------
//Prototypes for the functions in delay.c
//-----------------------------------------------------------
#if(USE_FUNC_DEL == 1)
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
#endif  //USE_FUNC_DEL == 1

#endif //__DELAY_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
