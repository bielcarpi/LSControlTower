#ifndef TAD_PORT_H
#define	TAD_PORT_H

#include <xc.h>

void PORT_Init(void);
//Pre: None
//Post: Inits the Menu TAD

void PORT_Mostrar(void);
//Pre: Que aquest menu no s'estigui mostrant
//Post: Comença a mostrar el menu PORT

char * PORT_GetName(void);
//Pre: None
//Post: Returns the name of the PORT

void PORT_Motor(void);
//Pre: None
//Post: Step of the Menu TAD Motor

#endif	/* TMENU_H */