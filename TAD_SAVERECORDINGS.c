#include "TAD_SAVERECORDINGS.h"
#include <xc.h>

static unsigned char state; //State of Motor
static unsigned char index; //Index to move through addresses in EEPROM
static unsigned char recordingField; //Field of the recording we are reading/writing


void SAVE_Init(void){
    //Pre: None
    //Post: Inits the Save Recordings TAD. Reads the recordings from the EEPROM and saves them in RAM
    // (for later quick use, as we have a lot of RAM remaining to be used)
    numRecordings = 0;
    state = 1; //Load the recordings from the EEPROM
    //2bytes for ID + 4bytes for timestamp = 6bytes per recording
    //20 recordings = 120bytes (byte 0 a byte 119)
    //Utilitzem el byte 120 per guardar el num de recordings
}

void SAVE_PutRecording(unsigned char id[2], char timestamp[4]){
    //Pre: numRecordings < MAX_RECORDINGS
    //Post: Saves the recording, given the id and the timestamp
    if(numRecordings >= MAX_RECORDINGS) return;

    //Save the new recording in RAM
    recordings[numRecordings].id[0] = id[0] - '0';
    recordings[numRecordings].id[1] = id[1] - '0';
    recordings[numRecordings].timestamp[0] = timestamp[0];
    recordings[numRecordings].timestamp[1] = timestamp[1];
    recordings[numRecordings].timestamp[2] = timestamp[2];
    recordings[numRecordings].timestamp[3] = timestamp[3];
    state = 5; //Save it in the EEPROM too (numRecordings++ is done when it is saved in the EEPROM)
    recordingField = 0;
}

void SAVE_Motor(void){
    //Pre: None
    //Post: Step of the Save Recordings TAD
    switch(state){
        case 0:
            break;
        case 1: //Load Recordings from EEPROM. 1st: Load numRecordings (byte 120)
            EEADR = 120; //Set address
            EECON1bits.EEPGD = 0; //Select EEPROM memory
            EECON1bits.CFGS = 0; //Access EEPROM memory
            EECON1bits.RD = 1; //Start the read operation
            state++;
            break;
        case 2: //Wait until read finishes
            if(EECON1bits.RD) break;
            numRecordings = EEDATA; //Save numRecordings
            if(numRecordings > MAX_RECORDINGS) numRecordings = 0; //The EEPROM default value is 0xFF, so if it is 0xFF, set it to 0
            index = recordingField = 0; //Start reading the recordings from the 0th address
            state++;
            break;
        case 3: //Read all numRecordings from EEPROM
            if(index == numRecordings){
                state = 0; //If we have read all recordings, stop
                break;
            }
            EEADR = (index*6) + recordingField; //Set address
            EECON1bits.EEPGD = 0; //Select EEPROM memory
            EECON1bits.CFGS = 0; //Access EEPROM memory
            EECON1bits.RD = 1; //Start the read operation
            state++;
            break;
        case 4: //Wait until read finishes, and save the recording in RAM
            if(EECON1bits.RD) break;
            if(recordingField < 2) //If we are reading the ID of the recording
                recordings[index].id[recordingField] = EEDATA;
            else
                recordings[index].timestamp[recordingField - 2] = EEDATA;
            
            if(recordingField == 5){ //If we have read the whole recording
                index++;
                recordingField = 0;
            }
            else recordingField++;

            state--;
            break;
        case 5: //Save a new recording in EEPROM
            if(recordingField >= 7){ //If we have written the whole recording
                state = 0; //Return to IDLE
                break;
            }

            //Set Address
            EEADR = (numRecordings*6) + recordingField;

            //Set Data (0 = ID[0], 1 = ID[1], 2 = timestamp[0], 3 = timestamp[1], 4 = timestamp[2], 5 = timestamp[3])
            if(recordingField < 2)
                EEDATA = recordings[numRecordings].id[recordingField];
            else if (recordingField >= 6){
                numRecordings++; //We have a new recording
                EEDATA = numRecordings;
                EEADR = 120; //Set numRecordings address
            }
            else
                EEDATA = recordings[numRecordings].timestamp[recordingField - 2];
            

            EECON1bits.EEPGD = 0; //Select EEPROM memory
            EECON1bits.CFGS = 0; //Access EEPROM memory
            EECON1bits.WREN = 1; //Enable writes

            //Disable interrupts & perform the unlock sequence
            INTCONbits.GIE = 0;
            EECON2 = 0x55;
            EECON2 = 0xAA;
            EECON1bits.WR = 1; //Start the write operation
            state++;
            break;
        case 6: //Wait until write finishes
            if(EECON1bits.WR) break;
            INTCONbits.GIE = 1; //Enable interrupts
            EECON1bits.WREN = 0; //Disable writes
            recordingField++;
            state--;
            break;
    }
}