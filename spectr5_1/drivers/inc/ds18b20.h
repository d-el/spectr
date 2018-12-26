/**
******************************************************************************
* @file       ds18b20.h
* @author     Storozhenko Roman + many people
* @version    V2.1
* @date       30.03.2014
* @brief      
******************************************************************************  
*/
#ifndef DS18B20_H
#define DS18B20_H

//ROM COMMANDS Definition
#define SEARCH_ROM          0xF0
#define MATCH_ROM           0x55
#define READ_SCRATCHPAD     0xBE
#define READ_ROM            0x33
#define SKIP_ROM            0xCC
#define ALARM_SEARCH        0xEC
#define CONVERT_T           0x44
#define WRITE_SCRATCHPAD    0x4E
#define COPY_SCRATCHPAD     0x48
#define RECALL_E2           0xB8
#define READ_POWER_SUPPLY   0xB4

typedef struct{ //Структура для сохранения температуры
    uint8_t     integer;
    uint8_t     frac;
    uint8_t     result;                 //Тут находятся коды ошибок, если они есть
    uint8_t     sign    :1;
    uint8_t     update  :1;
}tmpr_type;

extern uint8_t      ow_rom[8][3];
extern volatile     tmpr_type   tem;

//-----------------------------------------------------------
//Prototypes for the functions in ds18b20.c
//-----------------------------------------------------------
uint8_t Init_DS18B20(void);
void dsDelay(void);
void tmprDecode(uint8_t tmprReg1, uint8_t tmprReg0);


#endif //DS18B20_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
