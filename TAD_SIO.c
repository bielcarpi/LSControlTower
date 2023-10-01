#include "TAD_SIO.h"
#include <xc.h>

#define CONFIGURACIO_TXSTA 0x20     //0010 0100 Dades de 8 bits, TX Enable, Asincron, BRGH=1
#define CONFIGURACIO_RCSTA 0x90     //1001 0000 Enable canal serie, 8 bits, asincron, RX enable
#define DIVISOR_BAUDRATE 42


void SIO_Init(void){
    // Post: Configuracio de la UART en mode asincron, 9600 bauds, fOsc=40MHz.
    TRISCbits.TRISC6 = 1; //TX
    TRISCbits.TRISC7 = 1; //RX

    BAUDCONbits.BRG16 = 1;
    TXSTA = CONFIGURACIO_TXSTA;
	RCSTA = CONFIGURACIO_RCSTA;
    SPBRG = DIVISOR_BAUDRATE;
    SPBRGH = 0;
}

unsigned char SIO_RXAvailable() {
    //Post: Retorna CERT si hi ha una caracter disponible.
    return PIR1bits.RCIF == 1;
} 

unsigned char SIO_GetChar() {
    // Pre: SIO_RXAvailable() ha retornat 1.
    // Post: Fa lectura destructiva del caracter rebut.
    return RCREG;
}

unsigned char SIO_TXAvailable(void){
    //Post: Retorna CERT si hi ha espai per a enviar un caracter, FALS en cas contrari.
    return PIR1bits.TXIF == 1;
}

void SIO_PutChar (unsigned char Valor){
    // Pre: SIO_TXAvailable() ha retornat 1.
    // Post: Posa un nou caracter a enviament.
    TXREG = Valor;
}