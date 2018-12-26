#ifndef TYPES_H
#define TYPES_H

typedef enum PrmFrmt{
    sec2Frmt            ,//0   // старшие два байта 4 байтного параметра  2pice
    NtxtFrmt            ,//1   // номер текста                    1 s16
    Hex16Frmt           ,//2   // как int16Frmt                   2 s16
    int16Frmt           ,//3   // знаковое числовое значение 16 бит     2 s16
    HourMinFrmt         ,//4   // время:минуты, часы            2 s16
    MinSecFrmt          ,//5   // время:минуты, часы            2 s16
    floatFrmt           ,//6   // числовое значение в формате float     4 float
    int32Frmt           ,//7   // знаковое числовое значение 32 бит     4 s32
    HmsFrmt             ,//8   // часы:минуты:секунды               4 s32
    UTimeFrmt           ,//9   // линейное время по Unix          4 s32
    refFormat           ,//10  // ссылка:группа, параметр                   2 s16
    MonDayFrmt          ,//11  // дата: день:месяц               2 s16
    uns16Frmt           ,//12  // беззнаковое числовое значение 16 бит     2 s16
    uns32Frmt           ,//13  // беззнаковое числовое значение 32 бит     4 u32
    Hex32Frmt           ,//14  // беззнаковое числовое значение 32 бит     4 u32
    kolvFormat          ,//15  // количество форматов
}PrmFrmt_type;

//Коды ошибок в приводе
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
      ErrPrmBusy        ,       // 12  Параметр еще в обработке
      ErrQueueFull      ,       // 13  Очередь от драйвера забита

      ErrDebug = 99     ,       // 99  Очередь от драйвера забита
}Drv_err_type;

//Структура запроса на уровень драйвера
typedef struct{
    u8                  AdrNet;         // Сетевой адрес запроса
    u8                  Func;           // Функция
    u8                  Sz;             // Объем запрашиваемых данных
    u8                  NumParam;       // Номер параметра
    u32                 AdrData;        // Адрес данных
    u16                 Param;          // Дополнительные параметры
    u16                 SzBuf;          // Размер буфера
    u8                  *Buf;           // Указатель на буфер обмена
    Drv_err_type        Err;            // Ошибка связи
    u8                  cntRepeat;      // Количество повторов запроса
    struct{
        u8              CleanQueue  :1;
    };
    u32                 countByteWrite; // Количество данных для передачи по порту
    u32                 countByteRead;  // Ожидаемое количество данных в ответе от привода
    u32                 Busy;
}MsgReqDrvIn_type;

struct wmStruct;
typedef int (*pFuncMosAf_typedef)(struct wmStruct *,s16);   //указатель на функцию

#endif
