#ifndef LED_H
#define LED_H

#define ledOn()	\
	__asm	\
		clr P0.7	\
	__endasm

#define ledOff()	\
	__asm	\
		setb P0.7	\
	__endasm

void ledBling(void);

#endif /* LED_H */
