/**
*******************************************************************************
* @file    gpio.c
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    09-05-2015
*******************************************************************************
*/
#include "gpio.h"

/*!****************************************************************************
* MEMORY
*/
pinMode_type    pinsMode[] = {
/*0 */makepin(GPIOB,    11, inputWithPullUp,    0),  //B_MODE
/*1 */makepin(GPIOB,    10, inputWithPullUp,    0),  //B_SET
/*2 */makepin(GPIOB,    2,  inputWithPullUp,    0),  //B_SWITCH
/*3 */makepin(GPIOB,    1,  inputWithPullUp,    0),  //B_NEXT
/*4 */makepin(GPIOC,    15, inputWithPullUp,    0),  //B_UP
/*5 */makepin(GPIOC,    3,  inputWithPullUp,    0),  //B_DOWN
/*6 */makepin(GPIOA,    2,  inputWithPullUp,    0),  //B_ZERO
/*7 */makepin(GPIOB,    12, outOpenDrain,	    1),  //ONOFF
/*8 */makepin(GPIOA,    3,  floatingInput,      0),  //CC_CV
/*9 */makepin(GPIOA,    9,  outOpenDrain,	    1),  //DS18B20
/*10*/makepin(GPIOC,    9,  outPushPull,	    0),  //LED
};
uint32_t pinNum = sizeof(pinsMode) / sizeof(pinMode_type);
 
/*!****************************************************************************
* @brief    interrupt handler CC_CV
*/
void EXTI3_IRQHandler(void){
    switchOFF();
    ps.state.bit.ovfCurrent = 1;
    EXTI->PR    |= EXTI_PR_PR3;
}

/*!****************************************************************************
* @brief    init CC_CV interrupt
*/
void externalInterrupt_CcCv_init(void){
    EXTI_INIT(pinsMode[GP_CC_CV].p, pinsMode[GP_CC_CV].npin, EXTI_MODE_RISE, EXTI_CC_CV_Priority);
}

/*!****************************************************************************
* @brief    Init All Gpio
*/
void gpio_init(void){
    RCC->APB2ENR    |= RCC_APB2ENR_AFIOEN;                      //Включили тактирование AFIO
#if (JTAG_OFF > 0)
    AFIO->MAPR      |= AFIO_MAPR_SWJ_CFG_1; //JTAG
#endif
    pinMode_type *pgpios;
    pinMode_type *pgpiosEnd;
    
    pgpios = (pinMode_type*)pinsMode;
    pgpiosEnd = pgpios + pinNum;
    
    while(pgpios < pgpiosEnd){
        gppin_init(pgpios->p, pgpios->npin, pgpios->mode, pgpios->iniState);
        pgpios++;
    }
}

/*!****************************************************************************
* @brief    init single gpio
*/
void gppin_init(GPIO_TypeDef *port, uint8_t npin, gpioMode_type mode, uint8_t iniState){
    __IO uint32_t    *gpio_cr;
    
    uint8_t        pinInReg;
    
    //Clock enable
    if(port == GPIOA)   RCC->APB2ENR    |= RCC_APB2ENR_IOPAEN;
    if(port == GPIOB)   RCC->APB2ENR    |= RCC_APB2ENR_IOPBEN;
    if(port == GPIOC)   RCC->APB2ENR    |= RCC_APB2ENR_IOPCEN;
    if(port == GPIOD)   RCC->APB2ENR    |= RCC_APB2ENR_IOPDEN;
    //if(pgpios->port == GPIOE)   RCC->APB2ENR    |= RCC_APB2ENR_IOPEEN;
    
    if(iniState != 0){
        port->BSRR = (1<<npin);
    }
    else{
        port->BRR = (1<<npin);
    }
    
    if(npin <= 7){
        gpio_cr = &port->CRL;
        pinInReg = npin;
    }
    if((npin >= 8)&&(npin <= 15)){
        gpio_cr = &port->CRH;
        pinInReg = npin - 8;  
    }
    
    *gpio_cr &= ~(0x0F << (4 * pinInReg));       //Clear bit field
    
    if(mode == analogMode){
    }
    if(mode == floatingInput){
        *gpio_cr |= (GPIO_FLOATING_INPUT << 4 * pinInReg + 2);
    }
    if(mode == inputWithPullUp){
        *gpio_cr |= (GPIO_WHITH_PULLUP_PULL_DOWN << 4 * pinInReg + 2);
        port->BSRR = (1 << npin);
    }
    if(mode == inputWithPullDown){
        *gpio_cr |= (GPIO_WHITH_PULLUP_PULL_DOWN << 4 * pinInReg + 2);
        port->BSRR = (1 << npin + 16);
    }
    if(mode == outPushPull){
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
    if(mode == outOpenDrain){
        *gpio_cr |= (GPIO_OUTPUT_OPEN_DRAIN << 4 * pinInReg + 2);
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
    if(mode == alternateFunctionOutputPushPull){
        *gpio_cr |= (GPIO_ALTERNATIV_OUTPUT_PUSH_PULL << 4 * pinInReg + 2);
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
    if(mode == alternateFunctionOutputOpenDrain){
        *gpio_cr |= (GPIO_ALTERNATIV_OUTPUT_OPEN_DRAIN << 4 * pinInReg + 2);
        *gpio_cr |= (0x03 << 4 * pinInReg); //50 MHz
    }
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/