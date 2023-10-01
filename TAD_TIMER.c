// josepmaria.ribes@salle.url.edu (si hi trobeu alguna errada, si us plau envieu-me un correu :-)
// Arbeca, bressol de l'oliva arbequina
// Mar?, any del Senyor de 2023

// TAD TIMER. Honor i gl?ria

#include "TAD_TIMER.h"

// Definicions, per interrupci? cada 2ms.
#define T0CON_CONFIG 0xC3         // 11000011 -> TMR0ON=1, T08BIT=1, T0CS=0, T0SE=0, PSA=0, T0PS=011
#define RECARREGA_TMR0 100        // 256us, suposant FOsc a 40MHz (Tinst = 100ns).
                                  // 100*(10^-9) * 16 * (256-100) = 249,7us = 0.25ms

#define TI_NUMTIMERS 16              // Nombre de timers virtuals gestionats per aquest TAD. Si cal, s'incrementa o es disminueix...

// VARIABLES GLOBALS DEL TAD
struct Timer {
	unsigned long TicsInicials;
	unsigned char Busy;
} static Timers[TI_NUMTIMERS];

static volatile unsigned long Tics=0;

void RSI_Timer0 () {
    // Pre: IMPORTANT! Funci? que ha der ser cridada des de la RSI, en en cas que TMR0IF==1.
    TMR0L=RECARREGA_TMR0;
    TMR0IF=0;
    Tics++;    
}

void TI_Init () {
	for (unsigned char counter=0; counter<TI_NUMTIMERS; counter++) {
		Timers[counter].Busy=TI_FALS;
	}
	T0CON=T0CON_CONFIG;
    TMR0=RECARREGA_TMR0;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;
    // Caldr? que des del main o des d'on sigui s'activin les interrupcions globals!
}

unsigned char TI_NewTimer(unsigned char *TimerHandle) {
	unsigned char Comptador=0;
	while (Timers[Comptador].Busy==TI_CERT) {
		if (++Comptador == TI_NUMTIMERS) return (TI_FALS);
	}
	Timers[Comptador].Busy=TI_CERT;
	*TimerHandle=Comptador;
    return (TI_CERT);
}

void TI_ResetTics (unsigned char TimerHandle) {
	di(); Timers[TimerHandle].TicsInicials=Tics; ei();
}


unsigned long TI_GetTics (unsigned char TimerHandle) {
    di(); unsigned long CopiaTicsActual=Tics; ei();
	return (CopiaTicsActual-(Timers[TimerHandle].TicsInicials));
}

void TI_CloseTimer (unsigned char TimerHandle) {
	Timers[TimerHandle].Busy=TI_FALS;
}

void TI_Wait40ms (void) {
    unsigned char Timer;
    TI_NewTimer(&Timer);
    TI_ResetTics(Timer);
    while (TI_GetTics(Timer) < 160); //0.25ms * 4 = 1ms, * 40 = 40ms
    TI_CloseTimer(Timer);
}