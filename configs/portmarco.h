#ifndef PORTMARCO_H
#define PORTMARCO_H

#include "stc89c52.h"

/* Critical section management. */
#define portENTER_CRITICAL()		__asm		\
									push	ACC	\
									push	IE	\
									__endasm;	\
									EA = 0

#define portEXIT_CRITICAL()			__asm			\
									pop		ACC		\
									__endasm;		\
									ACC &= 0x80;	\
									IE |= ACC;		\
									__asm			\
									pop		ACC		\
									__endasm


#define portDISABLE_INTERRUPTS()	EA = 0
#define portENABLE_INTERRUPTS() 	EA = 1


#define TRUE	1
#define FALSE	0
#define OK	0
#define ERROR 	-1

typedef char int8_t;
typedef signed char sint8_t;
typedef unsigned char uint8_t;
typedef unsigned long uint32_t;

typedef unsigned long tick_t;
typedef uint8_t stack_t;

#endif /* PORTMARCO_H */


