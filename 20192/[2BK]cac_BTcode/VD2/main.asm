#include <at89x51.h>
#include "string.h"


char uart_data;
char message[20];
int index=0;

//Dinh nghia mot so chan de dieu khien den LCD
#define LCD_DATA P0
sbit EN=P1^2;
sbit RS=P1^0;
sbit RW=P1^1;	   
//Khai bao prototype cho cac ham
void Init_System();	
void Delay_ms(int interval);  
void LCD_init();
void Wait_For_LCD();
void LCD_Send_Command(unsigned char x);
void LCD_Write_One_Char(unsigned char c);
void LCD_Write_String(unsigned char *s);

void write(char c){
	//LCD_Send_Command(0x01);
	//LCD_Write_String(s);
	if(c=='#'){
		LCD_Send_Command(0x01);
	}if(c=='$'){
		LCD_Send_Command(0xC0);
	}else{
		LCD_Write_One_Char(c);
	}
	//LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
	//LCD_Write_String("***************");
}

void init_uart(){
   	SCON = 0x50; 			/* uart in mode 1 (8 bit), REN=1 */
  	TMOD = TMOD | 0x20 ; 	/* Timer 1 in mode 2 */
  	TH1 = 0xFD; 			/* 9600 Bds at 11.0592MHz */
  	TL1 = 0xFD; 			/* 9600 Bds at 11.0592MHz */
  	ES = 1; 				/* Enable serial interrupt*/
  	EA = 1; 				/* Enable global interrupt */
  	TR1 = 1;
}

void serial_IT(void) interrupt 4
{
  if (RI == 1)
  { 
   	RI = 0; 			/* prepare for next reception */
    uart_data = SBUF; 	/* Read receive data */
    SBUF = uart_data; 	/* Send back same data on uart*/
	write(uart_data);
  }
  else 
    TI = 0; 			/* if emission occur */
}

void main()
{
	init_uart();
	Init_System();
	LCD_init();
	while(1){		
	}
}



void Init_System()
{
	//Thiet lap LCD o che do doc
	RW=1;	
}
void Delay_ms(int interval)
{
	int i,j;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<interval;j++);
	}
}
//Ham thuc hien gui mot lenh xuong LCD
void LCD_Send_Command(unsigned char x)
{
	LCD_DATA=x;
	RS=0; //Chon thanh ghi lenh
	RW=0; //De ghi du lieu
	EN=1;
	Delay_ms(1);
	EN=0;		
	Wait_For_LCD(); //Doi cho LCD san sang
	EN=1;		  
}
//Ham kiem tra va cho den khi LCD san sang
void Wait_For_LCD()
{
	Delay_ms(10);
}
void LCD_init()
{
	LCD_Send_Command(0x38); //Chon che do 8 bit, 2 hang cho LCD
	LCD_Send_Command(0x0E); //Bat hien thi, nhap nhay con tro	
	LCD_Send_Command(0x01); //Xoa man hinh	
	LCD_Send_Command(0x80); //Ve dau dong
	
}
//Ham de LCD hien thi mot ky tu
void LCD_Write_One_Char(unsigned char c)
{
	LCD_DATA=c; //Dua du lieu vao thanh ghi 
	RS=1; //Chon thanh ghi du lieu
	RW=0;
	EN=1;
	Delay_ms(1);	
	EN=0;
	Wait_For_LCD();	
	EN=1;
}
//Ham de LCD hien thi mot xau
void LCD_Write_String(unsigned char *s)
{
	unsigned char length;
	length=strlen(s); //Lay do dai xau
	while(length!=0)
	{
		LCD_Write_One_Char(*s); //Ghi ra LCD gia tri duoc tro boi con tro
		s++; //Tang con tro
		length--;	  		 
	}
}

