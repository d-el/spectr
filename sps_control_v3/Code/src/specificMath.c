/******************************************************************************
* @file    specificMath.c
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    12-09-2015
* @brief
*/
#include "globalIncludes.h"

/******************************************************************************
* Вариант функции для параметров в формате iq
*/
_iq iq_Fy_x1x2y1y2x(_iq x1, _iq x2, _iq y1, _iq y2, _iq x)
{
    _iq     df, dx;
    df = y2-y1;
    dx = x2-x1;
    if(dx == 0){
        return 0x7FFFFFFFL;
    }
    return y1 + _IQmpy((_IQdiv(df, dx)), (x - x1));
}

/******************************************************************************
* Фильтр Скользящее Среднее
*/
uint16_t movingAverageFilter(uint16_t *src, u8 with){
    u16     *dataEnd = src + with;
    u32     sum = 0;

    while(src < dataEnd){   //Накопление
        sum += *src++;
    }
    return (sum / with);
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
