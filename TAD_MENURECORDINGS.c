#include <xc.h>
#include "TAD_MENURECORDINGS.h"
#include "TAD_MENU.h"
#include "TAD_LCD.h"
#include "TAD_KEYPAD.h"
#include "TAD_SAVERECORDINGS.h"
#include "TAD_JOYSTICK.h"
#include "TAD_KEYPAD.h"
#include "TAD_SIO.h"
#include "TAD_BUZZER.h"

#define PLAYING "PLAYING..."
#define NO_RECORDINGS "NO RECORDINGS..."

static unsigned char state; //State of Motor
static unsigned char option; //Current option at first line in LCD
static unsigned char indexSent; //Index of the ID of the recording that has been sent
char strAux[12]; //Aux string to print the ID and the timestamp of the recordings

void fillStrAux(unsigned char opt);


void MRCD_Init(void){
    //Pre: None
    //Post: Inits the Menu Recordings TAD
    state = 0;
}

void MRCD_Mostrar(void){
    //Pre: Que aquest menu no s'estigui mostrant
    //Post: Comença a mostrar el menu Recordings
    if(numRecordings == 0){
        LCD_Clear();
        LCD_PutString(NO_RECORDINGS);
        state = 1;
    }
    else{ //If there are recordings to show
        option = 0; //Show first option first
        indexSent = 0;
        state = 7;
        JOY_Enable();
    }

    KPAD_UnsetSMS();
    KPAD_StartInput();
}

void MRCD_OptionUp(void){
    //Pre: None
    //Post: Moves the options up
    if(option == numRecordings-1) return;
    state = 7;
    option++;
}

void MRCD_OptionDown(void){
    //Pre: None
    //Post: Moves the options down
    if(option == 0) return;
    state = 7;
    option--;
}

void MRCD_Motor(void){
    //Pre: None
    //Post: Step of the Menu Recordings TAD
    unsigned char aux;
    switch(state){
        case 0:
            break;
        case 1: //Showing NO RECORDINGS. Waiting for a *
            if((aux = KPAD_GetChar()) == '*'){
                state--;
                MENU_Mostrar();
            }
            break;
        case 2: //Showing the recordings. Waiting for a Joystick move, a '#' or a '*' press
            if((aux = JOY_GetMove()) != JOY_NO_MOVE){ //If Joystick has been moved UP or DOWN
                aux == JOY_UP ? MRCD_OptionUp() : MRCD_OptionDown();
            }
            else if((aux = KPAD_GetChar()) == '#'){ //If '#' has been pressed
                JOY_Disable();
                LCD_Clear();
                LCD_PutString(PLAYING);
                state++; //Play the recording
            }
            else if(aux == '*'){ //If '*' has been pressed
                state = 0;
                JOY_Disable();
                MENU_Mostrar();
            }
            break;
        case 3: //Sending a 'P' to alert the PC that we are going to play a recording.
            if(!SIO_TXAvailable()) break;
            SIO_PutChar('P');
            state++;
            break;
        case 4: //Playing request. Waiting for a 'K'
            if(!SIO_RXAvailable()) break;
            if(SIO_GetChar() == 'K') state++; //If we receive a 'K', we can send the ID of the recording
            break;
        case 5:
            if(!SIO_TXAvailable()) break;
            if(indexSent == 0 && recordings[option].id[indexSent] == 0){ //If the ID of the recording is 0, we don't send it
                indexSent++;
            }
            else if(indexSent == 2){ //If we already sent the 2 bytes of the ID, we can send a '\0' to indicate the end of the ID
                SIO_PutChar('\0');
                state++;
            }
            else{
                SIO_PutChar(recordings[option].id[indexSent++] + '0'); //Send the ID of the recording
            }
            break;
        case 6: //Playing request accepted. Playing on PC. Waiting for a 'F' to indicate that the PC has finished playing the recording
            if(SIO_RXAvailable() && SIO_GetChar() == 'F') state++;
            indexSent = 0;
            BUZZ_PlayMelody();
            break;
        case 7: //Print to LCD the first line 
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            LCD_Clear();
            LCD_GotoXY(0, 0);
            fillStrAux(option); //Fill strAux with the ID and the timestamp of the recording
            LCD_PutString(strAux);
            JOY_Enable();
            state++;
            break;
        case 8: //Print to LCD the second line
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            LCD_GotoXY(0, 1);
            if(option < numRecordings-1){
                fillStrAux(option+1); //Fill strAux with the ID and the timestamp of the recording
                LCD_PutString(strAux);
            }
            state = 2;
            break;
    }
}


void fillStrAux(unsigned char opt){
    strAux[0] = 'I';
    strAux[1] = recordings[opt].id[0] + '0';
    strAux[2] = recordings[opt].id[1] + '0';
    strAux[3] = ' ';
    strAux[4] = '-';
    strAux[5] = ' ';
    strAux[6] = recordings[opt].timestamp[0] + '0';
    strAux[7] = recordings[opt].timestamp[1] + '0';
    strAux[8] = ':';
    strAux[9] = recordings[opt].timestamp[2] + '0';
    strAux[10] = recordings[opt].timestamp[3] + '0';
    strAux[11] = '\0';
}