/* 
 * File:   fifo.h
 * Author: luis420516@gmail.com
 *
 * Created on 9/6/2020 10:15:56 PM UTC
 * "Created in MPLAB Xpress"
 */

#ifndef FIFO_H
#define	FIFO_H



char ler_atuador(char tecla);
void alt_atuador( char tecla );
void init_atuadores(void);                                 /*INICIA OS ATUADORES RECUADOS*/
void fifo_reset(void);                                     //
char fifo_lerPos(char i);                                  //
void fifo_modifica(char pos, char valor);                  //
char * fifo_adrs(void);
void fifo_delete(void);                                    //DELETA O ÚLTIMO PASSO PRESSIONADO PELO USUARIO
void fifo_add(const char passo);                           //ADICIONA OS PASSOS NA FILA
void fifo_add_tempo(const char t);
char fifo_indice(void);                                    //RETORNA O INDICE DA FILA
char fifo_disponivel( void );                              //VERFICA SE HÁ ESPAÇO DISPONIVEL NA FILA
char fifo_tam(void);                                       //RETORNA O TAMANHO DO VETOR
void fifo_indicePrint_inc( void );                         //MOVE A TELA DA SEQUÊNCIA PARA DIREITA
void fifo_indicePrint_dec( void );                         //MOVE A TELA DA SEQUÊNCIA PARA ESQUERDA
void fifo_print(void);                                     //IMPRIME O VALOR PRESSIONADO PELA TECLA
void dignum_conc(char dig, unsigned int * ptrNum );        //
void dignum_apagar(unsigned int * ptrNum );                //
unsigned char dignum_tam(int num);                         //
void fifo_add_ciclo(unsigned int ciclos);                  //
unsigned int getCiclos(void);                              //
unsigned int getContCiclos(void);                          //RETORNA O VALOR DA VARÁVEL "cont_ciclos"
void resetContCiclos(void);
void addContCiclos(void);
void addIndicePassos(void);
void decIndicePassos(void);
unsigned char getIndicePassos(void);
void resetIndicePassos(void);
void fifo_alinharPrint( void );
char posAtual(char ind);
void printExec (char col, char ind);

void fifo_add_control(const char c);     //Funcao criada para adicionar a porcentagem de controle na fila - PENDENTE
char salva_atuadores_init( void );	     //Funcao criada para retornar a condicao inicial dos atuadores para cada sequencia criada
void manipula_atuadores_init(char aux);  //Funcao criada para manipular os atuadores conforme a condicao inicial buscada na memoria

void decodifica( unsigned char *d );
void codifica( unsigned char tipo, unsigned char *dado );


#endif	/* FIFO_H */
