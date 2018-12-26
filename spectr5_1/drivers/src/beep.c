/**
******************************************************************************
* @file    beep.c
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    19.12.2014
* @brief
******************************************************************************
*/
#include "globalIncludes.h"

__irq void TIM6_DAC_IRQHandler(void){
    if((TIME_B_TIM->SR & TIM_SR_UIF) != 0){
        TIME_B_TIM->ARR = 0;
        beepOFF();
        TIME_B_TIM->SR = ~TIM_SR_UIF;                             //clear UIF flag
    }
}

void BeepTime(uint16_t ms){
    beepON();
    if((TIME_B_TIM->ARR - TIME_B_TIM->CNT) < ms){
        TIME_B_TIM->CNT = 0;
        TIME_B_TIM->ARR = ms;
    }
    TIME_B_TIM->CR1 |= TIM_CR1_CEN;                                 //��������� ������!
}

void Init_Beep(void){
    //TIM16_CH1 - PB8
    GPIOB->CRH &=~(GPIO_CRH_CNF8 | GPIO_CRH_CNF8);                  //�������� ������� MODE � CNF
    GPIOB->CRH |= GPIO_CRH_MODE8_1;                                 //����� � ��������������� 2MHz
    GPIOB->CRH |= GPIO_CRH_CNF8_1 ;                                 //Alternate function output Push-pull

    RCC->APB2ENR        |= RCC_APB2ENR_TIM16EN;                     //�������� ������������ Timer
    BEEP_TIM->PSC       = 1 - 1;                                    //������������ �� 1
    BEEP_TIM->CR1       |= TIM_CR1_ARPE;                            //TIMx_ARR register is buffered
    BEEP_TIM->CCER      |= TIM_CCER_CC1E;                           //�������� CH1
    BEEP_TIM->CCMR1     |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0);   //TIM16_CH1 - ��� ��������� ��������� ���� �������������
    BEEP_TIM->BDTR      |= TIM_BDTR_MOE; //�������� ����� OC
    BEEP_TIM->ARR       = SYSTEM_FREQUENCY / F_BEEP / 2;            //������ ��������������� ���...

    //������ ��� ������� ������� ������ �������
    RCC->APB1ENR        |= RCC_APB1ENR_TIM6EN;
    TIME_B_TIM->PSC     = SYSTEM_FREQUENCY / 1000 - 1;              //������������ ��� ��������� ������� ������������ 1ms
    TIME_B_TIM->ARR     = 0;
    TIME_B_TIM->CR1     |= TIM_CR1_OPM;
    TIME_B_TIM->DIER    |= TIM_DIER_UIE;                            //Update interrupt enable
    TIME_B_TIM->EGR     = TIM_EGR_UG;                               //���������� ������� ��� ���������� �� � ������� ��������
    TIME_B_TIM->SR      = ~TIM_SR_UIF;                              //Clear UIF flag
    NVIC_EnableIRQ(TIM6_DAC_IRQn);                                  //�������� ����������
    NVIC_SetPriority(TIM6_DAC_IRQn, TIM6_DAC_IRQn_Priority);        //���������� ���������
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
