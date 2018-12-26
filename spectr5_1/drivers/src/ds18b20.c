/**
******************************************************************************
* @file         DS18b20.c
* @author       Storozhenko Roman + many people
* @version      V2.1
* @date         30.03.2014
* @brief        ������� ������������� ds18b20
******************************************************************************
*/
#include "globalIncludes.h"

uint8_t     ow_rom[8][3];
volatile    tmpr_type   tem;

//===========================================================================
// ��������� ��������, ����� ���� ������� �� ��������������� �����������
// result:  0 - ���������� ����������, � �������� ds18b20,
//          1 - �� ���������� 1-wire ���������,
//          2 - �.�. �� �����,
//          3 - ������ CRC,
//          4 - ���������� �� �������� ds18b20.
//===========================================================================
uint8_t Init_DS18B20(void){
    uint8_t result, buff[8];
    uint8_t data,tmp,i,j,crc=0;

    owch1_init();
    
    result = ow_init();                                     //������������� ���� 1-wire
    if(result)  return result;                              //���� ���� ������ �� ���������� ��� ������
    ow_write(READ_ROM);                                     //READ ROM
    //��������� 8 ����
    for(i=0; i<8; i++){
        buff[i] = ow_read();                                //��������� ��������� ����
    }

    for(i=0; i<7; i++){
    data = buff[i];
        for(j=0; j<8; j++){             //���������� CRC - ������������ ������ ��� ��������� �����
            tmp = (crc ^ data) & 0x01;
            if (tmp==0x01) crc = crc ^ 0x18;
            crc = (crc >> 1) & 0x7F;
            if (tmp==0x01) crc = crc | 0x80;
            data = data >> 1;
        }
    }

    if(crc != buff[7])  return 3;
    if(buff[0] != 0x28) return 4;

    ow_init();                                      //������������� ���� 1-wire
    ow_write(SKIP_ROM);                             //SKIP ROM
    ow_write(WRITE_SCRATCHPAD);                     //Write scratchpad
    ow_write(127);                                  //TH
    ow_write(0);                                    //TL
    //����������������� ������ ������
    //ow_write(0x1F);         //9bit          93.75ms 0.5
    //ow_write(0x3F);         //10bit         187.5ms 0.25
    ow_write(0x5F);         //11bit 375ms   0.125
    //ow_write(0x7F);         //12bit 750ms   0.0625

    owch1_init();
    //ow_init();                                      //������������� ���� 1-wire
    //ow_write(SKIP_ROM);                             //SKIP ROM
    //ow_write(COPY_SCRATCHPAD);                       //COPY SCRATCHPAD
    //dsDelay();
    
    ow_init();                                 //������������� ���� 1-wire 	  
    ow_write(SKIP_ROM);                        //SKIP ROM
    ow_write(CONVERT_T);                       //Convert T

    return 0;
}

void dsDelay(void){
    volatile uint32_t   cnt;
    for(cnt = 0; cnt < 0xFFFF; cnt++){
        cnt++;
        cnt--;
    }
}

//===========================================================================
// tmprReg1 - MS BYTE Temperature Register
// tmprReg0 - LS BYTE Temperature Register
//===========================================================================
void tmprDecode(uint8_t tmprReg1, uint8_t tmprReg0){
    if(tmprReg1 & 0x80){
        tem.sign = 1;
        tmprReg1 = ~ tmprReg1;
        tmprReg0 = ~ tmprReg0;

        if(tmprReg0 == 0xFF){
            tmprReg0 = 0;
            tmprReg1++;
        }else{
            tmprReg0++;
        }

    }else{
        tem.sign = 0;
    }

    //================= �������� ����� ����� ===========
    tem.integer = ((tmprReg1 & 0x07) << 4) | ((tmprReg0 & 0xf0) >> 4);
    //===================================================

    //================= �������� ������� ����� ===========
    tem.frac = ((tmprReg0 & 0x0F) * 10) / 16;
    //===================================================
}
//===========================================================================

//===========================================================================
// tmprReg - Temperature Register
//===========================================================================
void reg2tmpr(uint16_t tmprReg){
    uint8_t     *ptr;
    ptr = (uint8_t*)&tmprReg;

    if(tmprReg & 0x8000){
        tem.sign = 1;
        tmprReg = ~tmprReg + 1;
    }else{
        tem.sign = 0;
    }
    tem.integer = ((ptr[1] & 0x0F) << 4) | (ptr[0] >> 4);
    tem.frac = (ptr[0] & 0x0F) * 10 / 16;
}
//===========================================================================

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
