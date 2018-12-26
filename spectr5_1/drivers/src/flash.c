/**
******************************************************************************
* @file         flash.c
* @author
* @version      V2.1
* @date         07-02-2015
* @brief        Функции для работы с FLASH памятью
*               Средства работы со встроенной в мк FLASH памятью
**/
#include "globalIncludes.h"

/******************************************************************************
* Memory
*/
uint8_t nvMemTmpBff[210];
nvMem_memreg_type   nvMem_memreg[] = {
    { &sysset,      sizeof(sysset)      },
    { &bs,          sizeof(bs)          },
    { &ch,          sizeof(ch)          },
};

nvMem_struct_type   nvMem = {
    sizeof(nvMem_memreg) / sizeof(nvMem_memreg_type),
    nvMem_memreg,
    nvMemBaseAdr(flashPage1),
    nvMemBaseAdr(flashPage2),
};

/******************************************************************************
* Функция стирает ВСЕ страницы. При её вызове прошивка самоуничтожается
*/
void flash_erase_all_pages(void)
{
    FLASH->CR |= FLASH_CR_MER;                      //Устанавливаем бит стирания ВСЕХ страниц
    FLASH->CR |= FLASH_CR_STRT;                     //Начать стирание
    while(flash_busy());                            //Ожидание готовности.. Хотя оно уже наверное ни к чему здесь...
    FLASH->CR &= FLASH_CR_MER;
}

/******************************************************************************
* Функция стирает одну страницу. В качестве адреса можно использовать любой
* принадлежащий диапазону адресов той странице которую нужно очистить
*/
void flash_erase_page(void *address)
{
    FLASH->CR|= FLASH_CR_PER;                       //Устанавливаем бит стирания одной страницы
    FLASH->AR = (uint32_t)address;                  //Задаем её адрес
    FLASH->CR|= FLASH_CR_STRT;                      //Запускаем стирание
    while(flash_busy());                            //Ждем пока страница сотрется
    FLASH->CR&= ~FLASH_CR_PER;                      //Сбрасываем бит обратно
}

/******************************************************************************
*
*/
nvMem_state_type flash_write2(void *dst, void *src, uint32_t num)
{
    uint16_t    *pRd;
    uint16_t    *pWr;
    uint16_t    *pEnd;

    flash_unlock();

    FLASH->CR |= FLASH_CR_PG;                       //Разрешаем программирование флеша
    while(flash_busy());                            //Ожидаем готовности флеша к записи

    pRd     = src;
    pWr     = dst;
    pEnd    = pWr + num;
    
    while(pWr < pEnd){
        *pWr++ = *pRd++;
        while(flash_busy());                        //Ожидаем готовности флеша
    }

    FLASH->CR &= ~FLASH_CR_PG;                      //Запрещаем программирование флеша
    flash_lock();
    return nvMem_ok;
}

/******************************************************************************
*
*/
nvMem_state_type nvMem_init(void)
{
    nvMem.fullSize  = 0;
    
    for(uint16_t i = 0; i < nvMem.numPrm; i++){
        nvMem.fullSize += nvMem.memreg[i].sizeofData;
    }
    nvMem.fullSize += sizeof(nvMem.saveCnt);
    nvMem.fullSize += sizeof(uint16_t);     //signature
    nvMem.fullSize += sizeof(uint16_t);     //CRC
    if(nvMem.fullSize > sizeof(nvMemTmpBff)){
        while(1)   __NOP();
    }
    return nvMem_ok;
}

/******************************************************************************
*
*/
nvMem_state_type nvMem_savePrm(void *adrNvMem)
{
    uint8_t     *ptr;
    
    ptr   = nvMemTmpBff;
    
    //Проверить флаг подготовленности памяти
    if(nvMem.flags.bit.prepareForWrite == 0){
        nvMem_prepareMemory(adrNvMem);
    }

    //Увеличить счетчик сохранений
    nvMem.saveCnt++;

    //Соединяем все в один массив
    //Signature
    *(uint16_t*)ptr = nvMemSignature;
    ptr += sizeof(uint16_t);
    //saveCnt
    *(uint16_t*)ptr = nvMem.saveCnt;
    ptr += sizeof(nvMem.saveCnt);
    //memreg
    for(uint16_t i = 0; i < nvMem.numPrm; i++){
        memcpy(ptr, nvMem.memreg[i].dataPtr, nvMem.memreg[i].sizeofData);
        ptr += nvMem.memreg[i].sizeofData;
    }
    //CRC
    *(uint16_t*)ptr = GetCrc(nvMemTmpBff, ptr - nvMemTmpBff);
    ptr += sizeof(uint16_t);
    
    //Сохранить
    flash_write2(adrNvMem, nvMemTmpBff, ptr - nvMemTmpBff);
    
    //Сбросить флаг подготовленности памяти
    nvMem.flags.bit.prepareForWrite = 0;
    
    return nvMem_ok;
}

/******************************************************************************
* 
*/
nvMem_state_type nvMem_loadPrm(void *adrNvMem)
{
    uint8_t     *ptr;
    uint16_t    l_crc, l_signature;
    
    ptr   = nvMemTmpBff;
    
    //Скопировать память
    memcpy(nvMemTmpBff,  adrNvMem, nvMem.fullSize);
    
    //Проверить CRC
    l_crc = GetCrc(nvMemTmpBff, nvMem.fullSize);
    
    if(l_crc == 0){ //CRC OK
        l_signature = *(uint16_t*)ptr;
        if(l_signature != nvMemSignature){  //signature error
            return nvMem_signatureError;
        }
        ptr += sizeof(l_signature);
        nvMem.saveCnt = *(uint16_t*)ptr;
        ptr += sizeof(nvMem.saveCnt);
        
        for(uint16_t i = 0; i < nvMem.numPrm; i++){
            memcpy(nvMem.memreg[i].dataPtr, ptr, nvMem.memreg[i].sizeofData);
            ptr += nvMem.memreg[i].sizeofData;
        }
        
        return nvMem_ok;
    }
    else{   //CRC ERROR
        return nvMem_CRCError;
    }
}

/******************************************************************************
*
*/
nvMem_state_type nvMem_prepareMemory(void *adrNvMem)
{
    //Стереть страницу
    flash_unlock();
    flash_erase_page(adrNvMem);
    flash_lock();
    
    //Поставить флаг подготовленности
    nvMem.flags.bit.prepareForWrite = 1;
    
    return nvMem_ok;
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
