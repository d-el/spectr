/**
******************************************************************************
* @file         flash.h
* @author
* @version      V2.1
* @date         07-02-2015
* @brief        ������� ��� ������ � FLASH �������
                �������� ������ �� ���������� � �� FLASH �������
******************************************************************************
**/
#ifndef FLASH_H
#define FLASH_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
#define FLASH_KEY1          ((uint32_t)0x45670123)
#define FLASH_KEY2          ((uint32_t)0xCDEF89AB)

#define flashPage1          126
#define flashPage2          127
#define nvMemBaseAdr(page)  ((void*)(FLASH_BASE +1024 * page))
#define nvMemSignature      0x2210

/******************************************************************************
* User enum
*/

/******************************************************************************
* User typedef
*/
typedef enum{
    nvMem_ok,
    nvMem_signatureError,
    nvMem_CRCError,
    nvMem_error
}nvMem_state_type;

typedef struct{
    void        *dataPtr;
    uint32_t    sizeofData;
}nvMem_memreg_type;

typedef union{
    struct{
        uint16_t    prepareForWrite :1;
    }bit;
    uint16_t    all;
}nvMem_flags_type;

typedef struct{
    uint16_t            numPrm;
    nvMem_memreg_type   *memreg;
    void                *nvMemBase;
    void                *nvMemBaseResrvCopy;
    nvMem_flags_type    flags;
    uint32_t            fullSize;
    uint32_t            saveCnt;
}nvMem_struct_type;

/******************************************************************************
* Extern viriables
*/
extern nvMem_struct_type   nvMem;

/******************************************************************************
* Macro functions
*/
#define flash_unlock()          \
    FLASH->KEYR = FLASH_KEY1;   \
    FLASH->KEYR = FLASH_KEY2;

#define flash_lock()            \
    FLASH->CR |= FLASH_CR_LOCK;

#define flash_busy()            \
    (FLASH->SR & FLASH_SR_BSY)

/******************************************************************************
* Prototypes for the functions
*/
void flash_erase_all_pages(void);
void flash_erase_page(void *address);
nvMem_state_type nvMem_init(void);
nvMem_state_type nvMem_savePrm(void *adrNvMem);
nvMem_state_type nvMem_loadPrm(void *adrNvMem);
nvMem_state_type nvMem_prepareMemory(void *adrNvMem);


#endif //FLASH_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
