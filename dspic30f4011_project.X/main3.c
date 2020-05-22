
/*

#include <xc.h>
#include <libpic30.h>
#include <p30f4011.h>


_FOSC(CSW_FSCM_OFF & FRC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);

void configure_pins();
unsigned int read_analog_channel(int n);
void Delay_x_mS(int);

void main(void) {
    TRISD = 0X00;
    TRISE = 0x00;
    TRISC = 0x00;
    TRISF = 0x00;
    int voltage;
    configure_pins();
    while(1)
    {
        
        _LATC13=1;
        Delay_x_mS(1000);
        _LATC14=1;
        Delay_x_mS(1000);
        _LATE8=1;
        Delay_x_mS(1000);
        _LATD1=1;
        Delay_x_mS(1000);
        _LATD3=1;
        Delay_x_mS(1000);
        _LATE3=1;
        Delay_x_mS(1000);
        _LATE4=1;
        Delay_x_mS(1000);
        _LATE5=1;
        Delay_x_mS(1000); 
        
        voltage = read_analog_channel(0);
        PDC1 = (int)((voltage/1023.0)*2*PTPER);
        
        voltage = read_analog_channel(1);
        PDC2 = (int)((voltage/1023.0)*2*PTPER);
        
        voltage = read_analog_channel(2);
        PDC3 = (int)((voltage/1023.0)*2*PTPER);
        
    }
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
void configure_pins(){
    // Configure RD0 as a digital output
    LATD = 0;
    TRISD = 0b11111110;
    // Configure analog inputs
    TRISB = 0x01FF;
    ADPCFG = 0xFF00;
    ADCON1 = 0;
    ADCON2 = 0;
    ADCON3 = 0x0005;
    ADCON1bits.ADON = 1;    //Turn ADC ON
    
    //Configure PWM for free Mode
    PWMCON1 = 0x00FF;
    PTCON = 0;
    _PTCKPS = 3;
    PTPER = 9470;
    PDC1 = 0;
    PDC2 = 0;
    PDC3 = 0;
    PTMR = 0;
    _PTEN = 1;     //Enable PWM time 
    
}
unsigned int read_analog_channel(int channel){
    ADCHS = channel;
    ADCON1bits.SAMP = 1;
    __delay32(30);
    ADCON1bits.SAMP = 0;
    while (!ADCON1bits.DONE);
    return ADCBUF0;
    
    
}

*/