/**
*******************************************************************************
* @file     onewire.h
* @author   Storozhenko Roman
* @version  V1.2
* @date     10.05.2015
* @brief    ������� �������������, ������/������ �� 1-wire
*/
#ifndef ONEWIRE_H
#define ONEWIRE_H
#include "globalIncludes.h"

/*******************************************************************************
* User define
*/
#define OWCH1PORT       GPIOA                               //������� ���� ow
#define OWCH1PIN        9                                   //������� ����� ���� ow
#define OWCH2PORT       GPIOA                               //������� ���� ow
#define OWCH2PIN        9                                  //������� ����� ���� ow
//----------------------------------------------
#define low_ow_dq   (*ow.odr    = ow.pin_mask_bit_reset)    //�������� ���� � �����
#define hi_ow_dq    (*ow.odr    = ow.pin_mask_bit_set)      //��������� ����
#define ow_dq       (*ow.idr    & ow.pin_mask)              //Inpun state
//----------------------------------------------
#define __delay_us(x) delay_us(x)

/******************************************************************************
* User typedef
*/
typedef struct{
    __IO uint32_t *odr;
    __IO uint32_t *idr;
    uint32_t pin_mask_bit_set;
    uint32_t pin_mask_bit_reset;
    uint16_t pin_mask;
}struct_ow_type;

typedef enum{
    owOk,               //���������� ����������
    owNotFound,         //�� ����������
    owShortCircle,      //�.�. �� �����
    owSearchLast,       //������ ��������� �����
    owSearchFinished,   //����� ����������, ��������� ����� ��� ������ � ���������� ���
    owSearchError
}owSt_type;

/******************************************************************************
* Extern viriables
*/
extern  struct_ow_type  ow;

/******************************************************************************
* Prototypes for the functions
*/
void        owch1_init(void);
void        owch2_init(void);
owSt_type   ow_init(void);
void        ow_write_bit(uint8_t bit);
uint8_t     ow_read_bit(void);
void        ow_write(uint8_t byte);
uint8_t     ow_read(void);
uint8_t     crc8(uint8_t *mas, uint8_t n);
owSt_type   ow_search_rom(uint8_t *pprev, uint8_t searchInit);

#endif //ndef ONEWIRE_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
