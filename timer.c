/*
 * timer.c
 *
 *  Created on: Jun 4, 2018
 *      Author: Michael McCaffrey
 */

#include "timer.h"

void TimerInit(){

    TA0CCTL0 |= CCIE;                             // TACCR0 interrupt enabled
    TA0CCR0 = 0xffff;
    TA0CTL |= TASSEL__SMCLK | MC__UPDOWN | ID_3;      // SMCLK, continuous mode
    TA0EX0 |= TAIDEX_7;
}


