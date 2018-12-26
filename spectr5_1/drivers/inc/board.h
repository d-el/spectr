/*!****************************************************************************
* @file    board.h
* @author  d_el
* @version V1.0
* @date    01.05.2016, Storozhenko Roman
* @brief   --
*/
#ifndef board_H
#define board_H

/*!****************************************************************************
* Include
*/
#include "gpio.h"

/*!****************************************************************************
* User define
*/
#define SYSTEM_FREQ         24000000    //[Hz]
#define APB1_FREQ           24000000    //[Hz]
#define APB2_FREQ           24000000    //[Hz]

//-----------------------------------------------------------------------------
//ПРИОРИТЕТЫ ПРЕРЫВАНИЙ, ПО УБЫВАНИЮ
#define EXTI_CC_CV_Priority         0
#define PVD_IRQ_Priority            1
#define DMA1_Channel1_IRQn_Priority 15  //Должен быть ниже configMAX_SYSCALL_INTERRUPT_PRIORITY, иначе нельзя испольховать API ф-и ОС
//-----------------------------------------------------------------------------
#define CALIBR_POINTS               2

/*!****************************************************************************
* User define
*/

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/
#define setDacI(u16val)     DAC->DHR12R1 = u16val
#define setDacU(u16val)     DAC->DHR12R2 = u16val
#define LED_ON()            gppin_set(GP_LED)
#define LED_OFF()           gppin_reset(GP_LED)
#define LED_TOGGLE()        gppin_togle(GP_LED)
#define IRQ_Limit_ON()      EXTI->IMR |= EXTI_IMR_MR3       //Interrupt request from Line x is not masked
#define IRQ_Limit_OFF()     EXTI->IMR &= ~EXTI_IMR_MR3      //Interrupt request from Line x is masked
#define MODE_IS_CC()        ((gppin_get(GP_CC_CV)) ? 1:0)   //Определяет проверка режима ограничения

/*!****************************************************************************
* Prototypes for the functions
*/

#endif //board_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** d_el ****/
