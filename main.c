/**
 * LSControlTower
 * P2 Fase B - Compus 2022/23
 * Alex Cano (alex.cano@students.salle.url.edu) i Biel Carpi (biel.carpi@students.salle.url.edu)
*/
#include "TAD_TIMER.h"
#include "TAD_PORT.h"
#include "TAD_LCD.h"
#include "TAD_KEYPAD.h"
#include "TAD_SIO.h"
#include "TAD_MENU.h"
#include "TAD_COUNTER.h"
#include "TAD_MICRO.h"
#include "TAD_JOYSTICK.h"
#include "TAD_MARQUESINA.h"
#include "TAD_BUZZER.h"
#include "TAD_SAVERECORDINGS.h"
#include "TAD_MENURECORDINGS.h"

#pragma config OSC = HSPLL //40Mhz
#pragma config PBADEN = DIG
#pragma config MCLRE = OFF
#pragma config DEBUG = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

#define LCD_ROWS 2
#define LCD_COLS 16


extern void __interrupt (high_priority) HighRSI(void){
    if (INTCONbits.TMR0IF == 1) RSI_Timer0();
}


void configADC(void){
    ADCON1 = 0x0D; //0000 1101 (0x0D) AN0 and AN1 as analog
    ADCON2 = 0x12; //0001 0010 (0x12) Right justified, 4TAD, FOSC/32
}

void configInterrupts(void){
    // Enable high priority interrupts only
    RCONbits.IPEN = 0;
    INTCONbits.GIE = 1;
}


void main(void){
    configInterrupts();
    configADC();

    // Init TADs & config ports
    TI_Init();
    TI_Wait40ms(); //Wait 40ms until other initializations (LCD needs it)
    CNT_Init();
    SIO_Init();
    PORT_Init();
    KPAD_Init();
    MENU_Init();
    MQ_Init();
    JOY_Init();
    MICRO_Init();
    BUZZ_Init();
    MRCD_Init();
    SAVE_Init();
    LCD_Init(LCD_ROWS, LCD_COLS);

    //Start with PORT Menu
    PORT_Mostrar();

    //Run TAD Motors
	while(1){
        CNT_Motor();
        LCD_Motor();
        KPAD_Motor();
        PORT_Motor();
        MENU_Motor();
        JOY_Motor();
        MICRO_Motor();
        MQ_Motor();
        BUZZ_Motor();
        SAVE_Motor();
        MRCD_Motor();
	}				
}

