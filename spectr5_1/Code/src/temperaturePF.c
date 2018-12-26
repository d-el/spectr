/******************************************************************************
* @file    temperaturePF.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    14-09-2015
* @brief
*/
#include "globalIncludes.h"

/******************************************************************************
* Memory
*/
temperature_type   temperature;

/******************************************************************************
*
*/
void temperaturePF(void){
    uint16_t    scratchpad;
    uint8_t     *p;
    owSt_type   st;
    
    p = (uint8_t*)&scratchpad;
    
    owch1_init();
    TATOMIC(st = ow_init());		                        //инициализаци€ шины 1-wire  
    
    if(st == owOk){                                         //≈сли все хорошо - датчик работает        
        TATOMIC(ow_write(SKIP_ROM));		                //SKIP ROM
        TATOMIC(ow_write(READ_SCRATCHPAD));		            //Read Scratchpad (spad) 
        TATOMIC(*p++ = ow_read());	                        //LSB //читаем младший байт данных
        TATOMIC(*p = ow_read());	                        //MSB //читаем старший байт данных
        TATOMIC(ow_init());                                 //инициализаци€ шины 1-wire 	  
        TATOMIC(ow_write(SKIP_ROM));                        //SKIP ROM
        TATOMIC(ow_write(CONVERT_T));                       //Convert T

        temperature.temperature = (scratchpad * 10 + 8) / 16;   //ƒеление с округлением
        temperature.state = temp_Ok;
    }else{
        temperature.state = temp_ErrSensor;
    }
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
