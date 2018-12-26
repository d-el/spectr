/**
*******************************************************************************
* @file    crc.h
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    23.06.2015
* @brief
*/
#ifndef crc_H
#define crc_H
#include "globalIncludes.h"

/******************************************************************************
* Prototypes for the functions
*/
uint16_t GetCrcPO(void);
u16 GetCrc(void *Bff, u32 szBff);
u16 GetCrc3Word(void *Bff);
u16 CrcPice(u8 *Bff, u32 szBff, u16 CrcPrev);

#endif  //crc_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
