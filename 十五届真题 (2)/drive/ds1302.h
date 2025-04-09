#ifndef __Ds1302_H
#define __Ds1302_H
#include <stc15f2k60s2.h>
#include <su_common_typedef.h>
#include <intrins.h>
sbit SCK = P1^7;
sbit SDA = P2^3;
sbit RST = P1^3;
void Write_Ds1302(unsigned  char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte ( unsigned char address );
void Read_Rtc(unsigned char* ucRtc);
void Set_Rtc(unsigned char* ucRtc);
#endif