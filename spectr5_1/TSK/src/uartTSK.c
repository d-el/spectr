/**
*******************************************************************************
* @file    uartTSK.c 
* @author  D_EL
* @version V1.0.0
* @date    2015-08-10
* @brief
*/
#include "globalIncludes.h"

void uartTSK(void *pPrm){
    BaseType_t res;
    uint16_t   crc;
    uint8_t    numRx;
    uint8_t    numNeedTx;
    
    numRx = 0;
    uart_read(uart3, uart3->pRxBff, PieceBufRx);
    
    while(1){
        res = xSemaphoreTake(uartSem, pdMS_TO_TICKS(2));
        numRx += PieceBufRx - uart3->pUartRxDmaCh->CNDTR;
        
        if(numRx != 0){
            if(res == pdTRUE){  //Значит прочитали 4 байта
                uart_read(uart3, uart3->pRxBff + numRx, PieceBufRx);
            }else{  //Прием закончен
                uart_stopRead(uart3);
                crc = GetCrc(uart3->pRxBff, numRx);
                if(crc == 0){
                    numNeedTx = ModbusFunc(uart3->pRxBff, numRx);
                    uart_write(uart3, uart3->pRxBff, numNeedTx);
                    xSemaphoreTake(uartSem, pdMS_TO_TICKS(10));
                }else{
                    uart3->errorRxCnt++;
                }

                numRx = 0;
                uart_read(uart3, uart3->pRxBff, PieceBufRx);
            }
        }
    }
}

/************ (C) COPYRIGHT ************ END OF FILE ************ DEL ********/
