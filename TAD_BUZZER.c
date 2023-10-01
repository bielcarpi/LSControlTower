#include "TAD_BUZZER.h"
#include <xc.h>
#include "TAD_TIMER.h"

#define NOTE_DURATION 400 //Duration of a note (0.25us * 400 = 0.1s)
#define MELODY_DURATION 20000 //Duration of a melody (0.25us * 20000 = 5s)
#define SPEAKER LATBbits.LATB5 //Buzzer LAT

static unsigned char state; //State of Motor
static unsigned char timerPWM, timerNote, timerMelody; //Timers for PWM, Note and Melody
static unsigned char period, dutyCycle; //Period and duty cycle of PWM
static unsigned char playingMelody;
static unsigned char melodyIndex;
static unsigned int melodyPause;


void BUZZ_Init(void){
    //Pre: None
    //Post: Inits the Buzzer TAD
    TRISBbits.TRISB5 = 0; //Buzzer as output
    SPEAKER = 0; //Buzzer off
    playingMelody = 0;
    TI_NewTimer(&timerPWM);
    TI_NewTimer(&timerNote);
    TI_NewTimer(&timerMelody);
}

void BUZZ_Play(unsigned char note){
    //Pre: None
    //Post: Plays the note
    state = 2;
    period = note;
    dutyCycle = (period >> 1); //50% duty cycle
    TI_ResetTics(timerNote);
}

void BUZZ_PlayMelody(void){
    playingMelody = 1;
    melodyIndex = 0;
    melodyPause = MELODY_DURATION/MELODY_NOTES;
    TI_ResetTics(timerMelody);
    BUZZ_Play(MELODY[melodyIndex]);
}

void BUZZ_Motor(void){
    //Pre: None
    //Post: Step of the Buzzer TAD
    switch (state){
        case 0:
            SPEAKER = 0;
            break;
        case 1:
            if(TI_GetTics(timerMelody) >= melodyPause){
                TI_ResetTics(timerMelody);
                melodyIndex++;
                BUZZ_Play(MELODY[melodyIndex]);
            }
            else if(melodyIndex >= MELODY_NOTES){
                playingMelody = 0;
                state = 0;
            }
            break;
        case 2:
            if(TI_GetTics(timerNote) >= NOTE_DURATION){
                TI_ResetTics(timerNote);
                state--;
                if(!playingMelody) state--; //Go to state 0 if not playing melody
                break;
            }
            TI_ResetTics(timerPWM);
            state++;
            break;
        case 3:
            if(TI_GetTics(timerPWM) >= dutyCycle){
                SPEAKER = 1;
                state++;
            }
            break;
        case 4:
            if(TI_GetTics(timerPWM) >= period){
                SPEAKER = 0;
                state = 2;
            }
            break;
    }
}