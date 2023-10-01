#include "TAD_MARQUESINA.h"
#include <xc.h>
#include "TAD_LCD.h"
#include "TAD_TIMER.h"

#define LCD_COLS 16

static unsigned char state; //State of Motor
static unsigned char indexL1, indexL2; //Index of the string (in line 1 or 2) at a given moment
static unsigned char sizeL1, sizeL2; //Length of the string (in line 1 or 2)
static unsigned char biggerSize; //Bigger size between sizeL1 and sizeL2
static char *strL1, *strL2; //The string to be printed in concurrent motor calls
static unsigned char timer; //Timer to count 1s every time

unsigned char strlen(char *s);


void MQ_Init(void){
    TI_NewTimer(&timer);
    MQ_Clear();
}

void MQ_Clear(void){
//Pre: None
//Post: Stops the Marquesina Effect with given strings
    state = 0;
    strL1 = strL2 = NULL;
}

void MQ_PutString(char *s, unsigned char line){
//Pre: s != null && line = 0 or 1
//Post: Prints a string in the LCD screen with the marquesina effect
    if(line){
        strL2 = s;
        sizeL2 = strlen(s);
    } 
    else{
        LCD_Clear();
        strL1 = s;
        sizeL1 = strlen(s);
    }
    biggerSize = (sizeL1 > sizeL2) ? sizeL1 : sizeL2; //Set Bigger size between sizeL1 and sizeL2
    indexL1 = indexL2 = 0; //Reset indexes if a new String is introduced
    TI_ResetTics(timer);
    state = 2; //Dont wait for timer, go directly to state 2
}

void MQ_Motor(void){
//Pre: None
//Post: Step of the Marquesina TAD Motor
    switch(state){
        case 0:
            break;
        case 1: //MQ ON
            if(TI_GetTics(timer) >= MQ_STEP){ //Every MQ_STEP, change the string
                TI_ResetTics(timer);
                LCD_Clear();
                state++;
            }
            break;
        case 2: //Print strL1 from indexL1 to the end
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            if(strL1 != NULL){
                if(indexL1 > biggerSize) indexL1 = 0;
                LCD_GotoXY(0, 0);
                if(indexL1 <= sizeL1) LCD_PutString(strL1 + indexL1); //Prints the string from indexL1 to the end at line 1
                indexL1++;
            }
            state++;
            break;
        case 3: //Print strL1 from 0 to LCD_COLS - (sizeL1 - indexL1)
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            if(strL1 != NULL && sizeL1 - indexL1 < LCD_COLS){ //If the string printed from the indexL1 to the end is smaller than LCD_COLS, print the rest of the string
                LCD_GotoXY((biggerSize-indexL1) + 2, 0);
                LCD_PutString(strL1); //Prints the string from 0 to the end at line 1
            }
            state++;
            break;
        case 4: //Print strL2 from indexL2 to the end
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            if(strL2 != NULL){
                if(indexL2 > biggerSize) indexL2 = 0;
                LCD_GotoXY(0, 1);
                if(indexL2 <= sizeL2) LCD_PutString(strL2 + indexL2); //Prints the string from indexL2 to the end at line 2
                indexL2++;
            }
            state++;
            break;
        case 5: //Print strL2 from 0 to LCD_COLS - (sizeL2 - indexL2)
            if(!LCD_IsAvailable()) break; //If LCD is not available, wait
            if(strL2 != NULL && sizeL2 - indexL2 < LCD_COLS){ //If the string printed from the indexL1 to the end is smaller than LCD_COLS, print the rest of the string
                LCD_GotoXY((biggerSize-indexL2) + 2, 1);
                LCD_PutString(strL2); //Prints the string from 0 to the end at line 1
            }
            state = 1; //Go to state 1 to wait for 1s again
            break;
    }

}



unsigned char strlen(char *s){
    unsigned char i = 0;
    while(s[i] != '\0') i++;
    return i;
}