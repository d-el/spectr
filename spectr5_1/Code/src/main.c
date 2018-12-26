/**
*********************************************************************************************
*                                                                                           *
*       /\  \         /\  \         /\  \         /\  \         /\  \         /\  \         *
*      /::\  \       /::\  \       /::\  \       /::\  \        \:\  \       /::\  \        *
*     /:/\ \  \     /:/\:\  \     /:/\:\  \     /:/\:\  \        \:\  \     /:/\:\  \       *
*    _\:\~\ \  \   /::\~\:\  \   /::\~\:\  \   /:/  \:\  \       /::\  \   /::\~\:\  \      *
*   /\ \:\ \ \__\ /:/\:\ \:\__\ /:/\:\ \:\__\ /:/__/ \:\__\     /:/\:\__\ /:/\:\ \:\__\     *
*   \:\ \:\ \/__/ \/__\:\/:/  / \:\~\:\ \/__/ \:\  \  \/__/    /:/  \/__/ \/_|::\/:/  /     *
*    \:\ \:\__\        \::/  /   \:\ \:\__\    \:\  \         /:/  /         |:|::/  /      *
*     \:\/:/  /         \/__/     \:\ \/__/     \:\  \        \/__/          |:|\/__/       *
*      \::/  /                     \:\__\        \:\__\                      |:|  |         *
*       \/__/                       \/__/         \/__/                       \|__|         *
*                                                                                           *
* @file    main.c                                                                           *
* @author  Storozhenko Roman - D_EL                                                         *
* @version V2.0.0                                                                           *
* @date    19-12-2014                                                                       *
* @brief   Main program body                                                                *
*********************************************************************************************
*/
#include "globalIncludes.h"
#include "version.h"

/******************************************************************************
*
*/
__noreturn main(void){    
    nvMem_state_type nvMemState;
    
    nvMem_init();
    nvMemState = nvMem_loadPrm(nvMem.nvMemBase);
    if(nvMemState == nvMem_ok){
        sysset.startCounter++;
        //������ ��������� �����
        nvMem_savePrm(nvMem.nvMemBaseResrvCopy);
    }
    else{
        //��������� ������� ��������� �����
        nvMemState = nvMem_loadPrm(nvMem.nvMemBaseResrvCopy);
        if(nvMemState != nvMem_ok){ //��������� ����� ���������
            prmInitDef();
        }
    }
    nvMem_prepareMemory(nvMem.nvMemBase);
    
    Init_Hard();

    BeepTime(30);
    LED_ON();
    delay_ms(50);
    LED_OFF();
    delay_ms(50);
    BeepTime(30);
    LED_ON();
    delay_ms(50);
    LED_OFF();
    
    lcd_PrintImage((void*)ImStart);    //��������
    uiGraphicsColor = black;
    uiTextColor = white;

    sprintf(str, "#%u", sysset.startCounter);
    lcd_PutString6x8(00, 50, str);
    lcd_PutString6x8(00, 60, fwinfo_version);
    memcpy(str, fwinfo_date, 16);
    str[16] = 0;
    lcd_PutString6x8(00, 70, str);
    delay_ms(2000);
    
    enco.step = 1;
    enco.ntic = 0;
    
    beepSetFreq(sysset.freqBeep);       //��������� ������� �������
    lcd_contrast(sysset.lcdContrast);

    BtScan();
    if(ks.DOWN){
        QUIET_MODE_ON();                //����� �����
    }
    if((ks.UP)||(ps.error.bit.noCalibration != 0)){
        while(Setting() != 0);
    }

    if(ks.NEXT){
        lcd_PrintImage((void*)muzhuk);  //�����
        delay_ms(3000);
    }

    //Great mutex, semaphore, task; start scheduler
    OSinit(); 
 
    while(1);  
}	 
	
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
