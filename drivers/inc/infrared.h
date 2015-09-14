#ifndef INFRARED_H
#define INFRARED_H


//void infraredHock(void);
void timer1Init();	//定时器0初始化
void timer1(void) __interrupt(3);  //定时器0中断函数
void int1Init(void) __naked;

void int1_isr(void) __interrupt(2);
void infraredTask(void);

#endif
