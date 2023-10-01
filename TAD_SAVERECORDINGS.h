#ifndef TAD_SAVE_RECORDINGS_H_
#define TAD_SAVE_RECORDINGS_H_

#define MAX_RECORDINGS 10

struct Recording{
    unsigned char id[2];
    char timestamp[4];
}; 

struct Recording recordings[MAX_RECORDINGS]; //Array of recordings
unsigned char numRecordings; //Total num of recordings

void SAVE_Init(void);
//Pre: None
//Post: Inits the Save Recordings TAD. Reads the recordings from the EEPROM and saves them in RAM
// (for later quick use, as we have a lot of RAM remaining to be used)

void SAVE_PutRecording(unsigned char id[2], char timestamp[4]);
//Pre: numRecordings < MAX_RECORDINGS
//Post: Saves the recording, given the id and the timestamp

void SAVE_Motor(void);
//Pre: None
//Post: Step of the Save Recordings TAD

#endif