/**
******************************************************************************
* @file			globalIncludes.h
* @author		Storozhenko Roman
******************************************************************************  
*/
#ifndef GLOBALINCLUDES_H
#define GLOBALINCLUDES_H

/**************************************
* std lib
*/
#include "stm32f10x.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "math.h"

/**************************************
* LIB
*/
#include "IQmathLib.h"

/**************************************
* FreeRTOS
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/**************************************
* Code
*/
#include "main.h"
#include "OSinit.h"
#include "crc.h"
#include "specificMath.h"

/**************************************
* DRIVERS
*/
#include "drivers.h"
#include "drivers.h"
#include "flash.h"
#include "adc.h"
#include "initPeripheral.h"
#include "gpio.h"
#include "uart.h"
#include "owSlave.h"

/**************************************
* TASK
*/
#include "adcTSK.h"
#include "uartTSK.h"
#include "display.h"
#include "systemTSK.h"

/**************************************
* DEBUG
*/
#include "timeMeas.h"

#endif //GLOBALINCLUDES_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
