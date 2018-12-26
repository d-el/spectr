#ifndef MusCB_prog_NVRAM_h
#define MusCB_prog_NVRAM_h
 
 
 typedef struct {
/* 0x0000*/    s32   UST1;  //Сетевой адрес
/* 0x0004*/    s32   UST2;  //UDAC0
/* 0x0008*/    s32   UST3;  //IDAC0
/* 0x000C*/    s32   UST4;  //MODE0
/* 0x0010*/    s32   UST5;  //USTs16
/* 0x0014*/    s32   UST6;  //Д1
/* 0x0018*/    s32   UST7;  //N тик логера
/* 0x001C*/    s32   UST8;  //Тип логера
/* 0x0020*/    s32   UST9;  //N колич.перем Л
/* 0x0024*/    s32   UST10;  //№ переменной 1
/* 0x0028*/    s32   UST11;  //№ переменной 2
/* 0x002C*/    s32   UST12;  //№ переменной 3
/* 0x0030*/    s32   UST13;  //№ переменной 4
/* 0x0034*/    s32   UST14;  //№ переменной 5
/* 0x0038*/    s32   UST15;  //№ переменной 6
/* 0x003C*/    s32   UST16;  //№ переменной 7
/* 0x0040*/    s32   UST17;  //№ переменной 8
/* 0x0044*/    s32   UST18;  //№ переменной 9
/* 0x0048*/    s32   UST19;  //Лог время записи
/* 0x004C*/    s32   UST20;  //IMAX0
/* 0x0050*/    s32   USTD21;  //USTs32
/* 0x0054*/    s32   USTD21_Sec;
/* 0x0058*/    s32   USTD22;  //USTfloat
/* 0x005C*/    s32   USTD22_Sec;
//-------------------- 
 } AllNV_Type;
// 96
#endif //MusCB_prog_NVRAM_h
