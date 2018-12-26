/******************************************************************************
* @file    crc.c 
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    23.06.2015
* @brief
*/
#include "globalIncludes.h"

/******************************************************************************
* Memory
*/
wm_type     wmem;   // Глобальная структура 
uint16_t    netAdr = 1;

/******************************************************************************
*
*/
char ModbusFunc(unsigned char *Bff, char len){
    uint16_t         sz, CrcVal;
  	
	if((Bff[0] == 0) && (Bff[1] == 0x2b)){
		;//обрабатываем запрос по 2b функции  
	}
	else if((Bff[0] != netAdr) && Bff[0] != AdrBlanketMail){
   	 	return 0;
	}
	
	switch(Bff[1]){     //номер функции
		case 0x03:
		case 0x04:  sz = modbus_0x03_4_67(Bff, len, 1);	break;
		case 0x06:  sz = modbus_0x06(Bff, len);         break;
		case 0x10:  sz = modbus_0x10(Bff, len);         break;
		case 0x2b:	sz = modbus_0x2B(Bff, len);			break;
        case 0x68:  sz = modbus_0x68(Bff, len);		    break;
        
		default  :
			Bff[1] |= 0x80;
			Bff[2] = Illegal_Function_merr;
			sz = 3;
	}
    
    if((!sz)||(sz >255)) return 0;
    CrcVal = GetCrc(Bff,sz);
    Bff +=sz;
    *Bff++ = CrcVal&0xFF;           //low
    *Bff++ = (CrcVal>>8)&0xFF;      //high
    return sz+2;
}

/******************************************************************************
*
*/
int ErrMb(unsigned char *Bff, int code){                             //sz + заполненный буфер
    Bff[1] |= 0x80;
    Bff[2] = code;
    return 3;
}

/******************************************************************************
*
*/
int modbus_0x03_4_67(unsigned char *Bff, int len, int StepEdIsm){	//sz + заполненный буфер
    uint16_t             Adr, Kolv, tmp;
    uint16_t             *First2 = 0;
  	unsigned char   *pnt;
    wm_type         *wm;
    Drv_err_type    code;

    wm = &wmem;
    if(len!=8)   return 0;
    Adr = (Bff[2]<<8)|Bff[3];
    Kolv = (Bff[4]<<8)|Bff[5];
    
    pnt = Bff+2;                                        //отступ на netAdr, Func
    *pnt++ = Kolv*2;                                    //ByteCount
    if((Kolv>0x7D)||(Kolv<1)) 		return ErrMb(Bff,Illegal_DataVal_merr);
	for(;Kolv>0;Kolv--,Adr++){
		wm->AdrPrm = Adr;               
    
        code = GetFlagsPrm(wm);
    
        if(code != Ok_err)              return  ErrMb(Bff, code);
      
        if(wm->kolvShort > Kolv ) return  ErrMb(Bff, BadAdr_err);  
       
        if(wm->kolvShort == 2){  
          First2 = wm->valPnt; 
        }
        if(wm->TypeVar == sec2Frmt){ 
          if( wm->valPnt != First2 + 1) return  ErrMb(Bff, BadAdr_err);     
        }	
        tmp = ModbusMakeOpis(wm, 1);
		
		
		
        if(tmp) return ErrMb(Bff,tmp);
	
        *pnt++ = wm->Val2byte>>8;		
        *pnt++ = wm->Val2byte&0xFF;	
    }  
    return      Bff[2]+3;
}

int ExtInfoPrm(unsigned char *Bff, uint16_t Adr, uint16_t	Kolv){
	wm_type         	*wm;
	typedef union{
		SysPrm2_type	SysPrm2;
		SysPrm4_type	SysPrm4;
	}dataPrm_type;
	unsigned char	   	*pntDst;

	wm = &wmem;
	wm->AdrPrm = Adr;
	MakeOpis(wm, 1);
	
	pntDst = Bff+3;			//отступ на netAdr, Func и к-во байтов
	if(wm->TypeVar == sec2Frmt){
		return ErrMb(Bff,Illegal_DataAdr_merr);
	}
	else 
      if(wm->kolvShort == 1){
		*pntDst++ = (unsigned char)((wm->val.s32.Value>>8) & 0xFF);   // текущая величина
		*pntDst++ = (unsigned char)(wm->val.s32.Value & 0xFF);
		Kolv--;
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->StepEdIsm>>8) & 0xFF);   // степень и единица изм.
   		    *pntDst++ = (unsigned char)(wm->StepEdIsm & 0xFF); 
		    Kolv--; }
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->val.s32.LowLim>>8) & 0xFF);  // нижний предел
		    *pntDst++ = (unsigned char)(wm->val.s32.LowLim & 0xFF);
		    Kolv--; }
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->val.s32.HighLim>>8) & 0xFF); // верхний предел
		    *pntDst++ = (unsigned char)(wm->val.s32.HighLim & 0xFF);
		    Kolv--; }
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->Flags>>8) & 0xFF);       // флаги
		    *pntDst++ = (unsigned char)(wm->Flags & 0xFF);
		    *pntDst++ = (unsigned char)((wm->Flags>>24) & 0xFF);
		    *pntDst++ = (unsigned char)((wm->Flags>>16) & 0xFF); }
	}
	else{
		*pntDst++ = (unsigned char)((wm->val.s32.Value>>8) & 0xFF);   // текущая величина
		*pntDst++ = (unsigned char)(wm->val.s32.Value & 0xFF);
		*pntDst++ = (unsigned char)((wm->val.s32.Value>>24) & 0xFF);
		*pntDst++ = (unsigned char)((wm->val.s32.Value>>16) & 0xFF);
		Kolv--;
		Kolv--;
		if (Kolv)
		{   *pntDst++ = ((wm->StepEdIsm>>8) & 0xFF);   // степень и единица изм.
   		    *pntDst++ = (wm->StepEdIsm & 0xFF); 
		    Kolv--; }
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->val.s32.LowLim>>8) & 0xFF);  // нижний предел
		    *pntDst++ = (unsigned char)(wm->val.s32.LowLim & 0xFF);
		    Kolv--; }
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->val.s32.HighLim>>8) & 0xFF); // верхний предел
		    *pntDst++ = (unsigned char)(wm->val.s32.HighLim & 0xFF);
		    Kolv--; }
		if (Kolv)
		{   *pntDst++ = (unsigned char)((wm->Flags>>8) & 0xFF);       // флаги
		    *pntDst++ = (unsigned char)(wm->Flags & 0xFF);
		    *pntDst++ = (unsigned char)((wm->Flags>>24) & 0xFF);
		    *pntDst++ = (unsigned char)((wm->Flags>>16) & 0xFF); }
	}
	return      Bff[2]+3;
}

/******************************************************************************
*
*/
const char TblModbus_0x2B[]={
    0x2B,//Function0x2B
    0x0E,//MEI Type0x0E
    0x01,//Read Dev Id Code0x01
    0x01,//Conformity Level0x01
    0x00,//More Follows0x00
    0x00,//Next Object Id0x00
    0x03,//Number Of Objects0x03
    
    0x00,//Object Id0x00
    0x04,//Object Length0x05
    'D','_','E','L',//Object Value
    
    0x01,//Object Id0x01
    0x06,//Object Length0x05
    'S','P','E','C','T','R',//Object Value
    
    0x02,//Object Id0x02
    0x06,//Object Length0x09
    'v','2','.','0','2','8',//Object Value
};

int	modbus_0x2B(unsigned char *Bff, int len)
{
    unsigned char		*cPnt;
    
    if((Bff[2] != 0x0E)||(Bff[3] != 0x01)||(Bff[4] != 0x00))	ErrMb(Bff,Illegal_DataAdr_merr);
    Bff[0] = netAdr;
    cPnt = Bff+1;
    
    memcpy(cPnt, TblModbus_0x2B, sizeof(TblModbus_0x2B));
    
    return	GetOffset(cPnt, Bff) + sizeof(TblModbus_0x2B);
}

/******************************************************************************
*
*/
int modbus_0x06(unsigned char *Bff, int len){           //sz + заполненный буфер
    uint16_t             tmp;
    wm_type         *wm;
    Drv_err_type    code;

    wm = &wmem;
    if(len!=8)      return 0;
    wm->Val2byte = (Bff[4]<<8)|Bff[5];
    wm->AdrPrm   = (Bff[2]<<8)|Bff[3];
    
    code = GetFlagsPrm(wm);
    
    if(code != Ok_err)            return  ErrMb(Bff, code);
    if(wm->kolvShort == 2)        return  ErrMb(Bff, BadAdr_err);
    if(wm->TypeVar == sec2Frmt)   return  ErrMb(Bff, BadAdr_err);    
        
    tmp = ModbusMakeOpis(wm, 0);


    if(tmp) return ErrMb(Bff,tmp);
	
    return 6;
}

/******************************************************************************
*
*/
int modbus_0x10(unsigned char *Bff, int len){           //sz + заполненный буфер
    uint16_t             Adr, Kolv,tmp;
    uint16_t             *First2 = 0;
    u8               Flag = 0;
    unsigned char   *pntVal;
    wm_type         *wm;
    Drv_err_type    code;
    
    wm = &wmem;
    Adr = (Bff[2]<<8)| Bff[3];
    Kolv = (Bff[4]<<8)|Bff[5];

    if(len != (9+Kolv*2))    return 0;
    if((Kolv<1)||(Kolv>=0x78)||(Bff[6]!=2*Kolv))    return ErrMb(Bff,Illegal_DataVal_merr);
      
    pntVal = &Bff[7];
    for(;Kolv>0;Kolv--, Adr++, pntVal+=2){
        wm->Val2byte = (pntVal[0]<<8)|pntVal[1];
        wm->AdrPrm = Adr;
        
        code = GetFlagsPrm(wm);
    
        if(code != Ok_err)              return  ErrMb(Bff, code);
        
        if(wm->kolvShort == 2){    
          First2 = wm->valPnt;   
          Flag++; 
        }
        if(wm->TypeVar == sec2Frmt){   
          if( wm->valPnt != First2 + 1){     
            return  ErrMb(Bff, BadAdr_err);   
          }    
          else Flag--;
        }  
        tmp = ModbusMakeOpis(wm, 0);
	

		
        if(tmp) return ErrMb(Bff,tmp);
    }  
    if(Flag) return  ErrMb(Bff, BadAdr_err);
    return 6;
}

/******************************************************************************
*
*/
uint16_t modbus_0x68(unsigned char *Bff, uint16_t len){
	union{  u32 L;
	        uint16_t W[2];
	}adr;

	type_codeFile	code;

	if(len!=10)   return 0;
	adr.W[0] = (Bff[5]<<8) | Bff[6];
	adr.W[1] = (Bff[3]<<8) | Bff[4];

#if LOGER == 1
		debLockWriteLog = 200; // 1 сек
#endif // LOGER

	code = fileRd( (void*)&Bff[8], Bff[2], adr.L, Bff[7] );

	if(code == ErrAdr_codeFile)
        return ErrMb(Bff,Illegal_DataAdr_merr);
	if(code != Ok_codeFile)
        return ErrMb(Bff,Failure_merr);

	return Bff[7]*2+8;
}

/******************************************************************************
* Функция реализована только для чтения файла из логера
*/
type_codeFile 	fileRd(unsigned char *Bff, uint16_t file, u32 adr, uint16_t size ){
	/*uint16_t                     *ptrFrom, tmp;
	uint16_t                     sizeOfFile;
	
	sizeOfFile = sizeof(logFile_type)*2;
    if( file != 1 )                         { return ErrAdr_codeFile; }
    if (adr == 0) {
    	logFile->Sz 	= LogLen;
        logFile->Ntic   = cmpPrescalLog1;
        logFile->PeriodCall = 2000;             //200 мкс
        logFile->Ntic    = cmpPrescalLog1;
        logFile->Nvar    = NlogVar;
    }

   
//    if(size > 234/2 )                       { return ErrAdr_codeFile; }
    if( adr >= sizeOfFile*2)                { return ErrAdr_codeFile; }
    if( adr + (size) >= sizeOfFile)         { return ErrAdr_codeFile; }

    ptrFrom = (uint16_t*)ExtRAMmem + adr;

    for( ; size; size--, ptrFrom++ ){
        tmp = *(ptrFrom);
        *Bff++  = (char)(tmp>>8);
		*Bff++  = (char)tmp;
    }*/

	return  Ok_codeFile;
}
