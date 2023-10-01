/****************
 * DICCIONARI.c
 * P2 Fase B - Compus 2022/23
 * Alex Cano i Biel Carpi
****************/

/* INTERFICIE 0 - MENU a MICRO */
void MICRO_ListenAndSend(void);
//Pre: Que hagi passat MICRO_LISTENING_TIME des de que s'ha cridat aquesta funcio per ultima vegada
//Post: Listens for 8 seconds while sending the data through SIO


/* INTERFICIE 34 - MENU a MARQUESINA */
void MQ_Clear(void);
//Pre: None
//Post: Clears the Marquesina TAD, and stops the Strings that are being printed

void MQ_PutString(char *s, unsigned char line);
//Pre: s != null && line = 0 or 1
//Post: Prints a string in the LCD screen with the marquesina effect


/* INTERFICIE 23 - MENU a PORT */
void PORT_Mostrar(void);
//Pre: Que aquest menu no s'estigui mostrant
//Post: Comença a mostrar el menu PORT

char * PORT_GetName(void);
//Pre: None
//Post: Returns the name of the PORT


/* INTERFICIE 33, 22, 29, 18 - PORT, MENURECORDINGS, COUNTER, MICRO a MENU */
void MENU_Mostrar(void);
//Pre: Que aquest menu no s'estigui mostrant
//Post: Comença a mostrar el menu principal


/* INTERFICIE 1 - MENU a SAVERECORDINGS */
void SAVE_PutRecording(unsigned char id[2], char timestamp[4]);
//Pre: numRecordings < MAX_RECORDINGS
//Post: Saves the recording, given the id and the timestamp


/* INTERFICIE 3, 4, 8 - PORT, MENU i MENURECORDINGS a KEYPAD */
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


/* INTERFICIES 5, 6 - MENU, MENURECORDINGS a JOYSTICK */
void JOY_Enable(void);
//Pre: None
//Post: Enables the Joystick TAD

void JOY_Disable(void);
//Pre: None
//Post: Disables the Joystick TAD

unsigned char JOY_GetMove(void);
//Pre: None
//Post: Returns the move of the Joystick, either UP, DOWN or NO_MOVE

/* INTERFICIE 19 - MENU a COUNTER */
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


/* INTERFICIE 10, 26, 27, 13, 3, 31, 9, 17 - PORT, MARQUESINA, TIMER, KEYPAD, MENU, COUNTER, MENURECORDINGS, MICRO a LCD*/
void LCD_Clear(void);
// Post: Clears the display and sets the cursor to its previous state. 
// Post: The next order can last up to 1.6ms. 

void LCD_CursorOn(void);
// Post: Turn on the cursor
// Post: The next order can last up to 40us. 

void LCD_CursorOff(void);
// Post: Turns off the cursor
// Post: The next order can last up to 40us. 

void LCD_GotoXY(char Column, char Row);
// Pre : Column between 0 and 39, row between 0 and 3. 
// Post: Sets the cursor to those coordinates. 
// Post: The next order can last up to 40us. 

void LCD_PutChar(char c);
// Pre: LCD_IsAvailable() == 1
// Post: Paints the char in the actual cursor position and increments 
// its position. If the column gets to 39 it returns to 0.
// The row of the LCD is increased when this happens until the second
// row and then it is reset back to row 0 if it has 2 rows total. 
// If the LCD has 4 rows it will reset back to row 0 when it
// reaches row 4 and the columns will go till 39 before reseting to 0.
// The one row LCDs returns to 0 when a column gets to 39. 
// The row is never increased. 
	// The char is written

void LCD_PreviewChar(char c);
// Pre: LCD_IsAvailable() == 1
// Post: Paints the char in the actual cursor position without incrementing

void LCD_PutString(char *s);
// Pre: LCD_IsAvailable() == 1
// Post: Paints the string from the actual cursor position. 
// The coordinate criteria is the same as the LcPutChar. 


/* INTERFICIE 12, 11 - KEYPAD, MENURECORDINGS a BUZZER */
void BUZZ_Play(unsigned char note);
//Pre: None
//Post: Plays the note

void BUZZ_PlayMelody(void);
//Pre: None
//Post: Plays the default melody


/* INTERFICIE 14, 25, 32, 28, 20, 24 - KEYPAD, MARQUESINA, BUZZEr, MENURECORDINGS, COUNTER, MICRO, MENU a TIMER */
unsigned char TI_NewTimer(unsigned char *TimerHandle) ;
	// Post: Retorna TI_CERT en cas que s'hagi creat adequadament un nou timer, i TI_FALS en cas contrati.
    // Posa a *TimerHandle l'identificador de timer assignat, necessari per usar les funcions TI_GetTics i TI_ResetTics.

void TI_ResetTics (unsigned char TimerHandle);
	// Pre: Handle ha estat retornat per Ti_NewTimer.
	// Post: Engega la temporitzaci? associada a 'TimerHandle', guardant la referencia temporal en el moment de la crida.

unsigned long TI_GetTics (unsigned char TimerHandle);
	// Pre: Handle ha estat retornat per TI_NewGetTimer.
	// Post: Retorna els tics transcorreguts des de la crida a TI_ResetTics per al mateix TimerHandle.


/* INTERFICIE 2 - MENU a MENURECORDINGS */
void MRCD_Mostrar(void);
//Pre: Que aquest menu no s'estigui mostrant
//Post: Comença a mostrar el menu Recordings


/* INTERFICIE 15, 23, 16, 30 - PORT, MENURECORDINGS, MICRO, COUNTER a SIO*/
unsigned char SIO_RXAvailable(void);
// Post: Retorna 1 si hi ha espai per a enviar un caracter, 0 en cas contrari.

unsigned char SIO_GetChar(void);
// Pre: SIO_CharAvailable() ha retornat 1.
// Post: Fa lectura destructiva del caracter rebut.

unsigned char SIO_TXAvailable(void);
//Post: Retorna 1 si hi ha espai per a enviar un caracter, 0 en cas contrari.

void SIO_PutChar (unsigned char Valor);
// Pre: SIO_TXAvailable() ha retornat 1.
// Post: Posa un nou caracter a enviament.