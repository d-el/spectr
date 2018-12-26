/**
*******************************************************************************
* @file    gpio.c
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    09-05-2015
*******************************************************************************
*/
#include "gpio.h"

/******************************************************************************
*
*/
void gppin_init(GPIO_TypeDef *port, uint8_t npin, gpioMode_type mod){
    __IO uint32_t    *gpio_cr;
    
    uint8_t        pinInReg;
    
    if(npin <= 7){
        gpio_cr = &port->CRL;
        pinInReg = npin;
    }
    if((npin >= 8)&&(npin <= 15)){
        gpio_cr = &port->CRH;
        pinInReg = npin - 8;  
    }
    
    *gpio_cr &= ~(0x0F << (4 * pinInReg));       //Clear bit field
    
    if(mod == analogMode){
        //
    }
    if(mod == floatingInput){
        *gpio_cr |= (GPIO_FLOATING_INPUT << 4 * pinInReg + 2);
    }
    if(mod == inputWithPullUp){
        *gpio_cr |= (GPIO_WHITH_PULLUP_PULL_DOWN << 4 * pinInReg + 2);
        port->BSRR = (1 << npin);
    }
    if(mod == inputWithPullDown){
        *gpio_cr |= (GPIO_WHITH_PULLUP_PULL_DOWN << 4 * pinInReg + 2);
        port->BSRR = (1 << npin + 16);
    }
    if(mod == outPushPull){
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
    if(mod == outOpenDrain){
        *gpio_cr |= (GPIO_OUTPUT_OPEN_DRAIN << 4 * pinInReg + 2);
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
    if(mod == alternateFunctionOutputPushPull){
        *gpio_cr |= (GPIO_ALTERNATIV_OUTPUT_PUSH_PULL << 4 * pinInReg + 2);
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
    if(mod == alternateFunctionOutputOpenDrain){
        *gpio_cr |= (GPIO_ALTERNATIV_OUTPUT_OPEN_DRAIN << 4 * pinInReg + 2);
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    } 
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/