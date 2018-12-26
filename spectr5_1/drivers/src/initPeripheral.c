/******************************************************************************
* @file         initPeripheral.c
* @author       Storozhenko Roman - DEL
* @date         10-01-2015
*/
#include "globalIncludes.h"

/*
*   TIM2    - энкодера
*   TIM3    - триггер для АЦП
*   TIM4    - ШИМ вентилятора
*   TIM6    - задает длительность работы пищалки
*   TIM16   - ШИМ пищалки
*   SPI2    - ЖКИ
*   DAC_CH1 - тока
*   DAC_CH2 - напряжение
*/

/******************************************************************************
* MEMORY
*/
clockSource_type            clockSource;

/******************************************************************************
* Инициализация PVD с прерыванием
*/
void Init_PVD(void){  
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                          //Power interface clock enable
    PWR->CR     |= PWR_CR_PLS_2V4;                              //Напряжение срабатывания
    PWR->CR     |= PWR_CR_PVDE;                                 //Power voltage detector enable

    EXTI->IMR   |= EXTI_IMR_MR16;                               //Interrupt request from Line 16 is not masked
    EXTI->EMR   |= EXTI_EMR_MR16;                               //Event request from Line 16 is not masked

    EXTI->RTSR  |= EXTI_RTSR_TR16;                              //Rising trigger event configuration bit of line 16

    NVIC_EnableIRQ (PVD_IRQn);                                  //PVD through EXTI Line detection Interrupt
    NVIC_SetPriority (PVD_IRQn, PVD_IRQ_Priority);              //Установить приоритет
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

/******************************************************************************
* Настройка системы тактирования
*/
void Init_CLK(void){
    uint32_t    timeOut;
    
    RCC->CFGR &=~(  RCC_CFGR_PLLSRC     |
                    RCC_CFGR_PLLXTPRE   |
                    RCC_CFGR_PLLMULL)   ;                       //Предочистка

    RCC->CR     |= RCC_CR_HSEON;                                //HSE oscillator ON
    timeOut     = RCC_TIMEOUT;
    while(((RCC->CR & RCC_CR_HSERDY) == 0) && (--timeOut != 0));
    if(timeOut != 0){                                           //HSE is ready
        RCC->CFGR   |= RCC_CFGR_PLLSRC_PREDIV1;                 //Тактировать PLL от HSE
        RCC->CFGR2  = HSE_PLL_PREDIV - 1;                       //Делитель
        RCC->CFGR   |=((HSE_PLL_MUL - 2)<<18);                  //Умножитель
        clockSource = clockSourceHse;
    }
    else{                                                       //HSE is not ready
        RCC->CR     &= ~RCC_CR_HSEON;                           //HSE oscillator OFF
        RCC->CR     |= RCC_CR_HSION;                            //HSI oscillator ON
        while((RCC->CR & RCC_CR_HSION) == 0);
        RCC->CFGR   |= RCC_CFGR_PLLSRC_HSI_Div2;                //Тактировать PLL от HSI/2
        RCC->CFGR2  = HSI_PLL_PREDIV - 1;                       //Делитель
        RCC->CFGR   |=((HSI_PLL_MUL - 2)<<18);                  //Умножитель
        clockSource = clockSourceHsi;
    }
    RCC->CR     |= RCC_CR_PLLON;                                //Запустить PLL
    while((RCC->CR & RCC_CR_PLLRDY) ==0);                       //Ожидание готовности PLL
    RCC->CFGR   &= ~RCC_CFGR_SW;                                //Очистить биты SW0, SW1
    RCC->CFGR   |= RCC_CFGR_SW_PLL;                             //Тактирование с выхода PLL
    while((RCC->CFGR&RCC_CFGR_SWS)!=0x08);                      //Ожидание переключения на PLL
}

/******************************************************************************
*
*/
void Init_SPI2(void){
    //Вывод SCK: выход двухтактный, альтернативная функция, 50MHz
    GPIOB->CRH      |=  GPIO_CRH_MODE13;                        //
    GPIOB->CRH      &= ~GPIO_CRH_CNF13;                         //
    GPIOB->CRH      |=  GPIO_CRH_CNF13_1;                       //

    //Вывод MOSI: выход двухтактный, альтернативнаяфункция, 50MHz
    GPIOB->CRH      |=  GPIO_CRH_MODE15;                        //
    GPIOB->CRH      &= ~GPIO_CRH_CNF15;                         //
    GPIOB->CRH      |=  GPIO_CRH_CNF15_1;                       //

    //Максимальная скорость - fPCLK/2
    RCC->APB1ENR    |= RCC_APB1ENR_SPI2EN;                      //Подать тактирование на блок SPI
    SPI2->CR1       = 0x0000;                                   //Очистить первый управляющий регистр
    SPI2->CR2       = 0x0000;                                   //Очистить второй управляющий регистр

    //SPI2->CR1       |= SPI_CR1_BR_0;
    SPI2->CR1       |= SPI_CR1_MSTR;                            //Master configuration
    SPI2->CR1       |= SPI_CR1_SSM;                             //Software slave management enabled
    SPI2->CR1       |= SPI_CR1_SSI;                             //Игнорирование NSS входа
    SPI2->CR1       |= SPI_CR1_SPE;                             //SPI enable
}

/******************************************************************************
* Для FAN_PWM
*/
void Init_Timer4(void){
    //TIM4_CH4 - PB9
    GPIOB->CRH    &=~(GPIO_CRH_CNF9 | GPIO_CRH_CNF9);           //Очистить разряды MODE и CNF
    GPIOB->CRH    |= GPIO_CRH_MODE9_1;                          //Настройка на выход с быстродействием 2MHz
    GPIOB->CRH    |= GPIO_CRH_CNF9_1 ;                          //Alternate function output Push-pull

    RCC->APB1ENR  |= RCC_APB1ENR_TIM4EN;                        //Включили тактирование Timer4
    TIM4->PSC     = 1 - 1;                                      //претделитель на 1
    TIM4->CCER    |= TIM_CCER_CC4E;                             //Разрешаем таймеру использовать ногу CH4 для ШИМа
    TIM4->CCMR2   |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;       //Прямой ШИМ
    TIM4->ARR     = 500;                                        //Таймер перезагружается при...
    TIM4->CCR4    = 200;                                        //Compare 4 value
    TIM4->CR1     |= TIM_CR1_ARPE;                              //TIMx_ARR register is buffered
    TIM4->CR1     |= TIM_CR1_CEN;                               //Запускаем таймер!
}

/******************************************************************************
*
*/
void Init_DAC_CH1(void){                                        //Выход DAC1 - PA4
    GPIOA->CRL      &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);       //Очистить разряды MODE и CNF (Режим входа)
    RCC->APB1ENR    |= RCC_APB1ENR_DACEN;                       //Включить тактирование ЦАПа
    DAC->CR         |= DAC_CR_BOFF1;                            //DAC channel1 output buffer disable
    DAC->CR         |= DAC_CR_EN1;                              //Включить канал №1
    DAC->DHR12R1    = 0;                                        //На выходе канала1 0В
}

/******************************************************************************
*
*/
void Init_DAC_CH2(void){    //Выход DAC2 - PA5
    GPIOA->CRL      &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);       //Очистить разряды MODE и CNF (Режим входа)
    RCC->APB1ENR    |= RCC_APB1ENR_DACEN;                       //Включить тактирование ЦАПа
    DAC->CR         |= DAC_CR_BOFF2;                            //DAC channel2 output buffer disable
    DAC->CR         |= DAC_CR_EN2;                              //Включить канал №2
    DAC->DHR12R2    = 0;                                        //На выходе канала2 0В
}

/******************* (C) COPYRIGHT ***************** END OF FILE ******************/
