#ifndef EEPROM_H
#define EEPROM_H

#define LIMPAR    0

char EEPROM_read( unsigned char addr );
void EEPROM_write( unsigned char addr, unsigned char data );
void write (char adrs, unsigned char *data);
void read ( char  adrs , unsigned char * eep ); 
void salvar_receita (char adrs,unsigned char *data );
void buscar_receita (char adrs,unsigned char *data );
void deletar_receita (char receita);
void receita_livre ( unsigned char * rec);
void formatar_receitas (void);

void salvar_dado (char adrs, char col, unsigned char data);
void buscar_dado (char adrs, char col, unsigned char *data); 


//                                           EEPROM
 struct
{
    void  (* salvar) (char adrs,unsigned char *data );
    void  (* buscar) (char adrs,unsigned char *data );
    void  (* deletar) (char receita);
    void  (* livre) (unsigned char * rec);
    void  (* formatar) (void);
} EEPROM = { salvar_receita, buscar_receita, deletar_receita, receita_livre, formatar_receitas };

#endif

