# Chenillard_Timer
msp430, C, interruption 

This project is based on the use of an msp430F5529 ( msp-EXP430F5529LP ), 8 leds, 1 button and 8 resistance that I configure to Blink one by one like a chenillard. 

          MSP430F5xxxLP
//         ---------------
//     /|\|            XIN|----|Switch|-----------|ground|
//      | |               |         
//      --|RST        XOUT|-
//        |               |
//        |-TIMERA    P3.2|-->LED0
//        |           P3.3|-->LED1
//        |           P3.4|-->LED2
//        |           P3.5|-->LED3
//        |           P4.0|-->LED4
//        |           P4.1|-->LED5
//        |           P4.2|-->LED6
//        |           P4.3|-->LED7

In this project, I use the TimerA of the Msp430 to manage the led's Blink and using the scrutation methods, to get when the button is pushed or not.

In the function, void init_TimerA0_Mode_Up_CCR0_Interrupt (void) ; I've need to get the time for which the led will blink.
To do that i've need to find for 200ms the equivalent in µs, 200 000µs but the max value that masked register TA0CCR0 can accept is 65535 so in order to manage the time of 200 000µs we need to divide this value by using the register TA0CTL of TIMERA.

By dividing with 4 the 200 000µs we have 50 000 which is less than 65535 so we can configure by putting 7th bit at 1 and 6th bit at 0 to have "10". Personnaly I configure to have a division by 8 for this value and have 25000 on TA0CCR0 in order to manage a timer speed at 200ms.

I also set the UP mode of the timer to made an interruption each 200ms, when the counter reach 25000 and return to 0, in order to make a loop.

The algorithm of the sequences is the following:
 
 VARIABLE:
 accumulator_1_1 : manage the on/off for the first sequence of the chenillard starting from LED0 -> LED7.
 accumulator_1_2 : manage the on/off for the first sequence of the chenillard starting from LED7 -> LED0.
 accumulator_2_1 : manage the on/off for the second sequence of the chenillard starting from LED0 -> LED3.
 accumulator_2_2 : manage the on/off for the second sequence of the chenillard starting from LED7 -> LED4.
 s1              : flag 0 = for the left to right display, flag 1 = for the right to left display.
 s2              : flag 0 = for the simultanous blink of the LED0 & LED7 going to the center LED3 & LED4 (end point of this flag), flag 1 = flag for the simultanous blink of the                    LED3 & LED4 going to the outside LED0 & LED7 (end point of this flag).
 Sequence        : Allow to manage the two different sequence, depending on push button.
 etat_precedent  : value dectect the push button.
 
 ALGORITHME:
 
 Sequence 1:
 
 IF accumulator_1_1 of Sequence 1 is on LED0 and we begin with s1 = flag 0 
  THEN
  ADD 1 to accumulator_1_1;
 ELSE IF accumulateur_1_1 doesn't reach led7 
  THEN
  ADD 1 to accumulator_1_1;
 ELSE IF accumulator_1_1 reach LED7 
  THEN
  SUBSTRACT 1 to accumulator_1_2;
  s1 = flag 1; // end the first chenillard 
 ELSE IF accumulator_1_2 doesn't reach LED0;
  THEN
  SUBSTRACT 1 to accumulator_1_2;
 ELSE IF accumulator_1_2 reach LED0
  THEN
  s1 = flag 0;
  accumulator_1_2 initialised to LED7;
  accumulator_1_1 initialised to LED0;
 END IF
 
 Sequence 2:
 
 IF accumulator_2_1 of Sequence 2 is on LED0 and accumulator_2_2 is on LED7 and we begin with s2 =    flag 0
  THEN
  ADD 1 to accumulator_2_1;
  SUBSTRACT 1 to accumulator_2_2;
 ELSE IF accumulator_2_1 doesn't reach LED3 and accumulator_2_2 doesn't reach LED4
  THEN
  ADD 1 to accumulator_2_1;
  SUBSTRACT 1 to accumulator_2_2;
 ELSE IF accumulator_2_1 reach LED3 and accumulator_2_2 reach LED4
  THEN
  s2 = flag 1;
 ELSE IF accumulator_2_1 doesn't reach LED0 and accumulator_2_2 doesn't reach LED7 and we begin s2 = flag 1
 THEN
 SUBSTRACT 1 to accumulator_2_1;
 ADD 1 to accumulator_2_2;
 ELSE IF accumulator_2_1 reach LED0 and accumulator_2_2 reach LED7 
  THEN
  s2 = flag 0;
 END IF
 
 DETECT PUSH BUTTON:
 
 IF Sequence == flag 0 and s1 == flag 0
  THEN
  Blink LED according to the value of accumulator_1_1;
  IF Push Button
    THEN 
    Sequence = 1;
  END IF
 ELSE IF Sequence == flag 0 and s1 == flag 1
  THEN
  Blink LED according to the value of accumulator_1_2;
  IF Push Button
    THEN
    Sequence = 1;
  END IF
 ELSE IF Sequence == flag 1 and s2 == flag 0
  THEN
  Blink LED according to the value of accumulator_2_1;
  IF Push Button
    THEN
    Sequence = 0;
  END IF
  ELSE IF Sequence == flag 1 and s2 == flag 1
    THEN
    Blink the LED according to the value of accumulator_2_2;
    IF Push Button
      THEN
      Sequence = flag 0;
    END IF
  END IF
