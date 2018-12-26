/******************************************************************************
* @file			charge.c
* @author		D_EL - Storozhenko Roman
* @version      V1.0
* @date			
*/
#include "globalIncludes.h"

charge_type ch;

void chargeTSK(void *pPrm){
    portTickType    xLastWakeTime;
    uint32_t        peCent, peCentPrev;
    int16_t         current;
    uint8_t         VarParam = 0;
    uint8_t         fVarParamNew = 1;
    uint8_t         l_on, l_onPrev;
    
    peCentPrev = 0;
    l_on       = 0;
    l_onPrev   = 0;

    xLastWakeTime = xTaskGetTickCount();            // Инициализируем xLastWakeTime текущим временем
    lcd_Clear();

    uiTextColor = RGB; 
    //печать статических символов 
    lcd_PutString8x12(12,2,"Mode:");
    lcd_PutString8x12(12,14,"Time:");
    lcd_PutString8x12(12,26,"U:");
    lcd_PutChar8x12(85,26,'V');
    lcd_PutString8x12(12,38,"I:");
    lcd_PutChar8x12(85,38,'A');
    lcd_PutString8x12(12,50,"C:");
    lcd_PutString8x12(82,50,"Ah");

    kc.scanningToFirstRewind = 15;
    kc.scanningToRewind = 1;

    while(1){	
        //======== обработка кнопок ========//			
        if(BtScan() != 0){
            BeepTime(30);
        }
        if(ks.NEXT){
            VarParam++; 
            fVarParamNew = 1;
            if(VarParam == 4) VarParam = 0;
        }	
        if(ks.BMDVW){
            selWindow(baseWindow);
        }
        
        if(ks.SWITCH){
            if(ps.state.bit.switchIsON == 0){
                ps.task.switchRequest = switchOnRequest;
            }else{
                l_on = 0;
                ps.task.switchRequest = switchOffRequest;
            }
            peCentPrev = 0;
        }
        if(ks.UP){
            enBigStepUp();
        }
        if(ks.DOWN){
            enBigStepDown();
        }
        if(ks.ZERO){
            enWriteVal(0);	//Обнуляем
        }

        //Ставим прочерк, если режим Mode_Current
        if((ch.Mode == Mode_Current)&&(VarParam == C_TIME)){
            VarParam++;	                                    //Не разрешаем фоновое редактирование TIME
            lcd_PutChar8x12(2,12, ' ');	                    //Удаляем курсор
        }
        
        //Настройка клавиатуры, настройка энкодера, загрузка параметра в энкодер
        if(fVarParamNew != 0){
            if(VarParam == C_MODE){
                kc.AutoPress = AutoPressOFF;                //Запретить автонажатие
                enSet(0, 1, 1, 1,
                      enNONCIRC, enU16, &ch.Mode);          //Установка диапазона значения энкодера
            }
            if(VarParam == C_TIME){
                kc.AutoPress = AutoPressON;
                kc.scanningToRewind = 5;
                enSet(0, 999, 1, 60,
                      enNONCIRC, enU16, &ch.Time);
            }
            if(VarParam == C_VOLT){
                kc.AutoPress = AutoPressON;
                kc.scanningToRewind = 5;
                enSet(0, MAX_VAL_U, 1, U_BIG_STEP,
                      enNONCIRC, enU16, &ch.Udac);
            }
            if(VarParam == C_CURR){
                kc.AutoPress = AutoPressON;
                kc.scanningToRewind = 5;
                enSet(0, MAX_VAL_I, 1, I_BIG_STEP,
                      enNONCIRC, enU16, &ch.Idac);
            }
        }
        
        if(ps.state.bit.switchIsON == 0){
            //Вынимаем значение с энкодера
            enGet();
        }

        ps.task.voltage = ch.Udac;
        ps.task.current = ch.Idac;
        ps.task.time    = ch.Time * 60;
        ps.task.mode    = (psMode_type)(ch.Mode + mode_timeShutdown);
        
        /**************************************
        * Вывод на дисплей
        */
        if(ps.state.bit.switchIsON == 0){
            //Печать курсора
            if(fVarParamNew != 0){
                if(VarParam == C_MODE){
                    lcd_PutChar8x12(2,38, ' ');                     //Удаляем старый курсор
                    lcd_PutChar8x12(2,2, 0x82);                     //Рисуем новый курсор
                }
                if(VarParam == C_TIME){
                    lcd_PutChar8x12(2,2, ' ');
                    lcd_PutChar8x12(2,14, 0x82);
                }
                if(VarParam == C_VOLT){
                    lcd_PutChar8x12(2,2, ' ');
                    lcd_PutChar8x12(2,14, ' ');
                    lcd_PutChar8x12(2,26, 0x82);
                }
                if(VarParam == C_CURR){
                    lcd_PutChar8x12(2,26, ' ');
                    lcd_PutChar8x12(2,38, 0x82);
                }
                fVarParamNew = 0;
            }
            //Печать значения режима
            if(ch.Mode == Mode_Time){
                lcd_PutString8x12(52,2,"Time   ");
            }
            if(ch.Mode == Mode_Current){
                lcd_PutString8x12(52,2,"VOLT.");
                lcd_PutString8x12(53,14," ---  ");
            }
            //Печать значения времени
            if(ch.Mode == Mode_Time){
            if(ch.Time <= 99){
                sprintf(str, "%um00s ", ch.Time);
            }else{
                sprintf(str, "%um0s ", ch.Time);
            }
                lcd_PutString8x12(53, 14, str);
            }
            
            //Печать значения напряжения
            sprintf(str, "%02u.%03u", ch.Udac/1000, ch.Udac%1000);
            lcd_PutString8x12(32, 26, str);

            current = ch.Idac;
            //Печать значения DAC_I 
            sprintf(str, "%01u.%03u", current/1000, current%1000);
            lcd_PutString8x12(40, 38, str);
            
            sprintf(str, "%01u.%03u", ps.meas.capacity/1000, ps.meas.capacity%1000);
            lcd_PutString8x12(40, 50, str);
        }
        else{   //ps.state.bit.switchIsON != 0
            //Печать курсора
            lcd_PutChar8x12(2,38, ' ');
            lcd_PutChar8x12(2,2, ' ');
            lcd_PutChar8x12(2,2, ' ');
            lcd_PutChar8x12(2,14, ' ');
            lcd_PutChar8x12(2,26, ' ');

            //Печать значения времени
            if(ch.Mode == Mode_Time){
                sprintf(str, "%um%02us  ", (ps.task.time - ps.meas.time)/60, (ps.task.time - ps.meas.time)%60);
                lcd_PutString8x12(53, 14, str);
            }
            
            //Печать значения напряжения
            sprintf(str, "%02u.%03u", ps.meas.voltage/1000, ps.meas.voltage%1000);
            lcd_PutString8x12(32, 26, str);

            current = ps.meas.current; 
            sprintf(str, "%01u.%03u", current/1000, current%1000);
            lcd_PutString8x12(40, 38, str);
            
            sprintf(str, "%01u.%03u", ps.meas.capacity/1000, ps.meas.capacity%1000);
            lcd_PutString8x12(40, 50, str);
            
            //Псевдоаналоговая шкала
            grf_rect(0, 62, 100, 6, RGB);
            if(ch.Mode != Mode_Time){
                peCent = (current * 100 + ch.Idac) / (ch.Idac *2);	        //Вычисление в процентах
            }else{
                peCent = (ps.meas.time * 100 + ps.task.time/2) / ps.task.time;	//Вычисление прошедшего времени в процентах
            }
            if(peCent > 100){
                peCent = 100;
            }
            if(peCent < peCentPrev){
                grf_fillRect(peCent +1, 63, peCentPrev - peCent, 4, black);
                //grf_fillRect(0, 56, 100, 6, black);
            }
            else{  //peCent >= peCentPrev
                grf_fillRect(1, 63, peCent, 4, R);
            }
            peCentPrev = peCent;
        }
        
        if((l_on == 1)&&(ps.state.bit.switchIsON == 0)){
            BeepTime(2000);
            grf_fillRect(0, 62, 100, 6, black);
            while(BtScan() == 0){
                printStatusBar();
                vTaskDelayUntil(&xLastWakeTime, CH_TSK_PERIOD);
            }
            lcd_PutString8x12(23, 55, "       ");
            fVarParamNew = 1;
            VarParam = C_MODE;
            l_on = 0;
        }
        if((l_onPrev != 0)&&(l_on == 0)){
            grf_fillRect(0, 62, 100, 6, black);
            fVarParamNew = 1;
            VarParam = C_MODE;
        }
        l_on = ps.state.bit.switchIsON;
        l_onPrev = l_on;
        
        printStatusBar();

        /*************************************/
        vTaskDelayUntil(&xLastWakeTime, CH_TSK_PERIOD);
    }
}

/******************* (C) COPYRIGHT ********** END OF FILE ********* D_EL *****/
