#ifndef MusCB_prog_RAM_h
#define MusCB_prog_RAM_h
 
typedef enum{ 
  MB1            =  0    ,     //MB_0X05
  DA87           =  1    ,     //NAMEFUNC16
  DA29           =  2    ,     //(A*B)/N
  DA23           =  3    ,     //BASE16
  DD133          =  4          //LOGER
}nameFuncExe_type; 
 
  #define defB_0_BIT                         16
  #define defB_1_BIT                         17
  #define defB_CLEAR_FAIL_NETWORK            18
  #define defB_FAIL_STOP_NETWORK             19
  #define defB_PUSK_NETWORK                  20
  #define defB_STOP_NETWORK                  21
  #define defNet_Fict_bit                    22
 
 typedef struct {
               u16  Net_Fict1_bit                 : 16 ;   //--
               u16  B_0_BIT                       : 1  ;   //16
               u16  B_1_BIT                       : 1  ;   //17
               u16  CLEAR_FAIL_NETWORK            : 1  ;   //18
               u16  FAIL_STOP_NETWORK             : 1  ;   //19
               u16  PUSK_NETWORK                  : 1  ;   //20
               u16  STOP_NETWORK                  : 1  ;   //21
               u16  Net_Fict_bit                  : 10  ;   //22
/* 0x0000*/    float              NET00014;
/* 0x0008*/    float              Fict_Float;
/* 0x000C*/    s32                NET00003;
/* 0x0010*/    s32                NET00406;
/* 0x0014*/    s32                Fict_S32;
/* 0x0018*/    s16                Net_0;
/* 0x001A*/    s16                Net_1;
/* 0x001C*/    s16                BASE_HZ;
/* 0x001E*/    s16                NET00189;
/* 0x0020*/    s16                NET00006;
/* 0x0022*/    s16                BASE_A;
/* 0x0024*/    s16                BASE_KWT;
/* 0x0026*/    s16                BASE_V;
/* 0x0028*/    s16                NET00000;
/* 0x002A*/    s16                NET00001;
/* 0x002C*/    s16                NET00002;
/* 0x002E*/    s16                NET00004;
/* 0x0030*/    s16                NET00005;
/* 0x0032*/    s16                NET00007;
/* 0x0034*/    s16                NET00008;
/* 0x0036*/    s16                NET00009;
/* 0x0038*/    s16                NET00010;
/* 0x003A*/    s16                NET00017;
/* 0x003C*/    s16                NET00037;
/* 0x003E*/    s16                NET00038;
/* 0x0040*/    s16                NET00039;
/* 0x0042*/    s16                NET00040;
/* 0x0044*/    s16                NET00191;
/* 0x0046*/    s16                NET00386;
/* 0x0048*/    s16                NET00387;
/* 0x004A*/    s16                NET00400;
/* 0x004C*/    s16                NET00402;
/* 0x004E*/    s16                NET00403;
/* 0x0050*/    s16                NET00404;
/* 0x0052*/    s16                NET00408;
/* 0x0054*/    s16                NET00409;
/* 0x0056*/    s16                NET00411;
/* 0x0058*/    s16                NET00413;
/* 0x005A*/    s16                NET00414;
/* 0x005C*/    s16                Fict_S16_1;
/* 0x005E*/    s16                Fict_S16_2;
/* 0x0060*/    s16                Fict_S16_3;
 
// распределение kolvState
 
/* 0x0064*/    s16                State_MB1[4];                //MB_0X05
 } AllPrm_Type;
// 108
#endif //MusCB_prog_RAM_h
