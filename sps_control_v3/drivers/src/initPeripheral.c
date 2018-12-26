/******************************************************************************
* @file         initPeripheral.c
* @author       Storozhenko Roman - DEL
* @version      V2.0
* @date         10-01-2015
*/
#include "globalIncludes.h"

/*
*   TIM2    - LED_PWM
*   TIM3    - триггер для АЦП
*   DAC_CH1 - для напряжения
*/

/******************************************************************************
*
*/
void Init_PVD(void){  //Инициализация PVD с прерыванием
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                          //Power interface clock enable
    PWR->CR     |= PWR_CR_PLS_2V4;                              //Напряжение срабатывания
    PWR->CR     |= PWR_CR_PVDE;                                 //Power voltage detector enable

    EXTI->IMR   |= EXTI_IMR_MR16;                               //Interrupt request from Line 16 is not masked
    EXTI->EMR   |= EXTI_EMR_MR16;                               //Event request from Line 16 is not masked

    EXTI->RTSR  |= EXTI_RTSR_TR16;                              //Rising trigger event configuration bit of line 16

    NVIC_EnableIRQ(PVD_IRQn);                                  //PVD through EXTI Line detection Interrupt
    NVIC_SetPriority(PVD_IRQn, PVD_IRQ_Priority);              //Установить приоритет
}

/******************************************************************************
*
*/
void Init_EXTI1(void){
    //PC1
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PC;
    EXTI->IMR       |= EXTI_IMR_MR1;                            //Interrupt request from Line x is not masked
    EXTI->RTSR      |= EXTI_RTSR_TR1;                           //Rising trigger enabled (for Event and Interrupt) for input line
    NVIC_EnableIRQ (EXTI1_IRQn);                                //Разрешаем прерывания
}

/*******************************************************************************
* Настройка системы тактирования
*/
//#define     USE_HSE                                           //Использовать External High Speed clock
#define     PLL_MUL 6                                           //Коэффициент умножения PLL
void Init_CLK(void){
    RCC->CFGR &=~(  RCC_CFGR_PLLSRC     |
                    RCC_CFGR_PLLXTPRE   |
                    RCC_CFGR_PLLMULL)   ;                       //Предочистка

#ifdef USE_HSE
    RCC->CFGR   |= RCC_CFGR_PLLSRC_PREDIV1;                     //Тактировать PLL от HSE/PREDIV1
#else
    RCC->CFGR   |= RCC_CFGR_PLLSRC_HSI_Div2;                    //Тактировать PLL от HSI/PREDIV2
#endif //USE_HSE

    RCC->CFGR   |=((PLL_MUL - 2)<<18);                          //Умножать частоту на PLL_MUL
    RCC->CR     |= RCC_CR_PLLON;                                //Запустить PLL

#ifdef USE_HSE
    RCC->CR     |= RCC_CR_HSEON;                                //External High Speed clock enable
#else
    RCC->CR     &= ~RCC_CR_HSEON;                               //External High Speed clock disable
#endif  //USE_HSE

    while((RCC->CR & RCC_CR_PLLRDY)==0) {;}                     //Ожидание готовности PLL
    RCC->CFGR   &= ~RCC_CFGR_SW;                                //Очистить биты SW0, SW1
    RCC->CFGR   |= RCC_CFGR_SW_PLL;                             //Тактирование с выхода PLL
    while((RCC->CFGR&RCC_CFGR_SWS)!=0x08) {;}                   //Ожидание переключения на PLL
}

/******************************************************************************
* Для LED_PWM
*/
void Init_Timer2(void){
    gppin_init(GPIOA, 1, alternateFunctionOutputPushPull);      //CH2
    gppin_init(GPIOA, 2, alternateFunctionOutputPushPull);      //CH3
    RCC->APB1ENR  |= RCC_APB1ENR_TIM2EN;                        //Включили тактирование Timer4
    TIM2->PSC     = 1 - 1;                                      //претделитель на 1
    
    TIM2->CCER    |= TIM_CCER_CC2E;                             //Разрешаем таймеру использовать ногу для ШИМа CH2
    TIM2->CCER    |= TIM_CCER_CC3E;                             //Разрешаем таймеру использовать ногу для ШИМа CH3
    TIM2->CCMR1   |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;       //Прямой ШИМ CH2
    TIM2->CCMR2   |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;       //Прямой ШИМ CH1
    
    TIM2->ARR     = 1000;                                       //Таймер перезагружается при...
    TIM2->CCR2    = 0;                                          //Compare value
    TIM2->CCR3    = 0;                                          //Compare value
    TIM2->CR1     |= TIM_CR1_ARPE;                              //TIMx_ARR register is buffered
    TIM2->CR1     |= TIM_CR1_CEN;                               //Запускаем таймер
}

/******************************************************************************
*
*/
void Init_Port(void){
    RCC->APB2ENR    |= RCC_APB2ENR_AFIOEN;                      //Включили тактирование AFIO
    AFIO->MAPR      |= AFIO_MAPR_SWJ_CFG_1;                     //JTAG-DP Disabled and SW-DP Enabled
    
    RCC->APB2ENR    |= RCC_APB2ENR_IOPAEN;                      //Включили тактирование порта A GPIOA
    RCC->APB2ENR    |= RCC_APB2ENR_IOPBEN;                      //Включили тактирование порта B GPIOB
    RCC->APB2ENR    |= RCC_APB2ENR_IOPCEN;                      //Включили тактирование порта С GPIOC
}

/******************************************************************************
*
*/
void Init_DAC_CH1(void){                                        //Выход DAC1 - PA4
    gppin_init(GPIOA, 4, analogMode);
    RCC->APB1ENR    |= RCC_APB1ENR_DACEN;                       //Включить тактирование ЦАПа
    DAC->CR         |= DAC_CR_BOFF1;                            //DAC channel1 output buffer disable
    DAC->CR         |= DAC_CR_EN1;                              //Включить канал №1
    DAC->DHR12R1    = 3000;                                        //Уровень выхода
}

/******************* (C) COPYRIGHT ***************** END OF FILE ******************/
