#include <seg.h>
uint8_t Seg_Wela[8] ={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uint8_t Seg_Dula[19]={0xc0  ,0xf9  ,0xa4  ,0xb0  ,0x99  ,0x92,  
											0x82  ,0xf8  ,0x80  ,0x90  ,0x88  ,0x83,  
											0xc6  ,0xa1  ,0x86  ,0x8e  ,0xff  ,0xbf,0xc1  
};//ʮ���� ʮ��- ʮ��U
void Seg_Disp(unsigned char Wela,unsigned char Dula,unsigned char point){
	P0 = 0xff;
	P2 = P2 & 0x1f | 0xe0;
	P2 &= 0x1f;

	P0 = Seg_Wela[Wela];
	P2 = P2 & 0x1f | 0xc0;
	P2 &= 0x1f;
	
	P0 = Seg_Dula[Dula];
	if(point)
		P0 &= 0x7f;
	P2 = P2 & 0x1f | 0xe0;
	P2 &= 0x1f;	
}