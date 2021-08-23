#include <xc.h>
#include <stdint.h>
#include "GLCD_KS0108.h"
#include <stdio.h>

#define _XTAL_FREQ 8000000

void putch(char data) {
    while( ! TXIF) continue;
    TXREG = data;
}

void init_uart(void) {
    SPBRG = 12; // 9600 baud @ 8 MHz
    TXEN = 1; // enable transmitter
    BRGH = 0;
    BRG16 = 0;
    SYNC = 0;
    SPEN = 1; // enable serial port
    CREN = 1; // enable continuous operation
}
 
void main(void) {
    init_uart();
    GLCD_Init();	/* GLCD Initialize function */
    __delay_ms(10);
    GLCD_Clear();	/* GLCD Display clear function */
    GLCD_Str       (30,20,"Hello",1);
    GLCD_Str       (30,32,"Hello",0);
    GLCD_ReDraw();
    while(1);
} 