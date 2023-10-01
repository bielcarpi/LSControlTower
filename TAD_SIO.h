#ifndef TAD_SIO_H_
#define TAD_SIO_H_

void SIO_Init(void);
// Post: Configuracio de la UART en mode asincron, 9600 bauds, fOsc=40MHz.

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

#endif