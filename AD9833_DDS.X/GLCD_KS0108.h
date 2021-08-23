#ifndef GLCD_KS0108_H
#define	GLCD_KS0108_H

#include <stdint.h>

void GLCD_Init(void);
void GLCD_Command(uint8_t cmd);
void GLCD_Data(uint8_t data);
void GLCD_Clear(void);
void GLCD_ReDraw(void);
void GLCD_SetPixel(uint8_t x, uint8_t y, uint8_t color);
void GLCD_Char(uint8_t x, uint8_t y, unsigned char c, uint8_t color);
void GLCD_Str(uint8_t x, uint8_t y, char * str, uint8_t color);

#endif