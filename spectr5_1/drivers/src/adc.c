/******************************************************************************
* @file    adc.c
* @author  Storozhenko Roman - D_EL
* @version V2.0
* @date    04.10.2015
* @PC1 - ADCIN11 - IADC
* @PC2 - ADCIN12 - UADC
*/
#include "globalIncludes.h"
#include "board.h"

/******************************************************************************
* MEMORY
*/
adcStct_type        adcStct = {
    10000,    //sampleRate
    {
        {//UADC
            0,  //adcDefVal
            1,  //oversampling
            5,  //recursiveK;   
            32, //MA_filter_WITH
        },
        {//IADC
            0,  //adcDefVal
            2,  //oversampling
            4,  //recursiveK;   
            32,  //MA_filter_WITH
        }
    },
};

/******************************************************************************
*
*/
void adcStctInit(void){
    for(uint8_t ch = 0; ch < ADC_NUM_CH; ch++){
        adcStct.adcFilt[ch].recursiveFilterCumul = adcStct.adcFilt[ch].adcDefVal << adcStct.adcFilt[ch].recursiveK;
        for(uint16_t i = 0; i < adcStct.adcFilt[ch].MA_filter_WITH; i++){
            adcStct.adcFilt[ch].MA_filterMas[i] = adcStct.adcFilt[ch].adcDefVal;
        }
    }
}

/******************************************************************************
* ������������� TIM3 -> ADC1 -> DMA1_Channel1 -> DMA1_Channel1_IRQHandler
* PC1 -> ADC1_IN11 ��� ����
* PC2 -> ADC1_IN12 ��� ����������
*/
void InitAdc(void){
    adcStctInit();

    //����� PC.1  ���� ����������
    gppin_init(GPIOC, 1, analogMode, 0);
    //����� PC.2  ���� ����������
    gppin_init(GPIOC, 2, analogMode, 0);
    
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
    ADC1->CR2       |= ADC_CR2_EXTTRIG;                         //Conversion on external event enabled
    ADC1->CR2       |= ADC_CR2_EXTSEL_2;                        //Timer 3 TRGO event
    ADC1->CR2       |= ADC_CR2_DMA;                             //DMA mode enabled
    
    ADC1->SMPR1     |= ADC_SMPR1_SMP11_2 |                      //Sample time 7.5 cycles
                       ADC_SMPR1_SMP12_0 ;                      //Sample time 7.5 cycles
    
    ADC1->SQR1      |= ADC_SQR1_L_0;                            //Regular channel sequence length 2 conversions
    
    ADC1->SQR3      |= ADC_SQR3_SQ2_3 |
                       ADC_SQR3_SQ2_1 |
                       ADC_SQR3_SQ2_0;                          //����� 11
    
    ADC1->SQR3      |= ADC_SQR3_SQ1_3 | ADC_SQR3_SQ1_2;         //����� 12
    
    
    ADC1->CR2       |=    ADC_CR2_ADON;                         //����� ��� �� ������ ����������� �����������������
    
    //���������� ���
    ADC1->CR2       |= ADC_CR2_RSTCAL;
    while((ADC1->CR2 & ADC_CR2_RSTCAL) != 0);
    ADC1->CR2       |= ADC_CR2_CAL;
    while((ADC1->CR2 & ADC_CR2_CAL) != 0);

    //������������� DMA
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;                           //�������� ������������ DMA1
    DMA1_Channel1->CCR = 0;
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);                //����� ���������
    DMA1_Channel1->CMAR = (uint32_t)&adcStct.adcreg[0];         //����� ������
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;                        //������ �� ���������, ������ � ������
    DMA1_Channel1->CNDTR = ADC_NUM_CH;                          //���������� ������������ ���������
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
    if(ps.state.bit.osInit != 0){
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(AdcEndConversionSem, &xHigherPriorityTaskWoken);  //������ ������� ������-�����������
        if(xHigherPriorityTaskWoken != pdFALSE) {
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
        }
    }
    else{
        adcCalc();
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
    syssetting_type     *s;
    adcFilt_type        *f, *pEnd;
    uint32_t            ch;
    uint32_t            val;
 
            measTimeStart();
            
    a       = &adcStct;
    s       = &sysset;
    f       = &a->adcFilt[0];
    pEnd    = f + ADC_NUM_CH;
    ch      = 0;
    
    while(f < pEnd){
        
        /**************************************
        * ����������������
        */
        val = a->adcreg[ch] * f->oversampling;
        
        /**************************************
        * ��������� ����������� ��������
        */
        f->recursiveFilterOut = iq_filtr(f->recursiveFilterCumul, val, f->recursiveK);

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
    
    measTimeStop(0);

    /**************************************
    * ������������ ��� ����
    */
    a->current = s32iq_Fy_x1x2y1y2x(s->pI[0].adc, s->pI[1].adc, s->pI[0].qi, s->pI[1].qi, a->filtered[CH_IADC]);
    if(a->current < 0)  a->current = 0;
}

/**************** (C) COPYRIGHT ************* END OF FILE ********* D_EL *****/
