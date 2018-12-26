/******************************************************************************
* @file    initPeripheral
* @author  d_el
* @version V1.0
* @date    10.01.2015, by d_el
* @brief   --
*/
#ifndef initPeripheral_H
#define initPeripheral_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define     HSE_PLL_MUL         2                               //2-16, Коэффициент умножения PLL
#define     HSE_PLL_PREDIV      2                               //1-16, Коэффициент деления PLL
#define     HSI_PLL_MUL         6                               //2-16, Коэффициент умножения PLL
#define     HSI_PLL_PREDIV      1                               //1-16, Коэффициент деления PLL
#define     RCC_TIMEOUT         100000

/******************************************************************************
* User typedef
*/
typedef enum{
    clockSourceHsi,
    clockSourceHse
}clockSource_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern clockSource_type            clockSource;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void Init_PVD(void);
void Init_EXTI1(void);
void Init_CLK(void);
void Init_SPI2(void);
void Init_Timer4(void);
void Init_Timer16(void);
void Init_USART3(void);
void Init_Port(void);
void InitTim1(void);
void Init_DAC_CH1(void);
void Init_DAC_CH2(void);

#endif //initPeripheral_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
