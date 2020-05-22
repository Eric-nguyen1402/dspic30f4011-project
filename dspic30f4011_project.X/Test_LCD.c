/*
#include "LCD.h"
#include <p30f4011.h>

_FOSC(CSW_FSCM_OFF & FRC_PLL16);  //Fosc=16x4MHz, Fcy=16MHz , Fpwm= 20kHz
_FWDT(WDT_OFF);
_FBORPOR(MCLR_DIS);

//-----------------------------------------------------------------

const char	My_String1[]=" Eric " ;	
char 		My_String2[]=" Fighting " ;

//-----------------------------------------------------------------

int	main( void )
{
	OpenLCD( ) ;						// Initialize LCD 
										// 4 bits Data mode
										// 5 * 7 Character 
    
	setcurLCD(0,0) ;					// write in the first line 
	putrsLCD( My_String1 ) ;			// 
										// putrsLCD( )
			
	setcurLCD(0,1) ;					// ¨write in the second line 
	putsLCD( My_String2 ) ;				// 
										// putsLCD( ) 

	while(1) ;							// Forever loop
}
 */