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

//Задача для работы с температурой.
//Инициализиует шину, если есть ошибка - выключает выход и включает куллер на максимум.
//Если все ок, то считывает температуру, устанавливает мощность куллера, запускает новое измерения температуры.
//Если темература >= MAX_TEMP, то выключает выход и включает куллер на максимум.
//Целое и дробное значение помещается в tem.integer, tem.frac соответственно.
void temTSK(void *pvParameters){
    uint16_t    pwmTsk;
    uint8_t     buff[2];
    owSt_type   st;
    
	while(1){
        vTaskDelay(1000);
        
        if(xSemaphoreTake(oneWireBusyMutex, portMAX_DELAY ) == pdTRUE){
        
            owch1_init();
            TATOMIC(st = ow_init());		                        //инициализация шины 1-wire  
            
            if(st == owOk){                                         //Если все хорошо - датчик работает
                
                TATOMIC(ow_write(SKIP_ROM));		                //SKIP ROM
                TATOMIC(ow_write(READ_SCRATCHPAD));		            //Read Scratchpad (spad) 
                TATOMIC(buff[0] = ow_read());	                    //LSB //читаем младший байт данных
                TATOMIC(buff[1] = ow_read());	                    //MSB //читаем старший байт данных
                TATOMIC(ow_init());                                 //инициализация шины 1-wire 	  
                TATOMIC(ow_write(SKIP_ROM));                        //SKIP ROM
                TATOMIC(ow_write(CONVERT_T));                       //Convert T
    
                tmprDecode(buff[1], buff[0]);
            
                pwmTsk = (uint16_t)tem.integer * DP - (MIN_TEMP * DP - COOLER_PWM_START);  //Выделяем разность, умножаем на крутизну и смещаем вверх до заполнения старта куллера
                if(pwmTsk < COOLER_PWM_START){
                    pwmTsk = COOLER_PWM_START;
                }
                
                if(tem.integer >= TEMP_OFF){
                    setFanPwm(COOLER_PWM_MAX);
                    switchOFF();
                    //Превышение температуры линейного регулятора
                    psStatus.bit.OvfLinearRegTemper = 1;
                }
                
                if(sysset.stopsFan != 0){    //С остановом
                    if(tem.integer <= sysset.lowTemp){
                        setFanPwm(0);
                    }
                    if(tem.integer >= sysset.hiTemp){
                        setFanPwm(pwmTsk);
                    }
                }
                else{       //Без останова
                    setFanPwm(pwmTsk);
                }
                
                tem.update = 1;     //Даем знать что температура обновлена
            }
            else{       //Если все плохо
                switchOFF();
                setFanPwm(COOLER_PWM_MAX);
                //Ошибка сенсора
                psStatus.bit.ErrorLinearRegTemperSens = 1;
            }
            
            xSemaphoreGive(oneWireBusyMutex);
        }
	}
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
