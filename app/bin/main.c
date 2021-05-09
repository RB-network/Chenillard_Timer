/*---------------------------------------------------------------
 * INCLUDE
 ----------------------------------------------------------------*/
#include <msp430.h> 
#include "Timer_chenillard.h"
#include "main.h"

/*----------------------------------------------------------------
 * CONSTANTES ET MACROS: TYPEDEF et DEFINE
 --------------------------------------------------------------- */
#define PORT2_VECTOR (2*2u) /*0xFFD2 Port 2*/
/*----------------------------------------------------------------
 * DECLARATIONS DES VARIABLES GLOBALES
 ----------------------------------------------------------------*/
volatile int accumulator_1_1=0; //flag d'incrément de la valeur des leds pour le chemin aller
volatile int accumulator_1_2=7; //flag d'incrément de la valeur des leds pour le chemin retour
volatile int accumulator_2_1=0; //flag d'incrément de la valeur des premières leds 0 -> 3
volatile int accumulator_2_2=7; //flag d'incrément de la valeur des dernières leds 4 -> 7
volatile int Sequence = 0; //Variable flag qui permet de gérer la Séquence à réaliser.
volatile int s1=0; //Variable flag aller de la séquence 1
volatile int s2=0; //Variable flag aller de la séquence 2

unsigned int etatprecedantP2p3;
/**
 * main.c
 */


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	// initialisation
	// frequence par defaut = 4 MHz
	// configurer les pattes ou pins
	//Initialisation du bouton poussoir P2.3
	Init_BP_P2SwitchNum(3);

	init_TimerA0_Mode_Up_CCR0_Interrupt();
	_BIS_SR(GIE);

	//Initialisation des 4 premiers LED
	P3DIR |= (1<<2);
	P3DIR |= (1<<3);
	P3DIR |= (1<<4);
	P3DIR |= (1<<5);
	//Initialisation des 4 dernières LED
	P4DIR |= (1<<0);
	P4DIR |= (1<<1);
	P4DIR |= (1<<2);
	P4DIR |= (1<<3);
	etatprecedentP2p3 = BP_GetState_SwitchNum(3);

	while(1){
	    if(Sequence==0 && s1==0){
	        chenillard(1<<accumulator_1_1);
	        if((BP_GetState_SwitchNum(3)==0)&&(etatprecedentP2p3==1)){
	            Sequence=1;
	        }etatprecedentP2p3=BP_GetState_SwitchNum(3);
	    }else if(Sequence==0 && s1==1){
	        chenillard(1<<accumulator_1_2);
	        if((BP_GetState_SwitchNum(3)==0)&&(etatprecedentP2p3==1)){
	            Sequence=1;
	        }etatprecedentP2p3=BP_GetState_SwitchNum(3);
	    }else if(Sequence==1 && s2==0){
	        chenillard((1<<accumulateur_2_1)|(1<<accumulateur_2_2));
	        if((BP_GetState_SwitchNum(3)==0)&&(etatprecedentP2p3==1)){
	            Sequence=0;
	        }etatprecedentP2p3=BP_GetState_SwitchNum(3);
	    }else if(Sequence==1 && s2==1){
	        chenillard((1<<accumulateur_2_1)|(1<<accumulateur_2_2));
	        if((BP_GetState_SwitchNum(3)==0)&&(etatprecedentP2p3==1)){
	            Sequence=0;
	        }etatprecedentP2p3=BP_GetState_SwitchNum(3);
	}
}
	/*----------------------------------------------------------------------------
	__interrupt void Timer_A(void) : Notez que cette n'est pas appélee dans le MAIN
	                         Mais est executée par déclenchement matérielle
	    Routine interruption du Timer Timer_A :
	    vecteur interruption ou adresse memoire = TIMER0_A0_VECTOR
	    Attention a la notation de TI :
	    le Vecteur d'interruption TIMER0_A0_VECTOR est utiliser uniqument par
	    les evenements declenches par CCR0 :
	    Valeur courant (TAR) du timer egale a celle de TA0CCR0
	    Cette fonction est appelee/declenchee a chaque fois que:
	    2. demande interruption suite a un evenement changement dans les bits de controle Capture/compare 1
	    la valeur courante du TIMER A vaut celle du registre TA0CCR0

	 @param  : aucun
	 @retval : aucun
	 *----------------------------------------------------------------------------*/

	#pragma vector=TA0CCR0     // vecteur interruption TA0CCR0
	__interrupt void Timer_A (void)
	{
	    //Séquence 1
	    if((((Sequence==0)&&(accumulator_1_1==0))|((Sequence==0)&&(accumulator_1_1<7)))&&(s1==0)){
	       accumulator_1_1++;
	       if(accumulator_1_1==7)s1=1;
	    }else if((((Sequence==0)&&(accumulator_1_1==7))|((Sequence==0)&&(accumulator_1_2>0)))&&(s1==1)){
	       accumulator_1_2--;
	       if(accumulator_1_2==0){
		       s1=0;
		       accumulator_1_1=0;
		       accumulator_1_2=7;
	       }
	    }

	    //Séquence 2
	    if((((Sequence==1)&&(accumulator_2_1==0)&&(accumulator_2_2==7))|((Sequence==1)&&(accumulator_2_1<3)&&(accumulator_2_2>4)))&&(s2==0)){
	        accumulator_2_1++;
	        accumulator_2_2--;
	        if(accumulator_2_1==3 && accumulator_2_2==4)s2=1;
	    }else if((((Sequence==1)&&(accumulator_2_1==3)&&(accumulator_2_2==4))|((Sequence==1)&&(accumulator_2_1>0)&&(accumulator_2_2<7)))&&(s2==1)){
	        accumulator_2_1--;
	        accumulator_2_2++;
	        if(accumulator_2_1 ==0 && accumulator_2_2 ==7)s2=0;
	    }

	 /* pas de rechargement de TA0CCR0 :
	    car en mode uP le timer  compte jusqu a la valeur TACCR0 et recommence
	    */

	}

