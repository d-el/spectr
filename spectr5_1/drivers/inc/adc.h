/******************************************************************************
* @file    adc.h
* @author  Storozhenko Roman - D_EL
* @version V2.0
* @date    04.10.2015
*/
#ifndef ADC_H
#define ADC_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define ADC_NUM_CH                  2
#define MA_FILTER_MAX_WITH          64
#define ADC_SYSTEM_FREQUENCY        24000000//[Hz]

/******************************************************************************
* User typedef
*/
typedef struct{
    uint16_t    adcDefVal;
    uint16_t    oversampling;
    uint16_t    recursiveK;
    uint16_t    MA_filter_WITH;         //������ 8
    uint32_t    recursiveFilterCumul;
    uint16_t    recursiveFilterOut;
    uint16_t    MA_filterMas[MA_FILTER_MAX_WITH];
    uint16_t    MA_filterIndex;
    
}adcFilt_type;

typedef struct{
    uint16_t        sampleRate;                         //[us] ������� �������������
    adcFilt_type    adcFilt[ADC_NUM_CH];
    uint16_t        adcreg[ADC_NUM_CH];                 //�������� ���������
    uint16_t        filtered[ADC_NUM_CH];
    
    /*******************************/
    _iq         voltage;
    _iq         current;
    _iq         outPower;
    _iq14       resistens;
    uint32_t    capacity;       		//[mA/h]
}adcStct_type;

/******************************************************************************
* User enum
*/
enum{
    CH_UADC,
    CH_IADC,
};

/******************************************************************************
* Extern viriables
*/
extern adcStct_type     adcStct;

/******************************************************************************
* Macro functions
*/
#define startSampling()             TIM3->CR1 |= TIM_CR1_CEN
#define stopSampling()              TIM3->CR1 &= ~TIM_CR1_CEN

/******************************************************************************
* Prototypes for the functions
*/
void InitAdc(void);
__irq void DMA1_Channel1_IRQHandler(void);
void adcCalc(void);

#endif //ADC_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
