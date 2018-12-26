#ifndef MusCB_prog_NVRAM_h
#define MusCB_prog_NVRAM_h
 
 
 typedef struct {
/* 0x0000*/    s32   UST1;  //Сетевой адрес
/* 0x0002*/    s32   UST2;  //UDAC0
/* 0x0004*/    s32   UST3;  //IDAC0
/* 0x0006*/    s32   UST4;  //MODE0
/* 0x0008*/    s32   UST5;  //USTs16
/* 0x000A*/    s32   UST6;  //Д1
/* 0x000C*/    s32   UST7;  //N тик логера
/* 0x000E*/    s32   UST8;  //Тип логера
/* 0x0010*/    s32   UST9;  //N колич.перем Л
/* 0x0012*/    s32   UST10;  //№ переменной 1
/* 0x0014*/    s32   UST11;  //№ переменной 2
/* 0x0016*/    s32   UST12;  //№ переменной 3
/* 0x0018*/    s32   UST13;  //№ переменной 4
/* 0x001A*/    s32   UST14;  //№ переменной 5
/* 0x001C*/    s32   UST15;  //№ переменной 6
/* 0x001E*/    s32   UST16;  //№ переменной 7
/* 0x0020*/    s32   UST17;  //№ переменной 8
/* 0x0022*/    s32   UST18;  //№ переменной 9
/* 0x0024*/    s32   UST19;  //Лог время записи
/* 0x0026*/    s32   UST20;  //IMAX0
/* 0x0028*/    s32   USTD21;  //USTs32
/* 0x002A*/    s32   USTD21_Sec;
/* 0x002C*/    s32   USTD22;  //USTfloat
/* 0x002E*/    s32   USTD22_Sec;
//-------------------- 
 } AllNV_Type;
// 48
#endif //MusCB_prog_NVRAM_h
