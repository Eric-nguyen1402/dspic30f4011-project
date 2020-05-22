
/*
#include <p30f4011.h>
#include <libpic30.h>
#include <xc.h>

_FOSC(CSW_FSCM_OFF & FRC_PLL8);  //Fosc=16x4MHz, Fcy=16MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);

 //-----------------------------------------------------------------

unsigned int ms_counter;
int voltage;

//-----------------------------------------------------------------

//void __attribute__((__interrupt__)) _T1Interrupt(void);
void PWM_init (void);
void ADC_init(void);
unsigned int readADC(int channel);
void Delay_x_mS( int N_mS );

//-----------------------------------------------------------------


void PWM_init (void)
{
    // Configure PWM
    // Fpwm=20KHz; Fcy=30MHz
     _PTOPS = 0;                 //PWM TIMER post-scale
     
    _PTCKPS = 0;                //PWM TIMER pre-scale (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    _PTMOD = 0;                  // PWM operate
    
    PTPER = 532.33333333;                 // PWM Timebase period 
    
    _PMOD1 = 1;                  // Enable PWM channel 1 in independent mode
    _PEN1H = 1;                  //PWM High pin is enable
    _PEN2L = 0;                  //  REVERSE MOTOR 1 OR 0
    // Can write by PWMCON1 = 0x0112; (_PMOD1,_PEN1H,_PEN2L)
    // Duty cycle has a max value of 2xPeriod since output can change on rising or falling edge of Tcy
     PDC1 = 0;
    _PTEN = 1;                   // Enable PWM Timerbase        
  
}
void ADC_init (void)
{
    ADPCFG = 0xFFFB;              // Set AN2/RB2 is an analog input
    // Configure analog inputs
    ADCHS = 0x0002;               // Connect RB2/AN2 as CH0 input
    ADCSSL = 0;
    ADCON1 = 0x0066;                  // SSRC bit =011 Motor control PWM 
                                     // ends sampling and starts converting.
    ADCON2 = 0x0000;
    ADCON3 = 0x0202;            // Sample time =2Tad, Tad = 95.24 ns @ 21MIPS
                               // which will give 1/((12+2)*95.24 ns)= 750 ksps
                              // Tcy=62.5ns, Tad=(Tcy(ADCS+1))/2, Tad must be at least 83.33ns.
                           //therefore, ADCS cannnot have any value less than 2      
    ADCON1bits.ADON=1;
   }
unsigned int readADC(int channel)

{
    
    ADCHS = channel;                         // Select the requested channel
	ADCON1bits.SAMP = 1;                     // start sampling
	Delay_x_mS(30);                          // 1us delay @ 30 MIPS
	ADCON1bits.SAMP = 0;                    // start Converting
	while (!ADCON1bits.DONE);               // Should take 2 * Tad = 190.48us
	return ADCBUF0;                         // Maximum is 799*2=1598
    
}
void Delay_x_mS( int N_mS ) 
{
		int	Loop_mS,Del_1mS;
		
		for ( Loop_mS = 0 ; Loop_mS < N_mS ; Loop_mS++ ) 
		{
			for (Del_1mS = 0 ; Del_1mS < 324 ; Del_1mS ++ );
					
		}
		 
}

//-----------------------------------------------------------------

int main(void)
{

    PWM_init();
    ADC_init();
    TRISBbits.TRISB2= 1;
    while (1)
    {
        // Analog input 0 controls PWM 1 duty cycle.
		voltage = readADC(0);
		PDC1 = int (voltage)*2*PTPER;
        

    }
    return 0;
}
 
*/