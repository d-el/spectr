/*
******************************************************************************
* @file			TemTSK.c
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			10.01.2015
* @brief		
******************************************************************************  
*/
#include "globalIncludes.h"

//������ ��� ������ � ������������.
//������������� ����, ���� ���� ������ - ��������� ����� � �������� ������ �� ��������.
//���� ��� ��, �� ��������� �����������, ������������� �������� �������, ��������� ����� ��������� �����������.
//���� ���������� >= MAX_TEMP, �� ��������� ����� � �������� ������ �� ��������.
//����� � ������� �������� ���������� � tem.integer, tem.frac ��������������.
void temTSK(void *pvParameters){
    uint16_t    pwmTsk;
    uint8_t     buff[2];
    owSt_type   st;
    
	while(1){
        vTaskDelay(1000);
        
        if(xSemaphoreTake(oneWireBusyMutex, portMAX_DELAY ) == pdTRUE){
        
            owch1_init();
            TATOMIC(st = ow_init());		                        //������������� ���� 1-wire  
            
            if(st == owOk){                                         //���� ��� ������ - ������ ��������
                
                TATOMIC(ow_write(SKIP_ROM));		                //SKIP ROM
                TATOMIC(ow_write(READ_SCRATCHPAD));		            //Read Scratchpad (spad) 
                TATOMIC(buff[0] = ow_read());	                    //LSB //������ ������� ���� ������
                TATOMIC(buff[1] = ow_read());	                    //MSB //������ ������� ���� ������
                TATOMIC(ow_init());                                 //������������� ���� 1-wire 	  
                TATOMIC(ow_write(SKIP_ROM));                        //SKIP ROM
                TATOMIC(ow_write(CONVERT_T));                       //Convert T
    
                tmprDecode(buff[1], buff[0]);
            
                pwmTsk = (uint16_t)tem.integer * DP - (MIN_TEMP * DP - COOLER_PWM_START);  //�������� ��������, �������� �� �������� � ������� ����� �� ���������� ������ �������
                if(pwmTsk < COOLER_PWM_START){
                    pwmTsk = COOLER_PWM_START;
                }
                
                if(tem.integer >= TEMP_OFF){
                    setFanPwm(COOLER_PWM_MAX);
                    switchOFF();
                    //���������� ����������� ��������� ����������
                    psStatus.bit.OvfLinearRegTemper = 1;
                }
                
                if(sysset.stopsFan != 0){    //� ���������
                    if(tem.integer <= sysset.lowTemp){
                        setFanPwm(0);
                    }
                    if(tem.integer >= sysset.hiTemp){
                        setFanPwm(pwmTsk);
                    }
                }
                else{       //��� ��������
                    setFanPwm(pwmTsk);
                }
                
                tem.update = 1;     //���� ����� ��� ����������� ���������
            }
            else{       //���� ��� �����
                switchOFF();
                setFanPwm(COOLER_PWM_MAX);
                //������ �������
                psStatus.bit.ErrorLinearRegTemperSens = 1;
            }
            
            xSemaphoreGive(oneWireBusyMutex);
        }
	}
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
