#include "dispLCD4vias.h"
#include "fifo.h"
#include "estados.h"


#define FIFO_INDICE_INICIO 2   //Mudança teste
#define TAM_VETOR 18


char fila[TAM_VETOR];
char ind_fila = FIFO_INDICE_INICIO;
char ind_print = FIFO_INDICE_INICIO;

unsigned int ciclos = 0;
unsigned int cont_ciclos = 0;
unsigned char indice_passos = FIFO_INDICE_INICIO;


void addIndicePassos(void)
{
    if(indice_passos < ind_fila)
        indice_passos++;
}

void decIndicePassos(void)
{
    if(indice_passos > FIFO_INDICE_INICIO)
        indice_passos--;
}

unsigned char getIndicePassos(void)
{
    return(indice_passos);
}

void resetIndicePassos(void)
{
    indice_passos = FIFO_INDICE_INICIO;  //indice_passos = 2
}

unsigned int getCiclos(void)
{
    int aux = fila[1];
    aux <<= 8;
    aux +=fila[0];
    return(aux);
}

unsigned int getContCiclos(void)
{
    return(cont_ciclos);
}

void resetContCiclos(void)
{
    cont_ciclos = 0;
}

void addContCiclos(void)
{
    cont_ciclos++;
}

union 
{ 
    struct
    {
        unsigned char A  :1;
        unsigned char B  :1;
        unsigned char C  :1;
        unsigned char D  :1;      
        unsigned char    :4;
    };  
}atuador;



/*       Manipulação do estado dos atuadores        */

char ler_atuador(char tecla)
{
    switch(tecla)
    {
        case 'A':   return(atuador.A);      break;
        case 'B':   return(atuador.B);      break;
        case 'C':   return(atuador.C);      break;
        case 'D':   return(atuador.D);      break;  
        default:    return 0;               break;
    }
}

void alt_atuador( char tecla )
{
    switch(tecla)
    {
        case 'A':   atuador.A = atuador.A ^ 1;      break;          //OPERAÇÃO XOR
        case 'B':   atuador.B = atuador.B ^ 1;      break;
        case 'C':   atuador.C = atuador.C ^ 1;      break;
        case 'D':   atuador.D = atuador.D ^ 1;      break;
    }
}

void init_atuadores(void)
{
    atuador.A = 0;
    atuador.B = 0;
    atuador.C = 0;
    atuador.D = 0;    
}



/*
 *              Manipulação dos elementos/dados na fila
 */

char salva_atuadores_init( void )	    
{
    char condi_init[4] = {ler_atuador('A'), ler_atuador('B'), ler_atuador('C'), ler_atuador('D')};
                            
    char *pt = condi_init;
    char dado = 0;
    char mask = 0x08;    
        
    while( mask )
    {
        if( *pt )
        {
            dado = dado | mask;
        }
        pt++;
        mask >>= 1;
    }
    return(dado);
}

void manipula_atuadores_init(char aux)
{
    char mask = 0x08;
    char atuador = 0x41;
    
    if(aux ==0xFF)
    {
        
    }
    else
        while(mask)
        {

            if(aux & mask)
            {
                alt_atuador(atuador);
            }
            atuador++;
            mask >>= 1;
        }    
}

void fifo_reset(void)
{        
    for( char i=0; i<FIFO_INDICE_INICIO; i++ )
        fila[i] = 0;

    fila[FIFO_INDICE_INICIO] = ' ';
    fila[FIFO_INDICE_INICIO + 1] = 0;
    ind_fila = FIFO_INDICE_INICIO;    
}
char fifo_lerPos(char i)
{
    return ( fila[i] );
}

void fifo_modifica(char pos, char valor)
{
    fila[pos] = valor;
}

char * fifo_adrs(void)
{
    return fila;
}

void fifo_delete(void)
{
    if(ind_fila)
    {
        if(ind_fila > FIFO_INDICE_INICIO)
            ind_fila--;
        if(fila[ind_fila] > 0xDC)
        {
            decodifica(&fila[ind_fila]);
            alt_atuador(fila[ind_fila] & ~0x20);
            codifica('a',fila[ind_fila]);
        }    
        fila[ind_fila] = 0;
    }
    fifo_alinharPrint();
}

void codifica( unsigned char tipo, unsigned char *dado )
{
    switch( tipo )
    {
        case 'p':   if(*dado == 0)
                       *dado = 254;        
                        break;                  // p[%]
                        
        case 't':   *dado += 0x64;    break;    // t[s] --> soma 100
        case 'a':  
        
        switch( *dado )
        {
            case 'A':   *dado = 221;  break;           
            case 'a':   *dado = 222;  break;     
            case 'B':   *dado = 223;  break;     
            case 'b':   *dado = 224;  break;    
            case 'C':   *dado = 225;  break;     
            case 'c':   *dado = 226;  break;     
            case 'D':   *dado = 227;  break;     
            case 'd':   *dado = 228;  break;     
        }                                       // atuador: [A,a,B,b,C,c,D,d]
        
        break;
        //return(dado);    
    }
}

void decodifica( unsigned char *d )
{
    if( *d <= 100 )
    {
        ;
    }
    else if( *d <= 220 )
    {
        *d -= 0x64; //subtrai 100
    } else
    {
        switch( *d )
        {
            case 221:   *d = 'A';  break;     // A: A+               
            case 222:   *d = 'a';  break;     // a: A-
            case 223:   *d = 'B';  break;     // B: B+
            case 224:   *d = 'b';  break;     // b: B-
            case 225:   *d = 'C';  break;     // C: C+
            case 226:   *d = 'c';  break;     // c: C-
            case 227:   *d = 'D';  break;     // D: D+
            case 228:   *d = 'd';  break;     // d: D-
            case 254:   *d = 0  ;  break; 
        }
    }
}

void fifo_add(const char passo)
{
    if(ind_fila < TAM_VETOR)                      
    {          
        if(passo)
        {
            codifica('a', &passo);                       
            fila[ind_fila] = passo;                                                             
            ind_fila++;                        
            fila[ind_fila] = 0;
        }
    }
     fifo_alinharPrint();                 
}

void fifo_add_tempo(const char t)
{
    if(ind_fila < TAM_VETOR)
    {    
        if(t)
        {
            codifica('t', &t);
            fila[ind_fila] = t;
            ind_fila++;
            fila[ind_fila] = 0;
        }
    }
    fifo_alinharPrint();
}

void fifo_add_control(const char c)
{
    if(ind_fila < TAM_VETOR)
    {    
        //if(c)  //alterado para acrescentar 0% a fila
        //{
            codifica('p', &c);
            fila[ind_fila] = c;
            ind_fila++;
            fila[ind_fila] = 0;
        //}
    }
    fifo_alinharPrint();
}

void fifo_add_ciclo(unsigned int ciclos)
{   
    fila[0] = ciclos & 0x00ff;
    fila[1] = ciclos >> 8;
}



char fifo_indice(void)
{
    return ( ind_fila );
}

char fifo_disponivel( void )
{
    return( ind_fila < TAM_VETOR );
}

char fifo_tam(void)
{
    return( TAM_VETOR );
}

void fifo_indicePrint_inc( void )
{
    if( ind_print < TAM_VETOR-1 )                               //ind_print começa com 2, se ind_print = 2 for menor que TAM_VET(62)-1, executa:
    {
        if( ind_print < ind_fila-1 )                            //ind_fila começa com 2, se ind_print = 2 for menor que ind_fila(2)-1, então incrementa a variável ind_print
            ind_print++;
        else
            ind_print = ind_fila-1;                             //senão, ind_print = Ind_fila(2)-1
    }
    else
        ind_print = TAM_VETOR-1;                                //Senão, ind_print = 62-1 = 61
}
void fifo_indicePrint_dec( void )
{
    if( ind_print > FIFO_INDICE_INICIO )                        //SE ind_fila for maior que FIFO_INDICE_INICIO(2), então decrementa o ind_print
        ind_print--;
}

//|_ _ _ _ _ _ _ _ |
// A+100A-B+10b-C+120
void fifo_print(void)
{
    char completar = 0;
    char comando = 0;
    char atraso = 0;
    char i = 0;
    char indPrint = ind_print;  //3

    dispLCD_lincol(0, 0);
    do
    {
        if( completar )
        {
            dispLCD_dataReg(' '); 
            ++i;
        }
        else
        {
            comando = fila[indPrint++];                 //comando == fila[4]
               
            if( comando == 0 )
                completar = 1;
            else if ( comando <= 0x64 )
            {
                atraso = comando;
                if( atraso >= 100 )
                {
                    dispLCD_dataReg('1'); 
                    ++i;
                    atraso %= 100;
                    if(atraso < 10)
                    {    
                        dispLCD_dataReg('0');
                        ++i;
                    }    
                }
                if( atraso >= 10 )
                {                    
                    dispLCD_dataReg(atraso/10 + '0');                    
                    ++i;
                    atraso %= 10;
                }
                
                dispLCD_dataReg(atraso + '0');
                ++i;
                    
                dispLCD_dataReg('%');
                ++i;           
            }    
                
            else if( comando > 0x64 && comando <= 0xDC)                     //se for maior q 100 e menor ou igual a 220
            {
                decodifica(&comando);
                atraso = comando; 
               
                if( atraso >= 100 )
                {
                    dispLCD_dataReg('1'); 
                    ++i;
                    atraso %= 100;
                    if(atraso < 10)
                    {    
                        dispLCD_dataReg('0');
                        ++i;
                    }    
                }
                if( atraso >= 10 )
                {                    
                    dispLCD_dataReg(atraso/10 + '0');                    
                    ++i;
                    atraso %= 10;
                }
                
                dispLCD_dataReg(atraso + '0');
                ++i;
                    
                dispLCD_dataReg('s');
                ++i;
            }
            else if( comando > 0xDC && comando < 0xFE )
            {        
                decodifica(&comando);
                switch( comando )
                {                    
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':                           
                                dispLCD_dataReg( comando & ~0x20 );            //IMPRIME O CARACTER EM MAIUSCULO
                                ++i;
                                dispLCD_dataReg( comando & 0x20 ? '-': '+' );  //IMPRIME O SINAL PARA O CARACTER
                                ++i;
                                break;
                }                   
            }
            else
            {
                if(comando == 0xFE)
                {    
                    decodifica(&comando);
                    atraso = comando;
                
                    dispLCD_dataReg(atraso + '0');
                    ++i;
                    
                    dispLCD_dataReg('%');
                    ++i;
                }    
            }    
        }
    }
    while( i <= LCD_COLS );
}

void dignum_conc(char dig, unsigned int * ptrNum )
{        
    *ptrNum = (*ptrNum * 10) + (dig - '0');        
}

void dignum_apagar(unsigned int * ptrNum )
{
    *ptrNum /= 10;    
}

unsigned char dignum_tam(int num)
{
    unsigned char tam = 0;
    
    while(num != 0)
    {
        num /= 10;
        tam++;
    }
    return ( tam );
}

void fifo_alinharPrint( void )
{
    unsigned char i;
    unsigned char aux;
    unsigned char cont = 0;
    
    for( i=ind_fila-1; i>=FIFO_INDICE_INICIO; i-- )
    {
        aux = fila[i];
        if( aux <= 0x64 )                                     //Se for controle (porcentagem) "se for menor ou igual a 100"
            cont += dignum_tam( aux )+1;                      //aux - 100
  
        if( aux > 0x64 && aux <= 0xDC )                       //Se for TEMPO "se dor maior que 100 e menor ou igual a 220
            cont += dignum_tam( aux - 0x64 )+1;               //aux - 100

        if( aux > 0xDC && aux < 0xFE )                        //Se for um atuador(CARACTER) aux >= 'A' && aux <= 'z'
            cont += 2;
            
        if( aux == 0xFE ) //Se for 0%
            cont += dignum_tam( aux )+1;               
        
        if( cont > LCD_COLS )
        {
            break;
        }
        else
            ind_print = i;
    }
}

char posAtual(char ind)
{
    char passo = 0;
    
    passo = fifo_lerPos( getIndicePassos() + ind );
    return(passo);
}

void printExec (char col, char ind)
{
    char passo = posAtual( ind ); //passo = fila[1] 
    
    if(passo > 0 && passo <= 0x64)
    {
        if( dignum_tam( (int) passo ) == 3 )
            col-=1;
        dispLCD_num(0, col, (int)passo, dignum_tam( (int)passo) );
        dispLCD_dataReg('%'); 
    }
    else if(passo > 0x64 && passo <= 0xDC)
    {
        decodifica(&passo);
        if( dignum_tam( (int) passo ) == 3 )
            col-=1;
        dispLCD_num(0, col, (int)passo, dignum_tam( (int)passo) );
        dispLCD_dataReg('s');
    }   
    else if( passo > 0xDC && passo <0xFE)
    {
        decodifica(&passo);
        dispLCD_lincol(0, col);
        dispLCD_dataReg( passo & ~0x20 );                                
        dispLCD_dataReg( passo & 0x20 ? '-': '+' );
    }
    else if( passo == 0xFE)
    {
        decodifica(&passo);
        dispLCD_num(0, col, (int)passo, dignum_tam( (int)passo) );
        dispLCD_dataReg('%');
    }    
}
