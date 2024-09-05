// Objective: To write an embedded c program for 8051 microcontroller to interface keypad and 16x2 LCD display with edsim51.
#include <reg51.h>

#define LCD_DATA P1       // LCD data port connected to P1.0 - P1.7
sbit RS = P2^3;           // Register select pin connected to P2.3
sbit EN = P2^2;           // Enable pin connected to P2.2
// RW is grounded, so no need to define it in code

void delay(unsigned int ms);
void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char dat);
void lcd_string(char *str);
unsigned char keypad_scan(void);

void main(void) {
    unsigned char key;
    
    lcd_init();
    lcd_string("Key Pressed:");
    
    while (1) {
        key = keypad_scan();
        if (key) {
            lcd_cmd(0xC0); // Move cursor to the second line
            lcd_data(key);
        }
    }
}

void delay(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1275; j++);
}

void lcd_init(void) {
    lcd_cmd(0x38);   // Function set: 8-bit, 2 lines, 5x7 matrix
    lcd_cmd(0x0C);   // Display on, cursor off
    lcd_cmd(0x06);   // Entry mode: Auto increment cursor
    lcd_cmd(0x01);   // Clear display
    delay(2);
}

void lcd_cmd(unsigned char cmd) {
    LCD_DATA = cmd;   // Send command to data pins
    RS = 0;           // Select command register
    EN = 1;           // Generate enable pulse
    delay(1);
    EN = 0;
    delay(2);         // Wait for the command to execute
}

void lcd_data(unsigned char dat) {
    LCD_DATA = dat;  // Send data to data pins
    RS = 1;           // Select data register
    EN = 1;           // Generate enable pulse
    delay(1);
    EN = 0;
    delay(2);         // Wait for the data to be written
}

void lcd_string(char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

unsigned char keypad_scan(void) {
    unsigned char row, col, key;

    for (row = 0; row < 4; row++) {
        P0 = ~(0x01 << row);  // Set the current row low and others high
        
        for (col = 0; col < 3; col++) {
            if (!(P0 & (0x10 << col))) {  // Check if the key is pressed
                while (!(P0 & (0x10 << col)));  // Wait until key is released
                
                key = row * 3 + col + 1;
                if (key > 9) key += 'A' - 10;
                else key += '0';
                return key;
            }
        }
    }
    return 0;  // Return 0 if no key is pressed
}
