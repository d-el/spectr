/**
******************************************************************************
* @file    uart.h 
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    17-04-2015
* @brief   
*
* UART1, use Tx DMA1_Channel4, use Rx DMA1_Channel5
* UART2, use Tx DMA1_Channel7, use Rx DMA1_Channel6
* UART3, use Tx DMA1_Channel2, use Rx DMA1_Channel3
******************************************************************************
*/
#include "globalIncludes.h"
#ifndef UART_H
#define UART_H

#include "stm32f10x.h"

/******************************************************************************
* User define
*/
//UART1
#define     UART1_USE                   0
#define     UART1_Tx_HOOK               0
#define     UART1_Rx_HOOK               0
#define     UART1_TxBffSz               32
#define     UART1_RxBffSz               32
#define     UART1_TxDmaInterruptPrior   15
#define     UART1_RxDmaInterruptPrior   15
#define     UART1_PINREMAP              0

//UART2
#define     UART2_USE                   0
#define     UART2_Tx_HOOK               0
#define     UART2_Rx_HOOK               0
#define     UART2_TxBffSz               32
#define     UART2_RxBffSz               32
#define     UART2_TxDmaInterruptPrior   15
#define     UART2_RxDmaInterruptPrior   15
#define     UART2_PINREMAP              0

//UART3
#define     UART3_USE                   1
#define     UART3_Tx_HOOK               1
#define     UART3_Rx_HOOK               1
#define     UART3_TxBffSz               1
#define     UART3_RxBffSz               256
#define     UART3_TxDmaInterruptPrior   15
#define     UART3_RxDmaInterruptPrior   15
#define     UART3_PINREMAP              1

/******************************************************************************
* User enum
*/

/******************************************************************************
* User typedef
*/
typedef enum{
    BR9600,
    BR38400
}usartBaudRate_type;

typedef enum{  
    uartTxFree,
    uartTxRun,
    uartTxSuccess,
    uartTxErr
}uartTxState_type;

typedef enum{  
    uartRxFree,
    uartRxRun,
    uartRxSuccess,
    uartRxStop,
    uartRxErr
}uartRxState_type;

typedef struct{
    USART_TypeDef           *pUart;
    uint8_t                 *pTxBff;
    uint8_t                 *pRxBff;
    DMA_Channel_TypeDef     *pUartTxDmaCh;
    DMA_Channel_TypeDef     *pUartRxDmaCh;
    uartTxState_type        txState     :8;
    uartRxState_type        rxState     :8;
    uint16_t                txCnt;
    uint16_t                rxCnt;
    uint16_t                errorRxCnt;
}uart_type;

/******************************************************************************
* Extern viriables
*/
#if (UART1_USE == 1)
extern uart_type            *uart1;
#endif //UART1_USE

#if (UART2_USE == 1)
extern uart_type            *uart2;
#endif //UART2_USE

#if (UART3_USE == 1)
extern uart_type            *uart3;
#endif //UART3_USE

/******************************************************************************
* Macro functions
*/
#define uartGetRemainRx(uartx)      (uartx->pUartRxDmaCh->CNDTR)
#define uart3RxHook(){                                                      \
    BaseType_t  xHigherPriorityTaskWoken;                                   \
    xHigherPriorityTaskWoken = pdFALSE;                                     \
    xSemaphoreGiveFromISR(uartSem, &xHigherPriorityTaskWoken);              \
    if(xHigherPriorityTaskWoken != pdFALSE){                                \
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);                    \
    }                                                                       \
}
#define uart3TxHook()    uart3RxHook()   

/******************************************************************************
* Prototypes for the functions
*/
void uart_init(USART_TypeDef *USARTx, usartBaudRate_type baudRate);
void uart_write(uart_type *uartx, void *src, uint16_t len);
void uart_read(uart_type *uartx, void *dst, uint16_t len);
void uart_stopRead(uart_type *uartStruct);

#endif //UART_H
/**************** (C) COPYRIGHT ************* END OF FILE ********* D_EL *****/
