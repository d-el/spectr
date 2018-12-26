/**
*******************************************************************************
* @file     onewire.c
* @author   Storozhenko Roman
* @version  V1.2
* @date     10.05.2015
* @brief    ������� �������������, ������/������ �� 1-wire
*/
#include "globalIncludes.h"

struct_ow_type  ow;

void owch1_init(void){
    ow.odr = &(OWCH1PORT->BSRR);
    ow.idr = &(OWCH1PORT->IDR);
    
    ow.pin_mask_bit_set = (1 << OWCH1PIN);
    ow.pin_mask_bit_reset = (1 << (OWCH1PIN + 16));
    ow.pin_mask = (1 << OWCH1PIN);
}

void owch2_init(void){
    ow.odr = &(OWCH2PORT->BSRR);
    ow.idr = &(OWCH2PORT->IDR);
    
    ow.pin_mask_bit_set = (1 << OWCH2PIN);
    ow.pin_mask_bit_reset = (1 << (OWCH2PIN + 16));
    ow.pin_mask = (1 << OWCH2PIN);
}

/******************************************************************************
* @brief   ������������� ���� 1-WIRE
* @param    None
* @retval   owSt_type
*/
owSt_type ow_init(void){
    uint16_t result;
    if(ow_dq == 0){
        return owShortCircle;       //��������� ����� �� ���������� ���������
    }
    low_ow_dq;                      //�������� ���� � �����
    __delay_us(500);                //����� 480..960 ���
    hi_ow_dq;                       //��������� ����
    __delay_us(60);                 //����� 60 �����������
    result = ow_dq;                 //��������� ����
    __delay_us(450);                //��������� ��������� �������������
    if(result != 0){
        return owNotFound;          //������ �� ���������
    }
    return owOk;                    //������ ���������
}

/*****************************************************************************
* @brief    �������� ����
* @param    Bit: 0, 1
*/
inline void ow_write_bit(uint8_t bit){
    low_ow_dq; //�������� ���� � �����
    if (bit != 0){
        __delay_us(5);              //������ �������, �� 1 �� 15 ��� (� ������ ������� �������������� ������)
        hi_ow_dq;                   
        delay_us(90);               //�������� �� ���������� ��������� (�� ����� 60 ���)
    }else{                        
        __delay_us(90);             //������ ������� �� ���� �������� (�� ����� 60 ���, �� ����� 120 ���)
        hi_ow_dq;                   
        __delay_us(5);              //����� �������������� �������� ������� �� ���� + 1 ��� (�������)
    }
}

/*****************************************************************************
* @brief  ������ ����
* @param  None
* @retval Bit: 0, 1
*/
inline uint8_t ow_read_bit(void){
    uint16_t val;
    low_ow_dq;                      //�������� ���� � �����
    __delay_us(2);                  //������������ ������� ������, ������� 1 ���
    hi_ow_dq;                       //��������� ����
    __delay_us(8);                  //����� �� ������� �������������, ����� �� ����� 15 ���
    val = ow_dq;                    //��������� ����
    __delay_us(55);                 //�������� �� ���������� ����-�����, ������� 60 ��� � ������ ������� ������
    if(val != 0)    return 1;
    return 0;
}

/*****************************************************************************
* @brief  ������ �����
* @param  Byte: data to be transmitted
* @retval None
*/
inline void ow_write(uint8_t byte){
    uint8_t i;
    for(i = 1; i != 0; i <<= 1){
        ow_write_bit(byte & i);
    }
}

/*****************************************************************************
* @brief  ������ �����
* @param  None
* @retval Result: the value of the received data
*/
uint8_t ow_read(void){
uint8_t i, result = 0;
    for(i = 1; i != 0; i <<= 1){
        if(ow_read_bit() != 0){
            result |= i;
        }
    }
    return result;
}

/*****************************************************************************
* @brief  ������� CRC-8-Dallas/Maxim
* @param  ��������� �� ������, ����������� ���������
* @retval Result: the value of the received data
*/
uint8_t crc8(uint8_t *mas, uint8_t n){
    uint8_t j , i, tmp, data, crc = 0;
    
    for(i = 0; i < n; i++){
        data = *mas;
        for(j = 0; j < 8; j++){                 //���������� CRC - ������������ ������ ��� ��������� �����
            tmp = (crc ^ data) & 0x01;
            if (tmp == 0x01) crc = crc ^ 0x18;
            crc = (crc >> 1) & 0x7F;
            if (tmp == 0x01) crc = crc | 0x80;
            data = data >> 1;
        }
        mas++;
    }
    return crc;
}

/*****************************************************************************
* @brief  �������� ��������� �����, ������ ����� �� ���������
          ��� ������ ������� ������ d �������� searchInit ����� �������� 0
* @param  pprev:        ��������� �� ������ �������� ������
* @param  searchInit:   0 - ������������� � ����� ������� ������
                        !0 - ����� ���������� ������
* @retval ������
*/
uint8_t ow_enum[8]; //��������� �������������� �����
uint8_t ow_enum_fork_bit = 65; //��������� ������� ���, ��� ���� ��������������� (������� � �������)

owSt_type ow_search_rom(uint8_t *pprev, uint8_t searchInit){
    uint8_t     newfork;
    uint8_t     p;
    uint8_t     bp;
    uint8_t     prev;
    uint8_t     next;
    owSt_type   owState;
    
    bp      = 8;
    prev    = *pprev;
    next    = 0;
    
    if(searchInit == 0){
        ow_enum_fork_bit = 65;                  //��������� ������� ���, ��� ���� ��������������� (������� � �������)
    }
    if(ow_enum_fork_bit == 0){                  //���� �� ���������� ���� ��� �� ���� �����������
        return owSearchFinished;
    }
    owState = ow_init();
    if(owState != 0){
        return owState;
    }

    p = 1;
    ow_write(0xF0);
    newfork = 0;
    while(1){
        uint8_t not0 = ow_read_bit();
        uint8_t not1 = ow_read_bit();
        if(not0 == 0){                                  //���� ������������ � ������� ��� ����
            if(not1 == 0){                              //�� ����� ������������� ��� 1 (�����)
                if(p < ow_enum_fork_bit){               //���� �� ����� �������� ������� ������������ ����,
                    if((prev & 1) != 0){
                        next |= 0x80;                   //�� �������� �������� ���� �� �������� �������
                    }
                    else{
                        newfork = p;                    //���� ����, �� �������� ����������� �����
                    }
                }
                else if(p == ow_enum_fork_bit){
                    next |= 0x80;                       //���� �� ���� ����� � ������� ��� ��� ������ �������� � ����, ������� 1
                }
                else{
                    newfork = p;                        //������ - ������� ���� � ���������� ����������� �����
                }
            }                                           //� ��������� ������ ���, ������� ���� � ������
        }
        else{
            if(not1 == 0){                              //������������ �������
                next |= 0x80;
            }
            else{                                       //��� �� ����� �� ������ - ��������� ��������
                return owSearchError;
            }
        }
        ow_write_bit(next & 0x80);
        bp--;
        if(bp == 0){
            *pprev = next;
            if(p >= 64){
                break;
            }
            next = 0;
            pprev++;
            prev = *pprev;
            bp = 8;
        }
        else{
            if(p >= 64){
                break;
            }
            prev >>= 1;
            next >>= 1;
        }
        p++;
    }
    ow_enum_fork_bit = newfork;
    return owOk;
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
