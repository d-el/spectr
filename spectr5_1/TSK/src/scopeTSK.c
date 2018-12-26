/*
******************************************************************************
* @file			scope.c
* @author		D_EL - Storozhenko Roman
* @version      V1.1
* @date			14.04.2015
* @brief		Функции
******************************************************************************  
*/
#include "globalIncludes.h"

void scopeTSK(void *pPrm){
    uint16_t		j;
    uint16_t        timeScale;
    uint16_t        timeScaleOld;
    uint16_t        ampDiv;
    uint16_t        ampDivOld;
    uint16_t        MA_filter_WITH_Old, recursiveK_Old;
    uint16_t        sampleRateOld;
    uint8_t         xIndex;
    uint16_t        yPos;
    uint16_t        i;
    
    ps.state.bit.scopeRun = 1;
    vTaskDelay(1);
    
    timeScale               = 1000;
    timeScaleOld            = 0;
    ampDiv                  = 1;
    ampDivOld               = 0;
    MA_filter_WITH_Old      = adcStct.adcFilt[CH_IADC].MA_filter_WITH;
    recursiveK_Old          = adcStct.adcFilt[CH_IADC].recursiveK;
    sampleRateOld           = adcStct.sampleRate;
    adcStct.adcFilt[CH_IADC].MA_filter_WITH = 1;
    adcStct.adcFilt[CH_IADC].recursiveK     = 0;
    xIndex                  = 0;
    adcStct.sampleRate      = timeScale;
    uiTextColor             = RGB; 
    uiGraphicsColor         = black;
    
    lcd_Clear();
    enSet(150, 65000, 1, 500,
          enNONCIRC, enU16, &timeScale);
    enWriteVal(timeScale);

    //Вертикальная линия
    for(j=0;j<64;j+=8){	                                        
        lcd_gotoxy(0,(79-j),0,79);
        lcd_data(RGB);
    }
    //Горизонтальная линия
    for(j=0;j<100;j+=12){	                                    
        lcd_gotoxy(j,79,100,79);
        lcd_data(RGB);
    }
    
    //Статические надписи
    lcd_PutString6x8(32, 3, "ms");
    lcd_PutString6x8(74, 3, "mA");
    
    kc.AutoPress = AutoPressOFF;
    
    while(1){
        //Обработка кнопок			
        if(BtScan()){
            BeepTime(30);
        }
        if(ks.ANY) BeepTime(100);
        if(ks.BMDVW){
            adcStct.adcFilt[CH_IADC].MA_filter_WITH = MA_filter_WITH_Old;
            adcStct.adcFilt[CH_IADC].recursiveK     = recursiveK_Old;
            adcStct.sampleRate = sampleRateOld;
            if(sysset.spsUse == 1){
                vTaskResume(spsTskHandle);
            }
            selWindow(baseWindow);                                 //Удаляем текущую задачу
        }
        if(ks.UP){
            enBigStepUp();
        }
        if(ks.DOWN){
            enBigStepDown();
        }
        
        xSemaphoreTake(AdcSem, portMAX_DELAY);
        
        lcd_gotoxy(xIndex + 1,16-1, xIndex+1, 79-1);            //Чистим столбец
        for(j = 0; j < 64; j++){
            lcd_data(RGB);
        }
        
        i = IQtoInt(adcStct.current, 1000);
        yPos = i / ampDiv;
        if(yPos > 63){
            ampDiv = i / 64 + 1;
            yPos = 63;
        }
        //yPos от 0 до 63 включительно
        lcd_gotoxy(xIndex + 1, 79 - yPos - 1, 100, 79);
        SPI2->DR = black;                                       //Загружаем данные для передачи
        while (!(SPI2->SR & SPI_SR_TXE));                       //Ожидание окончания передачи
        
        if(++xIndex > 99)   xIndex = 0;
        enGet();
        
        if(ampDiv != ampDivOld){
            sprintf(str, "%4u", ampDiv * 64);
            lcd_PutString6x8(50, 3, str);
            ampDivOld = ampDiv;
        }
        if(timeScale != timeScaleOld){
            sprintf(str, "%03u.%01u", timeScale/10, timeScale%10);
            lcd_PutString6x8(0, 3, str);
            adcStct.sampleRate = timeScale; //Настраиваем на новый период семплирования
            timeScaleOld = timeScale;
        }
    }
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
