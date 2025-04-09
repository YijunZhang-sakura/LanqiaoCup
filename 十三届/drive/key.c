#include <key.h>
unsigned char Key_Read(){
	unsigned char temp =0 ;
/*P44 = 0; P42 = 1; P35 = 1; P34 = 1;
if(P33 == 0) temp = 1;//S4
else if(P32 == 0) temp = 2;
else if(P31 == 0) temp = 3;
else if(P30 == 0) temp = 4;

	
	P44 = 1; P42 = 0; P35 = 1; P34 = 1;
if(P33 == 0) temp = 5;//S8
else if(P32 == 0) temp = 6;
else if(P31 == 0) temp = 7;
else if(P30 == 0) temp = 8;*/
	
		P44 = 1; P42 = 1; P35 = 0; P34 = 1;
if(P33 == 0) temp = 9;//S12
else if(P32 == 0) temp = 10;
else if(P31 == 0) temp = 11;
else if(P30 == 0) temp = 12;
	
		
		P44 = 1; P42 = 1; P35 = 1; P34 = 0;
if(P33 == 0) temp = 13;//S16
else if(P32 == 0) temp = 14;
else if(P31 == 0) temp = 15;
else if(P30 == 0) temp = 16;
	
return temp;
}