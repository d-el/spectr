/******************************************************************************
* @file    drivers.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    05-07-2013
*/
#include "globalIncludes.h"

/******************************************************************************
* MEMORY
*/
syssetting_type     sysset;
uint16_t            base[] = {
    4095
};

/******************************************************************************
* >>> Обработчик прерывания PVD
*/
__irq void PVD_IRQHandler(void){
    switchOFF();
    if(ps.state.bit.osInit != 0){
        nvMem_savePrm(nvMem.nvMemBase);
    }
    lcd_disable();                  //Выключаем LCD
    LED_ON();
    setFanPwm(500);
    BeepTime(100);
    while(1);
}

/******************************************************************************
* >>>Обработчик внешнего прерывания 1
*/
__irq void EXTI1_IRQHandler(void){
    switchOFF();
    ps.state.bit.ovfCurrent = 1;
    EXTI->PR    |= EXTI_PR_PR1;     //Очищаем флаг записью единици
}

/******************************************************************************
*
*/
void Init_Hard(void){
    Init_CLK();
    gpio_init();
    externalInterrupt_CcCv_init();
    
    InitDelTim();
    delay_ms(100);
    Init_Beep();
    Init_Timer4();              //FAN_PWM
    Init_Encoder();
    InitAdc();
    Init_DAC_CH1();
    Init_DAC_CH2();
    Init_DS18B20();
    Init_PVD();
    Init_EXTI1();
    Init_SPI2();
    uart_init(USART3, BR38400);
    lcd_init ();                //Глючная инициализация
    lcd_init ();
    lcd_init ();
    lcd_init ();
    
    timMeasInit();  //DEBUG
}

/******************************************************************************
*
*/
void switchON(void){
    _gppin_reset(GPIOB, pinm12);
    ps.state.bit.switchIsON = 1;
}

/******************************************************************************
*
*/
void switchOFF(void){
    _gppin_set(GPIOB, pinm12);
    ps.state.bit.switchIsON = 0;
}

/******************************************************************************
* Записывает параметры по умолчанию
*/
void prmInitDef(void){
    uint8_t i;
    
    ps.error.bit.noCalibration = 1;
    
    sysset.kKorHighvoltage  = 1125;
    sysset.kLval            = 10000;
    sysset.hiTemp           = 50;
    sysset.lowTemp          = 30;
    sysset.freqBeep         = 4000;
    sysset.lcdContrast      = 100;
    sysset.spsUse   = 0;            //Используется sps
    sysset.stopsFan = 1;            //Не будет останавливатся куллер
    
    /*******************************
    * U
    */
    sysset.pU[0].qu     = _IQ(0.039);
    sysset.pU[0].adc    = 60;
    sysset.pU[0].dac    = 31;

    sysset.pU[1].qu     = _IQ(0.999);
    sysset.pU[1].adc    = 384;
    sysset.pU[1].dac    = 193;
    
    /*******************************
    * I
    */
    sysset.pI[0].qi     = _IQ(0.002);
    sysset.pI[0].adc    = 50;
    sysset.pI[0].dac    = 25;

    sysset.pI[1].qi     = _IQ(0.061);
    sysset.pI[1].adc    = 110;
    sysset.pI[1].dac    = 55;

    /**************************************
    * BS
    */
    for(i = 0; i < NPRESET; i++){
        bs.set[i].i    = I_BIG_STEP;
        bs.set[i].u    = U_BIG_STEP;
        bs.set[i].Mode = baseImax;
    }

    /**************************************
    * CH
    */  
    ch.Idac = I_BIG_STEP;
    ch.Mode = 0;
    ch.Time = 60;
    ch.Udac = U_BIG_STEP;
}

/******************* (C) COPYRIGHT ********** END OF FILE ********* D_EL *****/
