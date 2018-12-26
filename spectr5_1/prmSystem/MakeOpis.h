#ifndef	MakeOpis_H
#define MakeOpis_H

#define writeLock()   0

/******************************************************************************
*
*/
struct wmStruct{
				u16					AdrPrm			;
                u8                  NumGrp          ;
                u8                  NumPrm          ;
				u16					Flags			;
				u16					StepEdIsm		;
				u16					kolvShort		;

				union{
					struct{
							 s32	Value			;
							 s32	LowLim			;
							 s32	HighLim			;
							}s32;
					struct{
							 float	Value			;
							 float	LowLim			;
							 float	HighLim			;
							}f;
                    struct{
							 _iq	Value			;
							 _iq	LowLim			;
							 _iq	HighLim			;
							}iq;
				}val;

			    const PrmOpis_type	*oPnt			;
				void				*valPnt			;
				pFuncMosAf_typedef  mosaf			;

				u16					Val2byte		;
                WriteType_type      WriteType       ;
                PrmFrmt_type		TypeVar			;
                u16                 nvSave          ;
				void 				*nvSavePtr		;
};

//********************************************************
typedef struct wmStruct wm_type;
//********************************************************

Drv_err_type ModbusMakeOpis(wm_type *wm, s16 Rd);
Drv_err_type GetFlagsPrm(wm_type *wm);
Drv_err_type MakeOpis(wm_type *wm, s16 Rd);
void StartExtMem(void);
void initPrmSystem(void);
void DebnitPrmSystem(void);
//********************************************************
void savePrmToFlash(void *Pnt, s16 Rd);
//********************************************************

#endif
