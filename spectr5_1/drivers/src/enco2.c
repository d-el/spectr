/**
******************************************************************************
* @file    enco.c
* @author  Storozhenko Roman - D_EL
* @version V2.2
* @date    28-12-2015 
* @brief   �������� ������ � ���������
******************************************************************************
**/
#include "globalIncludes.h"

enco_type   enco;

/*****************************************************************************
* @brief  ��������� ��������� �� ��� ������ � ��������������� ���������
* @param  ��������� �� ���������� �������� (�������� ������ ���� uint16_t)
* @retval None
*****************************************************************************/
void Init_Encoder(void){
    //����� GPIOA.0
    GPIOA->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);           //�������� ������� MODE � CNF
    GPIOA->CRL  |=  GPIO_CRL_CNF0_1;                            //���������� ����
    GPIOA->BSRR =  GPIO_BSRR_BS0;                               //�������� � �����
    //����� GPIOA.1
    GPIOA->CRL  &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);           //�������� ������� MODE � CNF
    GPIOA->CRL  |=  GPIO_CRL_CNF1_1;                            //���������� ����
    GPIOA->BSRR =  GPIO_BSRR_BS1;                               //�������� � �����

    //��������� �������
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;                         //������������ �������
    TIM2->CCMR1  = TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC1S_0;         //���������� ������� ��� ������� �����
    TIM2->CCER   = TIM_CCER_CC1P | TIM_CCER_CC2P;
    
    TIM2->CCMR1 |= (TIM_CCMR1_IC2PSC_1 | TIM_CCMR1_IC1PSC_1);   //Capture is done once every 4 events

    TIM2->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_IC2F;             //Input capture 1 filter fSAMPLING=fDTS/32, N=8

    TIM2->SMCR  = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;              //����� ��������
    TIM2->ARR = 0xFFFF;                                         //��������, �� �������� �������
    TIM2->SMCR  |=  TIM_SMCR_ETPS_1;
    TIM2->CR1   = TIM_CR1_CEN;                                  //�������� ������
}

/*****************************************************************************
* @brief  ��������� ������� �� �������
* @param  ��������� �� ���������� �������� (�������� ������ ���� uint16_t)
* @retval None
*****************************************************************************/
void enGet(void *dst){
    static uint16_t     prevVal;
    
    if(*(uint16_t*)dst != prevVal){ //���-�� ������� ��������?
        enSet(*(uint16_t*)dst);
        prevVal = *(uint16_t*)dst;
    }
    
    if(enco.circ == CIRC){
        if(__EnGet() > 15000)       enSet(enco.top);
        if(__EnGet() > enco.top)    enSet(0);
        if(__EnGet() < enco.bot)    enSet(enco.top);
    }else{
        if(__EnGet() > 15000)       enSet(0);
        if(__EnGet() > enco.top)    enSet(enco.top);            //������������ ������ �� �������� EnTop
        if(__EnGet() < enco.bot)    enSet(enco.bot);
    }
    *(uint16_t*)dst = __EnGet(); 
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
