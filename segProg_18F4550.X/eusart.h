#ifndef EUSART_H
#define EUSART_H

unsigned char rxEUSART( void );  // 
unsigned char rxStatusEUSART( void ); // 
unsigned char rxErrorEUSART( void );
void rxResetErrorEUSART( void );
void txEUSART( unsigned char d );
unsigned char txStatusEUSART( void );
void initEUSART( unsigned long baudRate );
int search (const char * x, const char * y);


struct eusartT
{
    unsigned char (*rx)( void );         // Ler o valor recebido pelo canal de comunicação serial
    unsigned char (*rxSTATUS)( void );   // Reinicializar/limpar os indicadores(flags) de erro.
    unsigned char (*rxERROR)( void );    // Reinicializar/limpar os indicadores(flags) de erro.
    void (* rxResetERROR) ( void );        // Verificar se algum dado foi recebido pelo canal de comunicação.
    void (* tx)( unsigned char d );       // Transmitir um dado pelo canal de comunicação serial.
    unsigned char (* txSTATUS) ( void );   // Indicar que um dado está sendo transmitido.
    void (* init)( unsigned long baudRate ); // Inicializar periférico de comunicação serial assíncrona.
    int (*search_string) (const char * x, const char * y);
};

extern struct eusartT eusart;


#endif
