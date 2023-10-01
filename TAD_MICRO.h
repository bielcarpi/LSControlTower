#ifndef TAD_MICRO_H
#define	TAD_MICRO_H

#define MICRO_SAMPLES_TO_SEND 32770 //4K samples per second, 8 seconds, 32768 samples

void MICRO_Init(void);
//Pre: None
//Post: Inits the Micro TAD

void MICRO_ListenAndSend(void);
//Pre: Que hagi passat MICRO_LISTENING_TIME des de que s'ha cridat aquesta funcio per ultima vegada
//Post: Listens for 8 seconds while sending the data through SIO

void MICRO_Motor(void);
//Pre: None
//Post: Step of the MICRO TAD Motor

#endif /* TAD_MICRO_H */