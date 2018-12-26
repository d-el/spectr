/**
******************************************************************************
* @file    key.h 
* @author  Storozhenko Roman - D_EL
* @version V1.2
* @date    05-01-2014
* @brief   объявление переменных
******************************************************************************
*/
#ifndef KEY_H
#define KEY_H
#include "globalIncludes.h"

#define b_rewind    (ks.UP || ks.DOWN)                  //Кнопки, на которых работает перемотка

enum{
    AutoPressON     = 1,
    AutoPressOFF    = 0,
};

enum{
    KEY_READY    = 0,
    KEY_BUSY     = 1,
};

typedef struct{                                             //Структура для кнопок
    uint8_t BMDVW   :1;
    uint8_t SET     :1;
    uint8_t SWITCH  :1;
    uint8_t NEXT    :1;
    uint8_t UP      :1;
    uint8_t DOWN    :1;
    uint8_t ZERO    :1;
}button_type;


typedef union{
    button_type;
    uint8_t ANY;                                            //Все кнопки в одной переменной
}ks_type;                                                   
                                                            
typedef struct{                                                        
    uint16_t    scanningToFirstRewind;                      //Колличество проверок, чтоб запустилось автонажатие
    uint16_t    scanningToRewind;
    uint8_t     fVirtualKey         :1;                     //Флаг использования виртуальной клавиатуры
    uint8_t     AutoPress           :1;                     //Разрешить / запретить автонажатие, при длительном удержании кнопки
}kc_type;

extern volatile ks_type ks;
extern kc_type kc;

//-----------------------------------------------------------
//Prototypes for the functions in key.c
//-----------------------------------------------------------
uint8_t BtScan(void);

#endif //KEY_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
