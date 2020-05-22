#include <p30f4011.h>
#include <xc.h>
#include "LCD.h"

_FOSC(CSW_FSCM_OFF & FRC_PLL16);  //Fosc=16x4MHz, Fcy=16MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);


//-----------------------------------------------------------------

#define FEEDBACK_PERIOD 20  // 5msec period for 4KHz PWM

#define PWM_PERIOD 4000 // 2s period

#define Designed_Count 6000 //designed counter for 90 degree,

#define P 1 // P parameter
 
#define RF0_INPUT  PORTFbits.RF0
//-----------------------------------------------------------------


// This bit structure provides status flags for the software

struct {
unsigned :1;
unsigned Restart:1;
unsigned :1;
unsigned :1;
unsigned :1;
unsigned PWMEvent:1;
unsigned Feedback:1;
unsigned Button1:1;
unsigned Button2:1;
} Flags;

unsigned int FeedbackCount;

unsigned int PWMCount;
unsigned int temp;

int error,Control_Output;

long Counter;
 
//-----------------------------------------------------------------

void __attribute__((__interrupt__)) _PWMInterrupt(void);

void InitQEI (void);

void PWM_Int (void);

void Delay(unsigned int count);

//-----------------------------------------------------------------



void __attribute__((__interrupt__)) _PWMInterrupt(void)
{
FeedbackCount--;
PWMCount--;

if(FeedbackCount == 0)
{
Flags.Feedback = 1;
FeedbackCount = FEEDBACK_PERIOD;
}
IFS2bits.PWMIF = 0;
if(PWMCount ==0)
{
Flags.PWMEvent = 1;
PWMCount = PWM_PERIOD;
}
IFS2bits.PWMIF = 0;
}

void InitQEI (void)
{
    ADPCFG |= 0x0038;               // Configure QEI pins as digital inputs
    QEICONbits.QEIM = 0;            // Disable QEI Module
    QEICONbits.CNTERR = 0;          // Clear any count errors
    QEICONbits.QEISIDL = 1;         // Continue operation during sleep
    QEICONbits.SWPAB = 0;           // QEA and QEB not swapped 
    QEICONbits.PCDOUT = 1;          // Normal I/O pin operation 
    QEICONbits.POSRES = 1;          // Index pulse resets position counter
    DFLTCONbits.CEID = 1;           // Count error interrupts disabled
    DFLTCONbits.QEOUT = 1;          // Digital filters output enable for QEn pins
    DFLTCONbits.QECK = 1;            // 1:2 clock devide for digital filter for QEn
    _TQGATE = 0;
    _TQCS = 0;
    //DFLTCONbits.INDOUT = 1;          // Digital filter output enable for index pin (only dsPIC30F6010 )
    //DFLTCONbits.INDCK = 0;           // 1:64 clock divide for digital filter for Index (only dsPIC30F6010 )
    POSCNT = 0;                      // Reset position counter
    _QEIIE = 1;
    QEICONbits.QEIM = 6;             // X4 mode with position counter reset by Index
    MAXCNT = 0xFFFE; //max value to reset
    return;   
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
  IFS2bits.PWMIF = 0;
  IEC2bits.PWMIE = 1; // Enable PWM interrupts.
}


//-----------------------------------------------------------------

int main ( void )
{
  OpenLCD();  
  PWM_Int();
  InitQEI();

  PDC1 = 0; //stop the motors initially
  //PORTB= 0;
  TRISB = 0xFFBF;
  
  FeedbackCount = 100;
  PWMCount = 4000;
  
while (1)
 {
    if(Flags.PWMEvent) // assign a new duty cycle every 2s.
        {
          PDC1 = 0.2*PTPER; // 20% of full speed in foward direction
          Flags.PWMEvent = 0;
        }

    if(Flags.Feedback) // Perform feedback control
       {
         //read counter from encoder
        Counter = POSCNT; // read the QEI
        // Perform control
        error = Designed_Count - Counter; //error position
        Control_Output = error * P; // multiply p parameter

        // assign a new duty cycle by scaling the output

        // 0 => motor stops; 0 => full speed in foward speed; 460=> full speed in reverse speed

        // moor: 10000 RPM, 66 gear ratio

        PDC1 =(int) 0 + 2.3*Control_Output/22.2;
        setcurLCD(0,0);
        put_Num3_LCD(PDC1);
        Flags.Feedback = 0;
        } // end if(Feedback)
    
//    else if (temp==0)
//    {
//        PDC1=0;
//    }
}
} // end main
 
//---------------------------------------------------------------------
// This is a generic delay routine

