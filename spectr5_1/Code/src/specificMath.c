/******************************************************************************
* @file    specificMath.c
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    12-09-2015
* @brief
*/
#include "globalIncludes.h"


/******************************************************************************
*
*/
_iq interpoly_x0f0_x1f1_x(_iq x0, _iq f0, _iq x1, _iq f1, _iq x){
    _iq         deltaf;
    int32_t     deltax;
    
    deltaf = f1 - f0;
    deltax = x1 - x0;
    
    return f0 + (deltaf / deltax) * (x - x0);
}

/******************************************************************************
* ������� ������� ��� ���������� � ������� iq
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
* ������� ������� ��� ���������� � ������� s32 iq
*/
_iq s32iq_Fy_x1x2y1y2x(int32_t x1, int32_t x2, _iq y1, _iq y2, int32_t x)
{
    _iq     df;
    int32_t dx;
    
    df = y2-y1;
    dx = x2-x1;
    if(dx == 0){
        return 0x7FFFFFFFL;
    }
    return y1 + (df / dx) * (x - x1);
}

/******************************************************************************
* ������� ������� ��� ���������� � ������� iq s32
*/
int32_t iqs32_Fy_x1x2y1y2x(_iq x1, _iq x2, int32_t y1, int32_t y2, _iq x)
{
    int32_t df;
    _iq     dx;
    
    df = y2-y1;
    dx = x2-x1;
    if(dx == 0){
        return INT32_MAX;
    }
    return y1 + (df / dx) * (x - x1);
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
