#include "TAD_MICRO.h"
#include <xc.h>
#include "TAD_SIO.h"
#include "TAD_TIMER.h"
#include "TAD_LCD.h"
#include "TAD_MENU.h"
#include "TAD_COUNTER.h"
#include "TAD_BUZZER.h"
#include "TAD_SAVERECORDINGS.h"

#define RECORDING "RECORDING..."
static unsigned char state; //State of Motor
static unsigned int sentSamples; //Num of sent samples
static unsigned char txTimer; //Timer to count 250us every time, to send the data through SIO
static unsigned char indexAux; //Aux variable to keep track of array indexes
static unsigned char newID[2]; //ID of the new recording

void MICRO_Init(void){
//Pre: None
//Post: Inits the Micro TAD
    TRISAbits.TRISA1 = 1; //Set RA1 as input
    state = 0;
    TI_NewTimer(&txTimer);
}

void MICRO_ListenAndSend(void){
//Pre: Que hagi passat LISTENING_TIME des de que s'ha cridat aquesta funcio per ultima vegada
//Post: Listens for 8 seconds while sending the data through SIO
    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_PutString(RECORDING);
    indexAux = sentSamples = 0;
    state = 1;
    SIO_PutChar('D');
}

void MICRO_Motor(void){
//Pre: None
//Post: Step of the MICRO TAD Motor
    switch(state){
        case 0:
            break;
        case 1: //Receive New Listening ID
            if(!SIO_RXAvailable()) break;
            newID[indexAux++] = SIO_GetChar();
            if(indexAux == 2){
                indexAux = 0;
                state++;
                SAVE_PutRecording(newID, CNT_GetTimeArr()); //Add new recording to EEPROM
            }
            break;
        case 2: //Send Time
            if(!SIO_TXAvailable()) break;
            SIO_PutChar(CNT_GetTime(indexAux) + '0');
            indexAux++;
            if(indexAux == 4) state++;
            break;
        case 3:
            if(!SIO_TXAvailable()) break;
            state++;
            break;
        case 4: //Start Listening for 8 seconds: send ADC converted value every 250us through SIO, 32768 samples in total
            ADCON0bits.CHS = 1; //Select RA1 as input for ADC 
            ADCON0bits.ADON = 1; //Enable ADC
            ADCON0bits.GO = 1; //Start ADC
            state++;
            TI_ResetTics(txTimer);
            break;
        case 5: //Send Data after 250us
            if(ADCON0bits.GO) break; //If ADC is not ready, wait
            ADCON0bits.ADON = 0; //Disable ADC
            if(TI_GetTics(txTimer) < 1) break; //If 250us have not passed, wait
            SIO_PutChar(ADRESH);
            sentSamples++;
            if(sentSamples >= MICRO_SAMPLES_TO_SEND){ //If 32768 samples have been sent (8s), stop listening
                state = 0;
                BUZZ_PlayMelody();
                MENU_Mostrar();
                break;
            }
            state--;
            break;
    }
}