#ifndef TAD_MQSINA_H_
#define TAD_MQSINA_H_

#define MQ_STEP 4000 //Step of the Marquesina. Each step, 1s (0.25ms * 4000)

void MQ_Init(void);
//Pre: None
//Post: Inits the Marquesina TAD

void MQ_Clear(void);
//Pre: None
//Post: Clears the Marquesina TAD, and stops the Strings that are being printed

void MQ_PutString(char *s, unsigned char line);
//Pre: s != null && line = 0 or 1
//Post: Prints a string in the LCD screen with the marquesina effect

void MQ_Motor(void);
//Pre: None
//Post: Step of the Marquesina TAD

#endif