/******************************************************************************
* @file    drivers.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    05-07-2013
*/

#ifndef DRIVERS_H
#define DRIVERS_H
#include "globalIncludes.h"
#include "IQmathLib.h"
#include "board.h"

/******************************************************************************
* User define
*/
//-----------------------------------------------------------------------------
//ПРИОРИТЕТЫ ПРЕРЫВАНИЙ, ПО УБЫВАНИЮ
//-----------------------------------------------------------------------------
#define TIM6_DAC_IRQn_Priority      15  //Не приоритетная работа
//-----------------------------------------------------------------------------
#define SW_PORT                     GPIOC
#define SW_PIN                      pinm13
//-----------------------------------------------------------------------------

/******************************************************************************
* User typedef
*/
typedef struct{
    _iq         qu;
    int16_t     adc;
    int16_t     dac;
}uCalibr_type;

typedef struct{
    _iq         qi;
    int16_t     adc;
    int16_t     dac;
}iCalibr_type;

typedef struct{
    uint16_t        startCounter;
    uint16_t        kLval;              //Коэффициент коррекции ограничителя
    uint16_t        kKorHighvoltage;
    uint16_t        freqBeep;           //Частота пищалки
    uint16_t        hiTemp;             //Температура включения вентилятора
    uint16_t        lowTemp;            //Температура выключения вентилятора
    uint8_t         lcdContrast;        //Контрастность дисплея
    uint8_t         spsUse      :1;     //Использовать цифровой реостат
    uint8_t         stopsFan    :1;     //Использовать алгоритм останова куллера    0 - всегда крутится
    uint8_t         modeSwitch  :1;     //0 - нормальный, 1 - инверсный
    uCalibr_type    pU[CALIBR_POINTS];
    iCalibr_type    pI[CALIBR_POINTS];
}syssetting_type;

/******************************************************************************
* User enum
*/
enum{
    BASE_DAC
};

/******************************************************************************
* Extern viriables
*/
extern syssetting_type      sysset;
extern uint16_t             base[];

/******************************************************************************
* Macro functions
*/
#define setFanPwm(perc){                /*[X_X %]*/\
    TIM4->CCR4 = (((uint32_t)perc * 500) / 1000);  \
}

#define QUIET_MODE_ON()     (RCC->APB2ENR &= ~RCC_APB2ENR_TIM16EN)  //Выключили тактирование Timer3 (тихий режим)
#define QUIET_MODE_OFF()    (RCC->APB2ENR |= RCC_APB2ENR_TIM16EN)   //Выключили тактирование Timer3 (тихий режим)
#define RTC_GET()           (RTC->CNTL)

/******************************************************************************
* Prototypes for the functions
*/
void        Init_Hard(void);
void        output_ON(uint16_t top);
void        charge_cap(uint16_t top);
void        switchON(void);
void        switchOFF(void);
void        prmInitDef(void);

#endif //DRIVERS_H
/************ (C) COPYRIGHT ************ END OF FILE ************ DEL ********/
