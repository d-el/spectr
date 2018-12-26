/******************************************************************************
* @file    owSlave.c
* @author  Storozhenko Roman - D_EL
* @version V1.0
* @date    12-09-2015
* @brief
*/
#include "globalIncludes.h"
#include "owSlave.h" 

/******************************************************************************
* 
*/
volatile scratchpad_type    spStr = {
    0,      //temperature
    0,      //voltage
    0,      //hvoltage
    {
        0,      //mhvPresent
    },
    0,      //crcRd
    {
        1,      //temperatureLimOn
    },
    25000,  //vTask
    0       //crcWr
};
volatile uint8_t    *scratchpad = (uint8_t*)&spStr;                         //[9] = {0x50,0x05, 0xC0,0x0C, 0x80, 0x00,0,0,0};  //Initial scratchpad
volatile uint8_t    scrc;                                                   //CRC calculation
volatile uint8_t    cbuf;                                                   //Input buffer for a command
const    uint8_t    sps_owid[8] = {'S', 'P', 'S', '_', 'V', '2', '0', 0x64};//set your own ID http://www.tm3d.de/index.php/tools/14-crc8-berechnung
volatile uint8_t    bitp;                                                   //pointer to current Byte
volatile uint8_t    bytep;                                                  //pointer to current Bit
volatile uint8_t    mode;                                                   //state
volatile uint8_t    wmode;                                                  //if 0 next bit that send the device is  0
volatile uint8_t    actbit;                                                 //current
volatile uint8_t    srcount;                                                //counter for search rom
volatile uint8_t    wrSz;                                                   //Колличество байт для чтения
volatile uint8_t    bffForWriteScr[12];                                     //

void owSlaveInit(void)
{
    mode    = OWM_SLEEP;
    wmode   = OWW_NO_WRITE;
    wrSz = (((uint32_t)&(spStr.crcWr) -  (uint32_t)&(spStr.bitTask)) + sizeof(spStr.crcWr));
    
    
    /*********************************
    * Init_TIM15
    */
    RCC->APB2ENR  |= RCC_APB2ENR_TIM15EN;
    OW_TIM->PSC     = SYSTEM_FREQUENCY / 1000000 - 1;               //Претделитель для получения периода тактирования 1us
    OW_TIM->ARR     = 0xFFFF;
    OW_TIM->DIER    |= TIM_DIER_UIE;                                //Update interrupt enable
    OW_TIM->EGR     = TIM_EGR_UG;                                   //Генерируем событие для перегрузки из в рабочие регистры
    OW_TIM->SR      = ~TIM_SR_UIF;                                  //Clear UIF flag
    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, TIM15_IRQn_Priority);     //Установить приоритет
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);                            //Включить прерывание  
    
    /*********************************
    * Init_EXTI12, PA12
    */
    //PA12, На 1WIRE SPS
    gppin_init(GPIOA, 12, outOpenDrain);
    RESET_LOW;
    
    AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI12_PA;
    EXTI->IMR       |= EXTI_IMR_MR12;                           //Interrupt request from Line x is not masked
    NVIC_SetPriority(EXTI15_10_IRQn, EXTI12_IRQn_Priority);     //Установить приоритет
    NVIC_EnableIRQ(EXTI15_10_IRQn);                             //Разрешаем прерывания

    SET_FALLING;
    DIS_TIMER;
}

/******************************************************************************
* The interrupt service routine
*/
__irq void EXTI15_10_IRQHandler(void){
    uint8_t lwmode = wmode;                                             //let this variables in registers
    uint8_t lmode = mode;

    if((lwmode == OWW_WRITE_0)) {SET_LOW;lwmode = OWW_NO_WRITE;}        //if necessary set 0-Bit
    DIS_OWINT;                                                          //disable interrupt, only in OWM_SLEEP mode it is active
    switch(lmode){
        case OWM_SLEEP:
            SET_TIME(OWT_MIN_RESET);
            EN_OWINT;                                                   //other edges ?
        break;

        //Start of reading with falling edge from master, reading closed in timer isr
        case OWM_MATCH_ROM:                                             //falling edge wait for receive
        case OWM_WRITE_SCRATCHPAD:
        case OWM_READ_COMMAND:
            SET_TIME(OWT_READLINE);                                     //wait a time for reading
            break;

        case OWM_SEARCH_ROM:                                            //Search algorithm waiting for receive or send
            if (srcount<2) {                                            //this means bit or complement is writing,
                SET_TIME(OWT_LOWTIME);
            } else
                SET_TIME(OWT_READLINE);                                 //init for read answer of master
            break;

        case OWM_READ_SCRATCHPAD:                                       //a bit is sending
        case OWM_READ_ROM:                                              //a bit is sending
            SET_TIME(OWT_LOWTIME);
            break;

        case OWM_CHK_RESET:                                             //rising edge of reset pulse
            SET_FALLING;
            SET_TIME(OWT_RESET_PRESENCE);                               //waiting for sending presence pulse
            lmode = OWM_RESET;
            break;
    }
    EN_TIMER;
    mode        =  lmode;
    wmode       =  lwmode;
    EXTI->PR    |= EXTI_PR_PR12;
}

/******************************************************************************
* The timer interrupt service routine
*/
__irq void TIM1_BRK_TIM15_IRQHandler(void)
{
    uint8_t lwmode  =  wmode;                                           //let this variables in registers
    uint8_t lmode = mode;
    uint8_t lbytep = bytep;
    uint8_t lbitp = bitp;
    uint8_t lsrcount = srcount;
    uint8_t lactbit = actbit;
    uint8_t lscrc = scrc;
    //Ask input line sate
    uint8_t p;
    
    p = OW_PIN;
    //Interrupt still active ?
    if(CHK_INT_EN){
        //maybe reset pulse
        if(p == 0){
            lmode = OWM_CHK_RESET;                                      //wait for rising edge
            SET_RISING;
        }
        DIS_TIMER;
    }else
    switch(lmode){
        case OWM_RESET:                                                 //Reset pulse and time after is finished, now go in presence state
            lmode = OWM_PRESENCE;
            SET_LOW;
            SET_TIME(OWT_PRESENCE);
            DIS_OWINT;                                                  //No Pin interrupt necessary only wait for presence is done
        break;
        
        case OWM_PRESENCE:
            RESET_LOW;                                                  //Presence is done now wait for a command
            lmode = OWM_READ_COMMAND;
            cbuf = 0;lbitp = 1;                                         //Command buffer have to set zero, only set bits will write in
        break;
        
        case OWM_READ_COMMAND:
            if(p){                                                      //Set bit if line high
                cbuf|= lbitp;
            }
            lbitp = (lbitp<<1);
            if(!lbitp){                                                 //8-Bits read
                lbitp = 1;
                switch(cbuf){
                    //case OWCOM_SKIP_ROM:
                    case OWCOM_READ_ROM:                                //READ_ROM
                        lmode = OWM_READ_ROM;
                        lbytep = 0;                                     //From first position
                        lactbit = (lbitp & sps_owid[0]) == lbitp;
                        lwmode = lactbit;                               //Prepare for send firs bit
                    break;

                    case OWCOM_MATCH_ROM:
                        lbytep = 0;
                        lmode = OWM_MATCH_ROM;
                    break;

                    case OWCOM_SEARCH_ROM:                              //Initialize search rom
                        lmode = OWM_SEARCH_ROM;
                        lsrcount = 0;
                        lbytep = 0;
                        lactbit = (sps_owid[lbytep] & lbitp) == lbitp;  //Sset actual bit
                        lwmode = lactbit;                               //Prepare for writing when next falling edge
                    break;

                    case OWCOM_WRITE_SCRATCHPAD:
                        lmode = OWM_WRITE_SCRATCHPAD;
                        lbytep  = 0;
                        lscrc   = 0;
                        bffForWriteScr[0] = 0;
                    break;

                    case OWCOM_READ_SCRATCHPAD:                             //READ_SCRATCHPAD
                        lmode   = OWM_READ_SCRATCHPAD;
                        lbytep  = 0;
                        lscrc   = 0;                                        //From first position
                        lactbit = (lbitp & scratchpad[0]) == lbitp;
                        lwmode  = lactbit;                                  //Prepare for send firs bit
                    break;

                    default: lmode = OWM_SLEEP;                             //all other commands do nothing
                }
            }
        break;

        case OWCOM_SKIP_ROM:
            lmode = OWM_READ_COMMAND;
        break;

        case OWM_READ_ROM:
            RESET_LOW;
            lbitp = (lbitp << 1);
            if(lbitp == 0){
                lbytep++;
                lbitp = 1;
                if(lbytep >= OWID_BUF_SIZE){
                    lmode = OWM_SLEEP;
                    break;
                }
            }
            lactbit = (lbitp & sps_owid[lbytep]) == lbitp;
            lwmode = lactbit;
        break;

        case OWM_SEARCH_ROM:
            RESET_LOW;                                                  //Set low also if nothing send (branch takes time and memory)
            lsrcount++;                                                 //next search rom mode
            switch(lsrcount){
                case 1:lwmode = !lactbit;                               //preparation sending complement
                    break;
                case 3:
                    if(p != (lactbit == 1)){                            //check master bit
                        lmode = OWM_SLEEP;                              //not the same go sleep
                    }else{
                        lbitp = (lbitp << 1);                           //prepare next bit
                        if(lbitp == 0){
                            lbitp = 1;
                            lbytep++;
                            if(lbytep >= 8){
                                lmode = OWM_SLEEP;                      //all bits processed
                                break;
                            }
                        }
                        lsrcount = 0;
                        lactbit = (sps_owid[lbytep]&lbitp) == lbitp;
                        lwmode = lactbit;
                    }
                    break;
            }
        break;
        case OWM_MATCH_ROM:
            if(p == ((sps_owid[lbytep] & lbitp) == lbitp)){             //Compare with ID Buffer
                lbitp = (lbitp<<1);
                if (lbitp == 0){
                    lbytep++;
                    lbitp = 1;
                    if(lbytep >= 8){
                        lmode = OWM_READ_COMMAND;                       //same? get next command

                        cbuf = 0;
                        break;
                    }
                }
            }else{
                lmode = OWM_SLEEP;
            }
        break;

        case OWM_WRITE_SCRATCHPAD:
            if(p != 0){
                bffForWriteScr[lbytep] |= lbitp;
            }
            if(lbytep < (wrSz - 1)){
                if((lscrc&1) != p)          lscrc = (lscrc>>1) ^ 0x8c;
                else                        lscrc >>= 1;
            }
            lbitp = (lbitp << 1);
            if(lbitp == 0){
                lbytep++;
                lbitp = 1;
                if(lbytep == wrSz){
                    if(bffForWriteScr[wrSz - 1] == lscrc){
                        memcpy((uint8_t*)&spStr.bitTask, (uint8_t*)bffForWriteScr, wrSz);
                        spStr.rxCnt++;
                    }else{
                        spStr.errCnt++;
                    }
                    lmode = OWM_SLEEP;
                    break;
                }else{
                    bffForWriteScr[lbytep] = 0;
                }
            }
        break;
        
        case OWM_READ_SCRATCHPAD:
            RESET_LOW;
            if((lscrc&1) != lactbit)    lscrc = (lscrc>>1) ^ 0x8c;
            else                        lscrc >>= 1;
            lbitp = (lbitp << 1);
            if(lbitp == 0){
                lbytep++;
                lbitp = 1;
                if(lbytep >= sizeof(spStr)){
                    lmode = OWM_SLEEP;
                    break;
                }
                else if(lbytep == ((uint8_t*)&spStr.crcRd - (uint8_t*)&spStr)){
                    spStr.crcRd = lscrc;
                }
                else if(lbytep == (sizeof(scratchpad_type) - 1)){
                    scratchpad[sizeof(scratchpad_type) - 1] = lscrc;
                }
            }
            lactbit = (lbitp&scratchpad[lbytep]) == lbitp;
            lwmode = lactbit;
        break;
    }
    if(lmode == OWM_SLEEP) {DIS_TIMER;}
    if(lmode != OWM_PRESENCE){
        SET_TIME(OWT_MIN_RESET-OWT_READLINE);                 //OWT_READLINE around OWT_LOWTIME
        EN_OWINT;
    }

    mode    = lmode;
    wmode   = lwmode;
    bytep   = lbytep;
    bitp    = lbitp;
    srcount = lsrcount;
    actbit  = lactbit;
    scrc    = lscrc;
    OW_TIM->SR = ~TIM_SR_UIF;                                   //clear UIF flag
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
