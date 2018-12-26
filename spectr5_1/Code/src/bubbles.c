/*
******************************************************************************
* @file			bubbles.c
* @author		Matthew Weathers + D_EL - Storozhenko Roman
* @version      V1.0
* @date			14-September-2004
* @Up date:     19,08,2014
* @brief		Функции
******************************************************************************  
*/
#include "globalIncludes.h"

uint8_t colorsPal[3] = {R, G, B};
uint8_t bcolorsCurrent;

void bubblesTask(void *pvParameters){
    uint16_t old_val_encoder = enGeReg();

    uiGraphicsColor = black;
    uiTextColor = RGB;
    lcd_Clear();
    
	initialize();

    while(1){
        moveball();
        physics();
        
        //======== обработка кнопок ========//		
        if(BtScan() || (old_val_encoder != enGeReg())){	//
            BeepTime(100);
            xTaskCreate(baseTSK, "baseTSK", BASE_TSK_SZ_STACK, NULL, 2, NULL);
            vTaskDelete(NULL); //Удаляем текущую задачу
        }
        vTaskDelay(60);
    }
}

void nextColor(void){
    static uint8_t n;
    n++;
    if(n > 2) n =0;
    bcolorsCurrent = colorsPal[n];
}

struct ball{
    int16_t     radius;
    int16_t     speedx;
    int16_t     speedy;
    int16_t     old_xcenter;
    int16_t     old_ycenter;
    int16_t     new_xcenter;
    int16_t     new_ycenter;
}ball1;

void paint_Fball(int16_t x, int16_t y, int16_t r)
{
    for(;r>0;r--){
        grf_circle(x, y, r, bcolorsCurrent);
    }
}

void paint_ball(int16_t x, int16_t y, int16_t r){
    uiTextColor = bcolorsCurrent;
    paint_Fball(x, y, r);
}

void clear_ball(int16_t x, int16_t y, int16_t r){
    uiTextColor = black;
    paint_Fball(x, y, r);
}

void initialize(void) // Initialize the game.
{
    //Intialise Ball
    ball1.radius = 5;

    ball1.speedx = 3; // Generate Random X direction.
    ball1.speedy = 5; // Generate Random Y direction.

    ball1.old_xcenter = xSize / 2;
    ball1.old_ycenter = ySize / 2;
    ball1.new_xcenter = xSize / 2;
    ball1.new_ycenter = ySize / 2;
}

void moveball(void)
{
    ball1.old_xcenter = ball1.new_xcenter;
    ball1.old_ycenter = ball1.new_ycenter;
    ball1.new_xcenter = ball1.new_xcenter + ball1.speedx;
    ball1.new_ycenter = ball1.new_ycenter + ball1.speedy;

    clear_ball(ball1.old_xcenter, ball1.old_ycenter, ball1.radius);
    paint_ball(ball1.new_xcenter, ball1.new_ycenter, ball1.radius);
}

void physics(void)
{
    if((ball1.new_ycenter - ball1.radius) <= 1){
        ball1.speedy = -ball1.speedy; // Reflect From Top
        nextColor();
    }
    if(ball1.new_ycenter + ball1.radius >= ySize -1){
        ball1.speedy = -ball1.speedy; // Reflect From Bottom
        nextColor();
    }
    if(ball1.new_xcenter - ball1.radius < 1){
        ball1.speedx = - ball1.speedx;
        nextColor();
        return;
    }
    if(ball1.new_xcenter +  ball1.radius >= xSize -1){
        ball1.speedx = - ball1.speedx;
        nextColor();
        return;
    }
}

void play(void)	// Our Budding Game Engine
{
    uint16_t i;
    
    i = 100;
    enSet(0, 500, 1, 1,
          enNONCIRC, enU16, &i);
	while(1){ // Check wether key press is Q if so exit loop
        moveball();
        physics();
        
        enGet();        //EN
        delay_ms(i);    // Reduce game speed to human playable level
    }
}

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
