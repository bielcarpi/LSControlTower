#include "TAD_PORT.h"
#include "TAD_LCD.h"
#include "TAD_SIO.h"
#include "TAD_KEYPAD.h"
#include "TAD_MENU.h"
#include "TAD_COUNTER.h"

#define PORT_MSG "PORT NAME:"

static unsigned char state; //State of Motor
static unsigned char numChars; //Num of written chars - num of sent chars through SIO
static char portName[4];

void PORT_Init(void){
    state = 0;
    portName[3] = '\0';
}

void PORT_Mostrar(void){
    state++;
}

char * PORT_GetName(void){
    return portName;
}

void PORT_Motor(void){
    unsigned char aux;
    switch(state) {
        case 0:
            break;
        case 1:
            LCD_Clear();
            LCD_PutString(PORT_MSG);
            state++;
            numChars = 0;
            break;
        case 2:
            if(!LCD_IsAvailable()) break; //If PORT_MSG hasn't been written yet, wait
            LCD_GotoXY(0, 1);
            LCD_CursorOn();
            KPAD_StartInput();
            KPAD_SetSMS();
            state++;
            break;
        case 3:
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            if((portName[numChars] = KPAD_GetChar()) == KPAD_NO_KEY) break; //If no key has been pressed, wait
            if(portName[numChars] == '*'){ //If * has been pressed, start again the PORT MENU
                state = 1;
                break;
            }
            LCD_PutChar(portName[numChars]);
            if(numChars == 2){ //If 3 chars have been written, go to next state
                LCD_CursorOff();
                KPAD_UnsetSMS();
                state++;
            } 
            else numChars++;
            break;
        case 4:
            if((aux = KPAD_GetChar()) == KPAD_NO_KEY) break; //If no key has been pressed, wait
            if(aux == '*'){ //If * has been pressed, start again the PORT MENU
                state = 1;
            }
            else if(aux == '#'){ //If # has been pressed, send PORTNAME through SIO
                SIO_PutChar('T'); //Send a 'T' to the PC and wait for a 'K' to send the PORTNAME
                state++;
                numChars = 0;
            }
            break;
        case 5:
            KPAD_StopInput(); //Stop listening the Keypad
            if(SIO_RXAvailable() && SIO_GetChar() == 'K') //Whenever PC sends us a K, go to next state
                state++;
            break;
        case 6:
            if(!SIO_TXAvailable()) break; //If SIO is not available, wait
            SIO_PutChar(portName[numChars++]);
            if(numChars == 4){ //If 3 chars have been sent, go to next state
                state++;
                numChars = 0;
            }
            break;
        case 7:
            CNT_ResetTime(); //Reset the time as we enter a new port
            MENU_Mostrar(); //Show the main menu
            state = 0;
            break;
    }
}