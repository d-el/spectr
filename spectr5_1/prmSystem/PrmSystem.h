#ifdef MENU_PRM_0	/*				    flagsMenu																											*/
/*              *nameItem	    ,   *dataPtr    ,	editType,	typePrm	    ,	*procPtr    ,minVal 	    , maxVal    ,defaultVal ,	unit    ,   power   ,	textPtr	    ,   nvSave, NnvSave ,   Base	*/
menuPrmMacro(	"temp0"	        ,	adcStct	    ,	2       ,	int16Frmt	,	NoneFunc    ,	0x03E8	    ,0x0BB8	    ,0x0000	    ,   " "     ,   3	    ,	NULL	    ,	nvSave, 0       ,   NotBase	)
#endif

#ifdef MENU_PRM_1	/*				    flagsMenu																											*/
/*           *nameItem	,   *dataPtr                ,editType,typePrm       ,*procPtr   ,minVal     ,maxVal     ,defaultVal ,unit,  power   ,	textPtr	,   nvSave, NnvSave ,   Base	*/
menuPrmMacro("TI"	    ,	ps.task.current 	    ,   2    ,	int16Frmt	,NoneFunc   ,0x0000	    ,0xFFFF	    ,0x0001	    ," " ,	3	    ,	NULL	,	nvSave, 1,  NotBase	)
menuPrmMacro("TV"	    ,	ps.task.voltage	        ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	3	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("TM"	    ,	ps.task.time	        ,   2    ,	int16Frmt	,NoneFunc   ,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 3,  NotBase	)
menuPrmMacro("TS"	    ,	ps.task.switchRequest	,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 4,  NotBase	)
menuPrmMacro("TC"	    ,	ps.task.control	        ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 5,  NotBase	)
menuPrmMacro("TM"	    ,	ps.task.mode	        ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 6,  NotBase	)
#endif

#ifdef MENU_PRM_2	/*				    flagsMenu																											*/
/*           *nameItem  ,	*dataPtr ,	editType,	typePrm	    ,	*procPtr	,minVal 	, maxVal    ,defaultVal ,	unit , power,	textPtr	    ,   nvSave, NnvSave, Base	*/
menuPrmMacro("TL"	    ,	ps.meas.temperatureLin 	,   2    ,	int16Frmt	,NoneFunc   ,0x0000	    ,0xFFFF	    ,0x0001	    ," " ,	1	    ,	NULL	,	nvSave, 1,  NotBase	)
menuPrmMacro("TS"	    ,	ps.meas.temperatureSps 	,   2    ,	int16Frmt	,NoneFunc   ,0x0000	    ,0xFFFF	    ,0x0001	    ," " ,	1	    ,	NULL	,	nvSave, 1,  NotBase	)
menuPrmMacro("I"	    ,	ps.meas.current 	    ,   2    ,	int16Frmt	,NoneFunc   ,0x0000	    ,0xFFFF	    ,0x0001	    ," " ,	0	    ,	NULL	,	nvSave, 1,  NotBase	)
menuPrmMacro("V"	    ,	ps.meas.voltage	        ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	3	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("HV"	    ,	ps.meas.hvoltage	    ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("POWER"	,	ps.meas.power	        ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("POWER"	,	ps.meas.power	        ,   2    ,	sec2Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("RESIST"	,	ps.meas.resistens	    ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("RESIST"	,	ps.meas.resistens	    ,   2    ,	sec2Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("TIME"	    ,	ps.meas.time	        ,   2    ,	int16Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)
menuPrmMacro("TIME"	    ,	ps.meas.time	        ,   2    ,	sec2Frmt	,NoneFunc	,0x0000	    ,0xFFFF	    ,0x0000	    ," " ,	0	    ,	NULL	,	nvSave, 2,  NotBase	)

#endif

#ifdef Group_Opis_m	/*				flagsMenu,        */
menuGrpMacro(	"Μενώ 0"	,	PrmItem0	  ,  0 , 0, 0	)
menuGrpMacro(	"Μενώ 1"	,	PrmItem1	  ,  0 , 0, 0	)
menuGrpMacro(	"Μενώ 2"	,	PrmItem2	  ,  0 , 0, 0	)
#endif
