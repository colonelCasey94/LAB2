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

volatile int col = 0;

void KeypadInitialize() {
	
	// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This 
	// configuration should ensure that if any key is pressed, a change notification interrupt 
	// will be generated.

    TRIS_ROW1 = 0; TRIS_COL1 = 1;
    TRIS_ROW2 = 0; TRIS_COL2 = 1;
    TRIS_ROW3 = 0; TRIS_COL3 = 1;
    TRIS_ROW4 = 0;

 
    //set cn2,cn3 and cn0 pullup resister off and the interupt on
    //set cn4,cn5,cn6,cn7 pullup resister on
    CNPU1 = 0x00F0;
    
    CNEN1 = 0x000D;

    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;

}

// ******************************************************************************************* //

char KeypadScan() {

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

	char key = -1;

        int row = 1;

        while ( row <= 5){
            if (row == 5){
                ROW1 = 1;
                ROW2 = 1;
                ROW3 = 1;
                ROW4 = 1;
                if (COL1&&COL2){
                    col = 0;
                } else if (COL1&&COL3){
                    col = 0;
                } else if (COL2&&COL3){
                    col = 0;
                }
                while( COL1&&COL2&&COL3);
            } else if (row == 1){
                ROW2 = 0;
                ROW3 = 0;
                ROW4 = 0;
                ROW1 = 1;
                  if (COL1&&!COL2&&!COL3){
                    col = 1;
                } else if (!COL1&&COL2&&!COL3) {
                    col = 2;
                } else if (!COL1&&!COL2&&COL3) {
                    col = 3;
                } else {
                    col = 0;
                }
            } else if (row == 2){
                ROW1 = 0;
                ROW3 = 0;
                ROW4 = 0;
                ROW2 = 1;
                if (COL1&&!COL2&&!COL3){
                    col = 1;
                } else if (!COL1&&COL2&&!COL3) {
                    col = 2;
                } else if (!COL1&&!COL2&&COL3) {
                    col = 3;
                } else {
                    col = 0;
                }
            } else if (row == 3){
                ROW1 = 0;
                ROW2 = 0;
                ROW4 = 0;
                ROW3 = 1;
                if (COL1&&!COL2&&!COL3){
                    col = 1;
                } else if (!COL1&&COL2&&!COL3) {
                    col = 2;
                } else if (!COL1&&!COL2&&COL3) {
                    col = 3;
                } else {
                    col = 0;
                }
            } else if (row == 4) {
                ROW1 = 0;
                ROW2 = 0;
                ROW3 = 0;
                ROW4 = 1;
                if (COL1&&!COL2&&!COL3){
                    col = 1;
                } else if (!COL1&&COL2&&!COL3) {
                    col = 2;
                } else if (!COL1&&!COL2&&COL3) {
                    col = 3;
                } else {
                    col = 0;
                }
            }
        }

        ROW1 = 0;
        ROW2 = 0;
        ROW3 = 0;
        ROW4 = 0;

        if (col != 0){
            if (row = 4){
                if (col = 1){
                    key = '*';
                }else if (col == 2){
                    key = '0';
                } else if (col == 3){
                    key = '#';
                } else {
                    return -1;
                }
            } else {
                key = '1'+col-1+(row*3);
            }
        }
	
	

       	return key;
}

// ******************************************************************************************* //