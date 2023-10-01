#ifndef TAD_COUNTER_H
#define	TAD_COUNTER_H

#include <xc.h>

void CNT_Init(void);
//Pre: None
//Post: Inits the Counter TAD

void CNT_ShowTime(void);
//Pre: None
//Post: Shows the time

void CNT_ModifyTime(void);
//Pre: None
//Post: Modifies the time

char CNT_GetTime(unsigned char index);
//Pre: None
//Post: Returns the time in the index position (0 = dMinuts, 1 = uMinuts, 2 = dSegons, 3 = uSegons)

char *CNT_GetTimeArr(void);
//Pre: None
//Post: Returns the time array

void CNT_ResetTime(void);
//Pre: None
//Post: Resets the time to 00:00

void CNT_Motor(void);
//Pre: None
//Post: Step

#endif /* TAD_COUNTER_H */