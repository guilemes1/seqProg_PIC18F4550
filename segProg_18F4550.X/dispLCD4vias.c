/*
 * File:   dispLCD8vias.c
 * Author: josewrpereira
 *
 * Created on 15 de Agosto de 2020, 16:06
 * 
 * -------------------------------------------------
 *          MAPA DE ENTRADAS E SAIDAS
 * -------------------------------------------------
 *  Pinos   |n�     |Conexão
 * ---------|-------|-------------------------------
 *  RB4     |37     | LCD_RS
 *  RB5     |38     | LCD_EN
 *  RB0     |33     | LCD_D4
 *  RB1     |34     | LCD_D5
 *  RB2     |35     | LCD_D6
 *  RB3     |36     | LCD_D7
 * -------------------------------------------------
 */

#include <xc.h>
#include "config.h" 
#include "fifo.h"
#include "delay.h"
#include "dispLCD4vias.h"


//***************** Interface com PORTs/Pinos
#define LCD_BUS         LCDbits.BUS
#define LCD_EN          LCDbits.EN
#define LCD_RS          LCDbits.RS
#define LCD_ADDRS       0x0F8A     //LATB PIC18F4550

typedef union 
{
    struct 
    {
        unsigned char     :2;   //ordem alterada para PIC18F4550 devido RBO e RB1 ser utilizado para I2C 
        unsigned char BUS :4;
        unsigned char RS  :1;
        unsigned char EN  :1;
        
    };
} LCDbits_t;

volatile LCDbits_t LCDbits __at(LCD_ADDRS);





//***************** Definicao de Comandos ao LCD 
#define LCD_CLEAR_DISPLAY           0x01

#define LCD_RETURN_HOME             0x02

#define LCD_ENTRY_MODE_SET          0x04
#define LCD_EMS_CURSOR_RIGHT        0x02
#define LCD_EMS_CURSOR_LEFT         0x00
#define LCD_EMS_SHIFT_DISPLAY       0x01

#define LCD_DISPLAY_CONTROL         0x08
#define LCD_DC_DISPLAY_ON           0x04
#define LCD_DC_DISPLAY_OFF          0x00
#define LCD_DC_CURSOR_ON            0x02
#define LCD_DC_CURSOR_OFF           0x00
#define LCD_DC_BLINK_ON             0x01
#define LCD_DC_BLINK_OFF            0x00

#define LCD_CURSOR_SHIFT            0x10
#define LCD_CS_LEFT                 0x00
#define LCD_CS_RIGHT                0x04

#define LCD_DISPLAY_SHIFT           0x10
#define LCD_DS_LEFT                 0x08
#define LCD_DS_RIGHt                0x0C

#define LCD_FUNCTION_SET            0x20
#define LCD_FS_DATA_LENGTH_8        0x10 //NÃO USADO
#define LCD_FS_DATA_LENGTH_4        0x00
#define LCD_FS_LINE_NUMBER_1        0x00
#define LCD_FS_LINE_NUMBER_2        0x08

#define LCD_FS_DOTS_FORMAT_5x8      0x00 //NÃO USADO
#define LCD_FS_DOTS_FORMAT_5x11     0x04 //NÃO USADO

#define LCD_SET_CGRAM_ADDRS( adrs ) (0x40+(adrs & 0x3F))

#define LCD_SET_DDRAM_ADDRS( adrs ) (0x80+(adrs & 0x7F))
#define LCD_ADDR_LINE_0             0x00
#define LCD_ADDR_LINE_1             0x40
#define LCD_ADDR_LINE_2             0x14        //Insercao de endereco para linha 2
#define LCD_ADDR_LINE_3             0x54        //Insercao de endereco para linha 3


#define DELAY_MS        2
#define DELAY_US        40


// Escreve um comando no display (Instruction Register)
void dispLCD_instReg( unsigned char i )
{
    LCD_RS = 0;
    LCD_BUS = (i>>4);

    LCD_EN = 0;
    if( i == LCD_CLEAR_DISPLAY || i == LCD_RETURN_HOME )
        __delay_ms( DELAY_MS );
    else
        __delay_us( DELAY_US );
    LCD_EN = 1;
    

    if( (i & 0xF0) == (LCD_FUNCTION_SET | LCD_FS_DATA_LENGTH_4) )
    {
        LCD_RS = 0;
        LCD_BUS = i>>4;
        LCD_EN = 0;
        __delay_us( DELAY_US );
        LCD_EN = 1;
    }

    LCD_RS = 0;
    LCD_BUS = i & 0x0F;
    LCD_EN = 0;
    if( i == LCD_CLEAR_DISPLAY || i == LCD_RETURN_HOME )
        __delay_ms( DELAY_MS );
    else
        __delay_us( DELAY_US );
    LCD_EN = 1;
}

// Escreve um dado no display (Data Register))
void dispLCD_dataReg( unsigned char d )
{
    LCD_RS = 1;
    LCD_BUS = d >> 4;
    LCD_EN = 0;
    __delay_us( DELAY_US );
    LCD_EN = 1;

    LCD_RS = 1;
    LCD_BUS = d & 0x0F;
    LCD_EN = 0;
    __delay_us( DELAY_US );
    LCD_EN = 1;
}

// Posicionar o cursor na coordenada: linha e coluna 
void dispLCD_lincol( unsigned char lin, unsigned char col)      //Realizado alteracao na funcao para implementacao do Display LCD 20x4
{
    char aux = LCD_ADDR_LINE_0;
    
    switch(lin)
    {
        case 1: aux = LCD_ADDR_LINE_1;      break;
        case 2: aux = LCD_ADDR_LINE_2;      break;
        case 3: aux = LCD_ADDR_LINE_3;      break;   
    }        
    
    dispLCD_instReg( LCD_SET_DDRAM_ADDRS( aux + (col + LCD_ADDR_LINE_0) ) );
}

// Inicializa os pinos conectados ao display
void dispLCD_init( void )
{
    ADCON1 = 0xFF;
//    ANSELH = 0x00;
   
    TRISBbits.TRISB2 = 0;   // D4     //Alterado para PIC18F4550
    TRISBbits.TRISB3 = 0;   // D5     //Alterado para PIC18F4550
    TRISBbits.TRISB4 = 0;   // D6     //Alterado para PIC18F4550
    TRISBbits.TRISB5 = 0;   // D7     //Alterado para PIC18F4550
      
    TRISBbits.TRISB6 = 0;   // RS     //Alterado para PIC18F4550
    TRISBbits.TRISB7 = 0;   // EN     //Alterado para PIC18F4550

    LCD_EN = 1;
    delay(5);
    dispLCD_instReg( LCD_FUNCTION_SET|LCD_FS_DATA_LENGTH_4|LCD_FS_LINE_NUMBER_2);
    dispLCD_instReg( LCD_DISPLAY_CONTROL|LCD_DC_DISPLAY_ON|LCD_DC_CURSOR_OFF|LCD_DC_BLINK_OFF );
    dispLCD_instReg( LCD_CLEAR_DISPLAY );
    dispLCD_instReg( LCD_RETURN_HOME );
}

// Escreve uma string no display
void dispLCD( unsigned char lin, unsigned char col, const char * str )
{
    char pos = col;
    dispLCD_lincol( lin, col );

    while( *str )
    {
        dispLCD_dataReg( *str );
        ++str;
        ++pos;
    }
}

    // Escreve um número inteiro no display
void dispLCD_num(  unsigned char lin, unsigned char col,
                    int num, unsigned char tam )
{
    int div;
    unsigned char size;
    char sinal; // 0:+ 1:-
   
    sinal = num < 0;
    if( sinal )
        num = (~num) + 1;

    dispLCD_lincol(lin, col);
   
    div=10000;
    size = 5;
    while( div >= 1 )
    {
        if( num/div == 0 )
            --size;
        else
            break;
        div/=10;
    }

    while( tam > (size+sinal) && tam > 1 )
    {
        dispLCD_dataReg(' ');
        --tam;
    }  

    if( sinal )
        dispLCD_dataReg('-');
 
    do
    {
        dispLCD_dataReg( (num / div) + '0' );
        num = num % div;
        div/=10;
    }
    while( div >= 1 );
}

// Apaga todos os caracteres no display
void dispLCD_clr( void )
{
    dispLCD_instReg(LCD_CLEAR_DISPLAY);
}


void dispLCD_str( const char * str )
{
    char lin = 0;
    char c;
    dispLCD_lincol(0,0);

    while( c = *str )
    {
        if( c != '\n' )
            dispLCD_dataReg( c );
        else
            dispLCD_lincol(++lin,0);
        ++str;
    }
}
