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
    BaseType_t  res;
    uint8_t     rxp, norxp;
    
    rxp = 0;
    norxp = 0;

    uart_read(uart1, uart1->pRxBff, PieceBufRx);
    
    while(1){
        res = xSemaphoreTake(uartSem, pdMS_TO_TICKS(80));
        
        if(res == pdTRUE){
            rxp = 1;
            norxp = 0;
            spStr.stateSps.mhvPresent = 1;
            uart_read(uart1, uart1->pRxBff, PieceBufRx);
        }else{
            if((rxp != 0)&&(PieceBufRx - uartGetRemainRx(uart1)) == 0){
                uart_stopRead(uart1);
                uint8_t t = ~uart1->pRxBff[1];
                if(uart1->pRxBff[0] == t){
                    spStr.hvoltage = uart1->pRxBff[0];
                    uart1->rxCnt++;
            if(uart1->pRxBff[0] < uart1->pRxBff[1]){
                uart1->errorRxCnt++;
            }
                }else{
                    uart1->errorRxCnt++;
                }
            }
            else{
                if(norxp < NoRxP){
                    norxp++;
                }else{
                    spStr.stateSps.mhvPresent = 0;
                }
            }
            rxp = 0;
            uart_read(uart1, uart1->pRxBff, PieceBufRx);
        }
    }
}

/************ (C) COPYRIGHT ************ END OF FILE ************ DEL ********/
