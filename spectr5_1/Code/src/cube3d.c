/**
******************************************************************************
* @file    cube3d.c 
* @author  
* @version V1.0
* @date    31-October-2013
* @brief   
******************************************************************************
*/

#include "globalIncludes.h"
	
uint16_t colors[12] = {	//����� ������
    R,
    G,
    B,
    R,
    G,
    B,
    R,
    G,
    B,
    R,
    G,
    B,
};

const float cospi[]= {
    1,  //0
    0.99518473,
    0.98078528,
    0.95694034,
    0.92387953,
    0.88192126,
    0.83146961,
    0.77301045, //7
    0.70710678,
    0.63439328,
    0.55557023,
    0.47139674,
    0.38268343,
    0.29028468,
    0.19509032,
    0.09801714, //15
    0  //16
};

float cos_(unsigned char angle){
    unsigned char a1, a2;
    a1 = angle & 0x0f;
    a2 = angle >> 4; // ���������� ������
    if(a2 == 0) return (float)(cospi[a1]);
    if(a2 == 1) return (float)(cospi[16-a1] * (-1));
    if(a2 == 2) return (float)(cospi[a1] * (-1));
    if(a2 == 3) return (float)(cospi[16-a1] );
    return 0;
}

float sin_(unsigned char angle){
    unsigned char a1, a2;
    a1 = angle & 0x0f;
    a2 = angle >> 4;
    if(a2 == 0) return (float)(cospi[16-a1]);
    if(a2 == 1) return (float)(cospi[a1]);
    if(a2 == 2) return (float)(cospi[16-a1] * (-1));
    if(a2 == 3) return (float)(cospi[a1] * (-1));
    return 0;
}

/** ������� �������� ������ ��������� (s1) � �������� (f1) �����,
 *  �� ��� ��������� ��� � ������ �����(�����) 
 */
const unsigned char  s1[MESH_COUNT] = {
    0, 1, 2, 3, 0, 1, 2, 3, 4 ,5, 6, 7
};

const unsigned char f1[MESH_COUNT] = {
    1, 2, 3, 0, 4, 5, 6, 7, 5, 6, 7, 4
};


void cube3dTSK(void *pPrm){
    uint16_t old_val_encoder;
    /** ���� �������� �� ����, �� 0 �� 63 ������������,
    *  ��� ������ ������ �� 360
    */
    unsigned char           dir_x = 0; 
    unsigned char           dir_y = 0;
    unsigned char           dir_z = 0; 
    //�����, ����� �������� �� long
    signed short            Xa[DOTS_COUNT], Ya[DOTS_COUNT], Za[DOTS_COUNT];
    unsigned char           i;
    unsigned char           angle;                              //����� ��� ����
    signed char             x1,y1,z1;                           // ���������� ����� �� ������ �������
    float                   f;                                  // ��������� ����������
    char                    x2d[MESH_COUNT], y2d[MESH_COUNT];   // �������� �����
    pairsOfPoints_type      pairsOfPoints[MESH_COUNT];
    
    old_val_encoder = enGeReg();

    while(1){
        //����������� ����� ������, ������� ��� ����� ���� 
        //��������� � ������� ���������
        Xa[0] = -CUBE_SIZE; Ya[0] =  CUBE_SIZE; Za[0] =  CUBE_SIZE;
        Xa[1] =  CUBE_SIZE; Ya[1] =  CUBE_SIZE; Za[1] =  CUBE_SIZE;
        Xa[2] =  CUBE_SIZE; Ya[2] = -CUBE_SIZE; Za[2] =  CUBE_SIZE;
        Xa[3] = -CUBE_SIZE; Ya[3] = -CUBE_SIZE; Za[3] =  CUBE_SIZE;
        Xa[4] = -CUBE_SIZE; Ya[4] =  CUBE_SIZE; Za[4] = -CUBE_SIZE;
        Xa[5] =  CUBE_SIZE; Ya[5] =  CUBE_SIZE; Za[5] = -CUBE_SIZE;
        Xa[6] =  CUBE_SIZE; Ya[6] = -CUBE_SIZE; Za[6] = -CUBE_SIZE;
        Xa[7] = -CUBE_SIZE; Ya[7] = -CUBE_SIZE; Za[7] = -CUBE_SIZE;

        //������� ���� �����, ���������� ������� �������� ����������
        for (i = 0; i < DOTS_COUNT; i++){ //�� X
            y1 = Ya[i] ;
            z1 = Za[i] ;
            angle = dir_x ;
            Ya[i] =  (signed short)(cos_(angle) * y1 - sin_(angle) * z1);
            Za[i] =  (signed short)(cos_(angle) * z1 + sin_(angle) * y1);
        }

        for (i = 0; i < DOTS_COUNT; i++){ //�� Y
            x1 = Xa[i] ;
            z1 = Za[i] ;
            angle = dir_y ;
            Xa[i] =  (signed short)(cos_(angle) * x1 + sin_(angle) * z1);
            Za[i] = (signed short)(-sin_(angle) * x1 + cos_(angle) * z1);
        }

        for (i = 0; i < DOTS_COUNT; i++){ //� Z ��������!
            x1 = Xa[i] ;
            y1 = Ya[i] ;
            angle = dir_z ;
            Xa[i] =  (signed short)(cos_(angle) * x1 - sin_(angle) * y1);
            Ya[i] =  (signed short)(cos_(angle) * y1 + sin_(angle) * x1);
        }

        //������������� ��������� ������ � ��������
        for (i = 0; i < DOTS_COUNT ; i++){
            //1000 � 1200 ���������� ���������� ��  ������� �� ������ � 
            f = 1000 / (1100 -  (float)Za[i]);
            // ������ ������ � ������� � ��-������ ������
            x2d[i] = (unsigned char)((f * (float)Xa[i]) + LCD_X_SIZE/2);
            y2d[i] = (unsigned char)((f * (float)Ya[i]) + LCD_Y_SIZE/2);
        }

        lcd_FillScreen(black);// �������
        //������ �����/�����
        for(i = 0; i < MESH_COUNT; i++){
            grf_line(   pairsOfPoints[i].x1, pairsOfPoints[i].y1,       //������ ����� ������ ������
                        pairsOfPoints[i].x2, pairsOfPoints[i].y2,
                        black);
            
            pairsOfPoints[i].x1 = x2d[s1[i]];
            pairsOfPoints[i].y1 = y2d[s1[i]];
            pairsOfPoints[i].x2 = x2d[f1[i]];
            pairsOfPoints[i].y2 = y2d[f1[i]];
            
            grf_line(   pairsOfPoints[i].x1, pairsOfPoints[i].y1,       //������ �����
                        pairsOfPoints[i].x2, pairsOfPoints[i].y2,
                        colors[i]);
        }

        //������������� ����� ���� -
        //�������������� ����
        if(dir_x++ >= 63) dir_x = 0;
        if(dir_y++ >= 63) dir_y = 0;  
        if(dir_z++ >= 63) dir_z = 0;

        if(BtScan() || (old_val_encoder != enGeReg())){	//
            BeepTime(100);
            xTaskCreate(baseTSK, "baseTSK", BASE_TSK_SZ_STACK, NULL, 2, NULL);
            vTaskDelete(NULL); //������� ������� ������
        }
        vTaskDelay(30);
    }
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
