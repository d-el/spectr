/******************************************************************************
* @file    prmTypes.h
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    10.08.2015
*/
#ifndef	prmTypes_H
#define prmTypes_H

#include "prmTypes.h"
#include "ModbusTypes.h"

/******************************************************************************
* User typedef
*/
typedef enum{
    NotBase,
    X_X_Hz,
    XX_V,
    X_X_A,
    XX_Wt,
    X_X_PerCent
}nameBase_type;

typedef enum{
    WriteTypeNone,
    WriteTypeAlways,
    WriteTypeStop
}WriteType_type;

typedef struct{
  u16     f1  :1;
  u16     f2  :1;
  u16     f3  :1;
}GRPflag_type;

typedef const struct{
    void                *dataPtr        ;           // указатель на данные
    u16                 minVal          ;           // Минимальное  значение для параметра
    u16                 maxVal          ;           // Максимальное значение для параметра
    u16                 defaultVal      ;           // Значение по умолчанию
	const void 			*nvSavePtr		;

    union{
        struct{
          PrmFrmt_type   TypeVar    :5  ;           // тип данных
          WriteType_type WriteType  :2  ;           // возможность записи
          u16            NumShort   :2  ;           // Количество слов по 2 байта
		  u16            nvSave     :1  ;           // флаг записи во флеш
          nameBase_type  nameBase   :4  ;           // Номер базы
        }bit                            ;
     u16                all             ;
    }flags                     ;// Флаги меню
    void                (*funcPtr)(void *, s16 Rd); // Указатель на ф-ю
}PrmOpis_type;

typedef const struct{
    const PrmOpis_type      *pntOpisPrm;
    u16                     QPrmInGrp  ;
    GRPflag_type            flag;
}OpisGrp_type;

/******************************************************************************
* Extern viriables
*/
extern PrmOpis_type     PrmItem1[];
extern const u8         NumOpisGrp;
extern const u16        BaseMas[16];

#endif  //prmTypes_H
