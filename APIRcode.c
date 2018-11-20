#include<reg52.h>
sbit rs=P2^7;
sbit rw=P2^6;
sbit en=P2^5;
sbit motor=P1^0;
sbit buz=P1^1;
sbit led=P1^7;
 unsigned long int speed=0;
  /********************delay********************************************/
void delay(unsigned int ch) //delay function
{
unsigned int i=0,j=0;
for(i=0;i<=ch;i++)
for(j=0;j<=i;j++);
}
/*************************************lcd********************************/
void clcd(unsigned char ch)
 {
 P0=ch;
 rs=0;
  rw=0;
 en=1;
 delay(50);
 en=0;
 }  
void dlcd(unsigned char ch)
{
 P0=ch;
 rs=1;
 rw=0;
 en=1;
 delay(50);
 en=0;
 }
 void stringlcd(unsigned char ch,unsigned char *chrt)
 {
 unsigned int ix=0;
 if(ch==0x80)
 clcd(0x01);
 clcd(ch);
 for(ix=0;chrt[ix]!='\0';ix++)
 {
 dlcd(chrt[ix]);
 }
 }
 void initlcd()
 {
 clcd(0x38);
 clcd(0x0e);
 clcd(0x06);
 clcd(0x01);
 clcd(0x80);
 }
 void conv(unsigned long int ch)
 {
 unsigned long int temp=0,temp2=0;
 temp2=ch;
 temp=temp2/100000;
 dlcd(temp+0x30);
 temp2=temp2%100000;

 temp=temp2/10000;
 dlcd(temp+0x30); 
 temp2=temp2%10000;

 temp=temp2/1000;
 dlcd(temp+0x30); 
 temp2=temp2%1000;
 temp=temp2/100;
 dlcd(temp+0x30); 
 temp2=temp2%100;
 temp=temp2/10;
 dlcd(temp+0x30);
 temp2=temp2%10;
 dlcd(temp2+0x30);
 }

 /*******************************************serial*************************************/
void serialinit()
{
TMOD=0x20;// timer1,mode2
SCON=0x50;
TH1=-3;
TR1=1;
}
void tx(unsigned char ch)
{
SBUF=ch;
while(TI==0);  //wait until TI=1
TI=0;
}
void txs(unsigned char *chr)
{
unsigned char i=0;
for(i=0;chr[i]!='\0';i++)
tx(chr[i]);
}
unsigned char rx()
{
unsigned char ch;
while(RI==0);
ch=SBUF;
RI=0;
return ch;
}
 /* **********************************************     gsm   ***********************  */

void sendmsg(unsigned char *num,unsigned char *chrm)
{
int i=0;
txs("AT+CMGS=\"");
for(i=0;num[i]!='\0';i++)
{
tx(num[i]);
}
txs("\"\r\n");
delay(200);
for(i=0;chrm[i]!='\0';i++)
{
tx(chrm[i]);
}


tx(0x1a);
}
void gsminit()
{
do
{
txs("AT\r\n");
}while(rx()!='O');

txs("AT+CREG?\r\n");
delay(400);
txs("AT+CMGF=1\r\n");
delay(200);
txs("AT+CNMI=1,2,0,0\r\n");
delay(200);
}



  
 void main()
 {

 initlcd();
  stringlcd(0x80,"    WELCOME    ");
 serialinit();
   gsminit()	;
   clcd(0x01) ;
   delay(500);
    stringlcd(0x80,"GSM INETIALISED");
 delay(500);
 clcd(0x01);
 stringlcd(0xC0,"   STATUS:ON    ");

 motor=1;
 buz=1;
 led=1;
 EA=1;
 IT0=1;//EDGE TRIGGER
 EX0=1;//ENTERNAL
 ET0=1;//TIMER
 TH0=0X00;
 TL0=0X00;
 TR0=1;
 

 while(1)
 {
 delay(400);
 }
 }
 void intr() interrupt 0  
 {
 speed++;
 ET0=1;
 }
 unsigned int count=0;
 void timer0() interrupt 1 
 {
 count++;
 TF0=0;
 EA=0;
 TR0=0;

 if(count==555)	//111 for one second //555 for five sec
 {
 EA=0;

if(speed==0)
{
motor=0;
stringlcd(0x80,"STOPPED  ");dlcd(speed+0x30);
buz=0;
 stringlcd(0xC0," STATUS:LOCKED ");
sendmsg("8886646669","STOPPED*******");
while(1);
}
if(speed==1)
{
motor=0;
stringlcd(0x80,"STOPPED  ");dlcd(speed+0x30);
buz=0;
 stringlcd(0xC0," STATUS:LOCKED ");
sendmsg("8886646669","STOPPED*******");
while(1);
}
if(speed==2)
{
motor=1;
stringlcd(0x80,"DANGER    ");dlcd(speed+0x30);
buz=0;
delay(600);
buz=1;
 stringlcd(0xC0,"   STATUS:ON    ");
sendmsg("8886646669","DANGER*******");
}
if(speed==3)
{
motor=1;
stringlcd(0x80,"ALERT     ");dlcd(speed+0x30);
buz=0;
delay(400);
buz=1;
 stringlcd(0xC0,"   STATUS:ON    ");
sendmsg("8886646669","ALERT********");
}
if(speed==4)
{
motor=1;
stringlcd(0x80,"ABNORMAL..........  ");dlcd(speed+0x30);
 stringlcd(0xC0,"   STATUS:ON    ");
buz=0;
delay(300);
buz=1;
}
if(speed>=5)
{
motor=1;
stringlcd(0x80,"NORMAL.............    ");dlcd(speed+0x30);
 stringlcd(0xC0,"   STATUS:ON    ");
}
 led=0;
 delay(200);
 led=1;

 speed=0;
 count=1;
 }


 TH0=0X00;
 TL0=0X00;
 TR0=1;
  EA=1;


 }
