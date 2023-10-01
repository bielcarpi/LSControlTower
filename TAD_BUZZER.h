#ifndef TAD_BUZZER_H
#define	TAD_BUZZER_H

#define NOTE_1 4
#define NOTE_2 8
#define NOTE_3 12
#define NOTE_4 16
#define NOTE_5 20
#define NOTE_6 52
#define NOTE_7 56   
#define NOTE_8 60
#define NOTE_9 64
#define NOTE_10 20
#define NOTE_11 26
#define NOTE_12 14
#define MELODY_NOTES 12

const char MELODY[MELODY_NOTES] = {
    NOTE_1, NOTE_3, NOTE_8, NOTE_2, NOTE_1, NOTE_8, NOTE_10, NOTE_8, NOTE_9, NOTE_10, NOTE_10, NOTE_12
};

void BUZZ_Init(void);
//Pre: None
//Post: Inits the Buzzer TAD

void BUZZ_Play(unsigned char note);
//Pre: None
//Post: Plays the note

void BUZZ_PlayMelody(void);
//Pre: None
//Post: Plays the default melody

void BUZZ_Motor(void);
//Pre: None
//Post: Step of the Buzzer TAD

#endif