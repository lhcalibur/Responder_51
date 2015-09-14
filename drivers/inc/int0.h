#ifndef INT0_H
#define INT0_H

void int0Init(void);
void int0_isr(void) __interrupt(0);
void light_Off(void);

#endif /* INT0_H */
