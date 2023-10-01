#include <xc.h>
#include "TAD_MENU.h"
#include "TAD_LCD.h"
#include "TAD_KEYPAD.h"
#include "TAD_PORT.h"
#include "TAD_COUNTER.h"
#include "TAD_MARQUESINA.h"
#include "TAD_JOYSTICK.h"
#include "TAD_MICRO.h"
#include "TAD_MENURECORDINGS.h"
#include "TAD_TIMER.h"

#define LINE1 "1. START RECORDING"
#define LINE2 "2. PLAY RECORDINGS"
#define LINE3 "3. MODIFY CURRENT TIME"
#define LINE4 "4. SHOW CURRENT TIME"
#define LINE5 "5. END COMMUNICATION"
#define LINE6 " "
#define BYE_BYE "bye bye "
#define MAX_OPTIONS 5

static unsigned char state; //State of Motor
static unsigned char option; //Current option at first line in LCD
static unsigned char printNext;
static unsigned char timerByeBye; //Timer to count 2s for bye bye

void exitMenu(void); //Boilerplate code to exit the menu


void MENU_Init(void){
    //Pre: None
    //Post: Inits the Menu TAD
    state = 0;
    TI_NewTimer(&timerByeBye);
}

void MENU_Mostrar(void){
    //Pre: Que aquest menu no s'estigui mostrant
    //Post: Comença a mostrar el menu principal
    state = 2;
    option = 1; //Show first option first
    KPAD_UnsetSMS();
    KPAD_StartInput();
    JOY_Enable();
}

void MENU_OptionUp(void){
    if(option == MAX_OPTIONS) return;
    state = 2;
    option++;
}

void MENU_OptionDown(void){
    if(option == 1) return;
    state = 2;
    option--;
}

void MENU_Motor(void){
    unsigned char aux;
    switch(state){
        case 0:
            break;
        case 1:
            if((aux = JOY_GetMove()) != JOY_NO_MOVE){ //If Joystick has been moved UP or DOWN
                aux == JOY_UP ? MENU_OptionUp() : MENU_OptionDown();
            }
            else if(KPAD_GetChar() == '#'){ //If '#' has been pressed
                exitMenu();
                switch(option){
                    case 1:
                        MICRO_ListenAndSend();
                        break;
                    case 2:
                        MRCD_Mostrar();
                        break;
                    case 3:
                        CNT_ModifyTime();
                        break;
                    case 4:
                        CNT_ShowTime();
                        break;
                    case 5: //Say Bye Bye and exit to PORT MENU
                        state = 9;
                        break;
                }
            }
            break;
        case 2:
            state = option + 2; //Print the option selected + next one
            printNext = 0;
            break;
        case 3:
            if(!LCD_IsAvailable()) break;
            MQ_PutString(LINE1, printNext);
            printNext++;
            state++;
            break;
        case 4:
            if(!LCD_IsAvailable()) break;
            MQ_PutString(LINE2, printNext);
            //If it's the first line, go to next state, else go to state 1
            if(!printNext){
                state++;
                printNext++;
            } 
            else state = 1;
            break;
        case 5:
            if(!LCD_IsAvailable()) break;
            MQ_PutString(LINE3, printNext);
            //If it's the first line, go to next state, else go to state 1
            if(!printNext){
                state++;
                printNext++;
            } 
            else state = 1;
            break;
        case 6:
            if(!LCD_IsAvailable()) break;
            MQ_PutString(LINE4, printNext);
            //If it's the first line, go to next state, else go to state 1
            if(!printNext){
                state++;
                printNext++;
            } 
            else state = 1;
            break;
        case 7:
            if(!LCD_IsAvailable()) break;
            MQ_PutString(LINE5, printNext);
            //If it's the first line, go to next state, else go to state 1
            if(!printNext){
                state++;
                printNext++;
            } 
            else state = 1;
            break;
        case 8:
            if(!LCD_IsAvailable()) break;
            MQ_PutString(LINE6, printNext);
            state = 1; 
            break;
        case 9: //Say Bye Bye and exit to PORT MENU
            if(!LCD_IsAvailable()) break;
            LCD_Clear();
            LCD_GotoXY(0, 0);
            LCD_PutString(BYE_BYE);
            state++;
            break;
        case 10: //Say Bye Bye and exit to PORT MENU
            if(!LCD_IsAvailable()) break;
            LCD_PutString(PORT_GetName());
            state++;
            break;
        case 11:
            if(!LCD_IsAvailable()) break;
            LCD_PutChar('!');
            TI_ResetTics(timerByeBye);
            state++;
            break;
        case 12:
            if(TI_GetTics(timerByeBye) >= 8000){
                state = 0;
                PORT_Mostrar();
            }
            break;
    }
}


void exitMenu(void){
    state = 0;
    MQ_Clear();
    JOY_Disable();
}