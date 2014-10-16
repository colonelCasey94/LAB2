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
		 BKBUG_ON & COE_ON & ICS_PGx1 &
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
volatile int clock_time;

// ******************************************************************************************* //

int main(void)
{

        char key;
        int state = 0;

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



        TMR1 = 0;
        // Set Timer 1's period value regsiter to value for 1s.
        //14745600/256 = 57600
        //1 s * 57600 = 57600
        PR1 = 57600;
        // Setup Timer 1 control register (T1CON) to:
 	//     TON           = 0     (start timer)
	//     TCKPS1:TCKPS2 = 11    (set timer prescaler to 1:256)
	//     TCS           = 0     (Fosc/2)
	T1CON = 0x8030;
        IFS0bits.T1IF = 0;
        IEC0bits.T1IE = 1;



	LCDInitialize();
	KeypadInitialize();

	// TODO: Initialize scanKeypad variable.
        scanKeypad = 0;

        char password[100][4];
        char entered_pass[6];
        int pass_position = 0;
        int mem_position = 0;
        int time = 0;
        int program = 0;

        password[0][0] = '1';  //default password
        password[0][1] = '2';
        password[0][2] = '3';
        password[0][3] = '4';

        entered_pass[0] = NULL;
        entered_pass[1] = NULL;
        entered_pass[2] = NULL;
        entered_pass[3] = NULL;
        entered_pass[4] = NULL;

        clock_time = 0;

        printf("working\n)");


	while(1)
	{
		// TODO: Once you create the correct keypad driver (Part 1 of the lab assignment), write
		// the C program that use both the keypad and LCD drivers to implement the 4-digit password system.

            switch(state){
                    case 0:
                        printf("State 0\n");
                        LCDMoveCursor(0,0);
                        LCDPrintString("Enter");

                        if(scanKeypad == 1){
                            state = 1;
                        }

                        if(entered_pass[0] == '*' && entered_pass[1] == '*'){ // program mode   state 4
                            state = 4;
                            entered_pass[0] = NULL; // clear password
                            entered_pass[1] = NULL;
                            entered_pass[2] = NULL;
                            entered_pass[3] = NULL;
                            entered_pass[4] = NULL;

                        }
                        else if(entered_pass[0] != '*' && entered_pass[1] == '*'){ //bad mode
                            state = 2;
                        }
                        else if(entered_pass[0] == '*' && entered_pass[1] != '*'){ // bad mode
                            state = 2;
                        }
                        else if(entered_pass[0] == '#' || entered_pass[1] == '#' || entered_pass[2] == '#' || entered_pass[3] == '#' || entered_pass[2] == '*' || entered_pass[3] == '*'){ // bad mode
                            state = 2;
                        }
                        else if(entered_pass[3] != 'NULL'){ // check password  determine if good or bad
                            while(password[mem_position][0] != 'NULL'){
                                if(password[mem_position][0] == entered_pass[0] && password[mem_position][1] == entered_pass[1] && password[mem_position][2] == entered_pass[2] && password[mem_position][3] == entered_pass[3]){
                                    state = 3;
                                }
                                     mem_position++; // increase memory location for the next char entered
                            }
                            if(state != 3){ // if the password was not valid
                                state = 2;
                            }

                            mem_position = 0; //rest position in memory

                            entered_pass[0] = NULL; // clear password
                            entered_pass[1] = NULL;
                            entered_pass[2] = NULL;
                            entered_pass[3] = NULL;
                            entered_pass[4] = NULL;
                        }
                        break;

//                    case 1:
//                        printf("State 1\n");
//                        key = KeypadScan();  // scan for the key being pressed
//                        if(key != -1){ // if not error
//                            LCDMoveCursor(1,(pass_position)); // set lcd position for the correct digit
//                            LCDPrintChar(key);
//                            entered_pass[pass_position] = key; // save entered password into the memory to check later
//                            pass_position++;
//                            if(pass_position == 4){
//                                pass_position = 0;
//                            }
//                        }
//
//                        scanKeypad = 0; // set low for next interrupt
//                        if(program == 0){
//                            state = 0; // go back to state 0
//                        }
//                        else if(program == 1){
//                            state = 4; // go back to the program state (state 4)
//                        }
//
//                        break;

//                     case 2: // bad state
//                         printf("State 2\n");
//                         LCDClear();
//                         LCDMoveCursor(0,0);
//                         LCDPrintString("Bad");
//
//                         clock_time = 0;
//
//                         while(time < 2){
//                             time = clock_time;
//                         }
//                         state = 0;
//                         time = 0;
//                         break;
//
//                    case 3:  // good state
//                        printf("State 3\n");
//                        LCDClear();
//                        LCDMoveCursor(0,0);
//                        LCDPrintString("Good :)");
//
//                        clock_time = 0;
//
//                        while(time < 2){
//                            time = clock_time;
//                        }
//                        state = 0;
//                        time = 0;
//                        break;
//
//                    case 4: // program state
//                        printf("State 4\n");
//                        program = 1;
//                        if(scanKeypad == 1){
//                            state = 1;
//                        }
//
//                        if(entered_pass[4] == '#'){ // enter new password into the password memory array
//                            if(entered_pass[0] != '*' && entered_pass[1] != '*' && entered_pass[2] != '*' && entered_pass[3] != '*' && entered_pass[0] != '#' && entered_pass[1] != '#' && entered_pass[2] != '#' && entered_pass[3] != '#'){
//                                while(password[mem_position][0] != 'NULL'){
//                                    mem_position++;
//                                 }
//
//                                password[mem_position][0] = entered_pass[0];
//                                password[mem_position][1] = entered_pass[1];
//                                password[mem_position][2] = entered_pass[2];
//                                password[mem_position][3] = entered_pass[3];
//
//                                mem_position = 0;
//                                program = 0;
//                                state = 5;
//                            }
//                            else
//                                state = 6;
//                        }
//
//                        break;
//                    case 5:
//                        printf("State 5\n");
//                        LCDClear();
//                        LCDMoveCursor(0,0);
//                        LCDPrintString("Valid :)");
//
//                        clock_time = 0;
//
//                        while(time < 2){
//                            time = clock_time;
//                        }
//                        state = 0;
//                        time = 0;
//
//                        break;
//
//                     case 6:
//                         printf("State 6\n");
//                        LCDClear();
//                        LCDMoveCursor(0,0);
//                        LCDPrintString("Invalid");
//
//                        clock_time = 0;
//
//                        while(time < 2){
//                            time = clock_time;
//                        }
//                        state = 0;
//                        time = 0;
//
//                        break;
//
//                     default:
//                         LCDClear();
//                         LCDMoveCursor(0,0);
//                         LCDPrintString("Error");
//
//                         break;
//            }

//		if( scanKeypad == 1 ) {
////                    printf("%c   %c", key, KeypadScan());
//			key = KeypadScan();
//
//			if( key != -1 ) {
//                            printf("%c\n", key);
//			}
//			scanKeypad = 0;
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
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
{
	// Clear Timer 1 interrupt flag to allow another Timer 1 interrupt to occur.
	IFS0bits.T1IF = 0;

        clock_time = clock_time + 1;


}