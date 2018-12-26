
#ifndef MENU_B_H
#define MENU_B_H

#define MAX_NUM_UST             511

//===================================================================
    /* �1 ��������� ������� ������� �� ������� �����������      */
    /* �2 ��������� ������� �� ������� �� ������� �����������   */
//===================================================================
#define M1_sec2Frmt                0       /*    ������� ��� ����� 4 �������� ���������    2pice   */
#define M1_NtxtFrmt                0       /*    ����� ������                              1 s16   */
#define M1_Hex16Frmt               1       /*    ��� int16Frmt                             2 s16   */
#define M1_int16Frmt               1       /*    �������� �������� �������� 16 ���         2 s16   */
#define M1_HourMinFrmt             0       /*    �����:������, ����                        2 s16   */
#define M1_MinSecFrmt              0       /*    �����:������, ����                        2 s16   */
#define M1_floatFrmt               0       /*    �������� �������� � ������� float         4 float */
#define M1_int32Frmt               1       /*    �������� �������� �������� 32 ���         4 s32   */
#define M1_HmsFrmt                 0       /*    ����:������:�������                       4 s32   */
#define M1_UTimeFrmt               0       /*    �������� ����� �� Unix                    4 s32   */
#define M1_MonDayFrmt              0       /*     ����: ����:�����                         2 s16   */
#define M1_uns16Frmt               1       /*     ����������� �������� �������� 16 ���     2 s16   */
#define M1_uns32Frmt               1       /*     ����������� �������� �������� 32 ���     4 u32   */
#define M1_Hex32Frmt               1

#define M2_sec2Frmt                1       /*    ������� ��� ����� 4 �������� ���������    2pice   */
#define M2_NtxtFrmt                1       /*    ����� ������                              1 s16   */
#define M2_Hex16Frmt               0       /*    ��� int16Frmt                             2 s16   */
#define M2_int16Frmt               0       /*    �������� �������� �������� 16 ���         2 s16   */
#define M2_HourMinFrmt             1       /*    �����:������, ����                        2 s16   */
#define M2_MinSecFrmt              1       /*    �����:������, ����                        2 s16   */
#define M2_floatFrmt               1       /*    �������� �������� � ������� float         4 float */
#define M2_int32Frmt               0       /*    �������� �������� �������� 32 ���         4 s32   */
#define M2_HmsFrmt                 1       /*    ����:������:�������                       4 s32   */
#define M2_UTimeFrmt               1       /*    �������� ����� �� Unix                    4 s32   */
#define M2_MonDayFrmt              1       /*     ����: ����:�����                         2 s16   */
#define M2_uns16Frmt               0       /*     ����������� �������� �������� 16 ���     2 s16   */
#define M2_uns32Frmt               0       /*     ����������� �������� �������� 32 ���     4 u32   */
#define M2_Hex32Frmt               0
//===================================================================
//                ���� ���������� ���� ���������
//===================================================================
#define typesec2Frmt                u16     /*    ������� ��� ����� 4 �������� ���������    2pice   */
#define typeNtxtFrmt                u16     /*    ����� ������                              1 s16   */
#define typeHex16Frmt               u16     /*    ��� int16Frmt                             2 s16   */
#define typeint16Frmt               s16     /*    �������� �������� �������� 16 ���         2 s16   */
#define typeHourMinFrmt             u16     /*    �����:������, ����                        2 s16   */
#define typeMinSecFrmt              u16     /*    �����:������, ����                        2 s16   */
#define typefloatFrmt               float   /*    �������� �������� � ������� float         4 float */
#define typeint32Frmt               s32     /*    �������� �������� �������� 32 ���         4 s32   */
#define typeHmsFrmt                 u32     /*    ����:������:�������                       4 s32   */
#define typeUTimeFrmt               u32     /*    �������� ����� �� Unix                    4 s32   */
#define typeMonDayFrmt              u16     /*     ����: ����:�����                         2 s16   */
#define typeuns16Frmt               u16     /*     ����������� �������� �������� 16 ���     2 s16   */
#define typeuns32Frmt               u32     /*     ����������� �������� �������� 32 ���     4 u32   */
#define typeHex32Frmt               u32     /*     ����������� �������� �������� 32 ���     4 u32   */
//===================================================================
#define szsec2Frmt                sizeof(u16)     /*    ������� ��� ����� 4 �������� ���������    2pice   */
#define szNtxtFrmt                sizeof(u16)     /*    ����� ������                              1 s16   */
#define szHex16Frmt               sizeof(u16)     /*    ��� int16Frmt                             2 s16   */
#define szint16Frmt               sizeof(s16)     /*    �������� �������� �������� 16 ���         2 s16   */
#define szHourMinFrmt             sizeof(u16)     /*    �����:������, ����                        2 s16   */
#define szMinSecFrmt              sizeof(u16)     /*    �����:������, ����                        2 s16   */
#define szfloatFrmt               sizeof(float)   /*    �������� �������� � ������� float         4 float */
#define szint32Frmt               sizeof(s32)     /*    �������� �������� �������� 32 ���         4 s32   */
#define szHmsFrmt                 sizeof(u32)     /*    ����:������:�������                       4 s32   */
#define szUTimeFrmt               sizeof(u32)     /*    �������� ����� �� Unix                    4 s32   */
#define szMonDayFrmt              sizeof(u16)     /*     ����: ����:�����                         2 s16   */
#define szuns16Frmt               sizeof(u16)     /*     ����������� �������� �������� 16 ���     2 s16   */
#define szuns32Frmt               sizeof(u32)     /*     ����������� �������� �������� 32 ���     4 u32   */
#define szHex32Frmt               sizeof(u32)     /*     ����������� �������� �������� 32 ���     4 u32   */
//===================================================================

enum{
    nvNotSave,
    nvSave
};

//===========================================================================
// ���������� ���� �� ���������� ������ � ������������� ������ ��� ������� �
// ���������� ���������� ������� ��������� � ���� �� �������.
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
//                  ���������� ����������
//===================================================================
extern const    PrmOpis_type    PrmItem0[];
extern const    OpisGrp_type    OpisGrp[];
extern const    prm_Save_type   ustavkiInFlash;
extern const    nvMem_type      nvMemDef;

//===================================================================
//===================================================================
#endif //MENU_B_H
