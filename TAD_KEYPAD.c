#include "TAD_KEYPAD.h"
#include "TAD_TIMER.h"
#include "TAD_LCD.h"
#include "TAD_BUZZER.h"

#define C0 PORTCbits.RC0
#define C1 PORTCbits.RC1
#define C2 PORTCbits.RC3
#define COLS PORTC //First 3 bits of PORTC are the COLS
#define ROWS PORTB //First 4 bits of PORTB are the ROWS

//Values for each button
static const char* KPAD_VALUES[12] = {
    "1","ABC2","DEF3",
    "GHI4","JKL5","MNO6",
    "PQRS7","TUV8","WXYZ9",
    "*","0 ","#"
};

static const char KPAD_INT_VALUES[4][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {9, 10, 11}
};

//How many presses can be done on each button
const char KPAD_PRESSES[12] = {
    1, 4, 4, 4, 4, 4, 5, 4, 5, 1, 2, 1
};

//Without SMS Mode, what char is returned when a button is pressed
const char KPAD_NO_SMS[12] = {
    0, 3, 3, 3, 3, 3, 4, 3, 4, 0, 0, 0
};

const char KPAD_NOTES[12] = {
    NOTE_1, NOTE_2, NOTE_3, NOTE_4, NOTE_5, NOTE_6, NOTE_7, NOTE_8, NOTE_9, NOTE_10, NOTE_11, NOTE_12
};

unsigned static char timerSms; //Timer for SMS Mode (KPAD_WAIT_MS)
unsigned static char timerRebots; //Timer for rebots on pressed keys
unsigned static char state; //State of Motor
unsigned static char smsMode; //SMS Mode

unsigned static char rowValues; //Values of the keypad rows (in LATC)

unsigned static char currentValue; //Last value pressed
unsigned static char previousValue; //Previous value pressed after currentValue

unsigned static char ready; //A new key is ready to be read with KPAD_GetChar()
unsigned static char smsPressed; //Whether the SMS Mode has ended (KPAD_WAIT_MS has passed)
unsigned static char pressCount; //Number of times a key has been pressed (SMS Mode)

void escombra(void);


void KPAD_Init(void){
    //Keypad Pinout (in order): F0 F1 F2 F3 i C0 C1 C2
    TRISCbits.TRISC0 = 1; //c0
    TRISCbits.TRISC1 = 1; //c1
    TRISCbits.TRISC3 = 1; //c2
    TRISBbits.TRISB0 = 0; //f0
    TRISBbits.TRISB1 = 0; //f1
    TRISBbits.TRISB2 = 0; //f2
    TRISBbits.TRISB3 = 0; //f3

    smsMode = ready = 0;
    state = 0; //Start with KPAD Stopped
    rowValues = 0x01; //First row powered
    TI_NewTimer(&timerSms);
    TI_NewTimer(&timerRebots);
}

void KPAD_StartInput(void){
    state = 1;
    ready = 0;
}

void KPAD_StopInput(void){
    state = ready = 0;
}

unsigned char KPAD_GetChar(void){
    if(!ready) return KPAD_NO_KEY;
    unsigned char aux = KPAD_VALUES[currentValue][smsMode ? pressCount : KPAD_NO_SMS[currentValue]];
    ready = pressCount = 0;
    return aux;
}

void KPAD_SetSMS(void){
    previousValue = KPAD_NO_KEY;
    smsMode = 1;
    smsPressed = 0;
}

void KPAD_UnsetSMS(void){
    smsMode = smsPressed = 0;
}

void KPAD_Motor(void){
    unsigned char currRow; //Last row pressed
    unsigned char currCol; //Last column pressed

    switch(state){
        case 0:
            break;
        case 1:
            if(smsPressed && TI_GetTics(timerSms) >= KPAD_WAIT_MS){
                smsPressed = 0;
                previousValue = KPAD_NO_KEY;
                ready = 1;
            }

            escombra();
            //If there is a column pressed, go to next state
            if(C0 == 1 || C1 == 1 || C2 == 1){
                ready = 0;
                state++;
                TI_ResetTics(timerRebots);
            }
            break;
        case 2:
            //If the column is not pressed anymore, return to state 0
            if(!(C0 == 1 || C1 == 1 || C2 == 1)){
                state = 1;
            }
            else if(TI_GetTics(timerRebots) >= KPAD_WAIT_REBOTS){ //If the column is still pressed, wait for KPAD_WAIT_REBOTS
                state++;
                TI_ResetTics(timerSms); //Start KPAD_WAIT_MS
            }
            break;
        case 3:
            //Read Column pressed (can be 8 (1000), 2 (0010) or 1 (0001))
            currCol = (COLS & 0x0B) >> 1; //0 or 1 or 2
            if(currCol == 4) currCol = 2; //If 4, set to 2
            //currCol = 0 or 1 or 2

            //Read Row pressed (can be 8 (1000), 4 (0100), 2 (0010) or 1 (0001))
            currRow = (ROWS & 0x0F) >> 1; //0 or 1 or 2 or 4
            if(currRow == 4) currRow--; //If 4, set to 3
            //currRow = 0 or 1 or 2 or 3

            currentValue = KPAD_INT_VALUES[currRow][currCol];

            if(smsMode && currentValue != 9 && currentValue != 11) state++;
            else state = 5;
            break;
        case 4: //SMS Mode
            //Check if previousValue == currentValue (if so, pressCount++)
            if(previousValue == currentValue){
                if(++pressCount == KPAD_PRESSES[currentValue]) pressCount = 0;
            }
            else{
                //If a key is pressed and the previous one is different, the other one is ready
                if(smsPressed){
                    currentValue = previousValue;
                    state = ready = 1;
                    smsPressed = 0;
                    break;
                }
                pressCount = 0;
            }

            smsPressed = 1;
            previousValue = currentValue;
            state++;
            break;
        case 5:
            //Wait for the column to be released
            if(!(C0 == 1 || C1 == 1 || C2 == 1)){
                state++;
                TI_ResetTics(timerRebots);
            }
            break;
        case 6:
            //Wait for KPAD_WAIT_REBOTS
            //If the column is pressed again, return to state 3 (wait for it to be released)
            if(C0 == 1 || C1 == 1 || C2 == 1){
                state--;
            }
            else if(TI_GetTics(timerRebots) >= KPAD_WAIT_REBOTS){
                state = 1;
                if(!smsMode || currentValue == 9 || currentValue == 11) ready = 1;
                else LCD_PreviewChar(KPAD_VALUES[currentValue][pressCount]);
                BUZZ_Play(KPAD_NOTES[currentValue]);
            }
            break;
    }
}


void escombra(void){
    rowValues <<= 1;
    if(rowValues == 0x10) rowValues = 0x01; //When goes from 00001000 to 00010000, reset to 00000001 (power the first row again)
    ROWS = (ROWS & 0xF0) | rowValues; //Modify only the first 4 bits of LATB
}