#include "portmarco.h"
#include "segments_7.h"
#include "marco.h"
extern __data volatile sint8_t seg_num;

__data uint8_t seg_back = 30;

extern __bit volatile seg7_isOn;

extern __bit volatile segSet;

extern __data volatile uint8_t scanning;
extern __bit volatile AFF1Bling; 
extern __bit volatile AFF2Bling;
extern __bit volatile AFF1BFlag;
extern __bit volatile AFF2BFlag;

void segments7_On(void) 	// Open AFF1 and AFF2
{
	portENTER_CRITICAL();
	AFF1Bling = 0;
	AFF2Bling = 0;
	seg_num = seg_back;		// 计时时间（默认 30 s）
	seg7_isOn = 1;
	portEXIT_CRITICAL();
}


void segments7_Off(void)	// 关闭数码管并复位
{
	AFF1_OFF();
	AFF2_OFF();
	portENTER_CRITICAL();
	AFF1Bling = 0;
	AFF2Bling = 0;
	scanning = 0;
	seg7_isOn = 0;
	segSet = 1;
	portEXIT_CRITICAL();
}

void segments7_Go(void)		// 数码管开始倒计时，并允许int0中断
{
	if (seg_num != 0) {
		portENTER_CRITICAL();
		seg_back = seg_num;
		AFF1Bling = 0;
		AFF2Bling = 0;
		AFF1BFlag = 0;
		AFF2BFlag = 0;
		scanning = 0;
		segSet = 0;
		portEXIT_CRITICAL();
	}
}

void segments7_AFF1_Bling(void)	// 数码管1闪烁程序
{
	portENTER_CRITICAL();
	AFF2Bling = 0;
	AFF2BFlag = 0;
	AFF1Bling = 1;
	portEXIT_CRITICAL();
}

void segments7_AFF2_Bling(void) // 数码管2闪烁程序
{
	portENTER_CRITICAL();
	AFF1Bling = 0;
	AFF1BFlag = 0;
	AFF2Bling = 1;
	portEXIT_CRITICAL();
}

void segments7_Setnum(uint8_t num) // 设置定时设置
{
	portENTER_CRITICAL();
	if (AFF1Bling) {
		seg_num = (seg_num/10*10+num);
	} else if (AFF2Bling) {
		seg_num = (seg_num%10)+num*10;
	}
	portEXIT_CRITICAL();
}

__bit segments7_Is_segSet(void) // 返回定时器是否设置完毕状态
{
	return segSet;
}
