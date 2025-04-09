#include <Key.h>
uint8_t Key_Read() {
    uint8_t temp	 = 0;
    P44 = 0;P42=1;P35=1;P34=1;
    if (P33 == 0)       temp = 1;//S4
    else if (P32 == 0)  temp = 2;//S5
    else if (P31 == 0)  temp = 3;//S6
    else if (P30 == 0)  temp = 4;//S7
	    P44 = 1;P42=0;P35=1;P34=1;
    if (P33 == 0)       temp = 5;//S8
    else if (P32 == 0)  temp = 6;//S9
    else if (P31 == 0)  temp = 7;//S10
    else if (P30 == 0)  temp = 8;//S11
	    P44 = 1;P42=1;P35=0;P34=1;
    if (P33 == 0)       temp = 9;//S12
    else if (P32 == 0)  temp = 10;//S13
    else if (P31 == 0)  temp = 11;//S6
    else if (P30 == 0)  temp = 12;//S7
		 /*P44 = 1;P42=1;P35=1;P34=0;
    if (P33 == 0)       temp = 13;//S4
    else if (P32 == 0)  temp = 14;//S5
    else if (P31 == 0)  temp = 15;//S6
    else if (P30 == 0)  temp = 16;//S7*/
	
		return temp;
}