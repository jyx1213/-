#include <REGX52.H>
#include "Delay.h"
#include "LCD1602.h"
#include <intrins.h>

unsigned char ADValue;
sbit CS   = P3^6;
sbit Clk  = P3^4;
sbit DATI = P3^5;
sbit DATO = P3^5;
char routinetimes;

unsigned char ADC0832_Read(unsigned char CH)
{
      unsigned char i,test,adval,dat;
     adval = 0x00;
     test = 0x00;
     Clk = 0;       //初始化
     DATI = 1;
    _nop_();
    CS = 0;
    _nop_();
    Clk = 1;
   _nop_();


   if ( CH == 0x00 )      //通道选择
   {
       Clk = 0;
       DATI = 1;      //通道0的第一位
       _nop_();
      Clk = 1;
        _nop_();
        Clk = 0;
      DATI = 0;      //通道0的第二位
      _nop_();
      Clk = 1;
      _nop_();
    } 
    else
    {
       Clk = 0;
    DATI = 1;      //通道1的第一位
      _nop_();
      Clk = 1;
      _nop_();
      Clk = 0;
      DATI = 1;      //通道1的第二位
    _nop_();
     Clk = 1;
     _nop_();
   }

      Clk = 0;
      DATI = 1;
    for( i = 0;i < 8;i++ )      //读取前8位的值
    {
       _nop_();
       adval <<= 1;
       Clk = 1;
       _nop_();
       Clk = 0;
       if (DATO)
          adval |= 0x01;
      else
          adval |= 0x00;
    }
      for (i = 0; i < 8; i++)      //读取后8位的值
      {
           test >>= 1;
           if (DATO)
              test |= 0x80;
           else 
              test |= 0x00;
          _nop_();
          Clk = 1;
          _nop_();
          Clk = 0;
      }
      if (adval == test)      //比较前8位与后8位的值，如果不相同舍去。若一直出现显示为零，请将该行去掉
       dat = test;
       nop_();
       CS = 1;        //释放ADC0832
       DATO = 1;
       Clk = 1;
       return dat;
}

void main(void)
{
	LCD_Init();
	while(1)
	{
		if (routinetimes==6) routinetimes=0;
		if (routinetimes==0) P1=0x00;
		if (routinetimes==1) P1=0x01;
		if (routinetimes==2) P1=0x02;
		if (routinetimes==3) P1=0x03;
		if (routinetimes==4) P1=0x04;
		if (routinetimes==5) P1=0x05;
		LCD_ShowString(1,1,"routine");
		LCD_ShowNum(1,9,routinetimes,1);
		ADValue=(ADC0832_Read(0x00))*5/255;
		LCD_ShowSignedNum(2,1,ADValue/1,1);
		LCD_ShowString(2,3,".");
		LCD_ShowNum(2,4,(ADC0832_Read(0x00))*500/255-ADValue*100,2);	
		if (ADValue<2) LCD_ShowString(2,8,"low");
		if (ADValue>2) LCD_ShowString(2,8,"high");
    Delay(1500);	
		LCD_Init();
    routinetimes++;		
//		LCD_ShowChar(1,2,'.');
//		LCD_ShowSignedNum(1,1,(ADValue%1)/0.1,1);
	}
}
