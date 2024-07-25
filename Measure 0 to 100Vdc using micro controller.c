#include <xc.h>

// Configuration bits for PIC16F877A
#pragma config FOSC = HS        // Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit
#pragma config BOREN = ON       // Brown-out Reset Enable bit
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define _XTAL_FREQ 20000000     // Define oscillator frequency

void ADC_Init() {
    ADCON0 = 0x41;   // Turn on the ADC module and select the analog channel (AN0)
    ADCON1 = 0x80;   // Configure the result to be right justified and select the voltage reference
}

unsigned int ADC_Read(unsigned char channel) {
    ADCON0 &= 0xC5;      // Clear the channel selection bits
    ADCON0 |= (channel << 3); // Select the required channel
    __delay_ms(2);       // Acquisition time to charge hold capacitor
    GO_nDONE = 1;        // Start A/D conversion
    while(GO_nDONE);     // Wait for conversion to complete
    return ((ADRESH << 8) + ADRESL); // Return result
}

void main(void) {
    unsigned int adc_value;
    float voltage;
    
    TRISA = 0xFF;       // Set Port A as input
    TRISB = 0x00;       // Set Port B as output (for LCD or other display)
    
    ADC_Init();         // Initialize ADC
    
    while(1) {
        adc_value = ADC_Read(0); // Read ADC value from channel 0 (AN0)
        voltage = (adc_value * 5.0) / 1023.0; // Convert ADC value to voltage (assuming Vref = 5V)
        voltage = voltage * (100.0 / 5.0); // Adjust the voltage based on the voltage divider ratio
        
        // Display or transmit the voltage value
        // (Implementation for displaying on LCD or transmitting via UART goes here)
        
        __delay_ms(500); // Delay for a while before the next reading
    }
}