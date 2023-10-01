#include <xc.h>
#include "TAD_COUNTER.h"
#include "TAD_TIMER.h"
#include "TAD_LCD.h"
#include "TAD_KEYPAD.h"
#include "TAD_MENU.h"
#include "TAD_SIO.h"

#define SEPARATOR ':' //Separator for the time
#define MODIFY_TIME "MODIFY TIME:"
#define CURRENT_TIME "CURRENT TIME:"

//VARIABLES
static char time[4] = {0, 0, 0, 0};     //time[0] es dMinuts, time[1] es uMinuts, time[2] es dSegons, time[3] es uSegons
static char timeAux[4] = {0, 0, 0, 0};
static char strAux[6]; //Aux string to print the time
unsigned static char timeAuxIndex; //Index of timeAux
unsigned static char timerHora; //Timer to count 1s every time
unsigned static char modifyTime; //Flag to indicate if modifyTime menu is being shown
unsigned static char showTime; //Flag to indicate if showTime menu is being shown
unsigned static char state; //State of Motor


void CNT_Init(void){
    showTime = modifyTime = state = 0;
    TI_NewTimer(&timerHora);
    TI_ResetTics(timerHora);
}

void CNT_ShowTime(void){
    showTime = 1;
    state = 1;
    KPAD_UnsetSMS();
    KPAD_StartInput();
}

void CNT_ModifyTime(void){
    modifyTime = 1;
    timeAuxIndex = 0;
    LCD_CursorOn();
    KPAD_UnsetSMS();
    KPAD_StartInput();
    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_PutString(MODIFY_TIME);
    state = 4;
}

void CNT_ResetTime(void){
    time[0] = time[1] = time[2] = time[3] = 0;
}

void CNT_Motor(void) {
    unsigned char aux;
    switch (state) {
        case 0:
            if(TI_GetTics(timerHora) >= 3980){ //Ha passat 1s
                aux = 'Z'; //Specific value to indicate that 1s has passed
                TI_ResetTics(timerHora);
                time[3]++; //Incrementem uSegons
                if(time[3] >= 10){ //Si uSegons == 10, resetejem i incrementem dSegons
                    time[3] = 0; //Reset uSegons
                    time[2]++; //Increment dSegons
                    if(time[2] >= 6){ //Si dSegons == 6, resetejem i incrementem uMinuts
                        time[2] = 0; //Reset dSegons
                        time[1]++; //Increment uMinuts
                        if(time[1] >= 10){ //Si uMinuts == 10, resetejem i incrementem dMinuts
                            time[1] = 0; //Reset uMinuts
                            time[0]++; //Increment dMinuts
                            if(time[0] >= 9){ //Si dMinuts == 9, resetejem
                                time[0] = 0; //Reset dMinuts
                            }
                        }
                    }
                }
            }
            if(showTime){
                if(aux == 'Z') state++;
                if((aux = KPAD_GetChar()) == '*'){
                    showTime = 0;
                    KPAD_StopInput();
                    MENU_Mostrar();
                }
            }
            else if(modifyTime){
                state = 5;
            }
            break;
        case 1:
            LCD_Clear();
            LCD_GotoXY(0, 0);
            LCD_PutString(CURRENT_TIME);
            state++;
            break;
        case 2:
            if(!LCD_IsAvailable()) break; //If CURRENT_TIME hasn't been written yet, wait
            LCD_GotoXY(0, 1);
            strAux[0] = time[0] + '0';
            strAux[1] = time[1] + '0';
            strAux[2] = SEPARATOR;
            strAux[3] = time[2] + '0';
            strAux[4] = time[3] + '0';
            strAux[5] = '\0';
            LCD_PutString(strAux);
            state = 0;
            break;
        case 4:
            if(!LCD_IsAvailable()) break; //If MODIFY_TIME hasn't been written yet, wait
            LCD_GotoXY(0, 1);
            state++;
            break;
        case 5:
            state = 0;
            if((aux = KPAD_GetChar()) == KPAD_NO_KEY) break;
            if(aux == '*'){ //If * has been pressed, exit and start again the PORT MENU
                modifyTime = 0;
                LCD_CursorOff();
                KPAD_StopInput();
                MENU_Mostrar();
            }
            else if(aux == '#'){
                if(timeAuxIndex == 4){
                    modifyTime = 0;
                    time[0] = timeAux[0];
                    time[1] = timeAux[1];
                    time[2] = timeAux[2];
                    time[3] = timeAux[3];
                    KPAD_StopInput();
                    MENU_Mostrar();
                }
            }
            else if(timeAuxIndex != 4){
                timeAux[timeAuxIndex] = aux - '0'; //Guardem el valor pressionat a timeAux
                LCD_PutChar(aux);
                timeAuxIndex++;
                if(timeAuxIndex == 2) LCD_PutChar(SEPARATOR);
                if(timeAuxIndex == 4) LCD_CursorOff();
            }
            break;
    }
    return;
}

char CNT_GetTime(unsigned char index){
    return time[index];
}

char *CNT_GetTimeArr(void){
    return time;
}