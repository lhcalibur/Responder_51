#include "stc89c52.h"
#include "marco.h"
#include "portmarco.h"
#include "systick.h"
#include "int0.h"
#include "led.h"
#include "segments_7.h"
#include "infrared.h"

__bit volatile infraIson = 1;               // 指示是否在用红外设定时间
                                            // __bit 定义一个bit的数据，详细请参考sdcc.pdf

int main(void)
{
	systickInit();                          // 初始化系统时钟(T0)，每10ms中断一次
	portENABLE_INTERRUPTS();                // 开中断
	segments7_Off();                        // 关掉数码管
	ledOff();
	int0Init();                             // init the int0 功能：检测抢答器按键按下
	int1Init();                             // inti the int1 功能：检测红外接收器
	timer1Init();
	
	while (systickGet() < 100)              // led 指示灯闪烁
	{                                       // 等待电路稳定后初始化中断
		ledBling();
	}
	ledOn();                                // turn on the LED
	SWITCH_OFF();                           // 关闭主持人按钮，打开红外接收器
	TIMER1_ON();                            // (T1)提供红外解码的精确计时
	INT1_ON();                              // 红外引脚（外部中断1）init
	segments7_On();                         // 打开数码管
	while (1) {
		infraredTask();                     // 红外接收程序
 	}



}


