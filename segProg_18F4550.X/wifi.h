/* 
 * File:   wifi.h
 * Author: 20187263
 *
 * Created on 5 de Outubro de 2021, 15:42
 */

#ifndef WIFI_H
#define	WIFI_H

unsigned char d = 0;

void wifi_init( long br );
void wifi_send( const char * msg );
unsigned char wifi_receive( unsigned char * rcv );
void Wifi_mode(unsigned char d );
void Wifi_connect( const char * ssid, const char * pass );
void Wifi_ip( void );
void Wifi_autoconnect( void );
void Wifi_scan (void);
void Wifi_config_servidor( void );
void Wifi_cipsend( void );



struct
{
    void (*init)( long br );
    void (*send)( const char * msg );
    unsigned char (*receive)( unsigned char * rcv );
    void (*mode)(unsigned char d );
    void (*connect)(const char * ssid, const char * pass);
    void (*ip)( void );
    void (*autoconnect)( void );
    void (*scan) (void);
    void (*config_servidor)( void );
    void (*cipsend)( void );
}wifi = {wifi_init, wifi_send, wifi_receive, Wifi_mode, Wifi_connect, Wifi_ip, Wifi_autoconnect, Wifi_scan, Wifi_config_servidor, Wifi_cipsend};

#endif	/* WIFI_H */

