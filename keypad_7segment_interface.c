// Objective: To write an embedded c code for 8051 microcontroller to interface keypad and seven segment displays with edsim51.
#include <reg51.h>

// Define control pins for the decoder
sbit CS = P0^7;     // Chip Select for decoder
sbit A1 = P3^4;     // Address line A1 for decoder
sbit A0 = P3^3;     // Address line A0 for decoder

void delay(unsigned int time); 
char keypad_scan();
void display(char num);

void main() {
    char key;
    while(1) {
        key = keypad_scan(); 
        if(key != 0xFF) {    
            display(key);    
        }
    }
}

// Function to create a delay
void delay(unsigned int time) {
    unsigned int i;
    for(i = 0; i < time; i++) {
        
    }
}

// Function to scan the keypad
char keypad_scan() {
    P0 = 0xF0; // Set rows (P0.0, p0.1...) high and columns low

    if(P0 != 0xF0) {  // If any key is pressed 11110111
        delay(20);    // Debounce delay

        // Scan each row
        P0 = 0xF7;
        if(P0 != 0xF7) {
            if(P0 == 0xE7) return '3';
            if(P0 == 0xD7) return '2';
            if(P0 == 0xB7) return '1';
        }

        P0 = 0xFB;
        if(P0 != 0xFB) {
            if(P0 == 0xEB) return '6';
            if(P0 == 0xDB) return '5';
            if(P0 == 0xBB) return '4';
        }

        P0 = 0xFD;
        if(P0 != 0xFD) {
            if(P0 == 0xED) return '9';
            if(P0 == 0xDD) return '8';
            if(P0 == 0xBD) return '7';
        }

        P0 = 0xFE;
        if(P0 != 0xFE) {
            if(P0 == 0xEE) return '0';
            if(P0 == 0xDE) return '0';
            if(P0 == 0xBE) return '0';
        }

    }
    return 0xFF; // Return 0xFF if no valid key is pressed
}



void display(char num) {
    char seg_code[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    char codei;
    unsigned char i;

    if (num >= '0' && num <= '9') {
        codei = seg_code[num - '0']; // Convert the character to the corresponding segment code
    } else {
        codei = 0xFF; // Default to all segments off if invalid number
    }

    // Display the same code on all 4 seven-segment displays
    for(i = 0; i < 4; i++) {
        A0 = i & 1;       // Set A0 based on the display number (0-3)
        A1 = (i >> 1) & 1; // Set A1 based on the display number (0-3)
        CS = 0;          // Enable the display (active low)
        P1 = codei;       // Send the segment code to the display
        CS = 1;          // Disable the display (active low)
        delay(10);       // Short delay for display
    }
}
	
