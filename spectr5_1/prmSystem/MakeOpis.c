
#include "globalIncludes.h"

//****************************************************************************
#define  mask4byteFormat       ((1<<floatFrmt)|(1<<int32Frmt)|(1<<HmsFrmt)|(1<<UTimeFrmt)|(1<<uns32Frmt)|(1<<Hex32Frmt))
//**************************************************************************
//флаги в описателе параметра
#define TypeVar_fprm            0U    //5бит  //format
#define MasterVis_fprm          5U    //1бит
#define MasterChange_fprm       6U    //1бит
#define TexnVis_fprm            7U    //1бит
#define TexnChange_fprm         8U    //1бит
#define UserVis_fprm            9U    //1бит
#define UserChange_fprm         10U   //1бит
#define Power_fprm              11U   //3бит  //Колич.отображаемых символов после десятичной точки для числовых значений
#define EditType_fprm           14U   //3бита
#define nonVolatile_fprm        17U   //1бит
#define LimChange_fprm          18U   //1бит
#define FormatChange_fprm       19U   //1бит
#define BaseNumber_fprm         20U   //4бит
#define FuncNumber_fprm         24U   //4бит

#define TypeVar_fprm_mask    	0x0000001F
#define Access_fprm_mask      	0x000007E0
#define EditType_fprm_mask      0x3
#define BaseNumber_fprm_mask    0xF
#define FuncNumber_fprm_mask    0xF // После сдвига вправо
//**************************************************************************
// Номера для баз в массиве баз
#define NUM_FREQ_BASE           1
#define NUM_VOLT_BASE           2
#define NUM_CUR_BASE            3
#define NUM_POWER_BASE          4
#define NUM_1000_BASE           5
#define NUM_HZ_BASE             6
#define NUM_BASE_REF_PID        7

//EditType_fprm
#define Non_RdkPrm              0
#define Stop_RdkPrm             1
#define Always_RdkPrm           2

#define _IQrmpyI32int(iqVal, u32Val)   ((s32)(((s64)iqVal* u32Val + _IQ(0.5) ) >>24))

/******************************************************************************
*
*/
Drv_err_type GetFlagsPrm(wm_type *wm){
    u8                      group,prm;
    const PrmOpis_type      *pntPrm;
    const PrmOpis_type      *pntGrpPrm;

    group = wm->AdrPrm >> 7;       		 //адрес группы   //Storozhenko было >> 8
    prm = wm->AdrPrm & 0x007F;     		 //адрес параметра

    if(group > (NumOpisGrp - 1))        return  BadAdr_err;
    if(prm >= OpisGrp[group].QPrmInGrp) return  BadAdr_err;

    pntGrpPrm = OpisGrp[group].pntOpisPrm;      //указатель на группу параметров
    pntPrm    = &pntGrpPrm[prm];                //указатель на параметр

    wm->NumGrp      = group;
    wm->NumPrm      = prm;

    wm->oPnt        = pntPrm;
    wm->Flags       = pntPrm->flags.all;
    wm->TypeVar     = pntPrm->flags.bit.TypeVar;
    wm->kolvShort   = ((1<<wm->TypeVar) & mask4byteFormat) ? 2:1;
    wm->valPnt      = pntPrm->dataPtr;
    wm->WriteType   = pntPrm->flags.bit.WriteType;
    wm->nvSave      = pntPrm->flags.bit.nvSave;
	wm->nvSavePtr	= (void*)pntPrm->nvSavePtr;
    return  Ok_err;
}

/******************************************************************************
*
*/
Drv_err_type ModbusMakeOpis(wm_type *wm, s16 Rd){
    Drv_err_type            code;
	static u16				adr;

    if((adr == wm->AdrPrm)&&(!Rd)){
    	code = Ok_err;
    }

    code = GetFlagsPrm( wm );

    if((wm->TypeVar == sec2Frmt)&&Rd){
        wm->Val2byte = (u16)(wm->val.s32.Value>>16);
        return  Ok_err;
    }
    if(code == Ok_err){
      if(!Rd){
        if(wm->kolvShort == 2){
          if(wm->TypeVar == floatFrmt){
            wm->val.f.Value = wm->Val2byte&0xFFFF;
            return  code;
          }
          else{
            wm->val.s32.Value = wm->Val2byte&0xFFFF;
            return  code;
          }
        }
        else if(wm->TypeVar == sec2Frmt){
          wm->val.s32.Value |= ((u32)wm->Val2byte<<16);
          wm->AdrPrm--;
        }
        else{
          if ((wm->Flags & TypeVar_fprm_mask) == int16Frmt){
            wm->val.s32.Value  = (s16)wm->Val2byte;
          }
          else{
            wm->val.s32.Value  = (u16)wm->Val2byte;
          }
        }
      }

      code = MakeOpis(wm, Rd);

      if(Rd){
        wm->Val2byte = (u16)wm->val.s32.Value&0xFFFF;
      }
    }
	/*********************************************************************************************************************/

    return  code;
}

/******************************************************************************
*
*/
Drv_err_type MakeOpis(wm_type *wm, s16 Rd)
{
    const PrmOpis_type          *oPnt;
    Drv_err_type                code;
    WriteType_type              WriteType;
    
    code = GetFlagsPrm(wm);

    if(code != Ok_err)  return  code;

    oPnt = wm->oPnt;

    if(wm->TypeVar == sec2Frmt)     return  BadAdr_err;
    WriteType = wm->oPnt->flags.bit.WriteType;

    if(wm->kolvShort == 2){     //параметр 4 байтный
        //wm->val.s32.LowLim  = (*(u32*)oPnt->minVal&0xFFFF) | ((*(u32*)oPnt[1].minVal) <<16);
        //wm->val.s32.HighLim = (*(u32*)oPnt->maxVal&0xFFFF) | ((*(u32*)oPnt[1].maxVal) <<16);
        //WARNING STOROZHENKO
        wm->val.s32.LowLim  = (u16)oPnt[0].minVal | ((u16)oPnt[1].minVal << 16);
        wm->val.s32.HighLim = (u16)oPnt[0].maxVal | ((u16)oPnt[1].maxVal << 16);
        
        if(Rd){
            wm->val.s32.Value = *(s32 *)(wm->valPnt);
        }
        else{
            if((WriteType == WriteTypeAlways) || ( (WriteType == WriteTypeStop) && (!writeLock()) ) ){
                if(wm->TypeVar == floatFrmt){
                    if(wm->val.f.Value < wm->val.f.LowLim)      return  Lim_err;
                    if(wm->val.f.Value > wm->val.f.HighLim)     return  Lim_err;

                    *(float *)(wm->valPnt) = (float)wm->val.f.Value;
                }
                else{
                    if(wm->val.s32.Value < wm->val.s32.LowLim)  return  Lim_err;
                    if(wm->val.s32.Value > wm->val.s32.HighLim) return  Lim_err;

                    *(s32 *)(wm->valPnt) = (s32)wm->val.s32.Value;
                }
            }
            else{
                return  NonRdk_err;
            }
        }
    }//wm->Flags
    else{       //параметр 2 байтный
        if(wm->oPnt->flags.bit.nameBase == NotBase){
            if(Rd){
                if ((wm->Flags & TypeVar_fprm_mask) == uns16Frmt){
                    wm->val.s32.Value = *(u16 *)(wm->valPnt);
                }
                else{
                    wm->val.s32.Value = *(s16 *)(wm->valPnt);
                }
            }
            else{
                if((WriteType == WriteTypeAlways) || ( (WriteType == WriteTypeStop) && (!writeLock() )) ){
                        if ((wm->Flags & TypeVar_fprm_mask) == uns16Frmt){
                            wm->val.s32.LowLim  = (u16)oPnt->minVal;
                            wm->val.s32.HighLim = (u16)oPnt->maxVal;

                            if(wm->val.s32.Value < wm->val.s32.LowLim)
                            return  Lim_err;
                            if(wm->val.s32.Value > wm->val.s32.HighLim)
                            return  Lim_err;

                            *(u16 *)(wm->valPnt) = (u16)wm->val.s32.Value;
                        }
                        else{
                            wm->val.s32.LowLim  = (s16)oPnt->minVal;
                            wm->val.s32.HighLim = (s16)oPnt->maxVal;

                            if(wm->val.s32.Value < wm->val.s32.LowLim)
                                return  Lim_err;
                            if(wm->val.s32.Value > wm->val.s32.HighLim)
                                return  Lim_err;

                            *(s16 *)(wm->valPnt) = (s16)wm->val.s32.Value;
                        }
                    }
                    else{
                        return  NonRdk_err;
                    }
            }
        }
        
        if(wm->oPnt->funcPtr != NULL){
            wm->oPnt->funcPtr(wm, Rd);
        }
    }

    return  Ok_err;
}

/******************************************************************************
* Сохранение переменных во флешь память
*/
void savePrmToFlash(void *Pnt, s16 Rd)
{
//    static u32          allPrm[MAX_NUM_UST];
//    wm_type             *pntWm;
//    PrmOpis_type        *pntOpisPrm;
//    u32                 *pCurrentPrm;
//    u16                 flagNvSave;
//    u16                 grCnt, prmCnt, maxPrm;
// 
//    pntWm   = Pnt;
//    pCurrentPrm = allPrm;
//
//    for(grCnt = 0; grCnt < NumOpisGrp; grCnt++){
//        maxPrm      = OpisGrp[grCnt].QPrmInGrp;
//        pntOpisPrm  = OpisGrp[grCnt].pntOpisPrm;         
//        
//        for(prmCnt = 0; prmCnt < maxPrm; prmCnt++){
//            flagNvSave      = pntOpisPrm->flags.bit.nvSave;
//            if(flagNvSave == nvSave){
//                
//                *pCurrentPrm = *(u32*)pntOpisPrm->dataPtr;
//                pCurrentPrm++;
//            }
//            pntOpisPrm++;
//        }
//    }
//
//    vPortEnterCritical();
//    
//    //writeToInternalFlash((void*)pntNvMem, allPrm, ((u32)pCurrentPrm - (u32)allPrm));
//    //*(u16*)pntWm->valPnt = 0;
//    //saveSettings = 0;   //Сбрасываем флаг
//    
//    vPortExitCritical();
}

/******************************************************************************
*
*/
void DebnitPrmSystem(void)
{
    u32                 nGrp, nPrm, maxNPrm;
    OpisGrp_type        *oPnt;
    PrmOpis_type        *oPrm;
    u16                 numBase;
//    u32                 data;

    for(nGrp = 0; nGrp < NumOpisGrp; nGrp++){
        oPnt = &OpisGrp[nGrp];
        maxNPrm = oPnt->QPrmInGrp;
        oPrm = oPnt->pntOpisPrm;
        for(nPrm=0; nPrm< maxNPrm; nPrm++){
            numBase = oPrm->flags.bit.nameBase;
            if( numBase == NotBase ){
                *(u16*)oPrm->dataPtr = oPrm->defaultVal;
            }
            else{
                *(s32*)oPrm->dataPtr = _IQ12toIQ((s16)oPrm->defaultVal);  //oPrm->defaultVal * Base[nameBase];            
            }
            oPrm++;
        }
    }
}

/******************************************************************************
*
*/
void initPrmSystem(void)
{
    static u32          allPrm[MAX_NUM_UST];
    PrmOpis_type        *pntOpisPrm;
    u32                 *pCurrentPrm;
    u16                 flagNvSave;
    u16                 grCnt, prmCnt, maxPrm;
    u16                 numBase;
    
    //memcpy(allPrm, (void*)pntNvMem, sizeof(allPrm));

    pCurrentPrm = allPrm;

    for(grCnt = 0; grCnt < NumOpisGrp; grCnt++){
        
        maxPrm      = OpisGrp[grCnt].QPrmInGrp;
        pntOpisPrm  = OpisGrp[grCnt].pntOpisPrm;         
        
        for(prmCnt = 0; prmCnt < maxPrm; prmCnt++){
            
            flagNvSave      = pntOpisPrm->flags.bit.nvSave;
            if(flagNvSave == nvSave){
                numBase = pntOpisPrm->flags.bit.nameBase;
                if( numBase == NotBase ){
                    *(u16*)pntOpisPrm->dataPtr = *(u16*)pCurrentPrm;
                }
                else{
                    *(u32*)pntOpisPrm->dataPtr = *(u32*)pCurrentPrm;
                }
                pCurrentPrm++;
            }
            pntOpisPrm++;
        }
    }

}
