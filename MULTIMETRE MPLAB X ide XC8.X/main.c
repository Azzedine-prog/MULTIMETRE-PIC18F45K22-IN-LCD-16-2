/*
 * File:   main.c
 * Author: Azzedine Lakhdar
 *ADC Configuration and voltmetre in screen 
 * INPT Embedded Student 
 * Created on January 7, 2021, 12:01 AM
 */


#include "config.h"
#include <stdio.h>
#include <xc.h>
#include "lcd.h"
#define trig PORTBbits.RB0 /*did not use that now*/
#define echo PORTBbits.RB1 // did not use that also

signed int result;                 //result of conversion from  to 1023 (10 bit ADC )
float voltage , current;
char v[6],i[6];                         //the string format of result  
int main()
{   ANSELD = 0;                       //setting port d as analog for lcd manipulation
    TRISD = 0;                         //port D as Output 
    OSCCONbits.IRCF  = 0b111;           // clock is 16MHz
    TRISAbits.RA0 = 1;                  // turning Led in port RA0 on
    ANSELAbits.ANSA0 = 1;                // A0 as analog
    ADCON0 = 0x00;                      // configuring ADC : AN0(RA0) - GO=0 hasn't started yet - hasn't enabled adc yet
    ADCON0bits.ADON = 1;                 //starting ADC 
    ADCON1bits.PVCFG = 0b00;             //Positive Voltage reference Configuration Vref+ = Vdd
    ADCON1bits.NVCFG = 0x00;            // Negative Voltage reference Configuration Vref- = Vss
    ADCON2bits.ACQT = 0b010;             //Aquesition Time (capacitor holding the value ) set to 4Tad
    ADCON2bits.ADFM = 1;                 //Right justified (------XX XXXXXXXX)
    ADCON2bits.ADCS = 0b101;             // ADCConversion Clock is set to Focs/16
    Lcd_Init();                         //initialisation of LCD 
    Lcd_Set_Cursor(1,1);                 // column 1 ad row 1
    Lcd_Write_String("voltage : ");        // this ligne is clear
    Lcd_Set_Cursor(2,1);                 // column 1 ad row 1
    Lcd_Write_String("current : ");        // this is clear
    Lcd_Set_Cursor(1,16);                            //column 1 row 2
    Lcd_Write_String("V"); 
    Lcd_Set_Cursor(2,16);                            //column 1 row 2
    Lcd_Write_String("A");
    while (1){
        ADCON0bits.GO = 1;                    //starting conversion 
        while(ADCON0bits.GO == 1){             // reading instant conversion
        result = 0;                             //initialisation of result 
        result = ADRESL;                       // asignement of result
        result = result | (ADRESH << 8) ;               // ADFM is one so it is right justified then we need little shifting
        voltage = (0.0049)*result;                        //result between 0 and 1024 - so we need to make it between 0 and 5V
        current = voltage * 0.001;
        snprintf(v, 6, "%f", voltage);             // int to string
        Lcd_Set_Cursor(1,10);                            //column 1 row 2
        Lcd_Write_String(v);                       //clear ligne hh
        snprintf(i, 6, "%f", current);             // int to string
        Lcd_Set_Cursor(2,10);                            //column 1 row 2
        Lcd_Write_String(i);                       //clear ligne hh
        
        }
        PORTDbits.RD7 = ~PORTDbits.RD7;                    // lighting led to verify that all is good
        
    }
    
}