/**
******************************************************************************
*  __      .______    __    __    ___    ______   ____    __
* |  |     |   _  \  |  |  |  |  / _ \  |____  | |___ \  /_ |
* |  |     |  |_)  | |  |__|  | | (_) |     / /    __) |  | |
* |  |     |   ___/  |   __   |  > _ <     / /    |__ <   | |
* |  `----.|  |      |  |  |  | | (_) |   / /     ___) |  | |
* |_______|| _|      |__|  |__|  \___/   /_/     |____/   |_|
*

* @file    LPH8731.h
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    12-01-2013
* @brief
* Библиотека для роботы с LCD контроллером S1D15G14
* Дисплеи на контроллере S1D15G14: LPH8731
******************************************************************************
*/
#ifndef LPH8731_H
#define LPH8731_H

#define GPIO_LCD        GPIOC
#define lcd_dc_pin      GPIO_BRR_BR4;

//Пин CE подключен на GND, пин RST через диф. цепь подключен на Vdd
#define LO_DC_PIN   GPIO_LCD->BRR = lcd_dc_pin
#define HI_DC_PIN   GPIO_LCD->BSRR = lcd_dc_pin

#define R       0xE0    //RED
#define RG      0XFC    //RG - YELLOW
#define G       0x1C    //GREN
#define GB      0X1F    //GB -САЛАТОВЫЙ
#define B       0x03    //BLUE
#define RB      0XE3    //RB - VIOLET
#define RGB     0xFF    //RGB
#define white   0xFF    //white
#define black   0x00    //BLACK

#define DISP_W  101
#define DISP_H  80

extern  char        str[32];
extern  uint8_t     uiGraphicsColor;
extern  uint8_t     uiTextColor;

//-----------------------------------------------------------
//Prototypes for the functions in LPH8731.c
//-----------------------------------------------------------
void lcd_data(uint8_t data);
void lcd_command(uint8_t data);
void lcd_init(void);
void lcd_disable(void);
void lcd_contrast(uint8_t data);
void lcd_gotoxy(uint8_t x0, uint8_t y0,  uint8_t x1, uint8_t  y1);
void lcd_PutChar4x6(uint8_t x, uint8_t y, uint8_t c);
void lcd_PutString4x6(uint8_t x, uint8_t y, const char *s);
void lcd_PutChar6x8(uint8_t x, uint8_t y, uint8_t c);
void lcd_PutString6x8(uint8_t x, uint8_t y, const char *s);
void lcd_PutChar8x12(uint8_t x, uint8_t y, uint8_t c);
void lcd_PutString8x12(uint8_t x, uint8_t y, const char *s);
void lcd_PutChar2x8x12(uint8_t x, uint8_t y, char c);
void lcd_PutString2x8x12(uint8_t x, uint8_t y, const char *s);
void lcd_PutChar14x19(uint8_t x, uint8_t y, char c);
void lcd_PutString14x19(uint8_t  x, uint8_t  y, const char *s);
void lcd_PutChar20x17(uint8_t x, uint8_t y, char c);
void lcd_FillScreen(uint8_t color);
void lcd_Clear(void);
void lcd_PrintImage(void *pntIm);
void lcd_setPixel(uint16_t x, uint16_t y, uint8_t color);

#endif //LPH8731_H
/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
