// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"
#include "keypad.h"

#define XTFREQ          7372800         	  // On-board Crystal frequency
#define PLLMODE         4               	  // On-chip PLL setting (Fosc)
#define FCY             (XTFREQ*PLLMODE)/2    // Instruction Cycle Frequency (Fosc/2)

#define BAUDRATE         115200
#define BRGVAL          ((FCY/BAUDRATE)/16)-1
// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
		 BKBUG_ON & COE_OFF & ICS_PGx1 &
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //

// Varible used to indicate that the current configuration of the keypad has been changed,
// and the KeypadScan() function needs to be called.

volatile char scanKeypad;
volatile int clock_time=0;
enum state_t {ENTER, PRINT, CHECK, GOOD, BAD, PROGRAM, VALID, INVALID, PRINT2  } state;
// ******************************************************************************************* //

int main(void)
{

        char key;


	// TODO: Initialize and configure IOs, LCD (using your code from Lab 1),
	// UART (if desired for debugging), and any other configurations that are needed.

        RPINR18bits.U1RXR = 9;
	RPOR4bits.RP8R = 3;
        U1BRG  = BRGVAL;
	U1MODE = 0x8000;
        U1STA  = 0x0440;
	IFS0bits.U1RXIF = 0;

        IFS1bits.CNIF = 0;
        IEC1bits.CNIE = 1;



        TMR4 = 0; TMR5 = 0;
        // Set Timer 1's period value regsiter to value for 1s.
        //14745600/8 = 1843200
        //1 s * 57600 = 1843200
        PR4 = 0x4000;
        PR5 = 0x38;
        // Setup Timer 4: Timer 5 in 32 but mode control register (T4CON) to:
 	//     TON           = 0     (start timer)
	//     TCKPS1:TCKPS2 = 01    (set timer prescaler to 1:8)
	//     TCS           = 0     (Fosc/2)
        T4CONbits.T32 = 1;
        T4CONbits.TCKPS0 = 1;
        T4CONbits.TCKPS1 = 0;
        T4CONbits.TCS = 0;

        IFS1bits.T5IF = 0;



	LCDInitialize();
	KeypadInitialize();

	// TODO: Initialize scanKeypad variable.
        scanKeypad = 0;

        char password[100][4];
        char entered_pass[5];
        int pass_position = 0;
        int pass_iterator = 0;
        int mem_position = 0;
        int time = 0;
        int program = 0;

        password[0][0] = '1';  //default password
        password[0][1] = '2';
        password[0][2] = '3';
        password[0][3] = '4';

        for(mem_position = 1; mem_position < 100; mem_position++){
            for(pass_iterator = 0; pass_iterator < 4; pass_iterator++){
                password[mem_position][pass_iterator] = NULL;
            }
        }

        entered_pass[0] = NULL;
        entered_pass[1] = NULL;
        entered_pass[2] = NULL;
        entered_pass[3] = NULL;
        entered_pass[4] = NULL;

        clock_time = 0;

        printf("working\n)");

        state = ENTER;
	while(1)
	{
		// TODO: Once you create the correct keypad driver (Part 1 of the lab assignment), write
		// the C program that use both the keypad and LCD drivers to implement the 4-digit password system.

            switch(state){
                case ENTER:

                    LCDMoveCursor(0,0);
                    LCDPrintString("ENTER");

                    if (scanKeypad == 1){
                        scanKeypad = 0;
                        state = PRINT;
                    }
                    break;
                case PRINT:

                    key = KeypadScan();

                    if (key == '#'){
                        state = BAD;
                    } else if (key == '*' && pass_position > 0 && entered_pass[0] != '*'){
                        state = BAD;
                    } else if (key == '*' && pass_position > 0 && entered_pass[0] == '*') {
                        state = PROGRAM;

                        entered_pass[0] = NULL;
                        entered_pass[1] = NULL;
                        entered_pass[2] = NULL;
                        entered_pass[3] = NULL;
                        entered_pass[4] = NULL;
                        pass_position = 0;

                    } else if ( key != -1 && entered_pass[0] == '*' ) {
                        state = BAD;
                    } else if ( key != -1){
                        entered_pass[pass_position] = key;
                        pass_position++;
                        if(pass_position == 4){
                            state = CHECK;
                        } else {
                            state = ENTER;
                        }
                    }
                    LCDMoveCursor(1,0);
                    for(pass_iterator = 0; entered_pass[pass_iterator] != NULL && pass_iterator <4; pass_iterator++){
                        LCDPrintChar(entered_pass[pass_iterator]);
                    }

                    break;
                case BAD:
                    LCDClear();
                    LCDMoveCursor(0,0);
                    LCDPrintString("BAD");
                    TMR4 = 0;
                    TMR5 = 0;
                    T4CONbits.TON = 1;
//
                    while(IFS1bits.T5IF == 0);
                    IFS1bits.T5IF = 0;
                    T4CONbits.TON = 0;

                    entered_pass[0] = NULL;
                    entered_pass[1] = NULL;
                    entered_pass[2] = NULL;
                    entered_pass[3] = NULL;
                    entered_pass[4] = NULL;
                    pass_position = 0;

                    state = ENTER;
                    break;
                case GOOD:
                    LCDClear();
                    LCDMoveCursor(0,0);
                    LCDPrintString("Good");
                    TMR4 = 0;
                    TMR5 = 0;
                     T4CONbits.TON = 1;
//
                    while(IFS1bits.T5IF == 0);
                    IFS1bits.T5IF = 0;
                    T4CONbits.TON = 0;

                    entered_pass[0] = NULL;
                    entered_pass[1] = NULL;
                    entered_pass[2] = NULL;
                    entered_pass[3] = NULL;
                    entered_pass[4] = NULL;
                    pass_position = 0;

                    state = ENTER;
                    break;
                case CHECK:
                    state = BAD;
                    for (mem_position = 0; mem_position < 100; mem_position++){
                        if (password[mem_position][0] == entered_pass[0] && password[mem_position][1] == entered_pass[1] && password[mem_position][2] == entered_pass[2] && password[mem_position][3] == entered_pass[3] ){
                            state = GOOD;
                        }
                    }
                    break;
                case PROGRAM:
                    LCDClear();
                    LCDMoveCursor(0,0);
                    LCDPrintString("Set Mode");
                    program = 1;
                    if(scanKeypad == 1){
                        state = PRINT2;
                    }

                    if(entered_pass[4] != NULL){
                        if(entered_pass[4] != '#'){
                            state = INVALID;
                        }
                        else if(entered_pass[0] == '*' || entered_pass[1] == '*' || entered_pass[2] == '*' || entered_pass[3] == '*'){
                            state = INVALID;
                        }
                        else if(entered_pass[0] == '#' || entered_pass[1] == '#' || entered_pass[2] == '#' || entered_pass[3] == '#'){
                            state = INVALID;
                        }
                        else{
                            state = VALID;
                            mem_position = 0;
                            while(password[mem_position][0] != NULL){
                                mem_position++;
                            }
                            password[mem_position][0] = entered_pass[0];
                            password[mem_position][1] = entered_pass[1];
                            password[mem_position][2] = entered_pass[2];
                            password[mem_position][3] = entered_pass[3];
                            entered_pass[0] = NULL;
                            entered_pass[1] = NULL;
                            entered_pass[2] = NULL;
                            entered_pass[3] = NULL;
                            entered_pass[4] = NULL;
                            mem_position = 0;
                        }
                    }
                    break;
                case PRINT2:
                    key = KeypadScan();
                    if ( key != -1){
                        entered_pass[pass_position] = key;
                        pass_position++;
                        if(pass_position == 5){
                            pass_position = 0;
                        }
                        state = PROGRAM;
                    }
                    LCDMoveCursor(1,0);
                    for(pass_iterator = 0; entered_pass[pass_iterator] != NULL && pass_iterator <4; pass_iterator++){
                        LCDPrintChar(entered_pass[pass_iterator]);
                    }
                    break;
                case VALID:
                    LCDClear();
                    LCDMoveCursor(0,0);
                    LCDPrintString("Valid");
                    TMR4 = 0;
                    TMR5 = 0;
                    T4CONbits.TON = 1;
//
                    while(IFS1bits.T5IF == 0);
                    IFS1bits.T5IF = 0;
                    T4CONbits.TON = 0;

                    entered_pass[0] = NULL;
                    entered_pass[1] = NULL;
                    entered_pass[2] = NULL;
                    entered_pass[3] = NULL;
                    entered_pass[4] = NULL;
                    pass_position = 0;

                    state = ENTER;
                    break;
                case INVALID:
                    LCDClear();
                    LCDMoveCursor(0,0);
                    LCDPrintString("Invalid");
                    TMR4 = 0;
                    TMR5 = 0;
                    T4CONbits.TON = 1;
//
                    while(IFS1bits.T5IF == 0);
                    IFS1bits.T5IF = 0;
                    T4CONbits.TON = 0;

                    entered_pass[0] = NULL;
                    entered_pass[1] = NULL;
                    entered_pass[2] = NULL;
                    entered_pass[3] = NULL;
                    entered_pass[4] = NULL;
                    pass_position = 0;

                    state = ENTER;
                    break;
		}
	}
	return 0;
}

// ******************************************************************************************* //
// Defines an interrupt service routine that will execute whenever any enable
// input change notifcation is detected.
//
//     In place of _ISR and _ISRFAST, we can directy use __attribute__((interrupt))
//     to inform the compiler that this function is an interrupt.
//
//     _CNInterrupt is a macro for specifying the interrupt for input change
//     notification.
//
// The functionality defined in an interrupt should be a minimal as possible
// to ensure additional interrupts can be processed.
void __attribute__((interrupt)) _CNInterrupt(void)
{
	// TODO: Clear interrupt flag
	IFS1bits.CNIF = 0;

	// TODO: Detect if *any* key of the keypad is *pressed*, and update scanKeypad
//	 variable to indicate keypad scanning process must be executed.
       if( PORTAbits.RA0 == 0 || PORTAbits.RA1 == 0 || PORTBbits.RB5 == 0 ){
           scanKeypad = 1;
       }
}

// ******************************************************************************************* //
//void __attribute__((interrupt)) _T5nterrupt(void)
//{
//	// Clear Timer 1 interrupt flag to allow another Timer 1 interrupt to occur.
//	IFS1bits.T5IF = 0;
//
//   //     clock_time = clock_time + 1;
//
//        clock_time = 1;
//}