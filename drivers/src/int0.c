#include "stc89c52.h"
#include "mcs51reg.h"
#include "int0.h"
#include "systick.h"
#include "portmarco.h"
#include "marco.h"
#include "segments_7.h"
#include "led.h"
#include "infrared.h"

extern __bit volatile infraIson;

__code static unsigned char led_array[8] = {0x00, 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf}; 	// led 灯查表表格

extern __code uint8_t aff2_table[10];
void int0Init(void) __naked		// 初始化int0
{
	__asm
		clr  EX0
		setb IT0
		ret
	__endasm;

}

void int0_isr(void) __interrupt(0) 	// int0 中断程序, 输入P0.0~P0.2 将对应的led灯通过查表的方式输出
{
	if (!(P0 & 0x07)){	//此处是为了防止开始抢答前按下抢答键 ，以至于打开int0的瞬间进入中断处理程序得到无效输入（P0.0~P0.2 = 000）；若开始抢答前已经有按键按下，则刷新输入
		SWITCH_OFF();
		SWITCH_ON();
		return;
	}
	INT0_OFF();
	segments7_Off();
	ledOff();
	P1 |= 0xff;
	P1 &= led_array[P0 & 0x07];
	P2 &= 0b10000000;
	P2 |= aff2_table[P0 & 0x07];		// 数码管2 显示个位
	AFF2_ON();
	//INT1_ON();
	//TIMER1_ON();
}

void light_Off(void)
{
	P1 |= 0x7f;
}
