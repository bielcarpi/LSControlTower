#ifndef TAD_JOYSTICK_H_
#define TAD_JOYSTICK_H_

#define JOY_NO_MOVE 0
#define JOY_UP 1
#define JOY_DOWN 2

void JOY_Init(void);
//Pre: None
//Post: Inits the Joystick TAD

void JOY_Enable(void);
//Pre: None
//Post: Enables the Joystick TAD

void JOY_Disable(void);
//Pre: None
//Post: Disables the Joystick TAD

unsigned char JOY_GetMove(void);
//Pre: None
//Post: Returns the move of the Joystick, either UP, DOWN or NO_MOVE

void JOY_Motor(void);
//Pre: None
//Post: Step of the Joystick TAD

#endif