#ifndef TAD_MENU_H
#define	TAD_MENU_H

#include <xc.h>

void MENU_Init(void);
//Pre: None
//Post: Inits the Menu TAD

void MENU_Mostrar(void);
//Pre: Que aquest menu no s'estigui mostrant
//Post: Comença a mostrar el menu principal

void MENU_Motor(void);
//Pre: None
//Post: Step of the Menu TAD Motor

#endif	/* TMENU_H */