/*
#include "LCD.h"
#include <xc.h>
#include <p30f4011.h>
#include "qei.h"


_FOSC(CSW_FSCM_OFF & FRC_PLL16);  //Fosc=16x4MHz, Fcy=16MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);

//-----------------------------------------------------------------

#define INT2 IFS1bits.INT2IF 
#define RF0_INPUT  PORTFbits.RF0
#define RF1_INPUT  PORTFbits.RF1
#define RF2_INPUT  PORTFbits.RF2
#define RD0_INPUT  PORTDbits.RD0

//-----------------------------------------------------------------

unsigned int voltage;
unsigned int voltage1;
unsigned char Convert_buf[10];
union
	{
	unsigned Word ; 
	struct		
		{	
			unsigned AD_DONE : 1 ;
			unsigned : 15 ;
		};
	} Flags ;

//-----------------------------------------------------------------

void PWM_init (void);
void ADC_init (void);
void InitQEI (void);
unsigned int readADC(int channel);
void show_speed(void);
//void _ISR _ADCInterrupt(void);
//void __attribute__((__interrupt__)) _ADCInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);
void	uitoa( unsigned char *ASCII_Buf , unsigned int IntegerValue );
void Delay_x_mS(int N_mS);

//-----------------------------------------------------------------

void PWM_init (void)
{
    // Configure PWM
    // Fpwm=20KHz; Fcy=30MHz
     _PTOPS = 0;                 //PWM TIMER post-scale
    _PTCKPS = 0;                //PWM TIMER pre-scale (0=1:1, 1=1:4, 2=1:16, 3=1:64)
    _PTMOD = 0;                  // PWM operate
    PTPER = 399;                 // PWM Timebase period (PTPER= (Tcy/4)*Tpwm - 1))
    _PMOD1 = 1;                  // Enable PWM channel 1 in independent mode
    _PEN1H = 1;                  //PWM High pin is enable
    _PEN2L = 0;                  //  REVERSE MOTOR 1 OR 0
    // Can write by PWMCON1 = 0x0112; (_PMOD1,_PEN1H,_PEN2L)
    // Duty cycle has a max value of 2xPeriod since output can change on rising or falling edge of Tcy
     PDC1 = 100;
     PDC2 = 0;
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
    ADCON3 = 0x0002;            // Sample time =2Tad, Tad = 95.24 ns @ 21MIPS
                               // which will give 1/((12+2)*95.24 ns)= 750 ksps
                              // Tcy=62.5ns, Tad=(Tcy(ADCS+1))/2, Tad must be at least 83.33ns.
                          //therefore, ADCS cannnot have any value less than 2      
    ADCON1bits.ADON=1;
   }
void InitQEI (void)
{
    ADPCFG |= 0x0038;               // Configure QEI pins as digital inputs
    QEICONbits.QEIM = 0;            // Disable QEI Module
    QEICONbits.CNTERR = 0;          // Clear any count errors
    QEICONbits.QEISIDL = 0;         // Continue operation during sleep
    QEICONbits.SWPAB = 0;           // QEA and QEB not swapped 
    QEICONbits.PCDOUT = 0;          // Normal I/O pin operation 
    QEICONbits.POSRES = 1;          // Index pulse resets position counter
    DFLTCONbits.CEID = 1;           // Count error interrupts disabled
    DFLTCONbits.QEOUT = 1;          // Digital filters output enable for QEn pins
    DFLTCONbits.QECK = 0;            // 1:1 clock devide for digital filter for QEn
    //DFLTCONbits.INDOUT = 1;          // Digital filter output enable for index pin (only dsPIC30F6010 )
    //DFLTCONbits.INDCK = 0;           // 1:64 clock divide for digital filter for Index (only dsPIC30F6010 )
    POSCNT = 0;                      // Reset position counter
    QEICONbits.QEIM = 6;             // X4 mode with position counter reset by Index
    return;   
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
void show_speed(void){
    ADCON1bits.SAMP = 1; 
    voltage1 = ReadQEI();
    voltage1=((long)voltage1*480/256);
    uitoa(Convert_buf,voltage1);
    setcurLCD(6,1);
    putcLCD(':');
    
    putsLCD(Convert_buf);
    
}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
		Flags.AD_DONE = 1 ;
		IFS0bits.ADIF = 0 ;
		voltage = ADCBUF0 ;
}
void	uitoa( unsigned char *ASCII_Buf , unsigned int IntegerValue )
{
	
	unsigned int TempValue ;
	unsigned char ZeroDisable ;
	unsigned int  BaseQty ;
	unsigned int  Loop ;

	ZeroDisable = 1 ;
	BaseQty = 10000 ;

	for ( Loop = 0 ; Loop < 4 ; Loop ++)
	{
		TempValue = IntegerValue / BaseQty ;

			if 	( TempValue > 0) 	
				{
					*ASCII_Buf++ = (unsigned char)TempValue + '0' ;
					ZeroDisable = 0 ;
				}
			else if	( ZeroDisable == 0 ) 
					*ASCII_Buf++ = '0' ;
		IntegerValue = IntegerValue - ( TempValue * BaseQty ) ;
		BaseQty = BaseQty / 10 ;

	}
		*ASCII_Buf++ = (unsigned char)IntegerValue + '0' ;
		*ASCII_Buf = (unsigned char) 0x00 ;
		
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

int main(void){
    
OpenLCD();
PWM_init();
ADC_init();
InitQEI();
setcurLCD(0,0);
putrsLCD("   MPF MOTOR");
setcurLCD(0,1);
putrsLCD("SPEED : ");
TRISBbits.TRISB2= 1;
TRISC=0; 
while(1){
    if (RF0_INPUT ==0){
    // Analog input 0 controls PWM 1 duty cycle.
        
		voltage = readADC(0);
		PDC1 = (long)((voltage)*2*PTPER)/100;
        Delay_x_mS(50);
        _LATC14 =1;
        
    }
    else
    {
        PDC1=0;
        _LATC14 =0;
    }
    show_speed();
}
}

*/