#include<reg51.h>

void delay(int);
sbit btn = P1^0;

int main(void)
{
	char ch = 0x55;
   	P0 = 0x00;   
	EA=1;		
	EX0=1;

	while(1)           
	{
		btn = 1;
		if (btn == 0)
		{
			P0 = ch;
			ch = ~ch;
			delay(30000);			  
		}
	}
}


void EX0_ISR()   interrupt 0    		
{		
	P0 = 0;				
}


void delay(int a)
{
   int i;
   for(i=0;i<a;i++);   
}
