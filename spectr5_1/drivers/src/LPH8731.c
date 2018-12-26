/**
******************************************************************************
*  __      .______    __    __    ___    ______   ____    __
* |  |     |   _  \  |  |  |  |  / _ \  |____  | |___ \  /_ |
* |  |     |  |_)  | |  |__|  | | (_) |     / /    __) |  | |
* |  |     |   ___/  |   __   |  > _ <     / /    |__ <   | |
* |  `----.|  |      |  |  |  | | (_) |   / /     ___) |  | |
* |_______|| _|      |__|  |__|  \___/   /_/     |____/   |_|
*

* @file    LPH8731.c
* @author  Storozhenko Roman - D_EL
* @version V1.0.0
* @date    12-01-2013
* @brief
* Ѕиблиотека дл€ роботы с LCD контроллером S1D15G14
* ƒисплеи на контроллере S1D15G14: LPH8731
******************************************************************************
*/
#include "globalIncludes.h"

char        str[32];
uint8_t     uiGraphicsColor;
uint8_t     uiTextColor;

/*****************************************************************************************************
передача данных
*/
void lcd_data(uint8_t data){
    SPI2->DR = data;                    //загружаем данные дл€ передачи
    while (!(SPI2->SR & SPI_SR_TXE));   //ожидание окончани€ передачи
}

/*****************************************************************************************************
передача команды
*/
void lcd_command(uint8_t data){
    LO_DC_PIN;                          //формирование уровн€ D/C - LOW
    SPI2->DR = data;                    //загружаем данные дл€ передачи
    while (!(SPI2->SR & SPI_SR_TXE));   //ожидание окончани€ передачи
    HI_DC_PIN;                          //формирование уровн€ D/C - HI
}

/*****************************************************************************************************
инициализаци€ LCD
*/
void lcd_init(void){
    delay_ms(10);

    //вывод D/C: выход двухтактный, 50MHz
    GPIOC->CRL   &= ~GPIO_CRL_CNF4;     //выход Push-Pull
    GPIOC->CRL   |= GPIO_CRL_MODE4;     //50MHz

    delay_ms(5);
    lcd_command(0x01);  //reset sw
    delay_ms(5);

    lcd_command(0xc6);  //initial escape

    lcd_command(0xb9);  //Refresh set
    lcd_data(0x00);
    delay_ms(5);

    lcd_command(0xb6);  //Display control
    lcd_data(0x80); //
    lcd_data(0x04); //
    lcd_data(0x8a);     //»меет параметр ориентации

    lcd_data(0x54); //
    lcd_data(0x45); //
    lcd_data(0x52); //
    lcd_data(0x43); //

    lcd_command(0xb3);      //Gray scale position set 0
    lcd_data(0x02); //
    lcd_data(0x0a); //
    lcd_data(0x15); //
    lcd_data(0x1f); //
    lcd_data(0x28); //
    lcd_data(0x30); //
    lcd_data(0x37); //
    lcd_data(0x3f); //
    lcd_data(0x47); //
    lcd_data(0x4c); //
    lcd_data(0x54); //
    lcd_data(0x65); //
    lcd_data(0x75); //
    lcd_data(0x80); //
    lcd_data(0x85); //

    lcd_command(0xb5);      //Gamma curve
    lcd_data(0x01); //

    lcd_command(0xbd);      //Common driver output select
    lcd_data(0x00); //

    lcd_command(0xbe);      //Power control
    lcd_data(0x50); //
    delay_us(5000);

    lcd_command(0x11);      //Sleep out

    lcd_command(0xba);      //Voltage control
    lcd_data(0x2f); //
    lcd_data(0x03); //

    lcd_command(0x25);      //Write contrast
    lcd_data(100); //

    lcd_command(0xb7);      //Temperature gradient
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //
    lcd_data(0x00); //

    lcd_command(0x03);      //Booster voltage ON
    delay_ms(5);

    lcd_command(0x3a);      //interfase pixel format
    lcd_data(0x02);         //0x02 for 8-bit/pixel 0x03 for 12bit/pixel

    lcd_command(0x2d);      //Colour set
    lcd_data(0x00); //
    lcd_data(0x03); //
    lcd_data(0x05); //
    lcd_data(0x07); //
    lcd_data(0x09); //
    lcd_data(0x0b); //
    lcd_data(0x0d); //
    lcd_data(0x0f); //
    lcd_data(0x00); //
    lcd_data(0x03); //
    lcd_data(0x05); //
    lcd_data(0x07); //
    lcd_data(0x09); //
    lcd_data(0x0b); //
    lcd_data(0x0d); //
    lcd_data(0x0f); //
    lcd_data(0x00); //
    lcd_data(0x05); //
    lcd_data(0x0b); //
    lcd_data(0x0f); //

    lcd_command(0x36);      //Memory access control
    lcd_data(0x48);         //»меет параметр ориентации

    lcd_command(0x29);      //Display ON
}

/*****************************************************************************************************
программное выключение LCD
*/
void lcd_disable(void){
    lcd_command(0x28);          //Display OFF
    lcd_command(0x10);          //Sleep in
}

void lcd_contrast(uint8_t data){
    lcd_command(0x25);          //Write contrast
    if((data > 85)&&(data < 115)){
        lcd_data(data);
    }
    else{
        lcd_data(100);
    }
}

void lcd_gotoxy(uint8_t x0, uint8_t y0,  uint8_t x1, uint8_t  y1){
    //x-координата
    lcd_command(0x2A);          //задаем область по X (x0 - начальный, x1 - конечный)
    lcd_data(x0);               //X
    lcd_data(x1);

    //Y-координата
    lcd_command(0x2B);          //задаем область по Y (y0 - начальный, y1 - конечный)
    lcd_data(y0 +1);            //у этого контроллера Y отсчитываетс€ от 1, а не 0
    lcd_data(y1 +1);

    lcd_command(0x2C);          //WR_MEM
}

/*=================================================================================
«накогенератор (вывод еденичного символа 4x6)
*/
void lcd_PutChar4x6(uint8_t x, uint8_t y, uint8_t c){
    uint8_t ch, p, h, mask;
    //X-координата
    lcd_command(0x2A);
    lcd_data(x);  //X
    lcd_data(x +4 -1);
    //Y-координата
    lcd_command(0x2B);
    lcd_data(y+1); //Y
    lcd_data(y+1 +6 -1);

    lcd_command(0x2C);              // WR_MEM

    c = c - 0x20;

    for(h =0; h <6; h++){           // every column of the character
        ch = fonts4x6[c*6 +h];
        mask =0x08;
        for (p =0; p <4; p++){      // write the pixels
        if (ch&mask) lcd_data(uiTextColor);
        else lcd_data(uiGraphicsColor);
        mask = mask >>1;
        }
    }
}

/*=================================================================================
¬ывод строки символов 4x6
*/
void lcd_PutString4x6(uint8_t x, uint8_t y, const char *s){
    while (*s){
        lcd_PutChar4x6(x, y, *s);
        s++;
        x+= 4;
    }
}

/*=================================================================================
«накогенератор (вывод еденичного символа 6x8)
*/
void lcd_PutChar6x8(uint8_t x, uint8_t y, uint8_t c){
    uint8_t ch, p, h, mask;
    //X-координата
    lcd_command(0x2A);
    lcd_data(x);  //X
    lcd_data(x +6 -1);
    //Y-координата
    lcd_command(0x2B);
    lcd_data(y+1); //Y
    lcd_data(y+1 +8-1);

    lcd_command(0x2C);              // WR_MEM

    c = c - 0x20;

    for(h =0; h <8; h++){           // every column of the character
        ch = fonts6x8[c*8 +h];
        mask =0x20;
        for(p =0; p <6; p++){       // write the pixels
            if (ch&mask) lcd_data(uiTextColor);
            else lcd_data(uiGraphicsColor);
            mask = mask >>1;
        }
    }
}

/*=================================================================================
¬ывод строки символов 6x8
*/
void lcd_PutString6x8(uint8_t x, uint8_t y, const char *s){
    while (*s){
        lcd_PutChar6x8(x, y, *s);
        s++;
        x+= 6;
    }
}


/*=================================================================================
вывод символа 8x12 на дисплей
*/
void lcd_PutChar8x12(uint8_t x, uint8_t y, uint8_t c){
    uint8_t     ch, mask, i, j;

    c = c - 0x20;

    //X-координата
    lcd_command(0x2A);
    lcd_data(x);  //X
    lcd_data(x +8 -1);
    //Y-координата
    lcd_command(0x2B);
    lcd_data(y +1); //Y
    lcd_data(y +12 +1);

    lcd_command(0x2C);              // WR_MEM

    for(i =0; i <12; i++){          // every column of the character
        ch = fonts8x12[c*12 +i];
        mask =0x80;
        for (j =0; j <8; j++){      // write the pixels
            if (ch&mask) lcd_data(uiTextColor);
            else lcd_data(uiGraphicsColor);
            mask = mask >> 1;
        }
    }
}


/*=================================================================================
вывод текстовой строки 8x12 на дисплей
*/
void lcd_PutString8x12(uint8_t x, uint8_t y, const char *s){
    while (*s){
    lcd_PutChar8x12(x, y, *s);
    s++;
    x+= 8;
    }
}

/*=================================================================================
вывод широкого символа 2x8x12 на дисплей
*/
void lcd_PutChar2x8x12(uint8_t x, uint8_t y, char c){
    uint8_t     ch, mask;
    uint8_t     i, j;

    c = c - 0x20;

    //X-координата
    lcd_command(0x2A);
    lcd_data(x);  //X
    lcd_data(x +16 -1);
    //Y-координата
    lcd_command(0x2B);
    lcd_data(y +1); //Y
    lcd_data(y + 12 +1);

    lcd_command(0x2C);              // WR_MEM

    for(i = 0; i < 12; i++){        // every column of the character
        ch = fonts8x12[c*12 +i];
        mask=0x80;
        for (j = 0; j < 8; j++){    // write the pixels
            if (ch&mask){
                lcd_data(uiTextColor);
                lcd_data(uiTextColor);
            }else{
                lcd_data(uiGraphicsColor);
                lcd_data(uiGraphicsColor);
            }
            mask = mask >> 1;
        }
    }
}

/*=================================================================================
вывод широкой текстовой строки 2x8x12 на дисплей
*/
void lcd_PutString2x8x12(uint8_t x, uint8_t y, const char *s){
    while (*s){
        lcd_PutChar2x8x12(x, y, *s);
        s++;
        x+= 16;
    }
}

/*=================================================================================
//вывод символа 14x19  на дисплей
*/
void lcd_PutChar14x19(uint8_t x, uint8_t y, char c){
    uint16_t    ch, mask;
    uint8_t     i, j;

    c = (c - 0x2C)*19;

    //X-координата
    lcd_command(0x2A);
    lcd_data(x);  //X
    lcd_data(x +14 -1);

    //Y-координата
    lcd_command(0x2B);
    lcd_data(y +1); //Y
    lcd_data(y +19 +1);

    lcd_command(0x2C);              // WR_MEM

    for(i = 0; i < 19; i++){        // every column of the character
        ch = fonts14x19[c +i];
        mask=0x0001;
        for (j = 0; j < 14; j++){   // write the pixels
            if (ch&mask) lcd_data(uiTextColor);
            else lcd_data(uiGraphicsColor);
            mask = mask << 1;
        }
    }
}

/*=================================================================================
//вывод текстовой строки 14x19 на дисплей
*/
void lcd_PutString14x19(uint8_t  x, uint8_t  y, const char *s){
    while (*s){
        lcd_PutChar14x19(x, y, *s);
        s++;
        x+= 14;
    }
}

/*=================================================================================
//вывод символа 20x17  на дисплей
*/
void lcd_PutChar20x17(uint8_t x, uint8_t y, char c){
    uint16_t    ch, mask;
    uint8_t     i, j;

    c = 3 * 17 * c;

    //X-координата
    lcd_command(0x2A);
    lcd_data(x);  //X
    lcd_data(x +20 -1);

    //Y-координата
    lcd_command(0x2B);
    lcd_data(y +1); //Y
    lcd_data(y +17 +1);

    lcd_command(0x2C);              // WR_MEM

    for(i = 0; i < 17; i++){        // every column of the character
        ch = fonts16x19[c + i * 3 + 0];
        mask = 0x80;
        for (j = 0; j < 8; j++){   // write the pixels
            if (ch & mask) lcd_data(uiTextColor);
            else lcd_data(uiGraphicsColor);
            mask = mask >> 1;
        }
        ch = fonts16x19[c + i * 3 + 1];
        mask = 0x80;
        for (j = 0; j < 8; j++){   // write the pixels
            if (ch & mask) lcd_data(uiTextColor);
            else lcd_data(uiGraphicsColor);
            mask = mask >> 1;
        }
        ch = fonts16x19[c + i * 3 + 2];
        mask = 0x80;
        for (j = 0; j < 4; j++){   // write the pixels
            if (ch & mask) lcd_data(uiTextColor);
            else lcd_data(uiGraphicsColor);
            mask = mask >> 1;
        }
    }
}

/*=================================================================================
//«аливка отдним цветом
*/
void lcd_FillScreen(uint8_t color){
    uint16_t i;
    
    lcd_gotoxy(0,0,100,79); //очистка LCD
    for(i=0;i<8080;i++){    //заливка LCD
        lcd_data(color);
    }
}

/*=================================================================================
//«аливка черным
*/
void lcd_Clear(void){
    uint16_t i;
    
    lcd_gotoxy(0, 0, DISP_W - 1, DISP_H - 1);
    for(i=0; i < DISP_W * DISP_H; i++){    //заливка LCD
        lcd_data(0x00);
    }
}

/*=================================================================================
//¬ывод картики
*/
void lcd_PrintImage(void *pIm){
    uint8_t     *pData; 
    uint8_t     *pDataEnd;
    
    pData     = pIm;
    pDataEnd  = pData + (DISP_W * DISP_H);
    
    lcd_gotoxy(0, 0, DISP_W - 1, DISP_H - 1);
    while(pData < pDataEnd){
        lcd_data(*pData++);
    }
}

/*=================================================================================
//”становка пиксел€
*/
void lcd_setPixel(uint16_t x, uint16_t y, uint8_t color){
    lcd_gotoxy(x, y, x, y);
    lcd_data(color);
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
