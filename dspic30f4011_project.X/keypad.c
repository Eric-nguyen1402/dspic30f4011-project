/*
#include "p30f4011.h"

#include "stdio.h"
#include "xc.h"
#include "stdlib.h"


_FOSC(CSW_FSCM_OFF & XT);
_FWDT(WDT_OFF);
_FBORPOR(PBOR_ON & MCLR_EN);
_FGS(CODE_PROT_OFF);

//-----------------------------------------------------------------

#define X_1  _LATD0
#define X_2  _LATF0
#define X_3  _LATF1
#define X_4  _LATF2
#define Y_1  _TRISF3
#define Y_2  _TRISF4
#define Y_3  _TRISF5
#define Y_4  _TRISF6
//#define  Keypad_PORT      PORTD
//#define  Keypad_PORT_1    PORTF
//#define Keypad_PORT_Direction     TRISD
//#define Keypad_PORT_Direction_1   TRISF

//void SerialInit(void);

//-----------------------------------------------------------------

//char KeyArray[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

unsigned int Count[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

unsigned int Col=0,Row=0,count=0,i,j;

unsigned int temp,temp1;

//-----------------------------------------------------------------

void ScanCol(void); 
void ScanRow(void);
void Delay_x_mS( int N_mS ); 

//-----------------------------------------------------------------

void ScanRow()

{

temp=(PORTF & 0x000F);

switch(temp)
{
case 0x000b: Row=3;
break;
case 0x000d: Row=2;
break;
case 0x000e: Row=1;
break;
}

temp1= (PORTD & 0x0001);
switch(temp1)
{
case 0x000e: Row=0;
break;
}
} 
void ScanCol()

{

temp=(PORTF & 0x000F);

switch(temp)

{

case 0x00b0: Col=3;

break;

case 0x00d0: Col=2;

break;

case 0x00e0: Col=1;

break;

case 0x0007: Col=0;

break;

}

} 
void SerialInit()

{

U1MODE = 0x8000;

U1STA = 0x8400;

U1BRG = BRGVAL;

IFS0bits.U1TXIF=1;

} 
void putch(unsigned int SendDat)

{

while(IFS0bits.U1TXIF==0);

U1TXREG=SendDat;

IFS0bits.U1TXIF=0;

}
void Delay_x_mS( int N_mS ) 
{
		int	Loop_mS,Del_1mS;
		
		for ( Loop_mS = 0 ; Loop_mS < N_mS ; Loop_mS++ ) 
		{
			for (Del_1mS = 0 ; Del_1mS < 324 ; Del_1mS ++ );
					
		}
		 
}
int main()
{

TRISF = 0x0000;

TRISD = 0x0000;

TRISC = 0;

ADPCFG = 0xffff;
Delay_x_mS(50);
//SerialInit();

Delay_x_mS(50);

while(1)
{

LATF = 0x000f;
LATD = 0x0001;

TRISF = 0x000f;
TRISD = 0x0001;

while(((PORTF & 0x000f)&(PORTD & 0x0001))==0x000f);

//ScanRow();
//ScanCol();
//LATF = 0x00f0;
//LATD = 0x00f0;
TRISF = 0x00f0;
TRISD = 0x00f0;
if (_LATB6==1)
{
    _LATC14 = 1;
    Delay_x_mS(100);
    _LATC14 = 0;
    Delay_x_mS(100);
}
while((PORTF & 0x00f0)==0x00f0);

}

}


char keypad_scanner(void)  
{           
            X_1 = 0; X_2 = 1; X_3 = 1; X_4 = 1;    
            if (Y_1 == 0) { Delay_x_mS(100); while (Y_1==0); return '1'; }
            if (Y_2 == 0) { Delay_x_mS(100); while (Y_2==0); return '2'; }
            if (Y_3 == 0) { Delay_x_mS(100); while (Y_3==0); return '3'; }
            if (Y_4 == 0) { Delay_x_mS(100); while (Y_4==0); return 'A'; }
            
            X_1 = 1; X_2 = 0; X_3 = 1; X_4 = 1;    
            if (Y_1 == 0) { Delay_x_mS(100); while (Y_1==0); return '4'; }
            if (Y_2 == 0) { Delay_x_mS(100); while (Y_2==0); return '5'; }
            if (Y_3 == 0) { Delay_x_mS(100); while (Y_3==0); return '6'; }
            if (Y_4 == 0) { Delay_x_mS(100); while (Y_4==0); return 'B'; }
            
            X_1 = 1; X_2 = 1; X_3 = 0; X_4 = 1;    
            if (Y_1 == 0) { Delay_x_mS(100); while (Y_1==0); return '7'; }
            if (Y_2 == 0) { Delay_x_mS(100); while (Y_2==0); return '8'; }
            if (Y_3 == 0) { Delay_x_mS(100); while (Y_3==0); return '9'; }
            if (Y_4 == 0) { Delay_x_mS(100); while (Y_4==0); return 'C'; }
           
            X_1 = 1; X_2 = 1; X_3 = 1; X_4 = 0;    
            if (Y_1 == 0) { Delay_x_mS(100); while (Y_1==0); return '*'; }
            if (Y_2 == 0) { Delay_x_mS(100); while (Y_2==0); return '0'; }
            if (Y_3 == 0) { Delay_x_mS(100); while (Y_3==0); return '#'; }
            if (Y_4 == 0) { Delay_x_mS(100); while (Y_4==0); return 'D'; }
            
    return 'n';                   
}
void InitKeypad(void)
{
            Keypad_PORT = 0x00; 
            Keypad_PORT_1 = 0x00; // Set Keypad port pin values zero
            Keypad_PORT_Direction = 0xFE;  
            Keypad_PORT_Direction_1 = 0xF8; // Last 4 pins input, First 4 pins output        
            //OPTION_REG &= 0x7F;
}
char switch_press_scan(void)                       // Get key from user
{
            char key = 'n';              // Assume no key pressed
            while(key=='n')              // Wait untill a key is pressed
            key = keypad_scanner();   // Scan the keys again and again
            return key;                  //when key pressed then return its value
}
  */