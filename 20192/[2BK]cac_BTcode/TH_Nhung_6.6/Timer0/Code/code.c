#include<reg51.h>

sbit Out = P2^0;		   

void InitTimer0(void);


int main(void)
{
   InitTimer0();           
 
   while(1)                
   {
   }
}

void InitTimer0(void)
{
	TMOD &= 0xF0;    
	TMOD |= 0x02;    
	
	TH0 = 0x05;      
	TL0 = 0x05;      
	
	ET0 = 1;         
	EA  = 1;         
	
	TR0 = 1;         
}


void Timer0_ISR (void) interrupt 1   
{
	Out = ~Out;  
	TF0 = 0;     
}

