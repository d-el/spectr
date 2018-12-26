/******************************************************************************
* @file    owSlave.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#ifndef owSlave_H
#define owSlave_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
/******************************************************************************
* Memory
*/
#define SCRATCHPAD_Pos_StartWr              stateToSps
#define SCRATCHPAD_POSITION_START_WRITE     ((uint8_t*)&spStr.SCRATCHPAD_Pos_StartWr - (uint8_t*)&spStr)
#define OWID_BUF_SIZE                       8
/**************************************
* OW Pin
*/
#define OW_PINN     12
#define OW_PORT     GPIOA
#define OW_PIN      ((gppin_get(OW_PORT, 1 << OW_PINN) != 0) ? 1:0)
#define SET_LOW     gppin_reset(OW_PORT, 1<<OW_PINN)                //set 1-Wire line to low
#define RESET_LOW   gppin_set(OW_PORT, 1<<OW_PINN)                  //set 1-Wire pin as input
/**************************************
* Pin interrupt
*/
#define EN_OWINT                EXTI->PR    |= EXTI_PR_PR1,  /*Очищаем флаг записью единици*/\
                                EXTI->IMR       |= EXTI_IMR_MR12;               //enable interrupt
#define DIS_OWINT               EXTI->IMR       &= ~EXTI_IMR_MR12;              //disable interrupt
#define SET_RISING              EXTI->RTSR      |= EXTI_RTSR_TR12;              //set interrupt at rising edge
#define SET_FALLING             EXTI->FTSR      |= EXTI_FTSR_TR12;              //set interrupt at falling edge
#define CHK_INT_EN              ((EXTI->IMR & EXTI_IMR_MR12) != 0)              //test if interrupt enabled 
/**************************************
* Timer Interrupt
*/
#define OW_TIM                  TIM15
#define EN_TIMER                OW_TIM->CR1 |= TIM_CR1_CEN;
#define DIS_TIMER               OW_TIM->CR1 &= ~TIM_CR1_CEN;
#define SET_TIME(x){            \
    OW_TIM->CNT = 0;            \
    OW_TIM->ARR = x;            \
}
/**************************************
* Times
*/
#define OWT_MIN_RESET           420
#define OWT_RESET_PRESENCE      32
#define OWT_PRESENCE            150
#define OWT_READLINE            32                   //for fast master, 4 for slow master and long lines
#define OWT_LOWTIME             32

/**************************************
* States / Modes
*/
#define OWM_SLEEP               0       //Waiting for next reset pulse
#define OWM_RESET               1       //Reset pulse received
#define OWM_PRESENCE            2       //Sending presence pulse
#define OWM_READ_COMMAND        3       //Read 8 bit of command
#define OWM_READ_ROM            4       //READ_ROM algorithms
#define OWM_SEARCH_ROM          5       //SEARCH_ROM algorithms
#define OWM_MATCH_ROM           6       //Test number
#define OWM_READ_SCRATCHPAD     7
#define OWM_WRITE_SCRATCHPAD    8
#define OWM_CHK_RESET           9       //Waiting of rising edge from reset pulse

//Write a bit after next falling edge from master
//its for sending a zero as soon as possible
#define OWW_NO_WRITE            2
#define OWW_WRITE_1             1
#define OWW_WRITE_0             0

/**************************************
* One wire commands
*/
#define OWCOM_SKIP_ROM          0xCC
#define OWCOM_READ_ROM          0x33
#define OWCOM_MATCH_ROM         0x55
#define OWCOM_SEARCH_ROM        0xF0
#define OWCOM_WRITE_SCRATCHPAD  0x4E
#define OWCOM_READ_SCRATCHPAD   0xBE

typedef struct{
    //---------------------------------------Readed
    uint16_t    temperature;        //[X_X °C]
    uint16_t    voltage;            //[mV]
    
    uint8_t     hvoltage;           //[V]
    struct{
        uint8_t     mhvPresent          :1;
    }stateSps;
    uint8_t     crcRd;
    //---------------------------------------Writable
    struct{
        uint8_t     temperatureLimOn    :1;
    }bitTask;
    
    uint16_t    vTask;              //[mV]
    uint8_t     crcWr;
    
    //--------------------------------------Counts
    uint8_t     res[15];
    uint16_t    errCnt;
    uint16_t    rxCnt;
}scratchpad_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
extern volatile scratchpad_type              spStr;

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void owSlaveInit(void);

#endif //owSlave_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
