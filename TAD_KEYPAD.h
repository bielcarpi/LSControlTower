#ifndef TKEYPAD_H
#define	TKEYPAD_H

#include <xc.h>

#define KPAD_WAIT_MS 4000 //Wait 0.25us * 4000 = 1s in SMS mode
#define KPAD_WAIT_REBOTS 80 //Wait X (0.25us * 4 * 20) MS for rebots on pressed keys
#define KPAD_NO_KEY 0xFF

void KPAD_Init(void);
//Pre: None
//Post: Inits the Keypad TAD

unsigned char KPAD_GetChar(void);
//Pre: None
//Post: Returns the char pressed in the Keypad

void KPAD_StartInput(void);
//Pre: None
//Post: Starts listening the Keypad

void KPAD_StopInput(void);
//Pre: None
//Post: Stops listening the Keypad

void KPAD_SetSMS(void);
//Pre: None
//Post: Sets the SMS mode to true

void KPAD_UnsetSMS(void);
//Pre: None
//Post: Sets the SMS mode to false

void KPAD_Motor(void);
//Pre: None
//Post: Step of the Keypad TAD Motor

#endif	/* TKEYPAD_H */