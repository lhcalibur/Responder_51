#include "led.h"
#include "systick.h"


void ledBling(void)		// LED 闪烁程序
{
	ledOff();	
	systickLoopdelay(10);
	ledOn();
	systickLoopdelay(10);
}


