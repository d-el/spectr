/******************************************************************************
* @file         initPeripheral.c
* @author       Storozhenko Roman - DEL
* @date         10-01-2015
*/
#include "globalIncludes.h"

/*
*   TIM2    - ��������
*   TIM3    - ������� ��� ���
*   TIM4    - ��� �����������
*   TIM6    - ������ ������������ ������ �������
*   TIM16   - ��� �������
*   SPI2    - ���
*   DAC_CH1 - ����
*   DAC_CH2 - ����������
*/

/******************************************************************************
* MEMORY
*/
clockSource_type            clockSource;

/******************************************************************************
* ������������� PVD � �����������
*/
void Init_PVD(void){  
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                          //Power interface clock enable
    PWR->CR     |= PWR_CR_PLS_2V4;                              //���������� ������������
    PWR->CR     |= PWR_CR_PVDE;                                 //Power voltage detector enable

    EXTI->IMR   |= EXTI_IMR_MR16;                               //Interrupt request from Line 16 is not masked
    EXTI->EMR   |= EXTI_EMR_MR16;                               //Event request from Line 16 is not masked

    EXTI->RTSR  |= EXTI_RTSR_TR16;                              //Rising trigger event configuration bit of line 16

    NVIC_EnableIRQ (PVD_IRQn);                                  //PVD through EXTI Line detection Interrupt
    NVIC_SetPriority (PVD_IRQn, PVD_IRQ_Priority);              //���������� ���������
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

/******************************************************************************
* ��������� ������� ������������
*/
void Init_CLK(void){
    uint32_t    timeOut;
    
    RCC->CFGR &=~(  RCC_CFGR_PLLSRC     |
                    RCC_CFGR_PLLXTPRE   |
                    RCC_CFGR_PLLMULL)   ;                       //�����������

    RCC->CR     |= RCC_CR_HSEON;                                //HSE oscillator ON
    timeOut     = RCC_TIMEOUT;
    while(((RCC->CR & RCC_CR_HSERDY) == 0) && (--timeOut != 0));
    if(timeOut != 0){                                           //HSE is ready
        RCC->CFGR   |= RCC_CFGR_PLLSRC_PREDIV1;                 //����������� PLL �� HSE
        RCC->CFGR2  = HSE_PLL_PREDIV - 1;                       //��������
        RCC->CFGR   |=((HSE_PLL_MUL - 2)<<18);                  //����������
        clockSource = clockSourceHse;
    }
    else{                                                       //HSE is not ready
        RCC->CR     &= ~RCC_CR_HSEON;                           //HSE oscillator OFF
        RCC->CR     |= RCC_CR_HSION;                            //HSI oscillator ON
        while((RCC->CR & RCC_CR_HSION) == 0);
        RCC->CFGR   |= RCC_CFGR_PLLSRC_HSI_Div2;                //����������� PLL �� HSI/2
        RCC->CFGR2  = HSI_PLL_PREDIV - 1;                       //��������
        RCC->CFGR   |=((HSI_PLL_MUL - 2)<<18);                  //����������
        clockSource = clockSourceHsi;
    }
    RCC->CR     |= RCC_CR_PLLON;                                //��������� PLL
    while((RCC->CR & RCC_CR_PLLRDY) ==0);                       //�������� ���������� PLL
    RCC->CFGR   &= ~RCC_CFGR_SW;                                //�������� ���� SW0, SW1
    RCC->CFGR   |= RCC_CFGR_SW_PLL;                             //������������ � ������ PLL
    while((RCC->CFGR&RCC_CFGR_SWS)!=0x08);                      //�������� ������������ �� PLL
}

/******************************************************************************
*
*/
void Init_SPI2(void){
    //����� SCK: ����� �����������, �������������� �������, 50MHz
    GPIOB->CRH      |=  GPIO_CRH_MODE13;                        //
    GPIOB->CRH      &= ~GPIO_CRH_CNF13;                         //
    GPIOB->CRH      |=  GPIO_CRH_CNF13_1;                       //

    //����� MOSI: ����� �����������, ���������������������, 50MHz
    GPIOB->CRH      |=  GPIO_CRH_MODE15;                        //
    GPIOB->CRH      &= ~GPIO_CRH_CNF15;                         //
    GPIOB->CRH      |=  GPIO_CRH_CNF15_1;                       //

    //������������ �������� - fPCLK/2
    RCC->APB1ENR    |= RCC_APB1ENR_SPI2EN;                      //������ ������������ �� ���� SPI
    SPI2->CR1       = 0x0000;                                   //�������� ������ ����������� �������
    SPI2->CR2       = 0x0000;                                   //�������� ������ ����������� �������

    //SPI2->CR1       |= SPI_CR1_BR_0;
    SPI2->CR1       |= SPI_CR1_MSTR;                            //Master configuration
    SPI2->CR1       |= SPI_CR1_SSM;                             //Software slave management enabled
    SPI2->CR1       |= SPI_CR1_SSI;                             //������������� NSS �����
    SPI2->CR1       |= SPI_CR1_SPE;                             //SPI enable
}

/******************************************************************************
* ��� FAN_PWM
*/
void Init_Timer4(void){
    //TIM4_CH4 - PB9
    GPIOB->CRH    &=~(GPIO_CRH_CNF9 | GPIO_CRH_CNF9);           //�������� ������� MODE � CNF
    GPIOB->CRH    |= GPIO_CRH_MODE9_1;                          //��������� �� ����� � ��������������� 2MHz
    GPIOB->CRH    |= GPIO_CRH_CNF9_1 ;                          //Alternate function output Push-pull

    RCC->APB1ENR  |= RCC_APB1ENR_TIM4EN;                        //�������� ������������ Timer4
    TIM4->PSC     = 1 - 1;                                      //������������ �� 1
    TIM4->CCER    |= TIM_CCER_CC4E;                             //��������� ������� ������������ ���� CH4 ��� ����
    TIM4->CCMR2   |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;       //������ ���
    TIM4->ARR     = 500;                                        //������ ��������������� ���...
    TIM4->CCR4    = 200;                                        //Compare 4 value
    TIM4->CR1     |= TIM_CR1_ARPE;                              //TIMx_ARR register is buffered
    TIM4->CR1     |= TIM_CR1_CEN;                               //��������� ������!
}

/******************************************************************************
*
*/
void Init_DAC_CH1(void){                                        //����� DAC1 - PA4
    GPIOA->CRL      &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);       //�������� ������� MODE � CNF (����� �����)
    RCC->APB1ENR    |= RCC_APB1ENR_DACEN;                       //�������� ������������ ����
    DAC->CR         |= DAC_CR_BOFF1;                            //DAC channel1 output buffer disable
    DAC->CR         |= DAC_CR_EN1;                              //�������� ����� �1
    DAC->DHR12R1    = 0;                                        //�� ������ ������1 0�
}

/******************************************************************************
*
*/
void Init_DAC_CH2(void){    //����� DAC2 - PA5
    GPIOA->CRL      &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);       //�������� ������� MODE � CNF (����� �����)
    RCC->APB1ENR    |= RCC_APB1ENR_DACEN;                       //�������� ������������ ����
    DAC->CR         |= DAC_CR_BOFF2;                            //DAC channel2 output buffer disable
    DAC->CR         |= DAC_CR_EN2;                              //�������� ����� �2
    DAC->DHR12R2    = 0;                                        //�� ������ ������2 0�
}

/******************* (C) COPYRIGHT ***************** END OF FILE ******************/
