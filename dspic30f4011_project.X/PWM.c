

#include <xc.h>
#include <libpic30.h>
#include <pic30f4011.h>

// Configuration settings
_FOSC(CSW_FSCM_OFF & FRC_PLL16); // Fosc=16x7.5MHz, Fcy=30MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);                  // Watchdog timer off
_FBORPOR(MCLR_DIS);              // Disable reset pin

void Delay_x_mS( int N_mS );
 
void __attribute__((interrupt, auto_psv)) _PWMInterrupt(void)
{
    // Reset PWM interrupt flag
    _PWMIF = 0;
     
    // Alternate between overriding PWM1L and overriding PWM1H.
    // Whichever pin is overridden will be driven low during
    // the next PWM period, suppressing its output pulse.
    if (_POVD1H)
    {
        _POVD1H = 0;
        _POVD2L = 1;
    }
    else
    {
        _POVD1L = 0;
        _POVD2H = 1;
    }
}
 
int main(void)
{
    // Configure RD0 as a digital output for an indicator LED
    TRISD = 0;
    TRISE = 0;
    TRISC = 0;
    _PMOD1 = 1;   // Enable PWM channel 1 in independent mode
    _PMOD2 = 1;
     _PEN1H = 1;   // Enable PWM1H pin
     _PEN2L = 0;
    _POUT2L = 0;  // When PWM1L is overriden, set it low
    _POUT1H = 0;  // When PWM1H is overriden, set it low
    _POVD2L = 1;  // Initially, enable override on PWM2L
    _POVD1H = 0;  // Initially, disable override on PWM1H
    _OSYNC = 1;   // Synchronise PWM override changes with PTMR reset
    _PWMIE = 1;   // Enable PWM interrupt
    _PTCKPS = 0;  // prescale=1:64 (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    PTPER = 1499;  // Set PWM frequency to 20 kHz
    //PDC1 = 400;   // Set duty cycle to 10%
    _PTEN = 1;    // Enable PWM time base
     
    // Now just blink an LED while the PWM ISR does the heavy lifting
    while(1)
    {
        _LATC14 = 1;          // Turn on LED on RD18
        Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.35*2*PTPER;
        _LATC14 = 0;          // Turn off LED on RD18
        Delay_x_mS(5000);     // 5 second delay
        
        _LATE8 = 1;          // Turn on LED on RD19
        Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.45*2*PTPER;
        _LATE8 = 0;          // Turn on LED on RD19
        Delay_x_mS(5000);     // 5 second delay
        
        _LATD1 = 1;          // Turn on LED on RD20
        Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.55*2*PTPER;
        _LATD1 = 0;          // Turn on LED on RD20
        Delay_x_mS(5000);     // 5 second delay
        
        _LATD3 = 1;          // Turn on LED on RD21
        Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.65*2*PTPER;
        _LATD3 = 0;          // Turn on LED on RD21
        Delay_x_mS(5000);     // 5 second delay
        
        _LATE3 = 1;          // Turn on LED on RD22
       Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.75*2*PTPER;
        _LATE3 = 0;          // Turn on LED on RD22
        Delay_x_mS(5000);     // 5 second delay
        
        _LATE4 = 1;          // Turn on LED on RD23
       Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.85*2*PTPER;
        _LATE4 = 0;          // Turn on LED on RD23
        Delay_x_mS(5000);     // 5 second delay
        
        _LATE5 = 1;          // Turn on LED on RD24
        Delay_x_mS(5000);     // 5 second delay
        PDC1 = 0.95*2*PTPER;
        _LATE5 = 0;          // Turn on LED on RD24
        Delay_x_mS(5000);     // 5 second delay
       
    return 0;
}   
    void Delay_x_mS( int N_mS ) 
{
		int	Loop_mS,Del_1mS;
		
		for ( Loop_mS = 0 ; Loop_mS < N_mS ; Loop_mS++ ) 
		{
			for (Del_1mS = 0 ; Del_1mS < 324 ; Del_1mS ++ );
					
		}
		 
}