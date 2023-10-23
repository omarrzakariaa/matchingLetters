/*
File: lcd_display_driver.c
Author: ozakaria
Source file for functions used to control LCD Display Driver on Explorer 16/32 board
Due on: September 22nd, 2023
 */
#include <xc.h>
#include "lcd_display_driver.h" // call header file

void lcd_display_driver_enable(){ // enable function, called after each function
    LATDbits.LATD4 = 1; // set port D to high
    int t = 0; // define time variable
    for(t=0; t<1000; t++){ // wait time to allow for enable
    }
    
    LATDbits.LATD4 = 0; // set port D to low
    for(t=0; t<1000; t++){ // wait time for enable function to come down
    }
}

void lcd_display_driver_initialize() { // initialization function, following guideline in datasheet
    LATBbits.LATB15 = 0; // set RS
    LATDbits.LATD5 = 0;
    int t = 0; // define time variable
    for(t=0; t<1000; t++){
    }
    
    LATE = 0b00111000; // function set
    lcd_display_driver_enable();
    for(t=0; t<1000; t++){ // shouldn't need those wait times since they are already in enable function
    } // using them just in case
    LATE = 0b00001100; // display on/off control
    lcd_display_driver_enable();
    for(t=0; t<1000; t++){
    }
    lcd_display_driver_clear(); // clear display
    for(t=0; t<1000; t++){
    }
    LATE = 0b00000110; // entry mode set
    lcd_display_driver_enable();
    for(t=0; t<1000; t++){
    }
}

void lcd_display_driver_clear(){ // clear function
    LATBbits.LATB15 = 0; // set RS
    LATDbits.LATD5 = 0; // set R/W

    LATE = 0b00000001; // Novatek page 24
    lcd_display_driver_enable();
}

void lcd_display_driver_write(char* data, int length){ // write function for array of chars
    
    int x = 0;
    for(x=0; x<length; x++){
        LATBbits.LATB15 = 1; // set RS
        LATDbits.LATD5 = 0;n // set R/W
        LATE = data[x];
        lcd_display_driver_enable();
    }
}

void display_driver_use_first_line(){ // writes in first line
    LATBbits.LATB15 = 0; // set RS
    LATDbits.LATD5 = 0; // set R/W
    LATE = 0b10000000; // set to first line
    lcd_display_driver_enable();
}

void display_driver_use_second_line(){ // writes in second line
    LATBbits.LATB15 = 0; // set RS
    LATDbits.LATD5 = 0; // set R/W
    LATE = 0b11000000; // set to second line
    lcd_display_driver_enable();
}