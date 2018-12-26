
#ifndef MENU_B_H
#define MENU_B_H

#define MAX_NUM_UST             511

//===================================================================
    /* М1 параметры которые зависят от степени отображения      */
    /* М2 параметры которые НЕ зависят от степени отображения   */
//===================================================================
#define M1_sec2Frmt                0       /*    старшие два байта 4 байтного параметра    2pice   */
#define M1_NtxtFrmt                0       /*    номер текста                              1 s16   */
#define M1_Hex16Frmt               1       /*    как int16Frmt                             2 s16   */
#define M1_int16Frmt               1       /*    знаковое числовое значение 16 бит         2 s16   */
#define M1_HourMinFrmt             0       /*    время:минуты, часы                        2 s16   */
#define M1_MinSecFrmt              0       /*    время:минуты, часы                        2 s16   */
#define M1_floatFrmt               0       /*    числовое значение в формате float         4 float */
#define M1_int32Frmt               1       /*    знаковое числовое значение 32 бит         4 s32   */
#define M1_HmsFrmt                 0       /*    часы:минуты:секунды                       4 s32   */
#define M1_UTimeFrmt               0       /*    линейное время по Unix                    4 s32   */
#define M1_MonDayFrmt              0       /*     дата: день:месяц                         2 s16   */
#define M1_uns16Frmt               1       /*     беззнаковое числовое значение 16 бит     2 s16   */
#define M1_uns32Frmt               1       /*     беззнаковое числовое значение 32 бит     4 u32   */
#define M1_Hex32Frmt               1

#define M2_sec2Frmt                1       /*    старшие два байта 4 байтного параметра    2pice   */
#define M2_NtxtFrmt                1       /*    номер текста                              1 s16   */
#define M2_Hex16Frmt               0       /*    как int16Frmt                             2 s16   */
#define M2_int16Frmt               0       /*    знаковое числовое значение 16 бит         2 s16   */
#define M2_HourMinFrmt             1       /*    время:минуты, часы                        2 s16   */
#define M2_MinSecFrmt              1       /*    время:минуты, часы                        2 s16   */
#define M2_floatFrmt               1       /*    числовое значение в формате float         4 float */
#define M2_int32Frmt               0       /*    знаковое числовое значение 32 бит         4 s32   */
#define M2_HmsFrmt                 1       /*    часы:минуты:секунды                       4 s32   */
#define M2_UTimeFrmt               1       /*    линейное время по Unix                    4 s32   */
#define M2_MonDayFrmt              1       /*     дата: день:месяц                         2 s16   */
#define M2_uns16Frmt               0       /*     беззнаковое числовое значение 16 бит     2 s16   */
#define M2_uns32Frmt               0       /*     беззнаковое числовое значение 32 бит     4 u32   */
#define M2_Hex32Frmt               0
//===================================================================
//                Типы приведения типа параметра
//===================================================================
#define typesec2Frmt                u16     /*    старшие два байта 4 байтного параметра    2pice   */
#define typeNtxtFrmt                u16     /*    номер текста                              1 s16   */
#define typeHex16Frmt               u16     /*    как int16Frmt                             2 s16   */
#define typeint16Frmt               s16     /*    знаковое числовое значение 16 бит         2 s16   */
#define typeHourMinFrmt             u16     /*    время:минуты, часы                        2 s16   */
#define typeMinSecFrmt              u16     /*    время:минуты, часы                        2 s16   */
#define typefloatFrmt               float   /*    числовое значение в формате float         4 float */
#define typeint32Frmt               s32     /*    знаковое числовое значение 32 бит         4 s32   */
#define typeHmsFrmt                 u32     /*    часы:минуты:секунды                       4 s32   */
#define typeUTimeFrmt               u32     /*    линейное время по Unix                    4 s32   */
#define typeMonDayFrmt              u16     /*     дата: день:месяц                         2 s16   */
#define typeuns16Frmt               u16     /*     беззнаковое числовое значение 16 бит     2 s16   */
#define typeuns32Frmt               u32     /*     беззнаковое числовое значение 32 бит     4 u32   */
#define typeHex32Frmt               u32     /*     беззнаковое числовое значение 32 бит     4 u32   */
//===================================================================
#define szsec2Frmt                sizeof(u16)     /*    старшие два байта 4 байтного параметра    2pice   */
#define szNtxtFrmt                sizeof(u16)     /*    номер текста                              1 s16   */
#define szHex16Frmt               sizeof(u16)     /*    как int16Frmt                             2 s16   */
#define szint16Frmt               sizeof(s16)     /*    знаковое числовое значение 16 бит         2 s16   */
#define szHourMinFrmt             sizeof(u16)     /*    время:минуты, часы                        2 s16   */
#define szMinSecFrmt              sizeof(u16)     /*    время:минуты, часы                        2 s16   */
#define szfloatFrmt               sizeof(float)   /*    числовое значение в формате float         4 float */
#define szint32Frmt               sizeof(s32)     /*    знаковое числовое значение 32 бит         4 s32   */
#define szHmsFrmt                 sizeof(u32)     /*    часы:минуты:секунды                       4 s32   */
#define szUTimeFrmt               sizeof(u32)     /*    линейное время по Unix                    4 s32   */
#define szMonDayFrmt              sizeof(u16)     /*     дата: день:месяц                         2 s16   */
#define szuns16Frmt               sizeof(u16)     /*     беззнаковое числовое значение 16 бит     2 s16   */
#define szuns32Frmt               sizeof(u32)     /*     беззнаковое числовое значение 32 бит     4 u32   */
#define szHex32Frmt               sizeof(u32)     /*     беззнаковое числовое значение 32 бит     4 u32   */
//===================================================================

enum{
    nvNotSave,
    nvSave
};

//===========================================================================
// Объявление типа со структурой данных и распределение памяти для уставок и
// измеренных параметров которые выводятся в меню на дисплее.
//===========================================================================
#ifndef MENU_STRUCT_DEF
#define MENU_STRUCT_DEF


//#def menuPrmMacro
#include "DefMenuPrm.h"

//===========================================================================
#undef menuPrmMacro
#undef Sec_PrmMacro

typedef struct{
 	u16 CRCFlash;
}prm_Save_type;

typedef struct{
    u32 mem[MAX_NUM_UST];
    u16 signatura;
}nvMem_type;

#include "UnDefMenuPrm.h"

#undef nvNotSavemenuPrmMacro
#undef nvSavemenuPrmMacro


#undef menuPrmMacro
#undef Sec_PrmMacro


#endif // MENU_STRUCT_DEF

//===================================================================
//                  Декларация переменных
//===================================================================
extern const    PrmOpis_type    PrmItem0[];
extern const    OpisGrp_type    OpisGrp[];
extern const    prm_Save_type   ustavkiInFlash;
extern const    nvMem_type      nvMemDef;

//===================================================================
//===================================================================
#endif //MENU_B_H
