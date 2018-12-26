/******************************************************************************
* @file    systemTSK.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    14-09-2015
* @brief
*/
#include "globalIncludes.h"

ps_type             ps;
selWindow_type		currentSelWindow;

/*!****************************************************************************
* @brief
*/
void selWindow(selWindow_type window){
	currentSelWindow = window;
	while(windowTskHandle != NULL){
		vTaskDelay(1000);
	}
}

/******************************************************************************
*
*/
void systemTSK(void *pPrm){
    BaseType_t 			Result = pdTRUE;
    portTickType        xLastWakeTime;      //Вемя ОС
    syssetting_type     *s;                 //Указатель на структуру с калибровками
    adcStct_type        *a;                 //Указатель на сруктуру с данными АЦП
    switchRequest_type  l_switchRequest;    //Локальный запрос на вкл / выкл выход
    typeContr_type      typeContrPrev;      //Предыдущий тип управления локальное / АСУ
    _iq                 qpwmTask;           //Заполнение ШИМ для венитлятора
    _iq                 qTask, qdac;        
    _iq14               qtemp;
    uint32_t            vTaskCumul;
    uint32_t            timeOffset;
    uint16_t            spsTime, temTime, ledTime;
    uint16_t            curOffTime;                                     //Время падения тока < 10%
    uint16_t            dac;
    selWindow_type      selWindowPrev = noneWindow;

    xLastWakeTime       = xTaskGetTickCount();                          //Инициализируем xLastWakeTime текущим временем
    s                   = &sysset;
    a                   = &adcStct;
    l_switchRequest     = switchNoRequest;
    typeContrPrev       = localControl;
    vTaskCumul          = 0;
    spsTime             = 0;
    temTime             = 0;
    ledTime             = 0;
    curOffTime          = 0;
    
    while(1){
        if(selWindowPrev != currentSelWindow){
    		if(windowTskHandle != NULL){
    			vTaskDelete(windowTskHandle);	//Удаляем текущее окно
    		}

    		switch(currentSelWindow){
    			case baseWindow:
    				Result = xTaskCreate(baseTSK, "baseTSK", BASE_TSK_SZ_STACK, NULL, BASE_TSK_PRIO, &windowTskHandle);
    				break;
    			case chargerWindow:
    				Result = xTaskCreate(chargeTSK, "chargeTSK", CHARG_TSK_SZ_STACK, NULL, CHARG_TSK_PRIO, &windowTskHandle);
    				break;
                case scopeWindow:
                    Result = xTaskCreate(scopeTSK,   "scopeTSK",     SCOPE_TSK_SZ_STACK, NULL, SCOPE_TSK_PRIO, &windowTskHandle);
                    break;
    			case cube3dWindow:
					Result = xTaskCreate(cube3dTSK, "cube3dTSK", CUBE_TSK_SZ_STACK, NULL, CUBE_TSK_PRIO, &windowTskHandle);
					break;
				default:
    				while(1);	//Fail windowTskHandle
    		}

    		if(Result != pdTRUE){
    			while(1);
    		}

    		selWindowPrev = currentSelWindow;
    	}
        
        /**************************************
        * Вызов периодических функций
        */
        if(sysset.spsUse != 0){
            if(spsTime++ >= (SPS_PROC_PERIOD / SYSTEM_TSK_PERIOD)){
                spsPF();
                spsTime = 0;
            }
        }
        if(temTime++ >= (TEM_PROC_PERIOD / SYSTEM_TSK_PERIOD)){
            temperaturePF();
            temTime = 0;
        }
        if(ledTime++ >= (LED_PROC_PERIOD / SYSTEM_TSK_PERIOD)){
            LED_TOGGLE();
            ledTime = 0;
        }
        
        /**************************************
        * Анализ SPS
        */
        /*if(sysset.spsUse != 0){
            if(sps.state == spsOk){
                ps.meas.temperatureSps = sps.scratchpad.temperature;
                ps.error.bit.errorSps = 0;
                ps.state.bit.mhvPresent = sps.scratchpad.stateSps.mhvPresent;
                if(ps.meas.temperatureSps > MAX_TEMP_SWS){
                    ps.error.bit.ovfSpsTemper = 1;          //Превышение температуры ИБП
                }else{
                    ps.error.bit.ovfSpsTemper = 0;
                }
            }
            else{
                ps.state.bit.mhvPresent = 0;
                ps.error.bit.errorSps = 1;
            }
        }*/
        
        /**************************************
        * Анализ датчика температуры линейного регулятора
        */
        if(temperature.state == temp_Ok){
            if(temperature.temperature > TEMP_OFF){
                ps.error.bit.ovfLinearRegTemper = 1;        //Превышение температуры линейного стабилизатора
            }else{
                ps.error.bit.ovfLinearRegTemper = 0;
            }
            ps.error.bit.errorLinearRegTemperSens = 0;
            ps.meas.temperatureLin = temperature.temperature;
        }
        else{
            ps.error.bit.errorLinearRegTemperSens = 1;
        }
        
        /**************************************
        * Проверяем на режим ограничения тока
        */
        if(ps.state.bit.switchIsON != 0){
            ps.state.bit.modeIlim = MODE_IS_CC();
        }else{
            ps.state.bit.modeIlim = 0;
        }
        
        /**************************************
        * Рассчет измерянного напряжения
        */
        a->voltage = s32iq_Fy_x1x2y1y2x(s->pU[0].adc, s->pU[1].adc, s->pU[0].qu, s->pU[1].qu, a->filtered[CH_UADC]);
        if(a->voltage < 0)  a->voltage = 0;
        
        /*****************************************
        * Рассчет сопротивления и мощности нагрузки
        */
        qtemp = _IQ14div(_IQtoIQ14(a->voltage), _IQtoIQ14(a->current));
        if(qtemp > _IQ14(100000)){  //100 kOhm
            qtemp = _IQ14(100000);
        }
        a->resistens = qtemp;
        a->outPower  = _IQmpy((a->voltage), (a->current));
        
        /*****************************************
        * Рассчет A/h от момента включения ключа
        */
        static uint64_t capacity;
        if(ps.state.bit.switchIsON != 0){
            capacity += a->current;
            if(capacity >= ((uint64_t)_IQ(0.001) * (1000000 / adcStct.sampleRate) * 60 * 60)){
                a->capacity += 1;
                capacity = capacity - ((uint64_t)_IQ(0.001) * (1000000 / adcStct.sampleRate) * 60 * 60);
            }
        }
        else{
            a->capacity = 0;
            capacity = 0;
        }
        
        /**************************************
        * Перекладываем измерянные значения
        */
        ps.meas.capacity  = a->capacity;
        ps.meas.current   = IQtoInt(a->current, 1000);
        ps.meas.voltage   = IQtoInt(a->voltage, 1000);
        ps.meas.power     = IQtoInt(a->outPower, 1000);
        ps.meas.resistens = IQ14toInt(a->resistens, 1000);
        ps.meas.hvoltage  = spsCodeToHV(sps.scratchpad.hvoltage, sysset.kKorHighvoltage);
        if(ps.state.bit.switchIsON != 0){
            ps.meas.time  = (xTaskGetTickCount() - timeOffset) / configTICK_RATE_HZ;
        }
        
        /**************************************
        * Регулятор Вентилятора
        */
        qpwmTask = iq_Fy_x1x2y1y2x(_IQ(MIN_TEMP), _IQ(MAX_TEMP),
                                   _IQ(COOLER_PWM_START), _IQ(1),
                                   (uint32_t)(((uint64_t)temperature.temperature << 24) / 10)
                                   );
        if(qpwmTask < _IQ(COOLER_PWM_START)){
            qpwmTask = _IQ(COOLER_PWM_START);
        }else if(qpwmTask > _IQ(1)){
            qpwmTask = _IQ(1);
        }
        if(sysset.stopsFan != 0){    //С остановом
            if(tem.integer <= sysset.lowTemp){
                setFanPwm(0);
            }
            if(tem.integer >= sysset.hiTemp){
                setFanPwm(IQtoInt(qpwmTask, 1000));
            }
        }
        else{       //Без останова
            setFanPwm(IQtoInt(qpwmTask, 1000));
        }
  
        /**************************************
        * Настройка режима по току
        */
        switch(ps.task.mode){
            case(mode_overcurrentShutdown):{
                qTask = IntToIQ(ps.task.current, 1000);
                IRQ_Limit_ON();                                                 //Включить прерывание от ограничения тока
            }break;

            case(mode_limitation):
            case(mode_timeShutdown):
            case(mode_lowCurrentShutdown):{
                qTask = IntToIQ(ps.task.current, 1000);
                IRQ_Limit_OFF();                                                //Выключить прерывание от ограничения тока
            }break;

            default:
                qTask = 0;
                IRQ_Limit_ON();                                                 //Включить прерывание от ограничения тока
            break;
        }
        if(qTask == 0){
            qdac = 0;
        }else{
            qdac = iq_Fy_x1x2y1y2x(s->pI[0].qi, s->pI[1].qi, IntToIQ(s->pI[0].dac, base[BASE_DAC]), IntToIQ(s->pI[1].dac, base[BASE_DAC]), qTask);
        }
        dac = IQtoInt(qdac, base[BASE_DAC]);
        if(dac > base[BASE_DAC]){
            dac = base[BASE_DAC];
        }
        setDacI(dac);
        
        /**************************************
        * Установка напряжения
        */
        qTask = IntToIQ(ps.task.voltage, 1000);
        if(qTask == 0){
            qdac = 0;
        }else{
            qdac = iq_Fy_x1x2y1y2x(s->pU[0].qu, s->pU[1].qu, IntToIQ(s->pU[0].dac, base[BASE_DAC]), IntToIQ(s->pU[1].dac, base[BASE_DAC]), qTask);
        }
        dac = IQtoInt(qdac, base[BASE_DAC]);
        if(dac > base[BASE_DAC]){
            dac = base[BASE_DAC];
        }
        setDacU(iq_filtr(vTaskCumul, dac, VTASK_FILTER_K));
        
        /**************************************
        * При любых авариях выключаем выход
        */
        if(ps.error.all != 0){  
            l_switchRequest = switchOffRequest;
        }
        
        /**************************************
        * Выключение по времени
        */
        if(ps.task.mode == mode_timeShutdown){
            if((ps.meas.time >= ps.task.time)&&(ps.state.bit.switchIsON != 0)){
                l_switchRequest = switchOffRequest;
            }
        }
        
        /**************************************
        * Выключение по падению тока до 10% от задания
        */
        if(ps.task.mode == mode_lowCurrentShutdown){
            if(((ps.meas.current <= (ps.task.current / 10))&&(ps.state.bit.switchIsON != 0)) || (ps.task.current == 0)){
                curOffTime++;
                if(curOffTime > (CUR_OFF_TIME / SYSTEM_TSK_PERIOD)){
                    l_switchRequest = switchOffRequest;
                }
            }
            else{
                curOffTime = 0;
            }
        }
        
        /**************************************
        * Запросы на включение / отключение по каналу управления
        */
        if(ps.task.switchRequest == switchOnRequest){
            l_switchRequest = switchOnRequest;
            ps.task.switchRequest = switchNoRequest;
        }
        if(ps.task.switchRequest == switchOffRequest){
            l_switchRequest = switchOffRequest;
            ps.task.switchRequest = switchNoRequest;
        }
        
        /**************************************
        * Включение / отключение
        */
        if(l_switchRequest == switchOnRequest){
            if(ps.state.bit.switchIsON == 0){
                ps.state.bit.ovfCurrent = 0;
                vTaskCumul = 0;
                setDacU(0);
                switchON();
            }
            //timeOffset = RTC_GET();
            timeOffset = xTaskGetTickCount();
            l_switchRequest = switchNoRequest;
        }
        if(l_switchRequest == switchOffRequest){
            if(ps.state.bit.switchIsON != 0){
                vTaskCumul = 0;
                setDacU(0);
                switchOFF();
            }
            l_switchRequest = switchNoRequest;
        }
        
        /**************************************
        * Если переключился режим управления с АСУ на локальное, то нужно запустить GUI
        */
        if((typeContrPrev == acsControl)&&(ps.task.control == localControl)){
            xTaskCreate(baseTSK,      "baseTSK",      BASE_TSK_SZ_STACK,      NULL, BASE_TSK_PRIO,    NULL);
        }
        typeContrPrev = ps.task.control;
        
        /*************************************/
        vTaskDelayUntil(&xLastWakeTime, msToSt(SYSTEM_TSK_PERIOD));
    }
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
