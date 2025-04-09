// 主程序系统
/*  头文件包含 */
#include <stc15f2k60s2.h>
#include <Key.h>
#include <Seg.h>
#include <su_common_typedef.h>
#include <ds1302.h>
#include <iic.h>
#include <Led.h>
/*  全局变量定义 */
uint8_t Seg_Mode; // 0-频率 1-参数 2-时间显示 3-回显模式
uint8_t Key_Slow_Down;
uint8_t Key_Old,Key_Val,Key_Down,Key_Up;
uint8_t Seg_Pos;
uint16_t Seg_Slow_Down;
uint8_t Seg_Buf[8] = {16,16,16,16,16,16,16,16};
uint8_t Frequence[5] = {16,2,3,5,0};
int16_t ChaoXian[4] = {2,0,0,0};
int16_t ChaoXian_Data = 2000;
int16_t JiaoZhun_Data;
int16_t JiaoZhun[4] = {16,16,16,0};
uint8_t ucRtc[3] = {13,03,05};
int16_t Time_500ms;
int16_t	Count_Frequence;
int16_t Total_Frequence;
uint8_t time00 = 65536-1000;
bit CanShu_Mode,HuiXian_Mode;
int16_t Max_Frequence[5] = {16,2,3,5,0};
uint8_t Max_Time[3] ={13,03,05};
int16_t Max_Count;
uint8_t Time_200ms;
uint8_t Led_Enable[8] ={0,0,0,0,0,0,0,0};
uint16_t Led_Time[3] ={0,0,0};
bit a,b,c;
//S5按键处理
void S5_Function(){
if(Seg_Mode ==1)//0是超限 1是校准
CanShu_Mode = ~CanShu_Mode;
if(Seg_Mode == 3)//0是频率 1是时间
HuiXian_Mode = ~HuiXian_Mode;

}

//S8按键处理
void S8_Function(){
	int16_t temp = 0;
if(Seg_Mode ==1){
	if(CanShu_Mode ==0)//超限
		ChaoXian_Data = (ChaoXian_Data>=1000&&ChaoXian_Data<=8000)?(ChaoXian_Data+1000):ChaoXian_Data;
	else//校准
		JiaoZhun_Data = (JiaoZhun_Data>=(-900)&&JiaoZhun_Data<=800)?(JiaoZhun_Data+100):JiaoZhun_Data;
	
	}//if
	
		//更新显示
		if(JiaoZhun_Data<0){
	JiaoZhun[0] = 19;
		temp = -JiaoZhun_Data;
	JiaoZhun[1] = temp/100%10;
	JiaoZhun[2] = temp/10%10;
	JiaoZhun[3] = temp%10;

		}//符号位 - 
	if(JiaoZhun_Data>0){ 
		JiaoZhun[0] = 16; //灭
	JiaoZhun[1] = JiaoZhun_Data/100%10;
	JiaoZhun[2] = JiaoZhun_Data/10%10;
	JiaoZhun[3] = JiaoZhun_Data%10;}
		
	if(JiaoZhun_Data == 0){
			JiaoZhun[0] = 16; //灭
	JiaoZhun[1] = 16;
	JiaoZhun[2] = 16;
	JiaoZhun[3] = 0;
	
	}
	
		ChaoXian[0] = ChaoXian_Data/1000%10;
		ChaoXian[1] = ChaoXian_Data/100%10;
		ChaoXian[2] = ChaoXian_Data/10%10;
		ChaoXian[3] = ChaoXian_Data%10;

}
	

//S9按键处理
void S9_Function(){
	int16_t temp = 0;
if(Seg_Mode ==1){
	if(CanShu_Mode ==0)//超限
		ChaoXian_Data = (ChaoXian_Data>=2000&&ChaoXian_Data<=9000)?(ChaoXian_Data-1000):ChaoXian_Data;
	else//校准
		JiaoZhun_Data = (JiaoZhun_Data>=(-800)&&JiaoZhun_Data<=900)?(JiaoZhun_Data-100):JiaoZhun_Data;
	
	}//if
		//更新显示
		if(JiaoZhun_Data<0){
	JiaoZhun[0] = 19;
		temp = -JiaoZhun_Data;
	JiaoZhun[1] = temp/100%10;
	JiaoZhun[2] = temp/10%10;
	JiaoZhun[3] = temp%10;
		}//符号位 - 
	if(JiaoZhun_Data>0){ 
		JiaoZhun[0] = 16; //灭
	JiaoZhun[1] = JiaoZhun_Data/100%10;
	JiaoZhun[2] = JiaoZhun_Data/10%10;
	JiaoZhun[3] = JiaoZhun_Data%10;
	}
	if(JiaoZhun_Data == 0){
			JiaoZhun[0] = 16; //灭
	JiaoZhun[1] = 16;
	JiaoZhun[2] = 16;
	JiaoZhun[3] = 0;
	
	}
	
		ChaoXian[0] = ChaoXian_Data/1000%10;
		ChaoXian[1] = ChaoXian_Data/100%10;
		ChaoXian[2] = ChaoXian_Data/10%10;
		ChaoXian[3] = ChaoXian_Data%10;

}
/* 键盘处理函数 */
void Key_Process(){
    if(Key_Slow_Down) return;
    Key_Slow_Down = 1;
    Key_Val = Key_Read();
    Key_Up=~Key_Val & (Key_Old^ Key_Val); // 检测键释放
    Key_Down = Key_Val & (Key_Val ^ Key_Old);
    Key_Old = Key_Val;
    switch(Key_Down){
        case 1://S4
				if(Seg_Mode == 0)  CanShu_Mode = 0;
				if(Seg_Mode == 2)  HuiXian_Mode = 0;
				if(++Seg_Mode == 4) Seg_Mode = 0;
            break;
        case 2://S5
				S5_Function();
				break;
        case 5://S8 增加键
				S8_Function();
            break;

        case 6://S9 减少键
				S9_Function();
            break;
    }
}
/*参数显示*/
void 	CanShu_Process(){
	if(CanShu_Mode == 0){//超限
	          Seg_Buf[0] = 17 ;//P
            Seg_Buf[1] = 1 ;//1
            Seg_Buf[2] = 16 ;
            Seg_Buf[3] = 16 ;
            Seg_Buf[4] = ChaoXian[0] ;
            Seg_Buf[5] = ChaoXian[1] ;
            Seg_Buf[6] = ChaoXian[2] ;
            Seg_Buf[7] = ChaoXian[3] ;
		return;
	}
	else{
		        Seg_Buf[0] = 17 ;//P
            Seg_Buf[1] = 2 ;//1
            Seg_Buf[2] = 16 ;
            Seg_Buf[3] = 16 ;
            Seg_Buf[4] = JiaoZhun[0] ;
            Seg_Buf[5] = JiaoZhun[1] ;
            Seg_Buf[6] = JiaoZhun[2] ;
            Seg_Buf[7] = JiaoZhun[3] ;
	}
}
void HuiXian_Process(){
	if(HuiXian_Mode == 0){//频率
	          Seg_Buf[0] = 18 ;//H
            Seg_Buf[1] = 15 ;//F
            Seg_Buf[2] = 16 ;
            Seg_Buf[3] = Max_Frequence[0] ;
            Seg_Buf[4] = Max_Frequence[1] ;
            Seg_Buf[5] = Max_Frequence[2] ;
            Seg_Buf[6] = Max_Frequence[3] ;
            Seg_Buf[7] = Max_Frequence[4] ;
		return;
	}
	else{ //时间
		        Seg_Buf[0] = 18 ;//H
            Seg_Buf[1] = 10;//A
            Seg_Buf[2] = Max_Time[0]/10%10;
            Seg_Buf[3] = Max_Time[0]%10;
            Seg_Buf[4] = Max_Time[1]/10%10 ;
            Seg_Buf[5] = Max_Time[1]%10 ;
            Seg_Buf[6] = Max_Time[2]/10%10 ;
						Seg_Buf[7] = Max_Time[2]%10;
	}

}
/* 段码处理函数 */
void Seg_Process(){
		Read_Rtc(ucRtc);
    if(Seg_Slow_Down) return;
    switch(Seg_Mode){
        case 0://频率显示
            Seg_Buf[0] = 15 ;//F
            Seg_Buf[1] = 16 ;
            Seg_Buf[2] = 16 ;
            Seg_Buf[3] = Frequence[0] ;
            Seg_Buf[4] = Frequence[1] ;
            Seg_Buf[5] = Frequence[2] ;
            Seg_Buf[6] = Frequence[3] ;
            Seg_Buf[7] = Frequence[4] ;
            break;

        case 1://参数显示
				CanShu_Process();
            break;

        case 2://时间显示
            Seg_Buf[0] = ucRtc[0]/10%10 ;
            Seg_Buf[1] = ucRtc[0]%10 ;
            Seg_Buf[2] = 19 ;//-
            Seg_Buf[3] = ucRtc[1]/10%10 ;
            Seg_Buf[4] = ucRtc[1]%10 ;
            Seg_Buf[5] = 19 ;//-
            Seg_Buf[6] = ucRtc[2]/10%10 ;
            Seg_Buf[7] = ucRtc[2]%10 ;
            break;

        case 3://回显模式
				HuiXian_Process();
            break;
    }
}
/* LED处理函数 */
void Led_Process(){
	if(Seg_Mode == 0){
		a=1;//L1闪烁
	}else{
		a =0;
			Led_Enable[0] = 0;
		Led_Disp(0,Led_Enable[0]);
	}
	
	if(Total_Frequence>ChaoXian_Data){//L2闪烁
		b=1;
	}
	else if((Total_Frequence<=ChaoXian_Data)&&Total_Frequence>=0){ 
		b = 0;
	Led_Enable[1] = 0;
		Led_Disp(1,Led_Enable[1]);
	}
	
	
		if(Total_Frequence < 0){ 
			c = 1;
		}else c = 0;
}

void Init_Beep_Led(){
    P2 = (P2 &0x1f) | 0xb0;
    P0 = 0x00;
    P2 = 0x1f;
    P0 = 0xff;
		P2 = (P2 &0x1f) | 0x80;
	  P0 =0xff;
		P2 = 0x1f;
}

/* 定时器0初始化 */
void Init_Time(){
	TMOD = 0x06;		//设置定时器模式
		TL1 = 63536%256;		//设置定时初值
		TH1 = 63536/256;		//设置定时初值
	TF1 = 0;	TF0 = 1;	//清除TF0标志
	TR1 = 1;	TR0 = 1;	//定时器0开始计时
	EA = 1;
	ET0 = 1; ET1 = 1;
}



void Update_Frequence(){
	uint8_t  i,j = 0; 
	long double temp,FZ,FM;
	Total_Frequence = 2*Count_Frequence + JiaoZhun_Data;
	if(Total_Frequence >= 0){
	Frequence[0] = (Total_Frequence/10000%10 == 0)?16:(Total_Frequence/10000%10);
	if(Frequence[0] == 16)
	Frequence[1] = (Total_Frequence/1000%10 == 0)?16:(Total_Frequence/1000%10);
	else Frequence[1] =(Total_Frequence/1000%10);
	if(Frequence[1] == 16)
	Frequence[2] = (Total_Frequence/100%10 == 0)?16:(Total_Frequence/100%10);
	else Frequence[2] = (Total_Frequence/100%10);
	if(Frequence[2] == 16)
	Frequence[3] = (Total_Frequence/10%10 == 0)?16:(Total_Frequence/10%10);
	else Frequence[3] = (Total_Frequence/10%10);
	Frequence[4] = Total_Frequence%10;}
	else{//负数情况
		Frequence[0] = 16;
		Frequence[1] = 16;
		Frequence[2] = 16;
	Frequence[3] = 20;
	Frequence[4] = 20;
	}
	//Da输出部分
	if(Total_Frequence < 0 )  Da_Write(0);
	if(Total_Frequence >=0&&Total_Frequence<= 500) Da_Write(51);
	if(Total_Frequence > 500 && Total_Frequence<= ChaoXian_Data ){
	FZ=Total_Frequence-500;
		FM = ChaoXian_Data-500;
		temp = 51+(204*(FZ/FM));
		Da_Write(temp);
	}
	if(Total_Frequence> ChaoXian_Data) Da_Write(255);
	if(Max_Count < Total_Frequence){
		Max_Count = Total_Frequence;
		for(j = 0;j<3;j++) Max_Time[j] = ucRtc[j];
		for(i=0;i<5;i++)Max_Frequence[i] = Frequence[i];
	}
	
	Count_Frequence = 0;
}

/* 定时器0中断服务 */
void Time0_Serve() interrupt 1{
    TH0 = 0xff;
    TL0 = 0xff;
		Count_Frequence++;
}

/* 定时器1中断服务 */
void Time1_Serve() interrupt 3{
		TL1 = 63536%256;		//设置定时初值
		TH1 = 63536/256;		//设置定时初值
		Led_Process();
		P2 = 0xff;
    if(++Key_Slow_Down == 10) Key_Slow_Down = 0;
    if(++Seg_Pos == 8) Seg_Pos = 0;
    if(Seg_Slow_Down == 100)		Seg_Slow_Down = 0;
		if(a){
		if(++Led_Time[0] == 100){  
		Led_Time[0] = 0;
			Led_Enable[0] =~Led_Enable[0];
			Led_Disp(0,Led_Enable[0]);
		}
		}
		if(b){
				if(++Led_Time[1] == 100){  
		Led_Time[1] = 0;
			Led_Enable[1] =~Led_Enable[1];
			Led_Disp(1,Led_Enable[1]);
		}
		}
		if(c){
						Led_Enable[1] =1;
			Led_Disp(1,Led_Enable[1]);
		}
		if(++Time_500ms == 250){
			Time_500ms = 0;
			Update_Frequence();
		}
		
		Led_Process();
    Seg_Display(Seg_Pos,Seg_Buf[Seg_Pos]);
		
}
/* 主函数 */
void main(){
    Init_Time();
    Init_Beep_Led();
    Set_Rtc(ucRtc);
    while(1){
				Key_Process();
				Seg_Process();
				Led_Process();
    }
}