// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"
#include "lcd.h"

// ******************************************************************************************* //

// set up conections for inverse multiplexing
#define ROW1 LATBbits.LATB11
#define ROW2 LATBbits.LATB10
#define ROW3 LATBbits.LATB2
#define ROW4 LATBbits.LATB3

#define COL1 PORTAbits.RA0
#define COL2 PORTAbits.RA1
#define COL3 PORTBbits.RB5

// TRIS Register mapping for the keypad signals
#define TRIS_ROW1 TRISBbits.TRISB11
#define TRIS_ROW2 TRISBbits.TRISB10
#define TRIS_ROW3 TRISBbits.TRISB2
#define TRIS_ROW4 TRISBbits.TRISB3

#define TRIS_COL1 TRISAbits.TRISA0
#define TRIS_COL2 TRISAbits.TRISA1
#define TRIS_COL3 TRISBbits.TRISB5


#define HIGH 1
#define LOW 0


//******************************************************************************************** //


void KeypadInitialize() {

	// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This
	// configuration should ensure that if any key is pressed, a change notification interrupt
	// will be generated.
//  outputs
    TRIS_ROW1 = 0;
    TRIS_ROW2 = 0;
    TRIS_ROW3 = 0;
    TRIS_ROW4 = 0;

//  inputs
    TRIS_COL3 = 1;
    TRIS_COL2 = 1;
    TRIS_COL1 = 1;

//  input set to digital
    AD1PCFGbits.PCFG0 = 1;
    AD1PCFGbits.PCFG1 = 1;
//  input pull up resistors
    CNPU1bits.CN2PUE = 1;
    CNPU1bits.CN3PUE = 1;
    CNPU2bits.CN27PUE = 1;
// configure open drain
    ODCBbits.ODB10 = 1;
    ODCBbits.ODB11 = 1;
    ODCBbits.ODB2 = 1;
    ODCBbits.ODB3 = 1;
//  input change notification
    CNEN1bits.CN2IE = 1;
    CNEN1bits.CN3IE = 1;
    CNEN2bits.CN27IE = 1;
//  turn on change notifications after setting flag to low
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;


        ROW1 = LOW;
        ROW2 = LOW;
        ROW3 = LOW;
        ROW4 = LOW;
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
        int count = 0;



        ROW1 = LOW;
        ROW2 = HIGH;
        ROW3 = HIGH;
        ROW4 = HIGH;
        DelayUs(250);
        if (COL1 == LOW) {key = '1'; count++; }
        if (COL2 == LOW) {key = '2'; count++; }
        if (COL3 == LOW) {key = '3'; count++; }
//        printf("state 0             %c\n", key);



        ROW1 = HIGH;
        ROW2 = LOW;
        ROW3 = HIGH;
        ROW4 = HIGH;
        DelayUs(250);
        if (COL1 == LOW) {key = '4'; count++; }
        if (COL2 == LOW) {key = '5'; count++; }
        if (COL3 == LOW) {key = '6'; count++; }
//        printf("state 1                 %c\n", key);



        ROW1 = HIGH;
        ROW2 = HIGH;
        ROW3 = LOW;
        ROW4 = HIGH;
        DelayUs(250);
        if (COL1 == LOW) {key = '7'; count++; }
        if (COL2 == LOW) {key = '8'; count++; }
        if (COL3 == LOW) {key = '9'; count++; }
//        printf("state 2                     %c\n", key);



        ROW1 = HIGH;
        ROW2 = HIGH;
        ROW3 = HIGH;
        ROW4 = LOW;
        DelayUs(250);
        if (COL1 == LOW) {key = '*'; count++; }
        if (COL2 == LOW) {key = '0'; count++; }
        if (COL3 == LOW) {key = '#'; count++; }
//        printf("state 3                         %c\n", key);



        ROW1 = LOW;
        ROW2 = LOW;
        ROW3 = LOW;
        ROW4 = LOW;

   while ( COL1 == LOW || COL2 == LOW || COL3 == LOW);
        if(COL1 == HIGH && COL2 == HIGH && COL3 == HIGH){
            if (count == 1){
                return key;
            }
            else {
                return -1;
            }
        }

        //return key;
}

// ******************************************************************************************* //

