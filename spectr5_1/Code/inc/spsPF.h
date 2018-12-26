/******************************************************************************
* @file    spsPF.h
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    14-09-2015
* @brief
*/
#ifndef spsPF_H
#define spsPF_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
//vreg param
#define kFiltrSpsTsk            6
#define SPS_MAX_VOLTAGE         32000   //[V]
#define SPS_MIN_VOLTAGE         12000   //[V]
#define SPS_MAX_ERR             30
#define ADDITIV_VOLTAGE         9000    //[X_XXX V]

/******************************************************************************
* User typedef
*/

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void spsPF(void);
uint16_t vreg(uint16_t tsk);

#endif //spsPF_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
