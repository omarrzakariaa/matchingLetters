/*
File: lcd_display_driver.h
Author: ozakaria
Header file for functions used to control LCD Display Driver on Explorer 16/32 board
Due on: September 22nd, 2023
 */
#define LCD_DISPLAY_DRIVER_H 

void lcd_display_driver_enable();
void lcd_display_driver_initialize();
void lcd_display_driver_clear();
void lcd_display_driver_write(char* data, int length);
void display_driver_use_first_line();
void display_driver_use_second_line();