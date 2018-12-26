/******************************************************************************
* @file    adc.c
* @author  Storozhenko Roman - D_EL
* @version V2.0
* @date    04.10.2015
*/
#include "globalIncludes.h"

/******************************************************************************
* MEMORY
*/
adcStct_type        adcStct = {
    2000,    //sampleRate
    {
        {//UADC
            7,  //recursiveK;   
            64, //MA_filter_WITH
        },
        {//TADC
            7,  //recursiveK;   
            64, //MA_filter_WITH
        }
    },
};

const uint16_t adcDefVal[] = {
    0,  //UADC
    1600//TADC
};

/******************************************************************************
*
*/
void adcStctInit(void){
    for(uint8_t ch = 0; ch < ADC_NUM_CH; ch++){
        adcStct.adcFilt[ch].recursiveFilterCumul = adcDefVal[ch] << adcStct.adcFilt[ch].recursiveK;
        for(uint16_t i = 0; i < adcStct.adcFilt[ch].MA_filter_WITH; i++){
            adcStct.adcFilt[ch].MA_filterMas[i] = adcDefVal[ch];
        }
    }
}

/******************************************************************************
* ������������� TIM3 -> ADC1 -> DMA1_Channel1 -> DMA1_Channel1_IRQHandler
* PC0 -> ADC1_IN10 ��� ����
* PC2 -> ADC1_IN12 ��� ����������
*/
void InitAdc(void){
    adcStctInit();

    //ADC1_IN9
    gppin_init(GPIOB, 1, analogMode);
    gppin_init(GPIOA, 15, analogMode);
    
    //������������� ���
    RCC->APB2ENR    |= RCC_APB2ENR_ADC1EN;                      //�������� ������������ ADC
    RCC->APB2RSTR   |= RCC_APB2RSTR_ADC1RST;                    //ADC 1 reset
    RCC->APB2RSTR   &= ~RCC_APB2RSTR_ADC1RST;  
    RCC->CFGR       &= ~RCC_CFGR_ADCPRE;
    RCC->CFGR       |=  RCC_CFGR_ADCPRE_DIV8;                   //ADC Prescaler / 8
    ADC1->CR1       = 0;                                        //�����������
    ADC1->CR2       = 0;                                        //�����������
    ADC1->SQR3      = 0;                                        //�����������
    ADC1->CR1       |= ADC_CR1_SCAN;                            //Scan mode enabled
    ADC1->CR2       |= ADC_CR2_TSVREFE;                         //Temperature sensor and VREFINT enable
    ADC1->CR2       |= ADC_CR2_EXTTRIG;                         //Conversion on external event enabled
    ADC1->CR2       |= ADC_CR2_EXTSEL_2;                        //Timer 3 TRGO event
    ADC1->CR2       |= ADC_CR2_DMA;                             //DMA mode enabled
    
    ADC1->SMPR1     |= ADC_SMPR1_SMP16_2;                       //Sample time 41.5 cycles
    ADC1->SMPR2     |= ADC_SMPR2_SMP9;                          //Sample time 239.5 cycles
    ADC1->SQR1      |= 2-1 << 20;                               //Regular channel sequence length 2 conversions
    ADC1->SQR3      |= 9  << (5*0);                             //SQ1 -> ����� 9
    ADC1->SQR3      |= 16 << (5*1);                             //SQ2 -> ����� 16
    
    ADC1->CR2       |= ADC_CR2_ADON;                            //����� ��� �� ������ ����������� �����������������
    //���������� ���
    ADC1->CR2       |= ADC_CR2_RSTCAL;
    while((ADC1->CR2 & ADC_CR2_RSTCAL) != 0);
    ADC1->CR2   |= ADC_CR2_CAL;
    while((ADC1->CR2 & ADC_CR2_RSTCAL) != 0);

    //������������� DMA
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;                           //�������� ������������ DMA1
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);                //����� ���������
    DMA1_Channel1->CMAR = (uint32_t)&adcStct.adcreg[0];         //����� ������
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;                        //������ �� ���������, ������ � ������
    DMA1_Channel1->CNDTR = 2;                                   //���������� ������������ ���������
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;                       //����� ��������� �� ���������������� ����� ������ ���������
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;                        //����� ������ ���������������� ����� ������ ���������
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;                     //����������� ������ ��������� - 16 ���
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;                     //����������� ������ ������ - 16 ���
    DMA1_Channel1->CCR |= DMA_CCR1_PL_0;                        //��������� - ����� ������� (Medium)
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;                        //Transfer complete interrupt enable
    DMA1_Channel1->CCR |= DMA_CCR2_CIRC;                        //����������� ������ DMA
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);                         //�������� ���������� �� DMA1_Channel1
    NVIC_SetPriority(DMA1_Channel1_IRQn, DMA1_Channel1_IRQn_Priority);  //���������� ���������
    DMA1_Channel1->CCR |= DMA_CCR2_EN;                          //��������� ������ ������

    //������������� �������
    RCC->APB1RSTR   |= RCC_APB1RSTR_TIM3RST;                    //Timer 3 reset
    RCC->APB1RSTR   &= ~RCC_APB1RSTR_TIM3RST;
    RCC->APB1ENR    |= RCC_APB1ENR_TIM3EN;                      //�������� ������������ TIM3
    TIM3->PSC       = ADC_SYSTEM_FREQUENCY / 1000000 - 1;       //������������ �� 1us
    TIM3->CR2       |= TIM_CR2_MMS_1;                           //Update - The update event is selected as trigger output (TRGO)
    TIM3->ARR       = adcStct.sampleRate;                       //sampleRate, [us]
}

/******************************************************************************
*---> ���������� �������� ���������� DMA1 Channel1
*/
__irq void DMA1_Channel1_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(AdcEndConversionSem, &xHigherPriorityTaskWoken);  //������ ������� ������-�����������
    if(xHigherPriorityTaskWoken != pdFALSE) {
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
    DMA1->IFCR      = DMA_IFCR_CTCIF1;                          //���������� ����
}

/******************************************************************************
* ������ ���������� �������
*/
static inline uint16_t movingAverageFilter(uint16_t *src, u8 with){
    u16     *dataEnd = src + with;
    u32     sum = 0;
    
    if(with == 1){
        return *src;
    }

    while(src < dataEnd){   //����������
        sum += *src++;
        sum += *src++;
        sum += *src++;
        sum += *src++;
        sum += *src++;
        sum += *src++;
        sum += *src++;
        sum += *src++;
    }
    return ((sum + (with>>1)) / with);
}

/******************************************************************************
* ��������� ����������
*/
void adcCalc(void){
    adcStct_type        *a;
    adcFilt_type        *f, *pEnd;
    uint32_t            ch;
    _iq                 qu;
 
    a       = &adcStct;
    f       = &a->adcFilt[0];
    pEnd    = f + ADC_NUM_CH;
    ch      = 0;
    
    while(f < pEnd){
        
        /**************************************
        * ��������� ����������� ��������
        */
        f->recursiveFilterOut = iq_filtr(f->recursiveFilterCumul, a->adcreg[ch], f->recursiveK);

        /**************************************
        * ���������� �������� ������� ����������� ��������
        */
        f->MA_filterMas[f->MA_filterIndex++] = f->recursiveFilterOut;
        if(f->MA_filterIndex >= f->MA_filter_WITH){
            f->MA_filterIndex = 0;
        }

        /**************************************
        * ��������� ���������� �������
        */
        a->filtered[ch] = movingAverageFilter(&f->MA_filterMas[0], f->MA_filter_WITH);
        
        f++;
        ch++;
    }

    /**************************************
    * ������ ����������
    */
    adcStct.voltage = adcStct.filtered[CH_UADC] * _IQ((AdcVref * (SPS_R1 + SPS_R2) * K_CORR) / (4096 * SPS_R2));
    
    /**************************************
    * ������ �����������
    * Temperature (in �C) = {(V25 - VSENSE) / Avg_Slope} + 25
    */
    qu = adcStct.filtered[CH_TADC] * _IQ(AdcVref / 4095);
    adcStct.temperature = _IQdiv((_IQ(AdcV25) - qu), _IQ(AdcAvg_Slope)) + _IQ(25);
}

/**************** (C) COPYRIGHT ************* END OF FILE ********* D_EL *****/
