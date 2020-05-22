

#include "p30f4011.h"
#include "LCD.h"


_FOSC(CSW_FSCM_OFF & FRC_PLL16);  //Fosc=16x4MHz, Fcy=16MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);

//-----------------------------------------------------------------

#define INT2 IFS1bits.INT2IF 
#define RF0_INPUT  PORTFbits.RF0
#define RF1_INPUT  PORTFbits.RF1
#define RF2_INPUT  PORTFbits.RF2
#define RD0_INPUT  PORTDbits.RD0
#define OneSecond  SystemFlag.Bit0

//-----------------------------------------------------------------
int temp;
struct
{
	unsigned Timer1_Flag :1;
    unsigned Timer2_Flag :1;
    unsigned Timer3_Flag :1;
} Timer;
unsigned char	Minute = 0 ;		
unsigned char	Second = 0 ;
unsigned int	miliSec = 0 ;
union 	{
			unsigned char ByteAccess ;
			struct 	{
						unsigned Bit0: 1 ;
						unsigned Bit1: 1 ;
						unsigned Bit2: 1 ;
						unsigned unused : 5 ;
					} ;
		} SystemFlag ;
        

//-----------------------------------------------------------------
 
void Interrupt_init(void);
void DelayMs(unsigned int);
void Timer1_Int(void);
void Timer2_Int(void);
void Timer3_Int(void);
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);
void Show_Time (void);

//-----------------------------------------------------------------

void Interrupt_init(void)
{

INTCON1 = 0x0000;

INTCON2 = 0x0000;

IFS0 = 0x0000;

IFS1 = 0x0000;

IEC1 = 0x0080;

IPC5 = 0x5000;

}

void DelayMs(unsigned int Ms)
{
unsigned int delay_cnst;
      while(Ms>0){
        Ms--;
           for(delay_cnst = 0;delay_cnst <276;delay_cnst++){
               
                  }
              }
}

void Timer1_Int(void)
{
    T1CON = 0;                                  // Clear the Timer 1 configuration
 
    TMR1 = 0;                                   // Reset Timer 1 counter
    PR1 = 10000;                                // Set the Timer 1 period (max 65535)
    //When the timer counter reaches PR1, it resets to 0 and triggers an interrupt. This will 
    //happen every PR1*Prescaler*Tcy= 10000*1*62.5ns=0.625ms
 
    T1CONbits.TCS = 0;                          // Select internal clock (Fosc/4)
    T1CONbits.TCKPS = 0;                        // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)

    _T1IP = 1;                                  // Set the Timer 1 interrupt priority
    _T1IF = 0;                                  // Clear the Timer 1 interrupt flag
    _T1IE = 1;                                  // Enable Timer 1 interrupt
    T1CONbits.TON = 1;                          // Turn on Timer 1

}

void Timer2_Int(void)
{
     T2CON = 0;                                  // Clear the Timer 2 configuration
 
    TMR2 = 0;                                    // Reset Timer 2 counter
    PR2 = 10000;                                 // Set the Timer 2 period (max 65535)
 
    T2CONbits.TCS = 0;                           // Select internal clock (Fosc/4)
    T2CONbits.TCKPS = 3;                         // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)

    _T2IP = 1;                                   // Set the Timer 2 interrupt priority
    _T2IF = 0;                                   // Clear the Timer 2 interrupt flag
    _T2IE = 1;                                   // Enable Timer 2 interrupt
    T2CONbits.TON = 1;                           // Turn on Timer 2
    
    
    
    //T2CONbits.T32 = 1;                          // 32-bit timer select 

}
void Timer3_Int(void)
{
    T3CON = 0;                                    // Clear the Timer 3 configuration
 
    TMR3 = 0;                                     // Reset Timer 3 counter
    PR3 = 10000;                                  // Set the Timer 3 period (max 65535)
 
    T3CONbits.TCS = 0;                            // Select internal clock (Fosc/4)
    T3CONbits.TCKPS = 3;                          // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)

    _T3IP = 1;                                    // Set the Timer 3 interrupt priority
    _T3IF = 0;                                    // Clear the Timer 3 interrupt flag
    _T3IE = 1;                                    // Enable Timer 3 interrupt
    T3CONbits.TON = 1;                            // Turn on Timer 3
    
    //T3CONbits.T32 = 1;                          // 32-bit timer select 

}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    
     // Clear Timer 1 interrupt flag
    IFS0bits.T1IF = 0;
    miliSec += 1 ;				//

	if (miliSec == 1000)		//
		{
			OneSecond = 1 ;
			miliSec  = 0 ;
		} 
}
void Show_Time (void)
{
    setcurLCD(5,1);
    put_Num_LCD(Minute);
    putcLCD(':');
    put_Num_LCD(Second);
}
//-----------------------------------------------------------------

int main()
{

TRISE=0;
//Interrupt_init();
Timer1_Int();
OpenLCD();
setcurLCD(0,0);
putrsLCD("Timer is begin");
setcurLCD(0,1);
putrsLCD("TIME 00:00");
OneSecond=0;
while(1) {
    
    if ( OneSecond ) 			    					
														
		{
			OneSecond = 0 ;							 
					
			Second += 1 ;								
																		
			if ( Second == 60 )
			{
				Second = 0 ;							// Second : 0 .. 59
				Minute +=1 ;
				if ( Minute == 60 )	
					Minute = 0 ;						// Minute : 0 .. 59
			}
			Show_Time();								// 
		}
	}		
return 0;

}

 
