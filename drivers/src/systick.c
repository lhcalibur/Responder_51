#include "systick.h"
#include "stc89c52.h"
#include "mcs51reg.h"
#include "led.h"
#include "segments_7.h"
// sdcc 编译器建议将中断中出现要在非中断函数中使用的变量定义为 volatile
__data volatile sint8_t seg_num; // 数码管的显示数字

__bit volatile seg7_isOn = 0;

__data static volatile tick_t systick = 0;
__data volatile uint8_t scanning = 0;
__bit volatile AFF1Bling = 0;
__bit volatile AFF2Bling = 0;
__bit volatile AFF1BFlag = 0;
__bit volatile AFF2BFlag = 0;
__bit volatile segSet = 1;
extern __bit volatile infraIson;

__code uint8_t aff1_table[10] = {0b01000000, 0b01110101, 0b00100010, 0b00100100, 0b00010101, 0b00001100, 0b00001000, 0b01100101, 0b00000000, 0b00000100};		// 数码管1显示查表表格

__code uint8_t aff2_table[10] = {0b00010000, 0b01110101, 0b00100010, 0b00100100, 0b01000101, 0b00001100, 0b00001000, 0b00110101, 0b00000000, 0b00000100};		// 数码管2显示查表表格
	
static void systickHock(void);

void systickInit(void) __naked
{
	__asm__(
			"\n\torl TMOD, #0b00000001"
			"\n\tanl TMOD, #0b11110001"     // 16 bit clock
			"\n\tmov TH0, #0xd8"
			"\n\tmov TL0, #0xef"		// 10 ms
			"\n\torl 0xB7, #0b00000010"  	// IPH
			"\n\tsetb PT0"			// 0 privilege
			"\n\tsetb ET0"
			"\n\tsetb TR0"
			"\n\tret"
	       );
}

void t0_isr(void) __interrupt(1)
{
	systick++;
		if (seg7_isOn) {	
	systickHock();
	}
	__asm__(
			"\n\tmov TH0, #0xd8"
			"\n\tmov TL0, #0xef"		// 10 ms
	       );

}

tick_t systickGet(void)
{
	tick_t systick_count;
	/*
	portENTER_CRITICAL();
	systick_count = systick;
	portEXIT_CRITICAL();
	*/
	do {
	systick_count = systick;
	} while (systick_count != systick);
	return systick_count;
}

void systickLoopdelay(tick_t tick)
{
	tick += systickGet();
	while(systickGet() < tick);
}
static void systickHock(void)		// 计时及数码管扫描程序
{
	if (scanning == 100 && !segSet) {
		seg_num--;
		scanning = 0;
		if (seg_num == -1) {
			// add code here
			segments7_Off();
			ledOff();
			INT0_OFF();
			SWITCH_OFF();
			return;
		}
	} else {
		if (!(scanning % 64) ) {
			if (infraIson)
				P0_7 = !P0_7;
			if (AFF1Bling)
				AFF1BFlag = !AFF1BFlag;
			else if (AFF2Bling)
				AFF2BFlag = !AFF2BFlag;
		}
	}

	scanning++;
	if (scanning % 2) {		
		AFF1_OFF();
		if (!AFF1BFlag) {
			P2 &= 0b10000000;
			P2 |= aff2_table[seg_num % 10];		// 数码管2 显示个位
			AFF2_ON();
		}
	} else {	// display aff1
		AFF2_OFF();
		if (!AFF2BFlag) {
			P2 &= 0b10000000;
			P2 |= aff1_table[seg_num / 10];		// 数码管1 显示十位
			AFF1_ON();
		}
	}
}

