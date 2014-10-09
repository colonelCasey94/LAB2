// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"

// ******************************************************************************************* //

// set up conections for inverse multiplexing
#define ROW1 LATBbits.LATB0
#define ROW2 LATBbits.LATB1
#define ROW3 LATBbits.LATB2
#define ROW4 LATBbits.LATB3

#define COL1 PORTAbits.RA0
#define COL2 PORTAbits.RA1
#define COL3 PORTAbits.RA4

// TRIS Register mapping for the keypad signals
#define TRIS_ROW1 TRISBbits.TRISB0
#define TRIS_ROW2 TRISBbits.TRISB1
#define TRIS_ROW3 TRISBbits.TRISB2
#define TRIS_ROW4 TRISBbits.TRISB3

#define TRIS_COL1 TRISAbits.TRISA0
#define TRIS_COL2 TRISAbits.TRISA1
#define TRIS_COL3 TRISAbits.TRISA4


//******************************************************************************************** //
void KeypadInitialize() {
	
	// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This 
	// configuration should ensure that if any key is pressed, a change notification interrupt 
	// will be generated.

    TRIS_ROW1 = 0; TRIS_COL1 = 1;
    TRIS_ROW2 = 0; TRIS_COL2 = 1;
    TRIS_ROW3 = 0; TRIS_COL3 = 1;
    TRIS_ROW4 = 0;

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    //set cn2,cn3 and cn0 pullup resister off and the interupt on
    CNPU1 = 0x0000;
    
    CNEN1 = 0x000D;

}

// ******************************************************************************************* //

char KeypadScan() {
	char key = -1;
	
	
	// TODO: Implement the keypad scanning procedure to detect if exactly one button of the 
	// keypad is pressed. The function should return:
	//
	//      -1         : Return -1 if no keys are pressed.
	//      '0' - '9'  : Return the ASCII character '0' to '9' if one of the 
	//                   numeric (0 - 9) keys are pressed.
	//      '#'        : Return the ASCII character '#' if the # key is pressed. 
	//      '*'        : Return the ASCII character '*' if the * key is pressed. 
	//       -1        : Return -1 if more than one key is pressed simultaneously.
	// Notes: 
	//        1. Only valid inputs should be allowed by the user such that all invalid inputs 
	//           are ignored until a valid input is detected.
	//        2. The user must release all keys of the keypad before the following key press
	//           is processed. This is to prevent invalid keypress from being processed if the 
	//           users presses multiple keys simultaneously.
	//
	return key;
}

// ******************************************************************************************* //
