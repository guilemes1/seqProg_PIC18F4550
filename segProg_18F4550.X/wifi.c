/*
 * File:   esp01.c
 * Author: 20187263
 *
 * Created on 5 de Outubro de 2021, 15:40
 */

#include <xc.h>
#include "eusart.h"
#include "wifi.h"
#include "delay.h"

void wifi_init( long br)
{
    eusart.init(br);
}

void wifi_send( const char * msg )
{
    while( *msg )
    {
        while( eusart.txSTATUS() );
        eusart.tx( *msg );
        ++msg;
    }
}
//unsigned char indice = 0;
unsigned char wifi_receive( unsigned char * rcv )
{
    unsigned char d = 0;
    unsigned char indice = 0;
    while( d != '\n' )
    {
        if( eusart.rxSTATUS() )
        {
            d = eusart.rx();
            if( (d >= 'A' && d <= 'Z') || (d >='a' && d <= 'z'))
            {   
                *(rcv+indice) = d;
                ++indice;
            }
        }
    }
    return( indice );
}

void Wifi_mode(unsigned char d )
{
    if(d == 1) // modo de estação STA(cliente)
    {
        wifi_send("AT+CWMODE=1\r\n");

        wifi_send("AT+CIPSERVER=0\r\n"); 
        wifi_send("AT+CIPMUX=0\r\n"); 
        wifi_send("AT+CIPMODE=1\r\n"); 
    }

    if(d == 2) // modo SoftAP(servidor)
    {
        wifi_send("AT+CWMODE=2\r\n");
        delay(1000);
        wifi_send("AT+CWSAP=\"TCC\",\"87654321\",1,2,4,0\r\n");
        delay(1000);
    }
    if(d == 3) // ambos(cliente + servidor)
    {
        wifi_send("AT+CWMODE=3\r\n");
        wifi_send("AT+CWSAP=\"Wireless\",\"********\",1,2,4,0\r\n");
    }
//    else // modo nulo
//        wifi_send("AT+CWMODE=0\r\n");
}

unsigned char *aux[10];

void Wifi_connect( const char * ssid, const char * pass )
{
    unsigned char i = 10;
    unsigned char str[30] = "AT+CWJAP=\"";
    while( *ssid )
    {
        str[i] = *ssid;
        *ssid++;
        i++;
    }
    str[i] = '"'; i++;
    str[i] = ','; i++;
    str[i] = '"'; i++;

    *aux = *ssid;   
    
    while( *pass )
    {
        str[i] = *pass;
        *pass++;
        i++;
    }
    str[i] = '"'; i++;
    str[i] = '\r'; i++;
    str[i] = '\n'; i++;
    str[i] = 0;
    wifi_send( str );

}

void Wifi_scan (void)
{
    wifi_send("AT+CWLAP\r\n");
}

void Wifi_ip( void )
{
    wifi_send("AT+CIFSR\r\n");
}

void Wifi_autoconnect( void )
{
    wifi_send("AT+CWAUTOCONN=0\r\n");
}
void Wifi_config_servidor( void )
{
    delay(1000);

    wifi_send("AT+CIPMUX=1\r\n"); 
    delay(1000);

    wifi_send("AT+CIPSERVER=1,333\r\n"); 
    delay(1000);

    wifi_send("AT+CIPMODE=0\r\n"); 
    delay(1000);
}
void Wifi_config_cliente( void )
{
    wifi_send("AT+CIPSERVER=0\r\n"); 
    wifi_send("AT+CIPMUX=0\r\n"); 
    wifi_send("AT+CIPMODE=1\r\n"); 
}

//void Wifi_cipsend( void )
void Wifi_cipsend( unsigned char *tam, unsigned char *msg )
{
                            
    unsigned char i = 11;                    unsigned char d = 1;        //unsigned char c = 0;
    unsigned char str[20] = "AT+CIPSEND=\""; unsigned char vtr[10] = ""; //unsigned char cmd[40] = "AT+CIPSTART=\"TCP\"\"\"\"333\"\""; 

//    wifi_send("AT+CIPSERVER=0\r\n"); 
//    wifi_send("AT+CIPMUX=0\r\n"); 
//    wifi_send("AT+CIPMODE=1\r\n"); 

wifi.send("AT+CIPSTART=\"TCP\",\"192.168.4.1\",\"333\"\r\n"); 

//        while( *cmd )
//    {
//        cmd[c] = *aux;
//        ++*aux;
//        c++;
//    }
//    cmd[c] = '\r'; c++;
//    cmd[c] = '\n'; c++;
//    cmd[c] = 0;
//    wifi_send( cmd );

    while( *tam )
    {
        str[i] = *tam;
        *tam++;
        i++;
    }
    str[i] = '\r'; i++;
    str[i] = '\n'; i++;
    str[i] = 0;
    wifi_send( str );
    
    while( *msg )
    {
        vtr[d] = *msg;
        *msg++;
        d++;
    }
    vtr[d] = '\r'; d++;
    vtr[d] = '\n'; d++;
    vtr[d] = 0;
    wifi_send(vtr); 

    wifi_send("AT+CIPCLOSE\r\n"); 
}