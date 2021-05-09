/*
 * Timer_chenillard.h
 *
 *  Created on: 7 mai 2021
 *      Author: ballo
 */

#ifndef APP_INC_TIMER_CHENILLARD_H_
#define APP_INC_TIMER_CHENILLARD_H_


void Init_BP_P2SwitchNum(unsigned int SwitchNum);
unsigned int BP_GetState_SwitchNum(unsigned int SwitchNum);
void chenillard(unsigned char val);
void init_TimerA0_Mode_Up_CCR0_Interrupt (void);



#endif /* APP_INC_TIMER_CHENILLARD_H_ */
