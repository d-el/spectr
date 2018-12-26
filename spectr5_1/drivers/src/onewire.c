/**
*******************************************************************************
* @file     onewire.c
* @author   Storozhenko Roman
* @version  V1.2
* @date     10.05.2015
* @brief    Функции инициализации, записи/чтения по 1-wire
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
* @brief   Инициализация шины 1-WIRE
* @param    None
* @retval   owSt_type
*/
owSt_type ow_init(void){
    uint16_t result;
    if(ow_dq == 0){
        return owShortCircle;       //Проверить линию на отсутствие замыкания
    }
    low_ow_dq;                      //Потянуть шину к земле
    __delay_us(500);                //Ждать 480..960 мкс
    hi_ow_dq;                       //Отпускаем шину
    __delay_us(60);                 //Ждать 60 микросекунд
    result = ow_dq;                 //Прочитать шину
    __delay_us(450);                //Дождаться окончания инициализации
    if(result != 0){
        return owNotFound;          //Датчик не обнаружен
    }
    return owOk;                    //Датчик обнаружен
}

/*****************************************************************************
* @brief    Передача бита
* @param    Bit: 0, 1
*/
inline void ow_write_bit(uint8_t bit){
    low_ow_dq; //Потянуть шину к земле
    if (bit != 0){
        __delay_us(5);              //Низкий импульс, от 1 до 15 мкс (с учётом времени восстановления уровня)
        hi_ow_dq;                   
        delay_us(90);               //Ожидание до завершения таймслота (не менее 60 мкс)
    }else{                        
        __delay_us(90);             //Низкий уровень на весь таймслот (не менее 60 мкс, не более 120 мкс)
        hi_ow_dq;                   
        __delay_us(5);              //Время восстановления высокого уровеня на шине + 1 мкс (минимум)
    }
}

/*****************************************************************************
* @brief  Чтение бита
* @param  None
* @retval Bit: 0, 1
*/
inline uint8_t ow_read_bit(void){
    uint16_t val;
    low_ow_dq;                      //Потянуть шину к земле
    __delay_us(2);                  //Длительность низкого уровня, минимум 1 мкс
    hi_ow_dq;                       //Отпускаем шину
    __delay_us(8);                  //Пауза до момента сэмплирования, всего не более 15 мкс
    val = ow_dq;                    //Прочитать шину
    __delay_us(55);                 //Ожидание до следующего тайм-слота, минимум 60 мкс с начала низкого уровня
    if(val != 0)    return 1;
    return 0;
}

/*****************************************************************************
* @brief  Запись байта
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
* @brief  Чтение байта
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
* @brief  Подсчет CRC-8-Dallas/Maxim
* @param  Указатель на массив, колличество элементов
* @retval Result: the value of the received data
*/
uint8_t crc8(uint8_t *mas, uint8_t n){
    uint8_t j , i, tmp, data, crc = 0;
    
    for(i = 0; i < n; i++){
        data = *mas;
        for(j = 0; j < 8; j++){                 //Вычисление CRC - обрабатываем каждый бит принятого байта
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
* @brief  Получает очередной адрес, кладет адрес по указателю
          При поиске первого адреса d параметр searchInit нужно передать 0
* @param  pprev:        указатель на массив приемник адреса
* @param  searchInit:   0 - инициализация и поиск первого адреса
                        !0 - поиск следующего адреса
* @retval статус
*/
uint8_t ow_enum[8]; //Найденный восьмибайтовый адрес
uint8_t ow_enum_fork_bit = 65; //Последний нулевой бит, где была неоднозначность (нумеруя с единицы)

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
        ow_enum_fork_bit = 65;                  //Последний нулевой бит, где была неоднозначность (нумеруя с единицы)
    }
    if(ow_enum_fork_bit == 0){                  //Если на предыдущем шаге уже не было разногласий
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
        if(not0 == 0){                                  //Если присутствует в адресах бит ноль
            if(not1 == 0){                              //Но также присустствует бит 1 (вилка)
                if(p < ow_enum_fork_bit){               //Если мы левее прошлого правого конфликтного бита,
                    if((prev & 1) != 0){
                        next |= 0x80;                   //То копируем значение бита из прошлого прохода
                    }
                    else{
                        newfork = p;                    //Если ноль, то запомним конфликтное место
                    }
                }
                else if(p == ow_enum_fork_bit){
                    next |= 0x80;                       //Если на этом месте в прошлый раз был правый конфликт с нулём, выведем 1
                }
                else{
                    newfork = p;                        //Правее - передаём ноль и запоминаем конфликтное место
                }
            }                                           //В противном случае идём, выбирая ноль в адресе
        }
        else{
            if(not1 == 0){                              //Присутствует единица
                next |= 0x80;
            }
            else{                                       //Нет ни нулей ни единиц - ошибочная ситуация
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
