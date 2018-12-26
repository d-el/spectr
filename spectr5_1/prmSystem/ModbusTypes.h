#ifndef TYPES_H
#define TYPES_H

typedef enum PrmFrmt{
    sec2Frmt            ,//0   // ������� ��� ����� 4 �������� ���������  2pice
    NtxtFrmt            ,//1   // ����� ������                    1 s16
    Hex16Frmt           ,//2   // ��� int16Frmt                   2 s16
    int16Frmt           ,//3   // �������� �������� �������� 16 ���     2 s16
    HourMinFrmt         ,//4   // �����:������, ����            2 s16
    MinSecFrmt          ,//5   // �����:������, ����            2 s16
    floatFrmt           ,//6   // �������� �������� � ������� float     4 float
    int32Frmt           ,//7   // �������� �������� �������� 32 ���     4 s32
    HmsFrmt             ,//8   // ����:������:�������               4 s32
    UTimeFrmt           ,//9   // �������� ����� �� Unix          4 s32
    refFormat           ,//10  // ������:������, ��������                   2 s16
    MonDayFrmt          ,//11  // ����: ����:�����               2 s16
    uns16Frmt           ,//12  // ����������� �������� �������� 16 ���     2 s16
    uns32Frmt           ,//13  // ����������� �������� �������� 32 ���     4 u32
    Hex32Frmt           ,//14  // ����������� �������� �������� 32 ���     4 u32
    kolvFormat          ,//15  // ���������� ��������
}PrmFrmt_type;

//���� ������ � �������
typedef enum {
      Ok_err            ,       // 0
      IllegalFunc_err   ,       // 1
      BadAdr_err        ,       // 2
      Lim_err           ,       // 3
      NonRdk_err        ,       // 4

      SysPrmInitBad_err ,       // 5
      ErrPostMB         ,       // 6
      ErrPendMB         ,       // 7
      ErrSizeQ          ,       // 8
      ErrSzMB           ,       // 9
      ErrKsMB           ,       // 10
      ErrReqMB          ,       // 11
      ErrPrmBusy        ,       // 12  �������� ��� � ���������
      ErrQueueFull      ,       // 13  ������� �� �������� ������

      ErrDebug = 99     ,       // 99  ������� �� �������� ������
}Drv_err_type;

//��������� ������� �� ������� ��������
typedef struct{
    u8                  AdrNet;         // ������� ����� �������
    u8                  Func;           // �������
    u8                  Sz;             // ����� ������������� ������
    u8                  NumParam;       // ����� ���������
    u32                 AdrData;        // ����� ������
    u16                 Param;          // �������������� ���������
    u16                 SzBuf;          // ������ ������
    u8                  *Buf;           // ��������� �� ����� ������
    Drv_err_type        Err;            // ������ �����
    u8                  cntRepeat;      // ���������� �������� �������
    struct{
        u8              CleanQueue  :1;
    };
    u32                 countByteWrite; // ���������� ������ ��� �������� �� �����
    u32                 countByteRead;  // ��������� ���������� ������ � ������ �� �������
    u32                 Busy;
}MsgReqDrvIn_type;

struct wmStruct;
typedef int (*pFuncMosAf_typedef)(struct wmStruct *,s16);   //��������� �� �������

#endif
