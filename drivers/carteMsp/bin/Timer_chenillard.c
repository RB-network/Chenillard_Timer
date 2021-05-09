/*
 * Timer_chenillard.c
 *
 *  Created on: 7 mai 2021
 *      Author: ballo
 */

/****
 * nom : BP_GetState_S1(void)
 *  Que la fait la fonction : test etat de pin : pin 1 de P2
 *  Parametre : Non
 *  return : etat du pin : 0 ou 1
 *  Effets de bord : initialiser le pin Input et activer pull-up
 */

#include <msp430.h>

unsigned int BP_GetState_SwitchNum(unsigned int SwitchNum)
{
    unsigned int state;

    if (/* appuyer : bit bx de P2IN a '0'*/ !((P2IN & (1<<SwitchNum)) == (1<<SwitchNum)) )  {

        state=0;

        }
        else {

        state=1;

        }
    return state;
}

/****
 * nom : Init_BP_S1()
 *  Que la fait la fonction : init P2.1 en Input avec Pull-Up
 *  Parametre : Non
 *  return : Rien
 *  Effets de bord : Rien
 */

 void Init_BP_P2SwitchNum(unsigned int SwitchNum) {
    // mettre P2.x en input pour bouton possoir
    // mettre le bit bx du registre P2DIR a '0'
     P2DIR &= ~ (1<<SwitchNum);
    // activer les resistances de tirage
    // ecrire '1' sur le bit bx du registre P2REN
    P2REN |= (1<<SwitchNum);
    // choisir pull up pour  P2.x
    P2OUT |= (1<<SwitchNum);
}


/*==============================================================================
   FUNCTION:    void chenillard(unsigned char val)
   DESCRIPTION:  BlinkLed + read the push on button
   PARAMETERS:   val : valeur de huit bits
   RETURNS:    rien
   REQUIREMENTS: Detection de l'état du bouton avant test d'appui
 ===============================================================================*/
void chenillard(unsigned char val){

    if(val & (1<<0)){P3OUT|=(1<<2);}else{P3OUT&= ~(1<<2);}
    if(val & (1<<1)){P3OUT|=(1<<3);}else{P3OUT&= ~(1<<3);}
    if(val & (1<<2)){P3OUT|=(1<<4);}else{P3OUT&= ~(1<<4);}
    if(val & (1<<3)){P3OUT|=(1<<5);}else{P3OUT&= ~(1<<5);}
    if(val & (1<<4)){P4OUT|=(1<<0);}else{P1OUT&= ~(1<<2);}
    if(val & (1<<5)){P4OUT|=(1<<1);}else{P1OUT&= ~(1<<3);}
    if(val & (1<<6)){P4OUT|=(1<<2);}else{P1OUT&= ~(1<<4);}
    if(val & (1<<7)){P4OUT|=(1<<3);}else{P1OUT&= ~(1<<5);}
    return;
}


/*===============================================================================
  FUNCTION:     init_TimerA0_Mode_Up_CCR0_Interrupt
  DESCRIPTION:  Configuration du Timer A en mode UP avec demande interruption
                CCR0
                choix horloge SMCLK sans diviseur
  PARAMETERS:   rien
  RETURNS:      rien
  REQUIREMENTS: rien
===============================================================================*/

 void init_TimerA0_Mode_Up_CCR0_Interrupt (void)
 {
// Configuration de l'horloge du TIMERA0 : Source et frequence
    // selection de la source horloge pour le timerA
    // ici on prend SMCLK
    // bits b9b8 (TASSELx) de TA0CTL :
    //TACLK->"00"; ACLK->"01", SMCLK->"10"

    TA0CTL |=(1<<9);
    TA0CTL =TA0CTL&(~(1<<8));

    // Definition de la frequence: diviseur de frequence
    // bits b7b6 (IDx)du registre TA0CTL
    // "00" -> /1
    // "01" -> /2
    // "10" -> /4
    // "11" -> /8
    // Ici  diviser par 1: b7b6->"00"
    TA0CTL =TA0CTL|((1<<7));
    TA0CTL =TA0CTL|((1<<6));
    // donc : TIMERA incremente chaque 1 miroseconde = 1/1.048576 MHzz

    // choix du mode de fonctionnement du TIMERA
    // Bits b5b4(MCx) de TA0CTL
    // "00" -> Stop mode: the timer is halted.
    // "01" -> Up mode: the timer counts up to TACCR0.
    // "10" -> Continuous mode: the timer counts up to 0FFFFh.
    // "11" -> Up/down mode: the timer counts up to TACCR0 then down to 0000h.
    // Ici on prend le mode UP
    // Le Timer compte de 0 a TACCR0et recommence
    // nous utiliserons l'interruption liee au Capture/compare : CCR0
    TA0CTL = TA0CTL &(~(1<<5));
    TA0CTL = TA0CTL|(1<<4);

    // comfiguration du temps
    // nous allons utiliser le registre CCR0 (capture/compare)
    // nous mettrons une valeur (ici 10000) dans CCR0
    // si la valeur courante du TIMER vaut celle de CCR0=10000
    // il y a generation d'une interruption (bit b0 (CCIFG) du registre CCTL0)
    // si nous souhaitons utiliser cette source Interruption
    // il nous faut :
    // 1) activer la source Interruption (instruction sous-dessous)
    // 2) ecrire une valeur dans le registre CCR0 : cette valeur determinera la periode
    // 3) demasquer la source Interruption (instruction avant le while(1)
    // 4) ecrire la routine Interruption: Ce que nous allons faire s'il y a une interruption

    TA0CCR0 = 25000;   // caque 10 ms on genere une interruption


    //activation de la source interruption Capture/compare registre CCR0
    // bit b4 (CCIE) de CCTL0
    TA0CCTL0 = TA0CCTL0|(1<<4);
 }


