/**
*******************************************************************************
* @file    enco.c
* @author  Storozhenko Roman - D_EL
* @version V2.3
* @date    13-05-2015 
* @brief   Средства работы с энкодером
*******************************************************************************
**/
#include "globalIncludes.h"

enco_type   enco;

/******************************************************************************
* @brief  Настройка периферии МК для работы с инкрементальным энкодером
* @param  Указатель на переменную приемник (приемник должен быть uint16_t)
* @retval None
*/
void Init_Encoder(void){
    //Вывод GPIOA.0
    GPIOA->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);           //Очистить разряды MODE и CNF
    GPIOA->CRL  |=  GPIO_CRL_CNF0_1;                            //Дискретный вход
    GPIOA->BSRR =  GPIO_BSRR_BS0;                               //Подтяжка к плюсу
    //Вывод GPIOA.1
    GPIOA->CRL  &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);           //Очистить разряды MODE и CNF
    GPIOA->CRL  |=  GPIO_CRL_CNF1_1;                            //Дискретный вход
    GPIOA->BSRR =  GPIO_BSRR_BS1;                               //Подтяжка к плюсу

    //Настройка таймера
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;                         //Тактирование таймера
    TIM2->CCMR1  = TIM_CCMR1_CC2S_0 | TIM_CCMR1_CC1S_0;         //Полярность сигнала для каждого входа
    TIM2->CCER   = TIM_CCER_CC1P | TIM_CCER_CC2P;
    
    TIM2->CCMR1 |= (TIM_CCMR1_IC2PSC_1 | TIM_CCMR1_IC1PSC_1);   //Capture is done once every 4 events

    TIM2->CCMR1 |= TIM_CCMR1_IC1F | TIM_CCMR1_IC2F;             //Input capture 1 filter fSAMPLING=fDTS/32, N=8

    TIM2->SMCR  = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;              //Режим энкодера
    TIM2->ARR   = 0xFFFF;                                       //Значение, до которого считает
    TIM2->SMCR  |=  TIM_SMCR_ETPS_1;
    TIM2->CNT = (0xFFFF / 4) / 2;
    TIM2->CR1   = TIM_CR1_CEN;                                  //Включаем таймер
}

uint32_t enReadVal(void){
    if(enco.memtype == enU8){
        return (*(uint8_t*)enco.pval);
    }
    else if(enco.memtype == enU16){
        return (*(uint16_t*)enco.pval);
    }
    else if(enco.memtype == enU32){
        return (*(uint32_t*)enco.pval);
    }
    return 0xFFFFFFFF;
}

void enWriteVal(int32_t val){
    if(enco.memtype == enU8){
        (*(uint8_t*)enco.pval) = val;
    }
    else if(enco.memtype == enU16){
        (*(uint16_t*)enco.pval) = val;
    }
    else if(enco.memtype == enU32){
        (*(uint32_t*)enco.pval) = val;
    }
}

void enAdd(int32_t val){
    int32_t temp;
    
    temp = enReadVal();
    temp += val;
    
    if(enco.mode == enCIRC){
        if(temp < enco.bot){
            enWriteVal(enco.top);
            //enWriteVal(enco.pval, enco.top + val + 1);
            return;
        }
        if(temp > enco.top){
            enWriteVal(enco.bot);
            //enWriteVal(enco.pval, enco.bot + val - 1);
            return;
        }
    }
    else{
        if(temp < enco.bot){
            enWriteVal(enco.bot);
            return;
        }
        if(temp > enco.top){
            enWriteVal(enco.top);
            return;
        }
    }
    enWriteVal(temp);
}

void enBigStepUp(void){
    enAdd(enco.bigStep);
}

void enBigStepDown(void){
    enAdd(-enco.bigStep);
}

/******************************************************************************
* @brief  Прочитать значние из экодера
* @retval None
*/
void enGet(void){
    int32_t             temp;
    int64_t             temp2;
    static uint16_t     ntic;
    
    if(ntic >= enco.ntic){
        temp = TIM2->CNT - (0xFFFF / 4) / 2;
        TIM2->CNT = (0xFFFF / 4) / 2;
        if(temp > 0){
            temp = (temp + 2) / 4;
        }else{
            temp = (temp - 2) / 4;
        }
        
        temp = temp * enco.step; ////Сюда еще шаг приделать можно
        temp2 = enReadVal();
        temp2 += temp;
            
        if(enco.mode == enCIRC){
            if(temp2 < enco.bot){
                enWriteVal(enco.top);
                //enWriteVal(dst, enco.top + temp + 1);
                return;
            }
            if(temp2 > enco.top){
                enWriteVal(enco.bot);
                //enWriteVal(dst, enco.bot + temp - 1);
                return;
            }
        }else{
            if(temp2 < enco.bot){
                enWriteVal(enco.bot);
                return;
            }
            if(temp2 > enco.top){
                enWriteVal(enco.top);
                return;
            }
        }

        if(enco.memtype == enU8){
            *(uint8_t*)enco.pval += temp;
        }
        else if(enco.memtype == enU16){
            *(uint16_t*)enco.pval += temp;
        }
        else if(enco.memtype == enU32){
            *(uint32_t*)enco.pval += temp;
        }
        
        ntic = 0;
    }else{
        ntic++;
    }
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
