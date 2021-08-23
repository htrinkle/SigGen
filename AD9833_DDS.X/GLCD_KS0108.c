#include "GLCD_KS0108.h"
#include "font5x8.h"
#include <stdio.h>

#include <xc.h>
#define _XTAL_FREQ 8000000

#define RS LATBbits.LATB0
#define EN LATBbits.LATB1
#define CS1 LATBbits.LATB2
#define CS2 LATBbits.LATB3

#define LCD_data LATD
#define Ctrl_dir TRISB
#define Data_dir TRISD

uint8_t   buf[8][128];

void GLCD_Str(uint8_t x, uint8_t y, char * str, uint8_t color) {
    uint8_t xx = x;
    while(*str != NULL) {
        GLCD_Char(xx, y, *str, color);
        xx += 6;
    }
}

void GLCD_Char(uint8_t x, uint8_t y, unsigned char c, uint8_t color) {
    unsigned char bitmap;
    uint8_t xi, yi;
    uint16_t index;
    index = 5*(c-' ');

    for (xi=0; xi<5; xi++) {
        bitmap = font5x8[index];
        if (color == 0) bitmap = ~bitmap;
        for (yi=0; yi<8; yi++) {
            GLCD_SetPixel(x+xi, y+yi, bitmap&0x01);
            bitmap = bitmap/2;
        }
        index++;
    }
}

void GLCD_Init()	/* GLCD Initialization function */
{
    Ctrl_dir = 0;	/* Make Control pin direction as output */
    Data_dir = 0;	/* Make Data pin direction as output */

	CS1 = 1; CS2 = 1;/* Select first Left half of display */
    __delay_ms(20);
    GLCD_Command(0x3E);	/* Display OFF */
    GLCD_Command(0x40);	/* Set Y address (column=0) */
    GLCD_Command(0xB8);	/* Set x address (page=0) */
    GLCD_Command(0xC0);	/* Set z address (start line=0) */
    GLCD_Command(0x3F);	/* Display ON */
}

void GLCD_Command(uint8_t cmd)/* GLCD Command function */
{
    LCD_data = cmd;	/* Copy command on data pin */
    RS = 0;		/* Make RS LOW for command register */
    EN = 1;		/* Make HIGH-LOW transition on Enable */
    __delay_us(2);
    EN = 0;
    __delay_us(2);
}

void GLCD_Data(uint8_t data)/* GLCD Data function */
{
    LCD_data = data;	/* Copy Data on data pin */
    RS = 1;		/* Make RS HIGH for data register */
    EN = 1;		/* Make HIGH-LOW transition on Enable */
    __delay_us(2);
    EN = 0;
    __delay_us(2);
}

void GLCD_Clear()
{
	uint8_t i,j;
	for(i=0;i<8;i++) {
        for(j=0;j<128;j++) {
            buf[i][j] = 0x00;
        }
    }
    GLCD_ReDraw();
}

void GLCD_ReDraw()
{
    uint8_t row, col;
   
    CS1=0; CS2=1; // Only Left
    for(row=0;row<8;row++) {
        GLCD_Command(0x40);	            /* Set Y address (column=0) */ 
        GLCD_Command((0xB8)+row);	    /* Increment page after 64 column */
		for(col=0;col<64;col++) {
			GLCD_Data(buf[row][col]);	/* Write data to all 64 column */
		}
	}
    CS1=1; CS2=0; // Only Right
    for(row=0;row<8;row++) {
        GLCD_Command(0x40);	            /* Set Y address (column=0) */ 
        GLCD_Command((0xB8)+row);	
		for(col=64;col<128;col++) {
			GLCD_Data(buf[row][col]);	/* Write data to all 64 column */
		}
	}
}

void GLCD_SetPixel(uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t bitmap = buf[y/8][x];
    uint8_t mask = (1 << (y % 8));
    switch (color) {
        case 0: 
            bitmap = bitmap & ~mask;
            break;
        default: 
            bitmap = bitmap | mask;
            break;
    }
    buf[y/8][x] = bitmap;
}