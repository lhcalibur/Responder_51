#ifndef SYSTICK_H
#define SYSTICK_H

#include "portmarco.h"
#include "marco.h"

void systickInit(void) __naked;
void t0_isr(void) __interrupt(1);
tick_t systickGet(void);
void systickLoopdelay(tick_t tick);

#endif /* SYSTICK_H */
