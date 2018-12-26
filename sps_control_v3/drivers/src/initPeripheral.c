/******************************************************************************
* @file         initPeripheral.c
* @author       Storozhenko Roman - DEL
* @version      V2.0
* @date         10-01-2015
*/
#include "globalIncludes.h"

/*
*   TIM2    - LED_PWM
*   TIM3    - ������� ��� ���
*   DAC_CH1 - ��� ����������
*/

/******************************************************************************
*
*/
void Init_PVD(void){  //������������� PVD � �����������
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                          //Power interface clock enable
    PWR->CR     |= PWR_CR_PLS_2V4;                              //���������� ������������
    PWR->CR     |= PWR_CR_PVDE;                                 //Power voltage detector enable

    EXTI->IMR   |= EXTI_IMR_MR16;                               //Interrupt request from Line 16 is not masked
    EXTI->EMR   |= EXTI_EMR_MR16;                               //Event request from Line 16 is not masked

    EXTI->RTSR  |= EXTI_RTSR_TR16;                              //Rising trigger event configuration bit of line 16

    NVIC_EnableIRQ(PVD_IRQn);                                  //PVD through EXTI Line detection Interrupt
    NVIC_SetPriority(PVD_IRQn, PVD_IRQ_Priority);              //���������� ���������
}

/******************************************************************************
*
*/
void Init_EXTI1(void){
    //PC1
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PC;
    EXTI->IMR       |= EXTI_IMR_MR1;                            //Interrupt request from Line x is not masked
    EXTI->RTSR      |= EXTI_RTSR_TR1;                           //Rising trigger enabled (for Event and Interrupt) for input line
    NVIC_EnableIRQ (EXTI1_IRQn);                                //��������� ����������
}

/*******************************************************************************
* ��������� ������� ������������
*/
//#define     USE_HSE                                           //������������ External High Speed clock
#define     PLL_MUL 6                                           //����������� ��������� PLL
void Init_CLK(void){
    RCC->CFGR &=~(  RCC_CFGR_PLLSRC     |
                    RCC_CFGR_PLLXTPRE   |
                    RCC_CFGR_PLLMULL)   ;                       //�����������

#ifdef USE_HSE
    RCC->CFGR   |= RCC_CFGR_PLLSRC_PREDIV1;                     //����������� PLL �� HSE/PREDIV1
#else
    RCC->CFGR   |= RCC_CFGR_PLLSRC_HSI_Div2;                    //����������� PLL �� HSI/PREDIV2
#endif //USE_HSE

    RCC->CFGR   |=((PLL_MUL - 2)<<18);                          //�������� ������� �� PLL_MUL
    RCC->CR     |= RCC_CR_PLLON;                                //��������� PLL

#ifdef USE_HSE
    RCC->CR     |= RCC_CR_HSEON;                                //External High Speed clock enable
#else
    RCC->CR     &= ~RCC_CR_HSEON;                               //External High Speed clock disable
#endif  //USE_HSE

    while((RCC->CR & RCC_CR_PLLRDY)==0) {;}                     //�������� ���������� PLL
    RCC->CFGR   &= ~RCC_CFGR_SW;                                //�������� ���� SW0, SW1
    RCC->CFGR   |= RCC_CFGR_SW_PLL;                             //������������ � ������ PLL
    while((RCC->CFGR&RCC_CFGR_SWS)!=0x08) {;}                   //�������� ������������ �� PLL
}

/******************************************************************************
* ��� LED_PWM
*/
void Init_Timer2(void){
    gppin_init(GPIOA, 1, alternateFunctionOutputPushPull);      //CH2
    gppin_init(GPIOA, 2, alternateFunctionOutputPushPull);      //CH3
    RCC->APB1ENR  |= RCC_APB1ENR_TIM2EN;                        //�������� ������������ Timer4
    TIM2->PSC     = 1 - 1;                                      //������������ �� 1
    
    TIM2->CCER    |= TIM_CCER_CC2E;                             //��������� ������� ������������ ���� ��� ���� CH2
    TIM2->CCER    |= TIM_CCER_CC3E;                             //��������� ������� ������������ ���� ��� ���� CH3
    TIM2->CCMR1   |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;       //������ ��� CH2
    TIM2->CCMR2   |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;       //������ ��� CH1
    
    TIM2->ARR     = 1000;                                       //������ ��������������� ���...
    TIM2->CCR2    = 0;                                          //Compare value
    TIM2->CCR3    = 0;                                          //Compare value
    TIM2->CR1     |= TIM_CR1_ARPE;                              //TIMx_ARR register is buffered
    TIM2->CR1     |= TIM_CR1_CEN;                               //��������� ������
}

/******************************************************************************
*
*/
void Init_Port(void){
    RCC->APB2ENR    |= RCC_APB2ENR_AFIOEN;                      //�������� ������������ AFIO
    AFIO->MAPR      |= AFIO_MAPR_SWJ_CFG_1;                     //JTAG-DP Disabled and SW-DP Enabled
    
    RCC->APB2ENR    |= RCC_APB2ENR_IOPAEN;                      //�������� ������������ ����� A GPIOA
    RCC->APB2ENR    |= RCC_APB2ENR_IOPBEN;                      //�������� ������������ ����� B GPIOB
    RCC->APB2ENR    |= RCC_APB2ENR_IOPCEN;                      //�������� ������������ ����� � GPIOC
}

/******************************************************************************
*
*/
void Init_DAC_CH1(void){                                        //����� DAC1 - PA4
    gppin_init(GPIOA, 4, analogMode);
    RCC->APB1ENR    |= RCC_APB1ENR_DACEN;                       //�������� ������������ ����
    DAC->CR         |= DAC_CR_BOFF1;                            //DAC channel1 output buffer disable
    DAC->CR         |= DAC_CR_EN1;                              //�������� ����� �1
    DAC->DHR12R1    = 3000;                                        //������� ������
}

/******************* (C) COPYRIGHT ***************** END OF FILE ******************/
