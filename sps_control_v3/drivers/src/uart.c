/**
******************************************************************************
* @file    uart.c 
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
#include "uart.h"

/******************************************************************************
* uart1 memory
*/
#if (UART1_USE == 1)
uart_type       uart1Sct;
uart_type       *uart1 = &uart1Sct;
uint8_t         uart1TxBff[UART1_TxBffSz];
uint8_t         uart1RxBff[UART1_RxBffSz];
#endif //UART1_USE

/******************************************************************************
* uart2 memory
*/
#if (UART2_USE == 1)
uart_type       uart2Sct;
uart_type       *uart2 = &uart2Sct;
uint8_t         uart2TxBff[UART2_TxBffSz];
uint8_t         uart2RxBff[UART2_RxBffSz];
#endif //UART2_USE

/******************************************************************************
* uart3 memory
*/
#if (UART3_USE == 1)
uart_type       uart3Sct;
uart_type       *uart3 = &uart3Sct;
uint8_t         uart3TxBff[UART3_TxBffSz];
uint8_t         uart3RxBff[UART3_RxBffSz];
#endif //UART3_USE

uint16_t usartBaudRate[3] = {
    0x9c4,                  //9600
    0x271,                  //38400
    //Добавить другие частоты
};

void uart_init(USART_TypeDef *USARTx, usartBaudRate_type baudRate){
    uart_type               *uartStruct;

    #if (UART1_USE == 1)
    if(USARTx == USART1){
        /************************************************
        * Memory setting
        */
        uartStruct = uart1;
        uartStruct->pUart        = USART1;
        uartStruct->pTxBff       = uart1TxBff;
        uartStruct->pRxBff       = uart1RxBff;
        uartStruct->pUartTxDmaCh = DMA1_Channel4;
        uartStruct->pUartRxDmaCh = DMA1_Channel5;
        
        /************************************************
        * IO
        */
        RCC->APB2ENR    |= RCC_APB2ENR_IOPAEN;                              //Enable clock for PORTA
        //PA9 USART1_TX
        gppin_init(GPIOA, 9, alternateFunctionOutputPushPull);
        //PA10 USART1_RX
        //gppin_init(GPIOA, 10, floatingInput);
        gppin_init(GPIOA, 10, inputWithPullUp);
        
        /************************************************
        * NVIC
        */
        NVIC_EnableIRQ(DMA1_Channel4_IRQn);                                 //Включить прерывания от DMA1_Channel 4
        NVIC_SetPriority(DMA1_Channel4_IRQn, UART1_TxDmaInterruptPrior);    //Установить приоритет
        NVIC_EnableIRQ(DMA1_Channel5_IRQn);                                 //Включить прерывания от DMA1_Channel 5
        NVIC_SetPriority(DMA1_Channel5_IRQn, UART1_RxDmaInterruptPrior);    //Установить приоритет
        
        /************************************************
        * USART clock
        */
        RCC->APB2ENR    |= RCC_APB2ENR_USART1EN;                            //USART1 clock enable
        RCC->APB2RSTR   |= RCC_APB2RSTR_USART1RST;                          //USART1 reset
        RCC->APB2RSTR   &= ~RCC_APB2RSTR_USART1RST;
        
        /************************************************
        * DMA clock
        */
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
    #endif //UART1_USE
    
    #if (UART2_USE == 1)
    if(USARTx == USART2){
        /************************************************
        * Memory setting
        */
        uartStruct = uart2;
        uartStruct->pUart        = USART2;
        uartStruct->pTxBff       = uart2TxBff;
        uartStruct->pRxBff       = uart2RxBff;
        uartStruct->pUartTxDmaCh = DMA1_Channel7;
        uartStruct->pUartRxDmaCh = DMA1_Channel6;
        
        /************************************************
        * IO
        */
        RCC->APB2ENR    |= RCC_APB2ENR_IOPAEN;                              //Enable clock for PORTA
        //PA2 USART1_TX
        gppin_init(GPIOA, 2, alternateFunctionOutputPushPull);
        //PA3 USART1_RX
        gppin_init(GPIOA, 3, floatingInput);
        
        /************************************************
        * NVIC
        */
        NVIC_EnableIRQ(DMA1_Channel7_IRQn);                                 //Включить прерывания от DMA1_Channel 7
        NVIC_SetPriority(DMA1_Channel7_IRQn, UART2_TxDmaInterruptPrior);    //Установить приоритет
        NVIC_EnableIRQ(DMA1_Channel6_IRQn);                                 //Включить прерывания от DMA1_Channel 6
        NVIC_SetPriority(DMA1_Channel6_IRQn, UART2_RxDmaInterruptPrior);    //Установить приоритет
        
        /************************************************
        * USART clock
        */
        RCC->APB1ENR    |= RCC_APB1ENR_USART2EN;                            //USART1 clock enable
        RCC->APB1RSTR   |= RCC_APB1RSTR_USART2RST;                          //USART1 reset
        RCC->APB1RSTR   &= ~RCC_APB1RSTR_USART2RST;
        
        /************************************************
        * DMA clock
        */
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
    #endif //UART2_USE
    
    #if (UART3_USE == 1)
    if(USARTx == USART3){
        /************************************************
        * Memory setting
        */
        uartStruct = uart3;
        uartStruct->pUart        = USART3;
        uartStruct->pTxBff       = uart3TxBff;
        uartStruct->pRxBff       = uart3RxBff;
        uartStruct->pUartTxDmaCh = DMA1_Channel2;
        uartStruct->pUartRxDmaCh = DMA1_Channel3;
        
        /************************************************
        * IO
        */
        #if (UART3_PINREMAP == 0)
        RCC->APB2ENR    |= RCC_APB2ENR_IOPBEN;                  //Enable clock for PORTB
        gppin_init(GPIOB, 10, alternateFunctionOutputPushPull); //PB10 USART3_TX
        gppin_init(GPIOB, 11, floatingInput);                   //PB11 USART3_RX
        #else
        RCC->APB2ENR    |= RCC_APB2ENR_IOPCEN;                  //Enable clock for PORTC
        AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_0;                 //Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
        gppin_init(GPIOC, 10, alternateFunctionOutputPushPull); //PC10 USART3_TX
        gppin_init(GPIOC, 11, floatingInput);                   //PC11 USART3_RX
        #endif
        /************************************************
        * NVIC
        */
        NVIC_EnableIRQ(DMA1_Channel2_IRQn);                                 //Включить прерывания от DMA1_Channel 2
        NVIC_SetPriority(DMA1_Channel2_IRQn, UART3_TxDmaInterruptPrior);    //Установить приоритет
        NVIC_EnableIRQ(DMA1_Channel3_IRQn);                                 //Включить прерывания от DMA1_Channel 3
        NVIC_SetPriority(DMA1_Channel3_IRQn, UART3_RxDmaInterruptPrior);    //Установить приоритет
        
        /************************************************
        * USART clock
        */
        RCC->APB1ENR    |= RCC_APB1ENR_USART3EN;                            //USART3 clock enable
        RCC->APB1RSTR   |= RCC_APB1RSTR_USART3RST;                          //USART3 reset
        RCC->APB1RSTR   &= ~RCC_APB1RSTR_USART3RST;
        
        /************************************************
        * DMA clock
        */
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
    #endif //UART3_USE
        
    /************************************************
    * USART
    */
    uartStruct->pUart->CR1   |= USART_CR1_UE;                                    //Включаем USART
    uartStruct->pUart->CR1   &= ~USART_CR1_M;                                    //Размерность слова данных - 8 бит
    uartStruct->pUart->CR2   &= ~USART_CR2_STOP;                                 //1 стоп-бит
    
    uartStruct->pUart->BRR   = usartBaudRate[baudRate];                          //Baud rate
    uartStruct->pUart->CR3   |= USART_CR3_DMAT;                                  //DMA enable transmitter
    uartStruct->pUart->CR3   |= USART_CR3_DMAR;                                  //DMA enable receiver
    
    uartStruct->pUart->CR1   |= USART_CR1_TE;                                    //Transmitter enable
    uartStruct->pUart->CR1   |= USART_CR1_RE;                                    //Receiver enable
    
    /************************************************
    * DMA
    */
    //DMA Channel USART TX
    uartStruct->pUartTxDmaCh->CCR |= DMA_CCR1_PL_0;                              //Channel priority level - Medium
    uartStruct->pUartTxDmaCh->CCR &= ~DMA_CCR1_MSIZE;                            //Memory size - 8 bit
    uartStruct->pUartTxDmaCh->CCR |= DMA_CCR1_PSIZE_1;                           //Peripheral size - 32 bit
    uartStruct->pUartTxDmaCh->CCR |= DMA_CCR1_MINC;                              //Memory increment mode enabled
    uartStruct->pUartTxDmaCh->CCR &= ~DMA_CCR1_PINC;                             //Peripheral increment mode disabled
    uartStruct->pUartTxDmaCh->CCR &= ~DMA_CCR1_CIRC;                             //Circular mode disabled
    uartStruct->pUartTxDmaCh->CCR |= DMA_CCR1_DIR;                               //Read from memory
    uartStruct->pUartTxDmaCh->CCR |= DMA_CCR1_TCIE;                              //Transfer complete interrupt enable
    uartStruct->pUartTxDmaCh->CNDTR = 0;                                         //Number of data
    uartStruct->pUartTxDmaCh->CPAR = (uint32_t)&(uartStruct->pUart->DR);         //Peripheral address
    uartStruct->pUartTxDmaCh->CMAR = (uint32_t)NULL;                             //Memory address
    
    //DMA Channel USART RX
    uartStruct->pUartRxDmaCh->CCR |= DMA_CCR1_PL_0;                              //Channel priority level - Medium
    uartStruct->pUartRxDmaCh->CCR &= ~DMA_CCR1_MSIZE;                            //Memory size - 8 bit
    uartStruct->pUartRxDmaCh->CCR |= DMA_CCR1_PSIZE_1;                           //Peripheral size - 32 bit
    uartStruct->pUartRxDmaCh->CCR |= DMA_CCR1_MINC;                              //Memory increment mode enabled
    uartStruct->pUartRxDmaCh->CCR &= ~DMA_CCR1_PINC;                             //Peripheral increment mode disabled
    uartStruct->pUartRxDmaCh->CCR &= ~DMA_CCR1_CIRC;                             //Circular mode disabled
    uartStruct->pUartRxDmaCh->CCR &= ~DMA_CCR1_DIR;                              //Read from peripheral
    uartStruct->pUartRxDmaCh->CCR |= DMA_CCR1_TCIE;                              //Transfer complete interrupt enable
    uartStruct->pUartRxDmaCh->CNDTR = 0;                                         //Number of data
    uartStruct->pUartRxDmaCh->CPAR = (uint32_t)&(uartStruct->pUart->DR);         //Peripheral address
    uartStruct->pUartRxDmaCh->CMAR = (uint32_t)NULL;                             //Memory address
}

/******************************************************************************
*
*/
void uart_write(uart_type *uartStruct, void *src, uint16_t len){
    uartStruct->pUartTxDmaCh->CCR &= ~DMA_CCR2_EN;                               //Channel disabled
    uartStruct->pUartTxDmaCh->CMAR = (uint32_t)src;                              //Memory address
    uartStruct->pUartTxDmaCh->CNDTR = len;                                       //Number of data
    uartStruct->pUartTxDmaCh->CCR |= DMA_CCR2_EN;                                //Channel enabled
    uartStruct->txState = uartTxRun;
}

/******************************************************************************
*
*/
void uart_read(uart_type *uartStruct, void *dst, uint16_t len){
    uartStruct->pUartRxDmaCh->CCR &= ~DMA_CCR2_EN;                               //Channel disabled
    (void)uartStruct->pUart->DR;
    (void)uartStruct->pUart->SR;
    uartStruct->pUartRxDmaCh->CMAR = (uint32_t)dst;                              //Memory address
    uartStruct->pUartRxDmaCh->CNDTR = len;                                       //Number of data
    uartStruct->pUartRxDmaCh->CCR |= DMA_CCR2_EN;                                //Channel enabled
    uartStruct->rxState = uartRxRun;
}

/******************************************************************************
*
*/
void uart_stopRead(uart_type *uartStruct){
    uartStruct->pUartRxDmaCh->CCR &= ~DMA_CCR2_EN;                               //Channel disabled
    uartStruct->rxState = uartRxStop;
}

/******************************************************************************
* Transfer complete interrupt DMA1_Channel4 (USART1 TX)
*/
#if (UART1_USE == 1)
__irq void DMA1_Channel4_IRQHandler(void){
    DMA1_Channel4->CCR &= ~DMA_CCR2_EN;                                     //Channel disabled
    uart1->txState  = uartTxSuccess;
    uart1->txCnt++;
    #if (UART1_Tx_HOOK == 1)
    extern void uart1TxHook( void );
    uart1TxHook();
    #endif //UART1_Tx_HOOK
    DMA1->IFCR      = DMA_IFCR_CTCIF4;                                      //Clear flag
}
/******************************************************************************
* Transfer complete interrupt DMA1_Channel5 (USART1 RX)
*/
__irq void DMA1_Channel5_IRQHandler(void){
    DMA1_Channel5->CCR &= ~DMA_CCR2_EN;                                     //Channel disabled
    uart1->rxState  = uartRxSuccess;
    #if (UART1_Rx_HOOK == 1)
    uart1RxHook();
    #endif //UART1_Rx_HOOK
    DMA1->IFCR      = DMA_IFCR_CTCIF5;                                      //Clear flag
}
#endif //UART1_USE

/******************************************************************************
* Transfer complete interrupt DMA1_Channel7 (USART2 TX)
*/
#if (UART2_USE == 1)
__irq void DMA1_Channel7_IRQHandler(void){
    DMA1_Channel4->CCR &= ~DMA_CCR2_EN;                                     //Channel disabled
    uart2->txState  = uartTxSuccess;
    uart2->txCnt++;
    #if (UART2_Tx_HOOK == 1)
    uart2TxHook();
    #endif //UART2_Tx_HOOK
    DMA1->IFCR      = DMA_IFCR_CTCIF4;                                      //Clear flag
}
/******************************************************************************
* Transfer complete interrupt DMA1_Channel6 (USART2 RX)
*/
__irq void DMA1_Channel6_IRQHandler(void){
    DMA1_Channel5->CCR &= ~DMA_CCR2_EN;                                     //Channel disabled
    uart2->rxState  = uartRxSuccess;
    uart2->rxCnt++;
    #if (UART2_Rx_HOOK == 1)
    uart2RxHook();
    #endif //UART2_Rx_HOOK
    DMA1->IFCR      = DMA_IFCR_CTCIF5;                                      //Clear flag
}
#endif //UART2_USE

/******************************************************************************
* Transfer complete interrupt DMA1_Channel2 (USART3 TX)
*/
#if (UART3_USE == 1)
__irq void DMA1_Channel2_IRQHandler(void){
    DMA1_Channel2->CCR &= ~DMA_CCR2_EN;                                     //Channel disabled
    uart3->txState  = uartTxSuccess;
    uart3->txCnt++;
    #if (UART2_Tx_HOOK == 1)
    uart3TxHook();
    #endif //UART3_Tx_HOOK
    DMA1->IFCR      = DMA_IFCR_CTCIF2;                                      //Clear flag
}
/******************************************************************************
* Transfer complete interrupt DMA1_Channel3 (USART3 RX)
*/
__irq void DMA1_Channel3_IRQHandler(void){
    DMA1_Channel5->CCR &= ~DMA_CCR2_EN;                                     //Channel disabled
    uart3->rxState  = uartRxSuccess;
    uart3->rxCnt++;
    #if (UART3_Rx_HOOK == 1)
    uart3RxHook();
    #endif //UART3_Rx_HOOK
    DMA1->IFCR      = DMA_IFCR_CTCIF3;                                      //Clear flag
}
#endif //UART3_USE

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
