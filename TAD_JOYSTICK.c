#include "TAD_JOYSTICK.h"
#include <xc.h>

static unsigned char enabled; //Flag to indicate if the Joystick is enabled (if not, we won't check the Joystick and save CPU)
static unsigned char lastMove; //Last move of the Joystick
static unsigned char ready; //Last move of the Joystick
static unsigned char state; //State of Motor

void JOY_Init(void){
    //Pre: None
    //Post: Inits the Joystick TAD
    TRISAbits.TRISA0 = 1; //Set RA0 as input
    state = 
    lastMove = JOY_NO_MOVE;
}

void JOY_Enable(void){
    enabled = 1;
}

void JOY_Disable(void){
    enabled = 0;
}

unsigned char JOY_GetMove(void){
    //Pre: None
    //Post: Returns the move of the Joystick, either UP, DOWN or NO_MOVE
    if(!ready) return JOY_NO_MOVE;
    ready = 0;
    unsigned char aux = lastMove;
    lastMove = JOY_NO_MOVE;
    return aux;
}

void JOY_Motor(void){
    //Pre: None
    //Post: Step of the Joystick TAD
    switch(state){
        case 0:
            if(!enabled) break;
            ADCON0bits.CHS = 0; //Select RA0 as input for ADC 
            ADCON0bits.ADON = 1; //Enable ADC
            ADCON0bits.GO = 1; //Start ADC
            state++;
            break;
        case 1:
            if(ADCON0bits.GO) break; //If ADC is not ready, wait
            ADCON0bits.ADON = 0; //Disable ADC
            if(ADRESH < 0x10){ //0xD0 = 11010000
                lastMove = JOY_DOWN; //If the value is less than 0xD0, the Joystick is UP
            } 
            else if(ADRESH > 0xF0){
                lastMove = JOY_UP; //If the value is more than 0xD0, the Joystick is DOWN
            } 
            else if(ADRESH >= 0x70 && ADRESH <= 0x90 && (lastMove == JOY_UP || lastMove == JOY_DOWN)){
                ready = 1; //If the value is between 0x70 && 0x90, the Joystick is in the middle
            } 
            state--;
            break;
    }
}