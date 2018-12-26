/*
******************************************************************************
* @file			bubbles.h
* @author		Matthew Weathers + D_EL - Storozhenko Roman
* @version      V1.0
* @date			14-September-2004
* @Up date:     19,08,2014
* @brief		Функции
******************************************************************************  
*/
#ifndef BUBBLES_H
#define BUBBLES_H

#define xSize 101
#define ySize 80

void nextColor(void);
void paint_Fball(int16_t x, int16_t y, int16_t r);
void paint_ball(int16_t x, int16_t y, int16_t r);
void clear_ball(int16_t x, int16_t y, int16_t r);
void initialize(void);
void moveball(void);
void physics(void);
void play(void);
void bubblesTask(void *pvParameters);

#endif //BUBBLES_H

/******************* (C) COPYRIGHT ***************** END OF FILE ********* D_EL *****/
