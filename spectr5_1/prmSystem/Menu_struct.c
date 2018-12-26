#include "globalIncludes.h"

/******************************************************************************
* Заполняем default значениями, если тип nvSave
*/
#include "DefMenuPrm.h" //Включаем все группы
#define macro_nvSave(val)  \
    val,
#define macro_nvNotSave(val)

#define Sec_PrmMacro(nameItem, dataPtr, editType, typePrm, procPtr, minVal, maxVal, defaultVal, unit, power, textPtr, NvSave, NnvSave, Base)\
    macro_##NvSave(defaultVal)

#define menuPrmMacro(nameItem, dataPtr, editType, typePrm, procPtr, minVal, maxVal, defaultVal, unit, power, textPtr, NvSave, NnvSave, Base)\
    macro_##NvSave(defaultVal)

const nvMem_type nvMemDef = {
    {
        #include "PrmSystem.h"
    },
    0   //PRM_SYS_SIGNATURA
};

#undef macro_nvSave
#undef macro_nvNotSave
#undef Sec_PrmMacro
#undef menuPrmMacro
#include "UnDefMenuPrm.h"

/******************************************************************************
* Инициализация структуры описателя параметров (PrmOpis_type)
*/
#define NoneFunc NULL

#define macro_nvSave(NnvSave)                \
    (const void *)((uint16_t*)&nvMemDef + NnvSave) 

#define macro_nvNotSave(NnvSave)             \
    NULL

#define Sec_PrmMacro( nameItem, dataPtr, editType, typePrm, procPtr, minVal, maxVal, defaultVal, unit, power, textPtr, NvSave, NnvSave, Base)\
{   (uint16_t*)&dataPtr + 1,                     \
    (uint16_t)minVal ,                           \
    (uint16_t)maxVal ,                           \
	(uint16_t)defaultVal ,                       \
	macro_##nvSave(NnvSave),                 \
    {typePrm, (WriteType_type)editType, sizeof(type##typePrm) / 2, NvSave, Base},      \
    procPtr,                                \
}   ,

#define menuPrmMacro( nameItem, dataPtr, editType, typePrm, procPtr, minVal, maxVal, defaultVal, unit, power, textPtr, NvSave, NnvSave, Base)\
{   &dataPtr,                               \
    (uint16_t)minVal ,                           \
    (uint16_t)maxVal ,                           \
	(uint16_t)defaultVal ,                       \
	macro_##nvSave(NnvSave),                 \
    {typePrm, (WriteType_type)editType, sizeof(type##typePrm) / 2, NvSave, Base},      \
    procPtr,                                \
}   ,

/**************************************
* Заполняем по-очереди каждую группу
*/
#define MENU_PRM_0
PrmOpis_type   PrmItem0[] = {
    #include "PrmSystem.h"
};
#undef MENU_PRM_0

#define MENU_PRM_1
PrmOpis_type   PrmItem1[] = {
    #include "PrmSystem.h"
};
#undef MENU_PRM_1

#define MENU_PRM_2
PrmOpis_type   PrmItem2[] = {
    #include "PrmSystem.h"
};
#undef MENU_PRM_2

#undef macro_nvSave
#undef macro_nvNotSave
#undef Sec_PrmMacro
#undef menuPrmMacro

/******************************************************************************
* Инициализация структуры описателя групп  (OpisGrp_type)
*/
#define menuGrpMacro( nameItem, pntOpisPrm, flag1, flag2 , flag3   )\
    {                                                               \
        &pntOpisPrm[0],                                             \
        (sizeof(pntOpisPrm) / sizeof(pntOpisPrm[0])),               \
        {flag1, flag2 , flag3 }                                     \
    },

#define Group_Opis_m

const OpisGrp_type OpisGrp[] = {
    #include "PrmSystem.h"
};

const uint8_t NumOpisGrp = (sizeof(OpisGrp) / sizeof(OpisGrp_type));

#undef menuGrpMacro
#undef Group_Opis_m

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
