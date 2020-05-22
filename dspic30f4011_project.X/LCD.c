 

#include 	<p30F4011.h>
#include    "LCD.h"




#define LCD_RS			LATFbits.LATF4		// The definition of control pins

#define LCD_E			LATBbits.LATB6
#define _NOP() do { __asm__ __volatile__ ("nop"); } while (0)


#define	DIR_LCD_RS		TRISFbits.TRISF4	// Direction of control pins
#define	DIR_LCD_E		TRISBbits.TRISB6
#define LCD_D4          LATDbits.LATD3
#define DIR_LCD_D4      TRISDbits.TRISD3  
#define LCD_D5          LATEbits.LATE3
#define DIR_LCD_D5      TRISEbits.TRISE3
#define LCD_D6          LATEbits.LATE4
#define DIR_LCD_D6      TRISEbits.TRISE4
#define LCD_D7          LATEbits.LATE5
#define DIR_LCD_D7      TRISEbits.TRISE5


//  LCD Module commands --- These settings can be found in the LCD datasheet
#define DISP_2Line_8Bit		0x0038		// 2 lines & 8 bits setting
#define DISP_2Line_4Bit		0x0028		// 2 lines & 4 bits setting
#define DISP_ON				0x00C		// Display on
#define DISP_ON_C			0x00E		// Display on, Cursor on
#define DISP_ON_B			0x00F		// Display on, Cursor on, Blink cursor
#define DISP_OFF			0x008		// Display off
#define CLR_DISP			0x001		// Clear the Display
#define ENTRY_INC			0x006		// Entry Increment and Cursor Move
#define ENTRY_INC_S			0x007		// Entry Increment and Display Shift
#define ENTRY_DEC			0x004		// Entry Decrement and Cursor Move
#define ENTRY_DEC_S			0x005		// Entry Decrement and Display Shift
#define DD_RAM_ADDR			0x080		// Least Significant 7-bit are for address
#define DD_RAM_UL			0x080		// Upper Left coner of the Display	
		
unsigned char 	Temp_CMD ;				// Temperary Buffers for Command,
unsigned char 	Str_Temp ;				// for String,
int				Temp_LCD_DATA ;			// for PORT data (This will be restored after Communication w/ LCD)
int LCD_DATA;
 
void LCD_DATA_TRANS(int SWAP_LCD_DATA)
{
	
	LCD_D7=((SWAP_LCD_DATA & 0x0008)>>3)	;
	LCD_D6=((SWAP_LCD_DATA & 0x0004)>>2)	;
	LCD_D5=((SWAP_LCD_DATA & 0x0002)>>1)	;
	LCD_D4= (SWAP_LCD_DATA & 0x0001)	;
	
}
void OpenLCD(void)

{		
	Temp_LCD_DATA = LCD_DATA ;				// Save the Port Value of LCD_DATA
   
				
	LCD_E = 0 ;	
    LCD_DATA &= 0XFFF0; 
    
	LCD_DATA_TRANS(LCD_DATA);
    
    DIR_LCD_D4 =0;
    DIR_LCD_D5 =0;
    DIR_LCD_D6 =0;
    DIR_LCD_D7 =0;
    
	DIR_LCD_E = 0;							// Set E pin as output
	DIR_LCD_RS = 0 ;
	

// Initialize the LCD following the standard operations
											// 1st
	LCD_DATA &= 0XFFF0;				        // Clear PORTDbits.RD3 ~ RE5 but save others by &
	LCD_DATA |= 0x0003 ;					// Send Data of 0x03 but keep others by |
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_CMD_W_Timing() ;					// LCD Command Write Sequence Function
	LCD_L_Delay() ;							// Delay for long enough time (4.1ms minimum)
											// 2nd
	LCD_DATA &= 0XFFF0;				        // Clear PORTDbits.RD3 ~ RE5
	LCD_DATA |= 0x0003 ;					// Send Data of 0x03
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_CMD_W_Timing() ;					// LCD Command Write Sequence Function
	LCD_L_Delay() ;							// Delay for long enough time (100us minimum)
											// 3rd
	LCD_DATA &= 0XFFF0;					// Clear PORTDbits.RD3 ~ RE5
	LCD_DATA |= 0x0003 ;					// Send Data of 0x03
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_CMD_W_Timing() ;					// LCD Command Write Sequence Function
	LCD_L_Delay() ;							// Delay for long enough time (Not Required)

	LCD_DATA &= 0XFFF0;				// Clear PORTDbits.RD3 ~ RE5
	LCD_DATA |= 0x0002 ;					// Send Data of 0x02 for 4-bit data bus interface
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_CMD_W_Timing() ;
	LCD_L_Delay() ;

	WriteCmdLCD(DISP_2Line_4Bit) ;			// Configure LCD - 2 lines display & 4-bit long bus
	LCD_S_Delay() ;

	WriteCmdLCD(DISP_ON) ;					// Configure LCD - Turn on display
	LCD_S_Delay() ;

	WriteCmdLCD(ENTRY_INC) ;				// Configure LCD - Entry increment (to the right)
	LCD_S_Delay() ;

	WriteCmdLCD(CLR_DISP) ;					// Configure LCD - Clear Display
	LCD_L_Delay() ;

	LCD_DATA = Temp_LCD_DATA ;				// Restore Port Data (Useful if Port is shared, such as w/ LED)
}

//*********************************************
//     _    ______________________________
// RS  _>--<______________________________
//     _____
// RW       \_____________________________
//                  __________________
// E   ____________/                  \___
//     _____________                ______
// DB  _____________>--------------<______
//***********************************************
// Subroutine to 
// Write Command to LCD module
//
void WriteCmdLCD( unsigned char LCD_CMD) 
{

	Temp_LCD_DATA = LCD_DATA ;					// Save Port data to Temp buffer
   
	Temp_CMD = (LCD_CMD & 0xF0)>>4 ;			// Send high nibble to LCD bus
    LCD_DATA= (LCD_DATA & 0xfff0)|Temp_CMD ;
    
	LCD_DATA_TRANS(LCD_DATA);
	
	LCD_CMD_W_Timing () ;

	Temp_CMD = LCD_CMD & 0x0F ;					// Send low nibble to LCD bus
	LCD_DATA= (LCD_DATA & 0xfff0)|Temp_CMD ;
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_CMD_W_Timing () ;

	LCD_DATA = Temp_LCD_DATA ;					// Restore Port data
	LCD_S_Delay() ;								// Delay 100uS for execution

}

//***********************************************
// Subroutine to 
// Write Data to LCD module
//
void WriteDataLCD( unsigned char LCD_CMD) 
{
	
	Temp_LCD_DATA = LCD_DATA ;					// Save Port data to Temp buffer
  
	Temp_CMD = (LCD_CMD & 0xF0)>>4 ;			// Send high nibble to LCD bus
	LCD_DATA= (LCD_DATA & 0xfff0)|Temp_CMD ;
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_DAT_W_Timing () ;

	Temp_CMD = LCD_CMD & 0x0F ;					// Send low nibble to LCD bus
	LCD_DATA= (LCD_DATA & 0xfff0)|Temp_CMD ;
    
	LCD_DATA_TRANS(LCD_DATA);
	LCD_DAT_W_Timing () ;

	LCD_DATA = Temp_LCD_DATA ;					// Restore Port data
	LCD_S_Delay() ;								// Delay 100uS for execution


}

//***********************************************
// Subroutine to 
// Write a character to LCD module
//
void putcLCD(unsigned char LCD_Char)
{
	WriteDataLCD(LCD_Char) ;

}

//***********************************************
// Subroutine to 
// Write a command to LCD module
// RS=0, R/W=0, E=H->L F(alling Edge)

void LCD_CMD_W_Timing( void )					// LCD Command writing timming
{
	LCD_RS = 0;	// Set for Command Input
    _NOP();
    _NOP();
	LCD_E = 1;
    _NOP();
    _NOP();
    _NOP();
    _NOP();
	LCD_E = 0;
}

//***********************************************
// Subroutine to 
// Write a command to LCD module
// RS=1, R/W=0, E=H->L (Falling Edge)

void LCD_DAT_W_Timing( void )					// LCD Data writing timming
{
	LCD_RS = 1;	// Set for Data Input
    _NOP();
    _NOP();
	LCD_E = 1;
    _NOP();
    _NOP();
    _NOP();
    _NOP();
	LCD_E = 0;
}

//***********************************************
//     Set Cursor position on LCD module
//			CurY = Line (0 or 1)
//      	CurX = Position ( 0 to 15)
//
void setcurLCD(unsigned char CurX, unsigned char CurY)
{
	WriteCmdLCD( 0x80 + CurY * 0x40 + CurX) ;
	LCD_S_Delay() ;
}

//***********************************************
//    Put a ROM string to LCD Module
//
void putrsLCD( const char *Str )
{
	while (1)
	{
		Str_Temp = *Str ;

		if (Str_Temp != 0x00 )
		   {
			WriteDataLCD(Str_Temp) ;
			Str ++ ;
		   }
		else
			return ;
   }
}

//***********************************************
//    Put a RAM string to LCD Module
//
void putsLCD( char *Str)
{
	while (1)
	{
		Str_Temp = *Str ;

		if (Str_Temp != 0x00 )
		   {
			WriteDataLCD(Str_Temp) ;
			Str ++ ;
		   }
		else
			return ;
   }
}

//***********************************************
//    Put a byte in Hex format to LCD Module
//

void puthexLCD(unsigned char HEX_Val)
{
	unsigned char Temp_HEX ;

	Temp_HEX = (HEX_Val >> 4) & 0x0f ;		// high nibble

	if ( Temp_HEX > 9 )Temp_HEX += 0x37 ;	// A~F
    else Temp_HEX += 0x30 ;					// 0~9

	WriteDataLCD(Temp_HEX) ;

	Temp_HEX = HEX_Val  & 0x0f ;			// low nibble
	if ( Temp_HEX > 9 )Temp_HEX += 0x37 ;
    else Temp_HEX += 0x30 ;

	WriteDataLCD(Temp_HEX) ;
}

//***********************************************
//    Put a byte in decimal format to LCD Module
//	  0~99 Only. Uncomment the first 2 lines for hundreds
void	put_Num2_LCD( unsigned char The_Number )
{

	unsigned char	Temp_Char_10, Temp_Char ;

		Temp_Char_10 = The_Number /10 ;		//
		putcLCD( Temp_Char_10 + '0' ) ;			//
		Temp_Char = The_Number - Temp_Char_10 * 10;	//
		putcLCD( Temp_Char + '0' ) ;
}
void	put_Num3_LCD( unsigned char The_Number )
{

	unsigned char	Temp_Char_100, Temp_Char_10, Temp_Char ;

		Temp_Char_100 = The_Number /100 ;		//
		putcLCD( Temp_Char_100 + '0' ) ;			//
		Temp_Char_10 = (The_Number - Temp_Char_100*100)  /10 ;		//
		putcLCD( Temp_Char_10 + '0' ) ;
		Temp_Char = The_Number - ( Temp_Char_100*100 + Temp_Char_10 * 10 ) ;	//
		putcLCD( Temp_Char + '0' ) ;
}
void	put_Num4_LCD( unsigned char The_Number )
{

	unsigned char	Temp_Char_1000,Temp_Char_100, Temp_Char_10, Temp_Char ;

		Temp_Char_1000 = The_Number /1000 ;
        putcLCD( Temp_Char_1000 + '0' ) ;
        Temp_Char_100 = (The_Number - Temp_Char_1000*1000)  /100 ;
		putcLCD( Temp_Char_100 + '0' ) ;			
		Temp_Char_10 = (The_Number - (Temp_Char_1000*1000 + Temp_Char_100*100))  /10 ;		
		putcLCD( Temp_Char_10 + '0' ) ;
		Temp_Char = The_Number - ( Temp_Char_1000*1000 + Temp_Char_100*100 + Temp_Char_10 * 10 ) ;	
		putcLCD( Temp_Char + '0' ) ;
}
void	put_Num5_LCD( unsigned char The_Number )
{

	unsigned char	Temp_Char_10000, Temp_Char_1000, Temp_Char_100, Temp_Char_10, Temp_Char ;

		Temp_Char_10000 = The_Number /10000 ;
        putcLCD( Temp_Char_10000 + '0' ) ;
        Temp_Char_1000 = (The_Number - Temp_Char_10000*10000)  /1000 ;
        putcLCD( Temp_Char_1000 + '0' ) ;
        Temp_Char_100 = (The_Number - (Temp_Char_10000*10000 + Temp_Char_1000*1000))  /100 ;
		putcLCD( Temp_Char_100 + '0' ) ;			                
		Temp_Char_10 = (The_Number - (Temp_Char_10000*10000 + Temp_Char_1000*1000 + Temp_Char_100*100))  /10 ;		
		putcLCD( Temp_Char_10 + '0' ) ;
		Temp_Char = The_Number - (Temp_Char_10000*10000 + Temp_Char_1000*1000 + Temp_Char_100*100 + Temp_Char_10 * 10 ) ;	
		putcLCD( Temp_Char + '0' ) ;
}
void	put_Num6_LCD( unsigned char The_Number )
{

	unsigned char	Temp_Char_100000, Temp_Char_10000, Temp_Char_1000, Temp_Char_100, Temp_Char_10, Temp_Char ;

		Temp_Char_100000 = The_Number /100000 ;
        putcLCD( Temp_Char_100000 + '0' ) ;
        Temp_Char_10000 = (The_Number - Temp_Char_100000*100000)  /10000 ;
        putcLCD( Temp_Char_10000 + '0' ) ;
        Temp_Char_1000 = (The_Number - (Temp_Char_100000*100000 + Temp_Char_10000*10000))  /1000 ;
        putcLCD( Temp_Char_1000 + '0' ) ;
        Temp_Char_100 = (The_Number - (Temp_Char_100000*100000 + Temp_Char_10000*10000 + Temp_Char_1000*1000))  /100 ;
		putcLCD( Temp_Char_100 + '0' ) ;			                
		Temp_Char_10 = (The_Number - (Temp_Char_100000*100000 + Temp_Char_10000*10000 + Temp_Char_1000*1000 + Temp_Char_100*100))  /10 ;		
		putcLCD( Temp_Char_10 + '0' ) ;
		Temp_Char = The_Number - (Temp_Char_100000*100000 + Temp_Char_10000*10000 + Temp_Char_1000*1000 + Temp_Char_100*100 + Temp_Char_10 * 10 ) ;	
		putcLCD( Temp_Char + '0' ) ;
}
// **********************************************
// Delay for atleast 10 ms 
// **********************************************
void LCD_L_Delay(void)
{
	int		L_Loop ;

	for 	( L_Loop = 0 ; L_Loop < 100 ; L_Loop ++ )
				LCD_S_Delay( ) ;		
}

// ***********************************************
// Delay for 100 us
// ***********************************************
void LCD_S_Delay(void)
{
	int		S_Loop ;
	int		Null_Var ;

	for  	( S_Loop = 0 ; S_Loop < 200 ; S_Loop ++ )
				Null_Var += 1 ;
		
}
