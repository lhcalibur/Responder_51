//#################################################################################################
//文件：51单片机的红外遥控解码程序
//属性：使用外部中断0连接遥控头，接收红外遥控键值送P2口显示，12MHz外部晶振
//#################################################################################################
#include "mcs51reg.h"
#include "stc89c52.h"
#include "portmarco.h"
#include "infrared.h"
#include "marco.h"
#include "led.h"
#include "segments_7.h"
#include "int0.h"



extern __bit infraIson;

__data volatile uint8_t irTimeCounts=0;	  		  //定时器0在方式2下8位自动重装时的中断计数值
__data uint8_t irTimeCountsArr[32]={0};	  //存放红外接收数据时的中断次数记录值,
__data uint8_t bitNum=0;		  		  //标志当前接收的是第几个比特位
__bit irReceFlag=0;	  		  //红外接收一帧数据未完成标志,为1时完成
__data uint8_t irCodeByteData[4]={0}; 	  //保存接收到的4个字节的有效数据
uint8_t irTimeCountsArrProcessOk=0;//对接收到的33位数据处理未完成标志,1完成

uint8_t irTimeCountsArrProcess();    //解析中断次数,即解码
/*
uint8_t* irCodeByteDataProcessForSmg();//将遥控器码值处理成数码管可显示数据

uint8_t* irCodeByteDataProcessForSmg() //将解码的4字节数据处理成数码管可显示的数据
{	
	if(irTimeCountsArrProcessOk)  //检测一帧数据是否解析完成
	{
		//这里的用户码只显示低八位,因为高八位反正都是00(手上2个遥控器都是00)
		//然后还显示遥控键值及其反码,我们的数码管只有6位,只显示3字节数据
		if(irCodeByteData[2]+irCodeByteData[3]!=0xff)//校验数据的完整性
		{									 //最后一个字节是键码的反码
			led6=0;	  //调试代码
		}
		else
		{
			smgWela[0]=irCodeByteData[1] >> 4 ;	   //取高4位
			smgWela[1]=irCodeByteData[1] & 0x0f;   //取低4位
			smgWela[2]=irCodeByteData[2] >> 4 ;
			smgWela[3]=irCodeByteData[2] & 0x0f;
			smgWela[4]=irCodeByteData[3] >> 4 ;
			smgWela[5]=irCodeByteData[3] & 0x0f;
		    smgWela[6]=0xff;	//小数点全不显	
		}
		irTimeCountsArrProcessOk=0;//标志清零,下一次有未解析的数据时才会再解析
	}
	return 	 smgWela;
}
*/
uint8_t irTimeCountsArrProcess()	//对接收到的32位的中断次数数据进行解析
{
	uint8_t i,j,k=0,value=0;
	if(irReceFlag)			   //检测是否已经接收到新的4字节的红外通信数据
	{
		for(j=0;j<4;j++)   		 //有4个字节
		{
			for(i=0;i<8;i++)	 //对每个字节的8位数据处理
			{
				value>>=1;
				if(irTimeCountsArr[k++]>5)	 //这里我们用6或7都是可以的
				{
					 value|=0x80;			 //大于6的话该位数据是1
				}
			}
			irCodeByteData[j]=value;//保存该字节,也就是遥控器的键码	
		}
		irReceFlag=0;	//接收标志清零,这样就会等到下次收到数据后才会再解析
		irTimeCountsArrProcessOk=1; //中断数据解析完毕标志置1
		return TRUE;		  	    //解析完成
	}
	return FALSE;	//未进行解析,该返回值主要是为了方便外部文件调用时判断的	
}


void int1Init(void) __naked
{
	__asm
		setb IT1		// 低电平触发
		ret
	__endasm;
}


void int1_isr(void) __interrupt(2)	// int1 中断程序
{	  
   if(irTimeCounts>30) //9ms的话中断32.4次,30这个取值差不多就可以了,不用太精确
   {	//这里9ms引导码需要timer1中断irTimeCounts=9*11059.2/(256*12)=32.4次
    	bitNum =0;
		irTimeCounts=0;//为接收第0位数据做准备
		return;		   //丢弃引导码,反正不是有效数据0或1的都丢弃,直接返回  		
   }				 

   irTimeCountsArr[bitNum]=irTimeCounts; //将中断次数数据存储起来
   irTimeCounts=0;     //存好了就立即清零,这样不会影响下一位数据的接收
   bitNum++;	       //继续下一位
   if(32==bitNum)      //32位数据已经接收完成(0~31已经存储)
   {
   		bitNum=0;  	   //清零,这里不清也可以,反正引导时也会清
		irReceFlag=1; //接收完成标志
   }				  
	
}

void timer1(void) __interrupt(3)  //定时器0中断函数
{
	 irTimeCounts++;	   //注:该值最大为255
}

void timer1Init()	//定时器0初始化
{
	TMOD &= 0x0f;
	TMOD |= 0x20;
	TH1 = 0x00;
	TL1 = 0x00;
	ET1 = 1;
}

void infraredTask(void)
{
	if (irTimeCountsArrProcess())
	{
		//if (irCodeByteData[2] + irCodeByteData[3] == 0xff)		debug
		if (infraIson) {						// 若处于红外设置阶段
			switch (irCodeByteData[2]) {				// 按前进键，选中数码管个位，并闪烁
				case 0x07:
					segments7_AFF2_Bling();
					break;
				case 0x09:					// 按后退键，选中数码管十位，并闪烁
					segments7_AFF1_Bling();
					break;
				case 0x16:					// 更改被选中的数码管的数值
					segments7_Setnum(0);
					break;
				case 0x0c:
					segments7_Setnum(1);
					break;
				case 0x18:
					segments7_Setnum(2);
					break;
				case 0x5e:
					segments7_Setnum(3);
					break;
				case 0x08:
					segments7_Setnum(4);
					break;
				case 0x1c:
					segments7_Setnum(5);
					break;
				case 0x5a:
					segments7_Setnum(6);
					break;
				case 0x42:
					segments7_Setnum(7);
					break;
				case 0x52:
					segments7_Setnum(8);
					break;
				case 0x4a:
					segments7_Setnum(9);
					break;
				case 0x15:					// 开始倒计时，并打开主持人按钮，等待外部中断		
					segments7_Go();
					if (!segments7_Is_segSet()) {
						infraIson = 0;
						ledOn();
						SWITCH_ON();	//打开主持人开关
						INT0_ON();
					}
					break;
				default:
					break;
			}
		} else{
			switch (irCodeByteData[2]) {				// 若并未在红外设置定时器初值阶段
				case 0x15:						// 若定时器正在倒数，按播放键则复位
					if (segments7_Is_segSet()) {		 
						SWITCH_OFF();
						light_Off();
						infraIson = 1;
						segments7_On();
					} else{						// 若定时器已经显示抢答号，按播放键则复位
						INT0_OFF();
						SWITCH_OFF();
						infraIson = 1;
						segments7_Off();
						segments7_On();
					}


				default:
					break;
			}
		}
	}
}

