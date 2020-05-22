/*

#include <xc.h>
#include <libpic30.h>
#include <p30f4011.h>


// Configuration settings
_FOSC(CSW_FSCM_OFF & FRC_PLL16); // Fosc=16x4MHz, Fcy=30MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);                  // Watchdog timer off
_FBORPOR(MCLR_DIS);              // Disable reset pin

//-----------------------------------------------------------------

#define RF0_INPUT  PORTFbits.RF0
struct
{
	unsigned T1_Flag:1;					
	unsigned T2_Flag:1;
	unsigned T3_Flag:1;
	unsigned T4_Flag:1;
} Timer; 
  
 //-----------------------------------------------------------------

void Delay_x_mS(int N_mS);
void __attribute__((interrupt, auto_psv)) _PWMInterrupt(void);
void Left_direction(void);
void Right_direction(void);


//-----------------------------------------------------------------
 


 void Delay_x_mS( int N_mS ){

		int	Loop_mS,Del_1mS;
		
		for ( Loop_mS = 0 ; Loop_mS < N_mS ; Loop_mS++ ) 
		{
			for (Del_1mS = 0 ; Del_1mS < 324 ; Del_1mS ++ );
					
		}
    }
 void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    // Clear Timer 2 interrupt flag
    LED2=!LED2;
	Timer.T2_Flag=1;
	T2_Count++;
	IFS0bits.T2IF = 0 ;
 }
 
 void Left_direction(void){
     Delay_x_mS(100);
     
     _LATE5 = 1;          // Turn on LED on RD18
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.85*2*PTPER;
        _LATE5 = 0;          // Turn off LED on RD18
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE4 = 1;          // Turn on LED on RD19
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.75*2*PTPER;
        _LATE4 = 0;          // Turn on LED on RD19
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE3 = 1;          // Turn on LED on RD20
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.65*2*PTPER;
        _LATE3 = 0;          // Turn on LED on RD20
        Delay_x_mS(10000);     // 5 second delay
        
        _LATD1 = 1;          // Turn on LED on RD21
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.55*2*PTPER;
        _LATD1 = 0;          // Turn on LED on RD21
        Delay_x_mS(10000);     // 5 second delay
        
        _LATD3 = 1;          // Turn on LED on RD22
       Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.45*2*PTPER;
        _LATD3 = 0;          // Turn on LED on RD22
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE8 = 1;          // Turn on LED on RD23
       Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.35*2*PTPER;
        _LATE8 = 0;          // Turn on LED on RD23
        Delay_x_mS(10000);     // 5 second delay
        
        _LATC14 = 1;          // Turn on LED on RD24
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.25*2*PTPER;
        _LATC14 = 0;          // Turn on LED on RD24
        Delay_x_mS(10000);     // 5 second delay
 }
 void Right_direction(void){
     Delay_x_mS(100);
     
     _LATC14 = 1;          // Turn on LED on RD18
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.35*2*PTPER;
        _LATC14 = 0;          // Turn off LED on RD18
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE8 = 1;          // Turn on LED on RD19
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.45*2*PTPER;
        _LATE8 = 0;          // Turn on LED on RD19
        Delay_x_mS(10000);     // 5 second delay
        
        _LATD1 = 1;          // Turn on LED on RD20
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.55*2*PTPER;
        _LATD1 = 0;          // Turn on LED on RD20
        Delay_x_mS(10000);     // 5 second delay
        
        _LATD3 = 1;          // Turn on LED on RD21
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.65*2*PTPER;
        _LATD3 = 0;          // Turn on LED on RD21
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE3 = 1;          // Turn on LED on RD22
       Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.75*2*PTPER;
        _LATE3 = 0;          // Turn on LED on RD22
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE4 = 1;          // Turn on LED on RD23
       Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.85*2*PTPER;
        _LATE4 = 0;          // Turn on LED on RD23
        Delay_x_mS(10000);     // 5 second delay
        
        _LATE5 = 1;          // Turn on LED on RD24
        Delay_x_mS(10000);     // 5 second delay
        PDC1 = 0.95*2*PTPER;
        _LATE5 = 0;          // Turn on LED on RD24
        Delay_x_mS(10000);     // 5 second delay
 }
 
 //-----------------------------------------------------------------
int main(void)
{
    // Configure RD0 as a digital output for an indicator LED
    TRISC=0;
    TRISE=0;
    TRISD=0;
    _PMOD1 = 1;   // Enable PWM channel 1 in independent mode
//     _PEN1H = 1;   // Enable PWM1H pin
//     _PEN2L = 0;
    _PTCKPS = 0;  // prescale=1:64 (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    PTPER = 799;  // Set PWM frequency to 20 kHz
    PDC1 = 0;   // Set duty cycle to 10%
    _PTEN = 1;    // Enable PWM time base
     
     T2CON = 0;            // Clear the Timer 2 configuration
 
    TMR2 = 0;             // Reset Timer 2 counter
    PR2 = 62500;          // Set the Timer 2 period (max 65535)
 
    T2CONbits.TCS = 0;    // Select internal clock (Fosc/4)
    T2CONbits.TCKPS = 3;  // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
  
    _T2IP = 1;            // Set the Timer 2 interrupt priority
    _T2IF = 0;            // Clear the Timer 2 interrupt flag
    _T2IE = 1;            // Enable Timer 2 interrupt
    T2CONbits.TON = 1;    // Turn on Timer 2
    while(1)
    {
        if (Timer.T2_Flag){
            Timer.T2_Flag = 0;
            Right_direction();
        }
        
 
    }
    return 0;
}    

 */
 

 
 