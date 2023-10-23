/*
File: main.c
Author: ozakaria
Main file to control LCD Display Driver on Explorer 16/32 board
Due on: September 22nd, 2023
*/

#include <xc.h>
#include "lcd_display_driver.h" // header file
#include <sys/attribs.h> // allows interrupts
char D1[3] = {'K','H','A'}; // array of chars for line 1, stays the same
char D2[3] = {'A','A','A'}; // array of chars for line 2, changed with push buttons

void __ISR(_CHANGE_NOTICE_VECTOR, IPL4SOFT)CNISR(void){  // change notifications interrups for push buttons
    if(PORTDbits.RD7 == 0){ // reads s6, responsible for first alphabet second line
        if(D2[0] == 'Z'){ // if loop, loops around alphabet
		D2[0] = 'A' - 1; // - 1 because after Z goes straight to A
        }
        D2[0]++; //increments array D2
        display_driver_use_second_line(); 
        lcd_display_driver_write(D2, 3); 
    } 
    else if(PORTDbits.RD13 == 0){ // reads s4, responsible for third alphabet second line
        //LATAbits.LATA1 = 1;
        if(D2[2] == 'Z'){ // if loop, loops around alphabet
		D2[2] = 'A' - 1; // - 1 because after Z goes straight to A
        }
        D2[2]++; //increments array D2
        display_driver_use_second_line(); 
        lcd_display_driver_write(D2, 3); 
    } 
    else if(PORTDbits.RD6 == 0){ // reads s3, resets second line to AAA
        //LATAbits.LATA2 = 1; 
        D2[0] = D2[1] = D2[2] = 'A'; // could us 65 instead of A
        display_driver_use_second_line(); 
        lcd_display_driver_write(D2, 3); 
    } 
    IFS1bits.CNIF = 0; // clear flag
}

void __ISR(_EXTERNAL_2_VECTOR, IPL2SOFT) EXT2ISR (void ){ // external interrupt (implement the jumper)
    //when button pressed => increment D2[1]++ 
    //if D2[1] == 'Z' reset D2[1]='A
    if(D2[1] == 'Z'){ // if loop, loops around alphabet
        D2[1] = 'A' - 1; // - 1 because after Z goes straight to A
    }
    D2[1]++; // increment D2 second letter
    display_driver_use_second_line(); 
    lcd_display_driver_write(D2, 3);
    IFS0bits.INT2IF = 0; // clear flag
}


main(void){


    INTCONbits.MVEC = 1; // multivector mode
     
    __builtin_disable_interrupts();
 
    CNCONbits.ON = 1; // enable change notification
    IPC6bits.CNIP = 4; // set priority
    IFS1bits.CNIF = 0; // set flag
    IEC1bits.CNIE = 1; // enable interrupt
    CNENbits.CNEN19 = 1; // enable change notif for push buttons
    CNENbits.CNEN16 = 1; // enable change notif for push buttons
    CNENbits.CNEN15 = 1;// enable change notif for push buttons
    
    
    INTCONbits.INT2EP = 1; // falling edge
    IPC2bits.INT2IP = 2; // set priority
    IFS0bits.INT2IF = 0; //set flag
    IEC0bits.INT2IE = 1; // enable interrupt

    __builtin_enable_interrupts();

    //lab2
    DDPCONbits.JTAGEN = 0; // enable LEDs
    
    // set tristate special register for different pins
    TRISDbits.TRISD4 = 0; 
    TRISDbits.TRISD5 = 0; 
    TRISBbits.TRISB15 = 0; 
    TRISE = 0xff00; 
    TRISA = 0xff80;
    TRISD = 0xFFCF;

     
    lcd_display_driver_initialize(); // call initialization function
    lcd_display_driver_clear(); // call clear function
    display_driver_use_first_line(); // set display to first line
    lcd_display_driver_write(D1, 3); // writes 'KHA' in line 1
    display_driver_use_second_line(); // set display to second line
    lcd_display_driver_write(D2, 3); // writes 'AAA' in line 2

    while(1){ 
        if(D2[0] == 'K' && D2[1] == 'H' && D2[2] == 'A'){  // Turns on LEDS if first line matches second line
            LATAbits.LATA1 = 1; // turn on LED 4 
            LATAbits.LATA2 = 1; // turn on LED 5
            LATAbits.LATA3 = 1; // turn on LED 6 
            LATAbits.LATA4 = 1; // turn on LED 7 
        } 
        else{ // Turns off LEDS if first line doesnt matches second line
            LATA = 0x0; // turn off all LEDS (3-8)
        }
    }

}

