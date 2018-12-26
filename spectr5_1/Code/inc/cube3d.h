/**
******************************************************************************
* @file    cube3d.h 
* @author  
* @version V1.0
* @date    31-October-2013
* @brief   
******************************************************************************
*/

#ifndef CUBE3D_H
#define CUBE3D_H

//���������
#define CUBE_SIZE    26     // (����� �����)/2
#define DOTS_COUNT    8     // ����� ������
#define MESH_COUNT   12     // ���������� �����
#define LCD_X_SIZE  101     // ���������� �� �����������
#define LCD_Y_SIZE  80      // ���������� �� ���������

typedef struct{
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
}pairsOfPoints_type;

//-----------------------------------------------------------
//Prototypes for the functions in cube3d.c
//-----------------------------------------------------------
void cube3dTSK(void *pPrm);
float cos_(unsigned char angle);
float sin_(unsigned char angle);

#endif //CUBE3D_H

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
