/******************************************************************************
* @file			globalIncludes.h
* @author		Storozhenko Roman
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
* PrmSystem
*/
#include "prmTypes.h"
#include "ModbusTypes.h"
#include "menu_b.h"
#include "MakeOpis.h"
#include "ModbusSlave.h"

/**************************************
* FreeRTOS
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/**************************************
* Code
*/
#include "main.h"
#include "temperaturePF.h"
#include "spsPF.h"
#include "cube3d.h"
#include "graphics.h"
#include "setting.h"
#include "bubbles.h"
#include "image.h"
#include "fonts.h"
#include "OSinit.h"
#include "crc.h"
#include "specificMath.h"

/**************************************
* DRIVERS
*/
#include "drivers.h"
#include "beep.h"
#include "LPH8731.h"
#include "ds18b20.h"
#include "onewire.h"
#include "drivers.h"
#include "flash.h"
#include "enco.h"
#include "key.h"
#include "adc.h"
#include "initPeripheral.h"
#include "gpio.h"
#include "delay.h"
#include "spsControl.h"
#include "uart.h"

/**************************************
* TASK
*/
#include "adcTSK.h"
#include "systemTSK.h"
#include "baseTSK.h"
#include "scopeTSK.h"
#include "chargeTSK.h"
#include "temTSK.h"
#include "spsTSK.h"
#include "uartTSK.h"

/**************************************
* DEBUG
*/
#include "timeMeas.h"

#endif //GLOBALINCLUDES_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
