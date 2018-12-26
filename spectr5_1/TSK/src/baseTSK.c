/******************************************************************************
* @file			base.h
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			
* @brief		
*/
#include "globalIncludes.h"

/******************************************************************************
* Memory
*/
base_type   bs;
uint8_t     retEf = 0;
    
/******************************************************************************
* baseTSK
*/
void baseTSK(void *pPrm){
    portTickType    xLastWakeTime;
    uint32_t        i = 0, j = 0;
    uint16_t        IdleCount;                                          //Счетчик простоя, нужен для вызова заставки
    uint8_t         varParam = 0;
    uint8_t         fvarParamNew = 1;
    uint8_t         stHig, stDiv = 1;

    xLastWakeTime   = xTaskGetTickCount();                              //Инициализируем xLastWakeTime текущим временем
    IdleCount       = 0;
    uiGraphicsColor = black;
    uiTextColor     = R;
    IdleCount       = 0;
    lcd_Clear();
    
    if(ps.state.bit.scopeRun == 1){
        ps.state.bit.scopeRun = 0;
    }
    
    //Печать статических символов
    lcd_PutChar8x12(90,10,'V');
    lcd_PutChar8x12(90,32,'A');
    lcd_PutString8x12(12,45,"Imax=");
    uiTextColor = B;
    grf_line(0, 69, 100, 69, 1);
    uiTextColor = RGB;
    lcd_gotoxy(41,39,43,43);
    for(j = 0; j < 9; j++){
        lcd_data(uiTextColor);                         //точка
    }
    
    //Настройка клавиатуры
    kc.AutoPress = AutoPressON;
    kc.scanningToFirstRewind = 15;
    kc.scanningToRewind = 1;
    BeepTime(300);
    
    while(1){       
        /**************************************
        * Обработка кнопок
        */
        if(BtScan() != 0){
            BeepTime(T_BEEP_PRESS_KEY);
            IdleCount = 0;
        }
        if(ks.NEXT){
            varParam++; 
            fvarParamNew = 1;
            if(varParam >= 3) varParam = 0;
        }
        if(ks.BMDVW){
            if(ps.state.bit.switchIsON == 0){
                selWindow(chargerWindow);                                                 //Удаляем текущую задачу
            }else{
                selWindow(scopeWindow);  
            }
        }
        if(ks.SET){                             //Переключаем набор параметров
            if(ps.state.bit.switchIsON == 0){   //Если выключен выход
                bs.crst++;
                if(bs.crst > 4){
                    bs.crst = 0;
                }
                if(varParam == VAR_VOLT){
                    enco.pval = &bs.set[bs.crst].u;
                }
                if(varParam == VAR_CURR){
                    enco.pval = &bs.set[bs.crst].i;
                }
                if(varParam == VAR_MODE){
                    enco.pval = &bs.set[bs.crst].Mode;
                }
            }else{      //Если включен выход
                BeepTime(T_BEEP_ERROR);
            }
        }
        if(ks.SWITCH){
            if(ps.state.bit.switchIsON == 0){
                ps.task.switchRequest = switchOnRequest;
            }else{
                ps.task.switchRequest = switchOffRequest;
            }
        }
        if(ks.UP){
            enBigStepUp();
        }
        if(ks.DOWN){
            enBigStepDown();
        }
        if(ks.ZERO){
            enWriteVal(0);    //Обнуляем
        }
        
        /**************************************
        * Настройка клавиатуры, настройка энкодера
        */
        if(fvarParamNew != 0){
            if(varParam == VAR_VOLT){
                kc.AutoPress = AutoPressON;                                         //Разрешить автонажатие
                kc.scanningToRewind = 5;                                            //Колличество сканирований до повторного срабатывания
                enSet(0, MAX_VAL_U, 6, U_BIG_STEP,
                      enNONCIRC, enU16, &bs.set[bs.crst].u);
            }
            if(varParam == VAR_CURR){
                kc.AutoPress = AutoPressON;
                kc.scanningToRewind = 5;
                enSet(0, MAX_VAL_I, 2, I_BIG_STEP,
                      enNONCIRC, enU16, &bs.set[bs.crst].i);
            }
            if(varParam == VAR_MODE){
                kc.AutoPress = AutoPressOFF;
                enSet(0, 2, 1, 1,
                      enCIRC, enU16, &bs.set[bs.crst].Mode);
            }
        }
        
        //Вынимаем значение с энкодера
        enGet();
        
        /***************************************
        * Задание БП
        */
        ps.task.voltage = bs.set[bs.crst].u;
        switch(bs.set[bs.crst].Mode){
            case(baseImax):{
                ps.task.mode = mode_overcurrentShutdown;
                ps.task.current = bs.set[bs.crst].i;
            }break;
            case(baseILimitation):{
                ps.task.mode = mode_limitation;
                ps.task.current = bs.set[bs.crst].i;
            }break;
            case(baseUnprotected):{
                ps.task.mode = mode_limitation;
                ps.task.current = I_SHORT_CIRCUIT;
            }break;
        }
        
        if(ps.task.control != localControl){
            ps.task.switchRequest = switchOffRequest;
            lcd_PrintImage((void*)connImg);    //Заставка
            nvMem_savePrm(nvMem.nvMemBase);
            BeepTime(300);
            vTaskDelete(NULL);
        }

        //Запуск заставки
        if(ps.state.bit.switchIsON != 0){
            IdleCount = 0;
        }
        else                IdleCount++;
        if(IdleCount >= IDLE_TIME){
            retEf = 1;
            xTaskCreate(cube3dTSK,"cube3dTSK", CUBE_TSK_SZ_STACK, NULL, 2, NULL);
            vTaskDelete(NULL);                                              //Удаляем текущую задачу
        }
        
        /**************************************
        * Вывод на дисплей
        */
        //Печать курсора
        if(fvarParamNew !=0){
            uiTextColor = G;
            if(varParam == VAR_VOLT){
                lcd_PutChar8x12(2,56, ' ');                                     //Удаляем старый курсор
                lcd_PutChar8x12(2,5, '\x82');                                   //Рисуем новый курсор
            }
            if(varParam == VAR_CURR){
                lcd_PutChar8x12(2,5, ' ');
                lcd_PutChar8x12(2,45, '\x82');
            }
            if(varParam == VAR_MODE){
                lcd_PutChar8x12(2,45, ' ');
                lcd_PutChar8x12(2,56, '\x82');
            }
            fvarParamNew = 0;
        }

        //Печать значения напряжения
        uiTextColor = RGB;
        if(ps.state.bit.switchIsON !=0){
            sprintf(str, "%05u", ps.meas.voltage);
        }else{
            sprintf(str, "%05u", ps.task.voltage);
        }
        lcd_PutChar14x19(12, 2, str[0]);
        lcd_PutChar14x19(26, 2, str[1]);            
        lcd_gotoxy(41, 17, 43, 21);
        for(j = 0; j < 9; j++){
            lcd_data(uiTextColor);
        }            
        lcd_PutChar14x19(44, 2, str[2]);
        lcd_PutChar14x19(58, 2, str[3]);
        lcd_PutChar14x19(72, 2, str[4]);  

        //Печать значения тока
        if(ps.state.bit.switchIsON !=0){
            sprintf(str, "%04u", ps.meas.current);
            lcd_PutChar14x19(26, 24, str[0]);        
            lcd_PutChar14x19(44, 24, str[1]);
            lcd_PutChar14x19(58, 24, str[2]);
            lcd_PutChar14x19(72, 24, str[3]);
        }else{  //Если выключено
            lcd_PutChar14x19(26, 24, '-');
            lcd_PutChar14x19(44, 24, '-');
            lcd_PutChar14x19(58, 24, '-');
            lcd_PutChar14x19(72, 24, '-');
        }
        
        //Псевтоаналоговая шкала
        if(ps.state.bit.switchIsON !=0){
            i = adcStct.adcreg[CH_IADC] - sysset.pI[0].adc;
            stHig = i / stDiv;
            while(stHig > DISP_H){
                stDiv = stDiv * 2;
                stHig = i / stDiv;
            }
            while((stHig <= DISP_H / 2 - 10)&&(i > DISP_H / 2)){ //-10 гистерезис
                stDiv = stDiv / 2;
                if(stDiv == 0){
                    stDiv = 1;
                    break;
                }
                stHig = i / stDiv;
            }
            if(stHig > DISP_H)  stHig = DISP_H;   
            lcd_gotoxy(100, 0, 100, 79);
            for(j = 0; j < 80 - stHig; j++)
                lcd_data(uiGraphicsColor);
            for(j = 0; j < stHig; j++)
                lcd_data(uiTextColor);          
        }else{  //Если выключено
            lcd_gotoxy(100, 0, 100, 79);
            for(j = 0; j < 80; j++){
                lcd_data(uiGraphicsColor);
            }
        }
        
        //Печать символа ограничения
        uiTextColor = G;
        if(ps.state.bit.modeIlim == 1){
            lcd_PutChar14x19(12,24,'\x2E');
        }else{
            lcd_PutChar14x19(12,24,'\x2F');                                 //печать символа ограничения            
        }
        
        //Печать текущий набор настроек
        uiTextColor = RGB;
        lcd_PutChar8x12(3, 28, bs.crst + '\x30' + 1);                         //Режимы от 0 до 4 показываем от 1 до 5

        //Печать значения Imax
        sprintf(str, "%04u", bs.set[bs.crst].i);                            //Печатаем удвоенное значение, т.к. единице оотвествует 2mA
        uiTextColor = R;
        lcd_PutChar8x12(58, 45, str[0]);
        lcd_PutChar8x12(66, 45, '.');
        lcd_PutChar8x12(74, 45, str[1]);
        lcd_PutChar8x12(82, 45, str[2]);
        lcd_PutChar8x12(90, 45, str[3]);
          
        //Печать режима по току
        uiTextColor = GB;
        if(bs.set[bs.crst].Mode == baseImax){
            lcd_PutString8x12(12, 56, "I max      ");
        }
        if(bs.set[bs.crst].Mode == baseILimitation){
            lcd_PutString8x12(12, 56, "Limitation ");
        }
        if(bs.set[bs.crst].Mode == baseUnprotected){
            lcd_PutString8x12(12, 56, "UNPROTECTED");
        }

        printStatusBar();
                    
        /*************************************/
        vTaskDelayUntil(&xLastWakeTime, BASE_TSK_PERIOD);                       //Запускать задачу каждые 30ms
    }
}

void printStatusBar(void){
    int32_t         power;
    static uint8_t  errPrev = 0;
    static uint8_t  modeIlimPrev = 0;
    //static uint8_t  cntLimState = 0;
    
    if(modeIlimPrev != ps.state.bit.modeIlim){
        BeepTime(T_BEEP_CC_CV);
        modeIlimPrev = ps.state.bit.modeIlim;
    }
    
    if(ps.error.all != 0){   //Если есть ошибки                                                            
        //BeepTime(30);
        uiTextColor = RGB;
        if(errPrev == 0){
            grf_fillRect(0, 71, 101, 8, black);
            errPrev = 0;
        }
        
        if(ps.error.bit.errorLinearRegTemperSens != 0){
            lcd_PutString6x8(0, 71, "ErTempSens");
        }
        if(ps.error.bit.errorSps != 0){
            lcd_PutString6x8(0, 71, "ErSpsConnect");
        }
        if(ps.error.bit.ovfLinearRegTemper != 0){
            lcd_PutString6x8(0, 71, "OvfTemp");
        }
        if(ps.error.bit.ovfSpsTemper != 0){
            lcd_PutString6x8(0, 71, "OvfSpsTemper");
        }
        errPrev = 1;
    }else{  //Аварий нету
        uiTextColor = RGB;
        if(errPrev != 0){
            grf_fillRect(0, 71, 101, 8, black);
            errPrev = 0;
        }
        
        //Выходная мощность
        power = IQtoInt(adcStct.outPower, 1000);
        if(power < 1000){
            sprintf(str, "%umW  ", power);
        }else{
            sprintf(str, "%u.%uW ", power/1000, (power%1000)/100);
        }
        lcd_PutString6x8(0, 71, str);
        
        //Печать температуры
        sprintf(str, "%02u.%u°  ", ps.meas.temperatureLin/10, ps.meas.temperatureLin%10);
        lcd_PutString6x8(38, 71, str);
        tem.update = 0;
        
        //MHV
        if(sps.scratchpad.stateSps.mhvPresent != 0){
            sprintf(str, "%uV ", ps.meas.hvoltage);
            lcd_PutString6x8(73, 71, str);
        }else{
            uiTextColor = RGB;
            lcd_PutString6x8(73, 71, "    ");
        }
        
        errPrev = 0;
    }
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
