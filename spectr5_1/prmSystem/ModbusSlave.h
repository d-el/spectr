#ifndef ModbusSlave_H
#define ModbusSlave_H

#include "globalIncludes.h"

#define MB_MEM_SIZE     128
#define AdrBlanketMail  0xff
#define	GetOffset(Pnt1, Pnt2)		((uint32_t)(Pnt1) - (uint32_t)(Pnt2))
//****************************************************************************
#define		AllOk_merr				0
#define		Illegal_Function_merr	1
#define		Illegal_DataAdr_merr	2
#define		Illegal_DataVal_merr	3
#define		Failure_merr			4
#define 	SLAVE_DEVICE_BUSY_merr	6
//****************************************************************************
//****************************************************************************
extern wm_type              wmem;   // Глобальная структура
extern uint16_t             netAdr;
//****************************************************************************
//****************************************************************************
 typedef struct	{
				uint16_t		Value    	;
				uint16_t		StepEdIsm  	;
				uint16_t		LowLim  	;
				uint16_t		HighLim  	;
				uint32_t		Flags   	;
		}SysPrm2_type;
//****************************************************************************
 typedef struct	{
				uint32_t		Value    	;
				uint16_t		StepEdIsm  	;
				uint16_t		zer			;
				uint32_t		LowLim	  	;
				uint32_t		HighLim	 	;
				uint32_t		Flags	   	;
		}SysPrm4_type;
//****************************************************************************
 typedef enum {
    Ok_codeFile		,	//0
    ErrAdr_codeFile	,	//1
    ErrHard_codeFile,	//2
    Disable_codeFile	//3
}type_codeFile;
//**************************************************************************** 
char ModbusFunc(unsigned char *Bff, char len);
int ErrMb(unsigned char *Bff, int code);					//sz + заполненный буфер
int	modbus_0x03_4_67(unsigned char *Bff, int len, int StepEdIsm);	//sz + заполненный буфер
int	modbus_0x05(unsigned char *Bff, int len);			//sz + заполненный буфер
int	modbus_0x06(unsigned char *Bff, int len);	 		//sz + заполненный буфер
int	modbus_0x10(unsigned char *Bff, int len);			//sz + заполненный буфер
int	modbus_0x2B(unsigned char *Bff, int len);			//sz + заполненный буфер
uint16_t modbus_0x64(unsigned char *Bff, uint16_t len);		//пошаговая отладка микропрограммы
uint16_t modbus_0x65(unsigned char *Bff, uint16_t len);       //навигация по быстрому журналу   
uint16_t modbus_0x66(unsigned char *Bff, uint16_t len);		//чтение файлов из ОЗУ 70        
uint16_t modbus_0x67(unsigned char *Bff, uint16_t len);		//чтение степени единиц измерений
uint16_t modbus_0x68(unsigned char *Bff, uint16_t len);
uint16_t modbus_0x69(unsigned char *Bff, uint16_t len);
uint16_t modbus_0x70(unsigned char *Bff, uint16_t len);
uint16_t modbus_0x71(unsigned char *Bff, uint16_t len);
uint16_t modbus_0x72(unsigned char *Bff, uint16_t len);

int ExtInfoPrm(unsigned char *Bff, uint16_t Adr, uint16_t	Kolv);
type_codeFile 	fileRd(unsigned char *Bff, uint16_t file, uint32_t adr, uint16_t size );

//****************************************************************************
int MakeOpis_un(wm_type		*wm,
				int			AdrPrm,
				int			cmd,
				int			Rd);
//****************************************************************************
uint16_t GetCrc(void *Bff, uint32_t szBff);
uint16_t crcPacket(unsigned char *Bff, uint32_t szBff);
unsigned short CrcPortion(unsigned short *Bff, unsigned short szBff, unsigned short PrevCrc);

extern const char TblSuecnType[6];
extern const char TblModbus_0x2B[];
//****************************************************************************
#ifdef	DEFINE_Modbus_GLOBAL
	#define Modbus_GLOBAL					
#else	
	#define Modbus_GLOBAL extern
#endif
//****************************************************************************
#endif



