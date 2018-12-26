/******************************************************************************
* @file         main.c
* @author       D_EL - Storozhenko Roman
* @version      V1.0
* @date         24.08.2015
* @brief        FREQ = 8000000MHz, 8bit, 1stop bit
*/
#include <ioavr.h>
#include <intrinsics.h>
#include <stdio.h>
#include <stdint.h>
#include "iotiny10.h"

/******************************************************************************
* User define
*/
#define pinTx   0
#define txHi()  PORTB |= (1<<pinTx)
#define txLo()  PORTB &= ~(1<<pinTx);

#define SYSFREQ     500000 //[Hz]
#define BAUD        9600
#define CYCLES      (SYSFREQ / BAUD -8)

uint16_t    adcSum;
uint8_t     cntSample;
    
void uartTx(uint8_t dat)
{
   uint8_t mask;

    txLo();
    __delay_cycles(CYCLES);
    for(mask = 1; mask != 0; mask <<= 1){
        if((dat & mask) != 0){
            txHi();
        }else{
            txLo();
        }
        __delay_cycles(CYCLES - 2);
    }
    txHi();
    __delay_cycles(CYCLES);
    txHi();
}

int main(void)
{
    uint8_t     txDat;
    
    DDRB |= 1<<pinTx;       //GPIO
    txHi();

    //SYS CLOCK
    CCP    = 0xD8;          //Protected I/O register
    CLKPSR = (1<<CLKPS2)|(0<<CLKPS1)|(0<<CLKPS0);             //Clock Division Factor 16
    
    ACSR = (1<<ACD);    //Analog Comparator Disable
    SMCR = (1<<SM0);    //ADC noise reduction
     
    //CNT0
    TCCR0A = 0;
    TCCR0B = (1<<WGM02);  //CTC (Clear Timer on Compare) OCR0A
    TIMSK0 = (1<<OCIE0A);
    
    //ADC
    ADMUX  = (0<<MUX1)|(1<<MUX0);                           //ADC1 PB1
    ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS1);     //Enable ADC, ADC Auto Trigger Enable, ADC Interrupt Enable, Division Factor 4
    ADCSRB = (1<<ADTS1)|(1<<ADTS0);                         //Timer/Counter 0 Compare Match A
    DIDR0  = (1<<ADC0D)|(1<<ADC1D)|(1<<ADC2D)|(1<<ADC3D);   //Digital Input Disable Register

    OCR0A  = 1560;                  //Sempl period
    TCCR0B |= (1<<CS00);            //clkI/O/1 (No prescaling)
    
    asm("sei");
    
    while(1){
        if(cntSample == 32){
            txDat = adcSum >> 5;
            adcSum    = 0;
            cntSample = 0;
            
            /*****************************************
            * Отправляем по UART
            */
            uartTx(txDat);
            uartTx(~txDat);
        }
        asm("sleep");
    }
}

#pragma vector = ADC_vect
__interrupt void ADC_Vect(void)
{   
    adcSum += ADCL;
    cntSample++;
}

#pragma vector = TIM0_COMPA_vect
__interrupt void TIM0_COMPA_Vect(void)
{  
}
