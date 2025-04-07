#include <STC15F2K60S2.H>
#include"ds1302.h"
#include"iic.h"

sbit row3 = P3^2;
sbit row4 = P3^3;
sbit col1 = P4^4;
sbit col2 = P4^2;
//ds1302相关参数
code unsigned char Seg_Table[] =
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xbf,0x8c,0x89,0xc7};  //0-15. 16 -. 17 P.18 H.19 L.
code unsigned char timewrite_addr[]={0x80,0x82,0x84,0x8a,0x86,0x88,0x8c};
code unsigned char timeread_addr[]={0x81,0x83,0x85,0x8b,0x87,0x89,0x8d};
unsigned char time[] = {0x05,0x03,0x13,0x06,0x13,0x04,0x24};
unsigned char h = 0;
unsigned char min = 0;
unsigned char sec = 0;
//界面转换相关参数
unsigned char facenumber = 0;
unsigned char facenumber1 = 1;
unsigned char facenumber3 = 1;
//计时相关参数
unsigned char count555 = 0;
unsigned char state_led = 0xff;
unsigned char countl1 = 0;
unsigned char countl2 = 0;
//频率相关参数
int freq = 0;
int freqout = 2350;
int freqpar = 2000;
int freqstandpar = 0;
int maxfreq = 0;
int tempfreq = 0;
float vinfloat = 0;
unsigned char vin = 0;
//标志位参数
bit flag_freqout = 0;
//
//138选择以及单个数码管显示
void set_138(unsigned char channel, unsigned char dat)
{
	P2 = (P2&0x1f) | 0x00;
	P0 = dat;
	switch(channel)
	{
		case 0:P2 = (P2&0x1f) | 0x00;break;
		case 4:P2 = (P2&0x1f) | 0x80;break;
		case 5:P2 = (P2&0x1f) | 0xa0;break;
		case 6:P2 = (P2&0x1f) | 0xc0;break;
		case 7:P2 = (P2&0x1f) | 0xe0;break;
	}
	P2 = (P2&0x1f) | 0x00;
}

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}


void Nixie(unsigned char location, unsigned char number)
{
	set_138(6,0x01<<(location-1));
	set_138(7,number);
	Delay1ms();
	set_138(6,0x01<<(location-1));
	set_138(7,0xff);
}
//
//数码管显示界面
void Nixieshowing()
{
	switch(facenumber)
	{
    case 0:
		{ if(flag_freqout==1)
			{
				Nixie(1,Seg_Table[15]);
				Nixie(7,Seg_Table[19]);
				Nixie(8,Seg_Table[19]);
			}
			else 
			{
				if(freqout>9999)
				{
					Nixie(1,Seg_Table[15]);
					Nixie(4,Seg_Table[freqout/10000]);
					Nixie(5,Seg_Table[(freqout/1000)%10]);
					Nixie(6,Seg_Table[(freqout/100)%10]);
					Nixie(7,Seg_Table[(freqout/10)%10]);
					Nixie(8,Seg_Table[freqout%10]);		
				}
				else if(freqout>999)
				{
					Nixie(1,Seg_Table[15]);
					Nixie(5,Seg_Table[freqout/1000]);
					Nixie(6,Seg_Table[(freqout/100)%10]);
					Nixie(7,Seg_Table[(freqout/10)%10]);
					Nixie(8,Seg_Table[freqout%10]);
				}
				else if(freqout>99)
				{
					Nixie(1,Seg_Table[15]);
					Nixie(6,Seg_Table[freqout/100]);
					Nixie(7,Seg_Table[(freqout/10)%10]);
					Nixie(8,Seg_Table[freqout%10]);
				}
				else if(freqout>9)
				{
					Nixie(1,Seg_Table[15]);
					Nixie(7,Seg_Table[freqout/10]);
					Nixie(8,Seg_Table[freqout%10]);
				}
				else
				{
					Nixie(1,Seg_Table[15]);
					Nixie(8,Seg_Table[freqout]);
				}
			}
		}
    break;
		case 1:
		{
			switch(facenumber1)
			{
				case 1:
					Nixie(1,Seg_Table[17]);
				  Nixie(2,Seg_Table[1]);
					Nixie(5,Seg_Table[freqpar/1000]);
					Nixie(6,Seg_Table[(freqpar/100)%10]);
				  Nixie(7,Seg_Table[(freqpar/10)%10]);
					Nixie(8,Seg_Table[freqpar%10]);
				break;
				case 2:
					if(freqstandpar>0)
					{
						Nixie(1,Seg_Table[17]);
				    Nixie(2,Seg_Table[2]);
						Nixie(6,Seg_Table[freqstandpar/100]);
				    Nixie(7,Seg_Table[(freqstandpar/10)%10]);
					  Nixie(8,Seg_Table[freqstandpar%10]);
					}
					else if(freqstandpar==0)
					{
						Nixie(1,Seg_Table[17]);
				    Nixie(2,Seg_Table[2]);
						Nixie(8,Seg_Table[0]);
					}
					else
					{
						Nixie(1,Seg_Table[17]);
				    Nixie(2,Seg_Table[2]);
						Nixie(5,Seg_Table[16]);
						Nixie(6,Seg_Table[(-freqstandpar)/100]);
				    Nixie(7,Seg_Table[((-freqstandpar)/10)%10]);
					  Nixie(8,Seg_Table[(-freqstandpar)%10]);
					}
				break;
			}
		}
		break;
		case 2:
			Nixie(1,Seg_Table[time[2]/16]);
		  Nixie(2,Seg_Table[time[2]%16]);
		  Nixie(3,Seg_Table[16]);
		  Nixie(4,Seg_Table[time[1]/16]);
		  Nixie(5,Seg_Table[time[1]%16]);
		  Nixie(6,Seg_Table[16]);
		  Nixie(7,Seg_Table[time[0]/16]);
		  Nixie(8,Seg_Table[time[0]%16]);
		break;
		case 3:
		{
		 switch(facenumber3)
		{
			case 1:
			{
					if(maxfreq>9999)
				{
					Nixie(1,Seg_Table[18]);
					Nixie(2,Seg_Table[15]);
					Nixie(4,Seg_Table[maxfreq/10000]);
					Nixie(5,Seg_Table[(maxfreq/1000)%10]);
					Nixie(6,Seg_Table[(maxfreq/100)%10]);
					Nixie(7,Seg_Table[(maxfreq/10)%10]);
					Nixie(8,Seg_Table[maxfreq%10]);		
				}
				else if(maxfreq>999)
				{
					Nixie(1,Seg_Table[18]);
					Nixie(2,Seg_Table[15]);
					Nixie(5,Seg_Table[maxfreq/1000]);
					Nixie(6,Seg_Table[(maxfreq/100)%10]);
					Nixie(7,Seg_Table[(maxfreq/10)%10]);
					Nixie(8,Seg_Table[maxfreq%10]);
				}
				else if(maxfreq>99)
				{
					Nixie(1,Seg_Table[18]);
					Nixie(2,Seg_Table[15]);
					Nixie(6,Seg_Table[maxfreq/100]);
					Nixie(7,Seg_Table[(maxfreq/10)%10]);
					Nixie(8,Seg_Table[maxfreq%10]);
				}
				else if(maxfreq>9)
				{
					Nixie(1,Seg_Table[18]);
					Nixie(2,Seg_Table[15]);
					Nixie(7,Seg_Table[maxfreq/10]);
					Nixie(8,Seg_Table[maxfreq%10]);
				}
				else
				{
					Nixie(1,Seg_Table[18]);
					Nixie(2,Seg_Table[15]);
					Nixie(8,Seg_Table[maxfreq]);
				}
			}
			break;
			case 2:
			{
					Nixie(1,Seg_Table[18]);
					Nixie(2,Seg_Table[10]);
					Nixie(3,Seg_Table[h/16]);
					Nixie(4,Seg_Table[h%16]);
					Nixie(5,Seg_Table[min/16]);
					Nixie(6,Seg_Table[min%16]);
					Nixie(7,Seg_Table[sec/16]);
					Nixie(8,Seg_Table[sec%16]);
			}
			break;
		 }
		}
		break;
	}
}
//
//keyfunction
void s4function()
{
	if(facenumber==3)
	{
		facenumber = 0;
	}
	else
	{
		facenumber++;
	}
	if(facenumber==1)
	{
		facenumber1 = 1;
	}
	if(facenumber==3)
	{
		facenumber3 = 1;
	}
}

void s5function()
{
	if(facenumber==1)
	{
		if(facenumber1==2)
		{
			facenumber1 = 1;
		}
		else
		{
			facenumber1++;
		}
	}
	if(facenumber==3)
	{
		if(facenumber3==2)
		{
			facenumber3 = 1;
		}
		else
		{
			facenumber3++;
		}
	}
}

void s8function()
{
	if(facenumber==1)
	{
	  if(facenumber1==1)
		{
			if(freqpar==9000)
			{freqpar = 1000;}
			else
			{freqpar += 1000;}
		}
	  if(facenumber1==2)
		{
			if(freqstandpar==900)
			{freqstandpar=-900;}
			else
			{freqstandpar += 100;}
		}
	}
}

void s9function()
{
		if(facenumber==1)
	{
	  if(facenumber1==1)
		{
			if(freqpar==1000)
			{freqpar = 9000;}
			else
			{freqpar -= 1000;}
		}
	  if(facenumber1==2)
		{
			if(freqstandpar==-900)
			{freqstandpar = 900;}
			else
			{freqstandpar -= 100;}
		}
	}
}
//
//keyscan
void delay(unsigned int t)
{
	while(t--);
}

void keyscan()
{
	row3=0;
	row4=col1=col2=1;
	if(col1==0)              //s5
	{
		delay(20);
		if(col1==0)
		{
			while(col1==0)
			{Nixieshowing();}
			s5function();
		}
	}
	if(col2==0)      //s9
	{
		delay(20);
		if(col2==0)
		{
			while(col2==0)
			{Nixieshowing();}
			s9function();
		}
	}
	row4=0;
	row3=col1=col2=1;
	if(col1==0)         //s4
	{
		delay(20);
		if(col1==0)
		{
			while(col1==0)
			{Nixieshowing();}
			s4function();
		}
	}
	if(col2==0)        //s8
	{
		delay(20);
		if(col2==0)
		{
			while(col2==0)
			{Nixieshowing();}
			s8function();
		}
	}
}
//ds1302
void ds1302_write()
{
  unsigned char i=7;
	Write_Ds1302_Byte(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Write_Ds1302_Byte(timewrite_addr[i],time[i]);
		Nixieshowing();
	}
	Write_Ds1302_Byte(0x8e,0x80);
}

void ds1302_read()
{
	unsigned char i=7;
	for(i=0;i<7;i++)
	{
		time[i] = Read_Ds1302_Byte(timeread_addr[i]);
	}
}
//
//NE555函数以及定时器
void InitTimer()
{
	TMOD = 0x15;
	AUXR = 0x00;
	TH1 =(65536-50000)/256;
	TL1 =(65536-50000)%256;
	
	TH0 = 0;
	TL0 = 0;
	
	EA = 1;
	
	ET1 = 1;
  TR1 = 1;
	
	ET0 = 0;
	TR0 = 1;
}

void SerivceTimer0() interrupt 3
{
	TH1 =(65536-50000)/256;
	TL1 =(65536-50000)%256;
	
	if(facenumber==0)
	{
		countl1++;
		if(countl1==4)
		{
			countl1 = 0;
			if((state_led&0x01)==0x01)      
			{
				state_led = (state_led&0xfe) | 0x00;         
				set_138(4,state_led);
			}
			else                        
			{
				state_led = (state_led&0xfe) | 0x01;
				set_138(4,state_led);
			}
		}
	}
	else                        
	{
		state_led = (state_led&0xfe) | 0x01;
		set_138(4,state_led);
	}
	if(freqout>freqpar)
	{
		countl2++;
		if(countl2==4)
		{
			countl2 = 0;
			if((state_led&0x02)==0x02)      
			{
				state_led = (state_led&0xfd) | 0x00;         
				set_138(4,state_led);
			}
			else                        
			{
				state_led = (state_led&0xfd) | 0x02;
				set_138(4,state_led);
			}
		}
	}
	else                        
	{
		state_led = (state_led&0xfd) | 0x02;
		set_138(4,state_led);
	}
	if(flag_freqout==1)
	{
		state_led = (state_led&0xfd) | 0x00;         
		set_138(4,state_led);
	}
	else                        
	{
		state_led = (state_led&0xfd) | 0x02;
		set_138(4,state_led);
	}
	count555++;
	if(count555==20)
	{
		count555 = 0;
		freq = (TH0<<8) | TL0;
		TH0 = 0;
		TL0 = 0;
		tempfreq = freqout;
		freqout = freq + freqstandpar;
		if(freqout<0)
		{
			flag_freqout = 1;
		}
		else
		{
			flag_freqout = 0;
		}
		if(flag_freqout==0)
		{
			if(freqout>maxfreq)
			{
				maxfreq = freqout;
				h = Read_Ds1302_Byte(timeread_addr[2]);
				min = Read_Ds1302_Byte(timeread_addr[1]);
				sec = Read_Ds1302_Byte(timeread_addr[0]);
			}
			else
			{
				maxfreq = maxfreq;
			}
		}
	}
}
//
//DAC mode
void pcf8591Init()
{
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x40);
	I2CWaitAck();
}

void freqtask()
{
	if(freqout>freqpar)
	{
		vin = 255;
	}
	else if((freqout>500)&&(freqout<freqpar))
	{
		vinfloat = (4*freqout-2000);
		vin = (vinfloat/(freqpar-500)+1)*256/5;
	}
	else
	{
		vin = 52;
	}
	if(flag_freqout==1)
	{
		vin = 0;
	}
}

void pcf8591()
{
	freqtask();
	I2CStart();
	I2CSendByte(0x90);
	I2CWaitAck();
	I2CSendByte(0x40);
	I2CWaitAck();
	I2CSendByte(vin);
	I2CWaitAck();
	I2CStop();
}
//
//系统初始化
void Initsys()
{
	set_138(6,0x00);
	set_138(7,0xff);
	set_138(4,0xff);
	set_138(5,0x00);
	
	pcf8591Init();
	InitTimer();
	ds1302_write();
}
//
//主函数
void main()
{
	Initsys();
	while(1)
	{
		pcf8591();
		ds1302_read();
		keyscan();
		Nixieshowing();
	}
}
