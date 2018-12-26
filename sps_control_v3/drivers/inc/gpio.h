/**
*******************************************************************************
* @file    gpio.c
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    09-05-2015
*******************************************************************************
*/
#ifndef GPIO_H
#define GPIO_H
#include "globalIncludes.h"

/******************************************************************************
* User define
*/
//In input mode (MODE[1:0]=00):
#define GPIO_ANALOG_MODE                    0x00
#define GPIO_FLOATING_INPUT                 0x01
#define GPIO_WHITH_PULLUP_PULL_DOWN         0x02
//In output mode (MODE[1:0] > 00):
#define GPIO_OUTPUT_PUSH_PULL               0x00
#define GPIO_OUTPUT_OPEN_DRAIN              0x01
#define GPIO_ALTERNATIV_OUTPUT_PUSH_PULL    0x02
#define GPIO_ALTERNATIV_OUTPUT_OPEN_DRAIN   0x03

/******************************************************************************
* User typedef
*/
typedef enum{
    pinm0   = 1 << 0,
    pinm1   = 1 << 1,
    pinm2   = 1 << 2,
    pinm3   = 1 << 3,
    pinm4   = 1 << 4,
    pinm5   = 1 << 5,
    pinm6   = 1 << 6,
    pinm7   = 1 << 7,
    pinm8   = 1 << 8,
    pinm9   = 1 << 9,
    pinm10  = 1 << 10,
    pinm11  = 1 << 11,
    pinm12  = 1 << 12,
    pinm13  = 1 << 13,
    pinm14  = 1 << 14,
    pinm15  = 1 << 15,
}gpioPin_type;

typedef enum{
    analogMode,
    floatingInput,
    inputWithPullUp,
    inputWithPullDown,
    outPushPull,
    outOpenDrain,
    alternateFunctionOutputPushPull,
    alternateFunctionOutputOpenDrain
}gpioMode_type;

typedef enum{
    speed2MHz,
    speed10MHz,
    speed50MHz,
}gpioSpeed_type;

/******************************************************************************
* Macro functions
*/
#define gppin_set(port, pinMask)        (port->BSRR = (pinMask))
#define gppin_reset(port, pinMask)      (port->BRR  = (pinMask))
#define gppin_get(port, pinMask)        (port->IDR & (pinMask))

/******************************************************************************
* Prototypes for the functions in gpio.c
*/
void gppin_init(GPIO_TypeDef *port, uint8_t npin, gpioMode_type mod);

#endif //GPIO_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
