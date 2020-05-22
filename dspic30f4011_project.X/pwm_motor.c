/*
#include <xc.h>
#include <libpic30.h>
#include <p30f4011.h>
#include "LCD.h"

_FOSC(CSW_FSCM_OFF & FRC_PLL8);  //Fosc=4MHz,PLL=16, Fcy=16MHz, Fpwm= 20kHz
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);
//-----------------------------------------------------------------

#define INT2 IFS1bits.INT2IF 
#define RF0_INPUT  PORTFbits.RF0
#define RF1_INPUT  PORTFbits.RF1
#define RF2_INPUT  PORTFbits.RF2
#define RD0_INPUT  PORTDbits.RD0

//-----------------------------------------------------------------

union
	{
	unsigned Word ; 
	struct		
		{	
			unsigned AD_DONE : 1 ;
			unsigned : 15 ;
		};
	} Flags ;
int state = 1;            // State variable for state machine
int ms_time = 0;          // Counts milliseconds in current state
 
int stepper_target = 0;   // Assign a value to this to set stepper angle
int stepper_position = 0; // Used to count steps towards target value
int count = 0;            // Counts timer interrupts - step every 4th time
 

//-----------------------------------------------------------------

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void PWM_Int (void);
void Interrupt_init(void);
void change_state(int n);
void led (int i);
void Timer1_Int(void);


//-----------------------------------------------------------------

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    // Clear Timer 1 interrupt flag
    IFS0bits.T1IF = 0;
  
    // Increment the millisecond count
    ms_time = ms_time + 1;
     
    // Stepping every 1ms would be too often for the stepper,
    // so only step once every 3 interrupts.
//    count = count + 1;
//    if (count == 3)
//    {
//        // This only happens every 3rd interrupt - i.e. once every 3ms
//        count = 0;
//        if (stepper_position < stepper_target) step_forward();
//        if (stepper_position > stepper_target) step_back();
    }

void change_state(int n)
{
    state = n;
    ms_time = 0;
}
void led (int i){
    if (i == 1)
    {
        _LATC14 = 1;
    }
    else _LATC14 = 0;
}
 void PWM_Int (void)
{
    // Configure PWM
    // Fpwm=20KHz; Fcy=16MHz
     _PTOPS = 0;                 //PWM TIMER post-scale
     
    _PTCKPS = 0;                //PWM TIMER pre-scale (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    _PTMOD = 0;                  // PWM operate
    //PTCON = 0x8000;              // Enable PWM channel 1 in independent mode
    
    PTPER = 399;                 // PWM Timebase period (PTPER= (Tcy/4)*Tpwm - 1))
    _PMOD1 = 1;                  // Enable PWM channel 1 in independent mode
    _PEN1H = 1;                  //PWM High pin is enable
    _PEN2L = 0;                  //  REVERSE MOTOR 1 OR 0
    // Can write by PWMCON1 = 0x0112; (_PMOD1,_PEN1H,_PEN2L)
    // Duty cycle has a max value of 2xPeriod since output can change on rising or falling edge of Tcy
     PDC1 = 0;
     PDC2 = 0;
    _PTEN = 1;                   // Enable PWM Timerbase        
  
}
 void Timer1_Int(void)
{
    T1CON = 0;                                  // Clear the Timer 1 configuration
 
    TMR1 = 0;                                   // Reset Timer 1 counter
    PR1 = 16000;                                // Set the Timer 1 period (max 65535)
    //When the timer counter reaches PR1, it resets to 0 and triggers an interrupt. This will 
    //happen every PR1*Prescaler*Tcy= 10000*1*62.5ns=0.625ms
 
    T1CONbits.TCS = 0;                          // Select internal clock (Fosc/4)
    T1CONbits.TCKPS = 0;                        // Prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)

    _T1IP = 1;                                  // Set the Timer 1 interrupt priority
    _T1IF = 0;                                  // Clear the Timer 1 interrupt flag
    _T1IE = 1;                                  // Enable Timer 1 interrupt
    T1CONbits.TON = 1;                          // Turn on Timer 1

}
 //-----------------------------------------------------------------
 
int main(void) {
    
    PWM_Int();
    OpenLCD();
    Timer1_Int();
setcurLCD(0,0);
putrsLCD(" MPF MOTOR ");
setcurLCD(0,1);
putrsLCD("Duty : 95 ");
_TRISC14 = 0;

    while(1){
    
        if (RF0_INPUT ==0)
        {
            PDC1=0.95*2*PTPER;
        }
         }
         
    }
*/