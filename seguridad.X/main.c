/*!
\file   seguridad.c
\date   2022-06-28
\author Fulvio Vivas <fyvivas@unicauca.edu.co>
\brief  Security House.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2022. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/
#include <xc.h>
#include <pic16f887.h>  /*Header file PIC16f887 definitions*/
#include <stdio.h>
#include <string.h>
#include "CONFIG.h"
#include "LCD.h"
#include "KEYPAD.h"
// Definición de los pines de los LEDs
#define LED_VERDE PORTEbits.RE0
#define LED_AZUL PORTEbits.RE1
#define LED_ROJO PORTEbits.RE2





const char password[5] ={'2','0','2','1',0};
char pass_user[5];
unsigned char idx = 0;
int intentos = 0;

void delay_ms_variable(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);  // Llamada a __delay_ms con un valor constante
    }
}

// Prototipos de funciones
void blink_led(unsigned char led, unsigned int on_time, unsigned int off_time, unsigned int duration) {
    unsigned int elapsed_time = 0;

    while (elapsed_time < duration) {
        if (led == 'V') {
            LED_VERDE = 1;  // Enciende LED verde
        } else if (led == 'A') {
            LED_AZUL = 1;   // Enciende LED azul
        } else if (led == 'R') {
            LED_ROJO = 1;   // Enciende LED rojo
        }
        delay_ms_variable(on_time);

        // Apagar LED
        if (led == 'V') {
            LED_VERDE = 0;
        } else if (led == 'A') {
            LED_AZUL = 0;
        } else if (led == 'R') {
            LED_ROJO = 0;
        }
        delay_ms_variable(off_time);

        elapsed_time += on_time + off_time;
    }
}

void secuencia5(void);
void secuencia6(void);
void secuencia7(void);
void monitoreo(void);
void main() {

    char key = '0';
    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71; //Configura oscilador interno (FOSC = 8Mhz)
     
    LCD_Init(); //Inicializa el LCD

    keypad_init(); //Inicializa el keypad
    TRISE = 0b00000000;
    TRISA = 0b00000000;   // Configura RD0, RD1, RD2 como salidas (para los LEDs)
    ANSELH = 0x00;
    Lmain();
    TRISD = 0b00000000;
    //secuencia5();
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
   

    while (1) {
        LCD_String_xy(0,0,"Press a Key");
        LCD_Char(2);
        LCD_Command(0xC0); 
        
       
        
        do{
            key = keypad_getkey();
            if(key != 0){
                LCD_Char('*');/* display pressed key on LCD16x2 */
                //LCD_Char(key);/* display pressed key on LCD16x2 */
                pass_user[idx++] = key;
            }
            __delay_ms(100);
        }while(idx < 5);
        
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Correcta");
            blink_led('V', 500, 500, 3000);  // Parpadear LED verde por 3 segundos
            intentos = 0;
            if (PORTAbits.RA0) {
                __delay_ms(100);
                secuencia5();
                __delay_ms(2000);
                monitoreo();
            }
            else if (PORTAbits.RA1) {
                __delay_ms(100);
                secuencia6();
                __delay_ms(2000);
                monitoreo();
            }
            else if (PORTAbits.RA2) {
                __delay_ms(100);
                secuencia7();
                __delay_ms(2000);
                monitoreo();
            }
            else{
                monitoreo();
            }
            
               
        }
        else{
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Incorrecta"); 
            blink_led('A', 300, 700, 2000);  // Parpadear LED azul por 2 segundos
            intentos = intentos + 1;
            if(intentos ==3){
                LCD_Clear();
                LCD_String_xy(0,0,"sistema bloqueado");
                blink_led('R', 300, 500, 8000);  // Parpadear LED rojo por 8 segundos
                intentos = 0;  // Reiniciar intentos después del bloqueo
                
            }
            
           
        }
        __delay_ms(2000);
        LCD_Clear();
        idx = 0;
    
}
}

void secuencia5(void){
volatile unsigned int var1=0xA0,var2=0x01,var12 = 0;
    for (int i = 0; i < 2; i++) {
        var12 = var1 | var2;
        PORTD = (unsigned char)(var12);
        __delay_ms(500);
        var1 = var1 >> 1;
        var2 = var2 << 1;
        var12 = var1 | var2;
        PORTD = (unsigned char)(var12);
        __delay_ms(500);
        var12 = var1 | var2;
        var1 = var1 << 1;
        var2 = var2 << 1;

    }

volatile unsigned int var3=0xA0,var4=0x08,var34 = 0;
    for (int i = 0; i < 2; i++){
        var34 = var3 | var4;
        PORTD = (unsigned char)(var34);
        __delay_ms(500);
        var3 = var3 >> 1;
        var4 = var4 >> 1;
        var34 = var3 | var4;
        PORTD = (unsigned char)(var34);
        __delay_ms(500);
        var34 = var3 | var3;
        var3 = var3 << 1;
        var4 = var4 >> 1;
        
    }
 }

void secuencia6(void){
volatile unsigned int var1=0x01,var2=0x80,var12 = 0;
    for (int i = 1; i < 5; i++) {
        
            var12 = var2 | var1;
            PORTD = (unsigned char)(var12);
             __delay_ms(500);
            var1 = var1 + 2;
            var2= var2 >> 1;
           }
volatile unsigned int var3=0x09,var4=0x10,var34 = 0;
    var34 = var4 | var3;
    PORTD = (unsigned char)(var34);
     __delay_ms(500);
    for (int i = 0; i < 3; i++){
        var3= var3 + 2;
        var4= var4 << 1;
        var34 = var4 | var3;
        PORTD = (unsigned char)(var34);
         __delay_ms(500);
    }
}
void monitoreo(void){
LCD_Clear();
LCD_String_xy(0,0,"Monitoreo");
 __delay_ms(1000);
}
   

void secuencia7(void){
volatile unsigned int var1=0x01,var2=0x80,var12 = 0;

    for (int i = 0; i < 4; i++) {
        var12 = var1 | var2;

        PORTD = (unsigned char)(var12);
         __delay_ms(500);
        STATUSbits.CARRY = 0;
        var1 = var1 << 1;
        STATUSbits.CARRY = 0;
        var2 = var2 >> 1;
        STATUSbits.CARRY = 0;
        var2 |= (1 << 7);
        STATUSbits.CARRY = 0;
 
    }

volatile unsigned int var3=0x10,var4=0x08,var34=0 ;
    for (int i = 0; i < 4; i++) {
        var34 = var3 | var4;
        PORTD = (unsigned char)(var34);
         __delay_ms(500);
        STATUSbits.CARRY = 0;
        var3 = var3 << 1;
        STATUSbits.CARRY = 0;
        var4 = var4 >> 1;
        STATUSbits.CARRY = 0;
        var4 |= (1 << 3);
        STATUSbits.CARRY = 0;
 
    }

}
            

    
