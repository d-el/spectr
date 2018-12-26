#define SYS_FREQ            72000000
#define measTimeEnable()    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk
#define measTimeStart()     DWT->CYCCNT = 0
#define measTimeStop(){                             \
    measTime = DWT->CYCCNT;                         \
    measTime = (measTime * 1000000000) / SYS_FREQ;  \
}

extern unsigned long long    measTime;
