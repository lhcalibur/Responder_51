#ifndef MARCO_H
#define MARCO_H

#define SWITCH_ON()	\
			__asm	\
				setb P3.6	\
			__endasm

#define SWITCH_OFF()	\
			__asm	\
				clr P3.6	\
			__endasm

#define AFF1_OFF()	\
			__asm	\
				setb P1.7	\
			__endasm

#define AFF1_ON()	\
			__asm	\
				clr P1.7	\
			__endasm

#define AFF2_OFF()	\
			__asm	\
				setb P2.7	\
			__endasm

#define AFF2_ON()	\
			__asm	\
				clr P2.7	\
			__endasm

#define INT0_ON()	EX0 = 1

#define INT0_OFF()	EX0 = 0

#define INT1_ON()	EX1 = 1

#define INT1_OFF()	EX1 = 0

#define TIMER1_ON()	TR1 = 1

#define TIMER1_OFF()	TR1 = 0



#endif /* MARCO_H */
