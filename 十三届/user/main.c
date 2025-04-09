/*	头文件声明区*/
#include <stc15f2k60s2.h>
#include <key.h>
#include <seg.h>
#include <su_common_typedef.h>
#include <onewire.h>
#include <ds1302.h>
#include <led.h>
/* 	变量声明区*/
uint16_t Seg_Mode;//0温度显示 1时间显示 2 参数控制
uint8_t Key_Slow_Down;
uint8_t Key_Old,Key_Val,Key_Down,Key_Up;
uint8_t Seg_Pos;
uint16_t Seg_Slow_Down;
uint8_t Seg_Buf[8] = {16,16,16,16,16,16,16,16};
uint8_t Seg_Point[8] = {0,0,0,0,0,0,0,0};
uint8_t ucRtc[3] ={23,25,0};
float Tp;
uint8_t Tp_Canshu = 23;
bit Work_Mode = 0;//0温度控制 1时间控制
bit Time_Disp = 0;
uint16_t Time_5s;
bit a = 0;


/* 按键处理*/
void Key_Process(){
		if(Key_Slow_Down) return;
	Key_Slow_Down =1;
	Key_Val = Key_Read();
	Key_Down = Key_Val & (Key_Val ^ Key_Old);
	Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
	Key_Old = Key_Val;
	switch(Key_Down){
		case 9://S12
		Seg_Mode++;
		if(Seg_Mode == 3) Seg_Mode = 0;		
		break;
		
		case 10: //S13
		Work_Mode =~Work_Mode;
		
		break;
		
		case 13://S16 加
		
		if(Seg_Mode == 2&&(Tp_Canshu>=10&&Tp_Canshu<=98))
		++Tp_Canshu;
		break;
		case 14://S17 减
		if(Seg_Mode == 2&&(Tp_Canshu>=11&&Tp_Canshu<=99)){
		--Tp_Canshu;
		return;}
		if(Seg_Mode == 1)
		Time_Disp = 1;//分秒
		break;
	}
	if(Key_Up == 14) Time_Disp = 0;
}

/*信息处理*/
void Seg_Process(){
	Read_Rtc(ucRtc);
if(Seg_Slow_Down) return;
	switch(Seg_Mode){
		case 0://温度显示
			Tp = rd_temperature();
		Seg_Point[6] = 1;
		Seg_Buf[0] = 18;//U
		Seg_Buf[1] = 1;//1
		Seg_Buf[2] = 16;
		Seg_Buf[3] = 16;
		Seg_Buf[4] = 16;
		Seg_Buf[5] = (uint8_t)Tp/10%10;
		Seg_Buf[6] = (uint8_t)Tp%10;
		Seg_Buf[7] = (uint16_t)(Tp*10)%10;
		
		break;
		
		case 1://时间显示
		Seg_Point[6] = 0;
		Seg_Buf[0] = 18;//U
		Seg_Buf[1] = 2;//2
		Seg_Buf[2] = 16;
		if(Time_Disp == 0){//不按下时 时分
		Seg_Buf[3] = ucRtc[0]/10%10;//hour
		Seg_Buf[4] = ucRtc[0]%10;
		Seg_Buf[5] = 17;
		Seg_Buf[6] = ucRtc[1]/10%10;
		Seg_Buf[7] = ucRtc[1]%10;
		}
		else{//分秒
		Seg_Buf[3] = ucRtc[1]/10%10;//min
		Seg_Buf[4] = ucRtc[1]%10;
		Seg_Buf[5] = 17;
		Seg_Buf[6] = ucRtc[2]/10%10;
		Seg_Buf[7] = ucRtc[2]%10;
		}
		break;
		
		
		case 2://参数设置
		Seg_Point[6] = 0;
		Seg_Buf[0] = 18;
		Seg_Buf[1] = 3;	
		Seg_Buf[2] = 16;
		Seg_Buf[3] = 16;
		Seg_Buf[4] = 16;
		Seg_Buf[5] = 16;
		Seg_Buf[6] = Tp_Canshu/10%10;
		Seg_Buf[7] = Tp_Canshu%10;		
		break;
		
		
	}
}
/* 其他显示函数*/
void Led_Process(){
	if((ucRtc[1] ==0)&& (ucRtc[2] == 0 )){//整点
	Led_Disp(0,1);//LED1
	if(Time_5s==0) 	Led_Disp(0,0);
	}
	if(Work_Mode ==0) 	Led_Disp(1,1);
	else Led_Disp(1,0);
}
/*继电器处理函数*/
void Relay_Process(){
	if(Work_Mode){//温度控制
	if(Tp>=Tp_Canshu) Relay(1);
		else Relay(0);
	
	}
	else{//时间控制
	if((ucRtc[1] ==0)&& (ucRtc[2] == 0 )){
	Relay(1);
	a= 1;
	if(Time_5s == 0) 	Relay(0);
	}
	}
}
/*定时器0初始化*/
void Init_Time0(){
		TMOD &= 0xF0;		//设置定时器模式
	TL0 = 64536%256;		//设置定时初值
	TH0 = 64536/256;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;
	ET0 = 1;
}
/*关闭BEEP*/
void Init(){
	P0 &=  0xbf;
	P2 = P2 &0x1f | 0xb0;
	P2 &=  0x1f;
	
	P0 |= 0xff;
	P2 = P2 &0x1f | 0x80;
	P2 &=  0x1f;
	


}
/*定时器0服务函数*/
void Time0_Serve() interrupt 1{
	TL0 = 64536%256;		//设置定时初值
	TH0 = 64536/256;		//设置定时初值
	if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
	if(++Seg_Pos == 8) Seg_Pos = 0;
	if(++Seg_Slow_Down == 200) Seg_Slow_Down = 0;
	if(a ==1){
		if(++Time_5s == 5000){
				Time_5s = 0;
				a = 0;
		}
	}
	Seg_Disp(Seg_Pos,Seg_Buf[Seg_Pos],Seg_Point[Seg_Pos]);
	
	
}
/*主函数*/
void main(){
	Init();
	Init_Time0();
	Set_Rtc(ucRtc);
		while(1){
		Key_Process();
		Seg_Process();
		Led_Process();
		Relay_Process();
		}
}