/**
******************************************************************************
* @file    drivers.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    05-07-2013
******************************************************************************
**/
#include "globalIncludes.h"

syssetting_type     sysset;

/******************************************************************************
*
*/
void Init_Hard(void){
    Init_CLK();
    Init_Port();
    Init_Timer2();
    owSlaveInit();
    InitAdc();
    uart_init(USART1, BR9600);
    Init_DAC_CH1();
    //Init_PVD();
}

/******************* (C) COPYRIGHT ********** END OF FILE ********* D_EL *****/
