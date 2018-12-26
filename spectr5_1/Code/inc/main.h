/**
******************************************************************************
* @file    main.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    10-03-2013
* @brief   Обьявление глобальных переменных, макросы
******************************************************************************
*/
#ifndef MAIN_H
#define MAIN_H

/******************************************************************************
* User define
*/
#define SYSTEM_FREQUENCY    24000000 //Hz

extern s16 usts16;
extern s32 usts32;
extern float ustfloat;

/******************************************************************************
* Prototypes for the functions
*/
__noreturn main(void);

#endif  //MAIN_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
