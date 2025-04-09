#include <led.h>

uint8_t temp = 0x00;
uint8_t temp_old = 0xff;
void Led_Disp(uint8_t addr,bit enable){
	if(enable){
	temp |= 0x01 <<addr;
	
	}
	else{
	temp &= ~(0x01 <<addr);
	
	}
	if(temp!= temp_old){
	P0 = ~temp;
	P2 = (P2&0x1f) |0x80;
	P2 &= 0x1f;
	temp_old = temp;
	
	}

}
void Relay(bit enable){
		if(enable){
	temp |= 0x10;
	
	}
	else{
	temp &= ~(0x10);
	
	}
	if(temp!= temp_old){
	P0 = temp;
	P2 = (P2&0x1f) |0xb0;
	P2 &= 0x1f;
	temp_old = temp;
	
	}



}