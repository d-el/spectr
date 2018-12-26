#ifdef MENU_PRM_1	/*				    flagsMenu																											*/
/*               *nameItem	            ,	    *dataPtr ,	editType,	typePrm	    ,	*procPtr	,minVal 	, maxVal    ,defaultVal ,	unit , power,	textPtr	    ,   nvSave, NnvSave, Base	*/
menuPrmMacro(	"Версия ПО"	        ,	VerSoft	,	0    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0000	    ,0x0000	    ," " ,	2	,	NULL	,	nvNotSave, 0,NotBase	)
menuPrmMacro(	"Подверсия ПО"	        ,	SubVerSoft	,	0    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0000	    ,0x0000	    ," " ,	3	,	NULL	,	nvNotSave, 0,NotBase	)
menuPrmMacro(	"CRC ПО"	        ,	temp0	,	0    ,	Hex16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0000	    ,0x0000	    ," " ,	0	,	NULL	,	nvNotSave, 0,NotBase	)
menuPrmMacro(	"Сетевой адрес"	        ,	NetAdr	,	0    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x00FF	    ,0x0000	    ," " ,	0	,	NULL	,	nvNotSave, 0,NotBase	)
menuPrmMacro(	"Сетевой адрес"	        ,	NetAdr	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x00FE	    ,0x0001	    ," " ,	0	,	NULL	,	nvSave, 0,NotBase	)
menuPrmMacro(	"UDAC0"	        ,	bs.set[0].Udac	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0FFF	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 1,NotBase	)
menuPrmMacro(	"IDAC0"	        ,	bs.set[0].Idac	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0FFF	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 2,NotBase	)
menuPrmMacro(	"MODE0"	        ,	bs.set[0].Mode	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x000A	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 3,NotBase	)
menuPrmMacro(	"IMAX0"	        ,	bs.set[0].Idac	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0FFF	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 4,NotBase	)

#endif


#ifdef MENU_PRM_2	/*				    flagsMenu																											*/
/*               *nameItem	            ,	    *dataPtr ,	editType,	typePrm	    ,	*procPtr	,minVal 	, maxVal    ,defaultVal ,	unit , power,	textPtr	    ,   nvSave, NnvSave, Base	*/
menuPrmMacro(	"USTs16"	        ,	usts16	,	2    ,	Hex16Frmt	,	NoneFunc	 ,	0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 5,NotBase	)
menuPrmMacro(	"USTs32"	        ,	usts32	,	2    ,	int32Frmt	,	NoneFunc	 ,	0x0000	    ,0xC350	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 6,NotBase	)
Sec_PrmMacro(      ""	        ,	usts32	,	0    ,	sec2Frmt	,	NoneFunc	 ,	0x0000	    ,0x0000	    ,0x0000	    ," " ,	0	,	NULL	,	nvNotSave, 0,NotBase	)
menuPrmMacro(	"USTfloat"	        ,	ustfloat	,	2    ,	floatFrmt	,	NoneFunc	 ,	0x0000	    ,0x2400	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 7,NotBase	)
Sec_PrmMacro(      ""	        ,	ustfloat	,	0    ,	sec2Frmt	,	NoneFunc	 ,	0x0000	    ,0x48F4	    ,0x0000	    ," " ,	0	,	NULL	,	nvNotSave, 0,NotBase	)

#endif


#ifdef MENU_PRM_4	/*				    flagsMenu																											*/
/*               *nameItem	            ,	    *dataPtr ,	editType,	typePrm	    ,	*procPtr	,minVal 	, maxVal    ,defaultVal ,	unit , power,	textPtr	    ,   nvSave, NnvSave, Base	*/
menuPrmMacro(	"Д1"	        ,	debIQ1	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0000	    ," " ,	0	,	NULL	,	nvNotSave, 0,NotBase	)

#endif


#ifdef MENU_PRM_5	/*				    flagsMenu																											*/
/*               *nameItem	            ,	    *dataPtr ,	editType,	typePrm	    ,	*procPtr	,minVal 	, maxVal    ,defaultVal ,	unit , power,	textPtr	    ,   nvSave, NnvSave, Base	*/
menuPrmMacro(	"N тик логера"	        ,	cmpPrescalLog1	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x2710	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 8,NotBase	)
menuPrmMacro(	"Тип логера"	        ,	TypeLog	,	2    ,	NtxtFrmt	,	NoneFunc	 ,	0x0000	    ,0x0003	    ,0x0000	    ," " ,	0	,	NULL	,	nvSave, 9,NotBase	)
menuPrmMacro(	"N колич.перем Л"	        ,	NlogVar	,	2    ,	int16Frmt	,	NoneFunc	 ,	0x0000	    ,0x0009	    ,0x0003	    ," " ,	0	,	NULL	,	nvSave, 10,NotBase	)
menuPrmMacro(	"№ переменной 1"	        ,	LogPnt.n[0]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0001	    ," " ,	0	,	NULL	,	nvNotSave, 0,NotBase	)
menuPrmMacro(	"№ переменной 2"	        ,	LogPnt.n[1]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0002	    ," " ,	0	,	NULL	,	nvSave, 11,NotBase	)
menuPrmMacro(	"№ переменной 3"	        ,	LogPnt.n[2]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0003	    ," " ,	0	,	NULL	,	nvSave, 12,NotBase	)
menuPrmMacro(	"№ переменной 4"	        ,	LogPnt.n[3]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0004	    ," " ,	0	,	NULL	,	nvSave, 13,NotBase	)
menuPrmMacro(	"№ переменной 5"	        ,	LogPnt.n[4]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0005	    ," " ,	0	,	NULL	,	nvSave, 14,NotBase	)
menuPrmMacro(	"№ переменной 6"	        ,	LogPnt.n[5]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0006	    ," " ,	0	,	NULL	,	nvSave, 15,NotBase	)
menuPrmMacro(	"№ переменной 7"	        ,	LogPnt.n[6]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0007	    ," " ,	0	,	NULL	,	nvSave, 16,NotBase	)
menuPrmMacro(	"№ переменной 8"	        ,	LogPnt.n[7]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0008	    ," " ,	0	,	NULL	,	nvSave, 17,NotBase	)
menuPrmMacro(	"№ переменной 9"	        ,	LogPnt.n[8]	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0009	    ," " ,	0	,	NULL	,	nvSave, 18,NotBase	)
menuPrmMacro(	"Лог время записи"	        ,	logEndTime	,	2    ,	uns16Frmt	,	NoneFunc	 ,	0x0000	    ,0x03E8	    ,0x0005	    ," " ,	0	,	NULL	,	nvSave, 19,NotBase	)

#endif


#ifdef Group_Opis_m	/*				flagsMenu,        */
menuGrpMacro(	"Меню 1"	,	PrmItem1	  ,  0 , 0, 0	)
menuGrpMacro(	"Меню 2"	,	PrmItem2	  ,  0 , 0, 0	)
menuGrpMacro(	"Меню 4"	,	PrmItem4	  ,  0 , 0, 0	)
menuGrpMacro(	"Меню 5"	,	PrmItem5	  ,  0 , 0, 0	)
#endif
