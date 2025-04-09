#ifndef _ONEWIRE_H
#define _ONEWIRE_H
#include <stc15f2k60s2.h>
#include <intrins.h>
sbit DQ = P1^4;
void Delay_OneWire(unsigned int t);  
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);
float rd_temperature();
#endif