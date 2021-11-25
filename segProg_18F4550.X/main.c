/*
 * File:   main.c
 * Author: josewrpereira
 *
 * Created on 15 de Agosto de 2020, 16:03:21
 *
 * -------------------------------------------------
 *          MAPA DE ENTRADAS E SAIDAS
 * -------------------------------------------------
 *  Pinos   |nº     | Conexao
 * ---------|-------|-------------------------------
 *  RA0     |2      | OUT : Output
 *  RA1     |3      | IN : Input 
 *  RA3     |5      | SHLD : Shift / Load
 *  RA2     |4      | CLK : Clock
 
 *  RC6     |25     | ESP01_TX
 *  RC7     |26     | ESP01_RX

 *  RB0     |33     | MCP4725_SDA
 *  RB1     |34     | MCP4725_SCL 
 *  RB2     |35     | LCD_D4
 *  RB3     |36     | LCD_D5
 *  RB4     |37     | LCD_D6
 *  RB5     |38     | LCD_D7
 *  RB6     |39     | LCD_RS
 *  RB7     |40     | LCD_EN
 
 *  RD0     |19     | TECLADO_L0
 *  RD1     |20     | TECLADO_L1
 *  RD2     |21     | TECLADO_L2
 *  RD3     |22     | TECLADO_L3
 *  RD4     |27     | TECLADO_C0
 *  RD5     |28     | TECLADO_C1
 *  RD6     |29     | TECLADO_C2
 *  RD7     |30     | TECLADO_C3
 * -------------------------------------------------
 * -------------------------------------------------
 */


#include <xc.h>
#include "delay.h"
#include "dispLCD4vias.h"
#include "teclado.h"
#include "fifo.h"
#include "serialIO.h"
#include "atuadores.h"
#include "timer1.h"
#include "estados.h"
#include "ihm.h"
#include "eeprom.h"
#include "MCP4725.h"
#include "spi_i2c.h"
#include "eusart.h"
#include "wifi.h"

void __interrupt() irq(void)
{
    INTCONbits.GIE = 0;
    if(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
    {
        PIR1bits.TMR1IF = 0;
        //INT_T1;
        intt1_1ms();
    }
    INTCONbits.GIE = 1;    
}


void main(void)
{    
    dispLCD_init();
    teclado_init();
    timer1_init();
    I2C_init(20000000);
    MCP4725(0);
    wifi.init(115200);
    delay(1000);
    wifi.mode(1);
    delay(1000);
    wifi.connect("LEMES","28090210");
    
//    wifi.init(115200);
//    wifi.mode(1);
//    wifi.connect("LEMES","28090210");
//    wifi.ip();
//    wifi.cipsend(3,"Gui");
    
//    char *texto;
    char tecla = 0;
    char estado = INICIO;
    char atuador;
    char vetor_aux[16];              //Vetor criado para buscar as sequencias salvas na memoria
    char init_cond = 0;              //Variavel que salvara as condições iniciais dos atuadores
    char *pt;                        //Ponteiro auxiliar utilizado para varrer o "vetor_aux"
//    char lin2[17] = "                ";
    char contPassos = 0;
    char vetorIn[1] = { 0 };
    char vetorOut[1] = { 0 };
    initSerialIO( vetorIn, vetorOut, 1 );
    char meAtuadores = -1;
    char auxPasso = 0; 
    char aux = 2;
    unsigned int num = 0;    
    unsigned int ciclos = 0;
    char botao = 0;
#define STOP    0
#define PLAY    1
#define PAUSE   2
#define STEP    3
//    char boolPause = 0;
//  #define boolPlay !boolPause
//    char boolStop = 0;
//    char pauseLeft = 0;
//    char pauseRight = 0;
//    char meAtAux = 0;
//    char next = 0;

    while(1)
    {      
         switch(estado)
        {        
             case INICIO:                                           //TELA DE INICIO(CARREGA BARRAS)
                            for(char i = 0; i <= 3; i++)
                            {
                                for(char j = 0; j <= 19; j++)
                                {
                                    IHM.printpos(i, j, "\377");
                                    delay(10);
                                }
                            }

                            for(char i = 0; i <= 3; i++)
                            {
                                for(char j = 0; j <= 19; j++)
                                {
                                    IHM.printpos(i, j, " ");
                                    delay(10);
                                }
                            }
                            estado = TELA_EMPRESA;
                            break;
                    
            case TELA_EMPRESA:
                            IHM.printpos(1, 0, "      SEGGUIT       ");         
                            
                            char lema[20] = "Mundo ao seu comando";         
                            char i = 0;
                            char *ptr;
                            
                            for(ptr = lema; *ptr != 0; ptr++)            //Inserido para exibicao de lema na TELA_EMPRESA
                            {
                                dispLCD_lincol( 2, i );
                                dispLCD_dataReg( *ptr );
                                i++;
                                delay(50);
                            }
                                               
                            setT1(1500);                            
                            estado = TELA_EMPRESA_DELAY;            
                            break;
                    
             case TELA_EMPRESA_DELAY:
                            if(!statusT1())                         
                                estado = TELA_MENU;                    
                            break;
                            
//             case INICIO_ATUADORES:
//                            init_atuadores();
//                            IHM.clr();
//                            IHM.print("\n"
//                                      "    INICIALIZANDO\n"                                                      
//                                      "       SAIDAS...");
//                            setT1(1000);
//                            estado = INICIO_ATUADORES_DELAY;
//                            break; 
                            
//             case INICIO_ATUADORES_DELAY:
//                            if(!statusT1())                         
//                                estado = TELA_MENU;
//                            break;               
                            
             case TELA_MENU:  
                            init_atuadores();                      //INICIA OS ATUADORES RECUADOS
                            IHM.print("1: INSERIR SEQUENCIA\n"
                                      "2: BUSCAR SEQUENCIA\n"
                                      "3: REPETIR ULT. SEQU\n"
                                      "4: CONFIGURACOES    ");   
                            setT1(1000);                            
                            estado = TELA_MENU_DELAY;
                            break;
                            
            case TELA_MENU_DELAY:
                            if(!statusT1())                         
                                estado = TELA_MENU_TECLA;                    
                            break;
                            
            case TELA_MENU_TECLA:   
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case '1':   estado = TELA_AJUSTE_INICIAL;         break;
                                case '2':   estado = TELA_BUSCAR_SEQUENCIA;       break;
                                case '3':   estado = TELA_REPETE_ULTIMA_SEQUENCIA; break;
                                case '4':   estado = TELA_CONFIGURACOES;          break;                                                        
                            }
                            break;                                         
             
             case TELA_REPETE_ULTIMA_SEQUENCIA:
                 
                            buscar_dado(10, 10, &init_cond);      //Busca o endereco de memoria onde foi salvo a condicao inicial dos atuadores da ultima sequencia executada e guarda na variavel "init_cond"                                         
                            manipula_atuadores_init(init_cond);   //Realiza a manipulacao dos atuadores conforme as condicoes iniciais da ultima sequencia executada expressa pela variavel "init_cond"
                                           
                            EEPROM.buscar(11, vetor_aux);         //Busca a ultima receita executada e insere em "vetor_aux"                                     
                            pt = vetor_aux;

                            if(*pt == 0xFF)                                  //Verifica se o primeiro byte do endereço de memoria esta no padrao defalt (0xFF) 
                            {
                                EEPROM.deletar(tecla % 0x30);
                                vetor_aux[0] = 0;
                            }


                            for(char *ptr = vetor_aux; *ptr != 0; ptr++)     //Varre o vetor_aux e insere os passos na fila (vetor fila)
                            {                                                  
                                if(*ptr <= 0x64 || *ptr == 0xFE)
                                {
                                    decodifica(&*ptr);
                                    fifo_add_control(*ptr);
                                }
                                else if (*ptr > 0x64 && *ptr <= 0xDC)
                                {
                                    decodifica(&*ptr);
                                    fifo_add_tempo(*ptr);
                                }
                                else
                                {    
                                    decodifica(&*ptr);
                                    switch( *ptr )
                                    {
                                        case 'A':
                                        case 'B':
                                        case 'C':
                                        case 'D':
                                        case 'a':
                                        case 'b':
                                        case 'c':
                                        case 'd':                           
                                                *ptr &= ~0x20;   //Converte os caracteres para maiusculo
                                                break;
                                        }            
                                        alt_atuador(*ptr);                                                  
                                        fifo_add( ler_atuador(*ptr) ? *ptr : *ptr|0x20 );
                                    }
                                fifo_print();
                            }                                      
                                           
                            dispLCD_clr();
                            estado = TELA_PRINTFILA;
                            break;
                            
             case TELA_CONFIGURACOES:
                            dispLCD_clr();
                            dispLCD(0,0,"1: VERIFICAR MEMORIA");  
                            dispLCD(1,0,"2: LIMPAR MEMORIA   ");
             
                            setT1(1000);                                   //INICIA A CONTAGEM DO TEMPO DE 1seg
                            estado = TELA_CONFIG_DELAY;
                            break;
                            
             case TELA_CONFIG_DELAY:
                            if(!statusT1())                         //QUANDO TERMINAR A CONTAGEM PASSA PARA O PROXIMO ESTADO
                                estado = TELA_CONFIG_TECLA;                    
                            break;               
                            
             case TELA_CONFIG_TECLA:                           
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case '1':   estado = TELA_VERIFICAR_MEMORIA;       break;
                                case '2':   estado = TELA_LIMPAR_MEMORIA;          break; 
                                case TECLA_DELETE:  estado = TELA_MENU;            break;  //Inserido para retornar a pagina anterior
                            }
                            break; 
                            
             case TELA_LIMPAR_MEMORIA: 
                            dispLCD(0, 0, " CERTEZA QUE DESEJA ");
                            dispLCD(1, 0, "   DELETAR TODA A   ");
                            dispLCD(2, 0, "      MEMORIA ?     ");
                            dispLCD(3, 0, "# - SIM      * - NAO");
                            
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case '#':   estado = TELA_LIMPANDO_MEMORIA;               break;
                                
                                case '*':   dispLCD_clr(); estado = TELA_CONFIGURACOES;   break;                        
                            }
                            break;
                            
             case TELA_LIMPANDO_MEMORIA:                               
                            EEPROM.formatar();                          //Formata a memoria - Trava sempre que executado no simulador
                            dispLCD_clr();
                            for(char i = 0; i <= 3; i++)
                            {
                                for(char j = 0; j <= 19; j++)
                                {
                                    dispLCD(i, j, "\377");
                                    delay(10);
                                }
                            }

                            for(char i = 0; i <= 3; i++)
                            {
                                for(char j = 0; j <= 19; j++)
                                {
                                    dispLCD(i, j, " ");
                                    delay(10);
                                }
                            }
                            dispLCD(1, 0, "       DELETED      ");         //EXIBE A MENSAGEM NA 1Âª LINHA DO DISPLAY
                            dispLCD(2, 0, "        MEMORY      ");
                            
                            setT1(1000);                                   //INICIA A CONTAGEM DO TEMPO DE 1seg
                            estado = TELA_LIMPANDO_MEMORIA_DELAY;       
                            break; 
                            
             case TELA_LIMPANDO_MEMORIA_DELAY:
                            if(!statusT1())                         //QUANDO TERMINAR A CONTAGEM PASSA PARA O PROXIMO ESTADO
                                estado = TELA_CONFIGURACOES;                    
                            break;   
                            
             case TELA_BUSCAR_SEQUENCIA:
                            IHM.clr();
                            IHM.print(" ESCOLHA A MEMORIA \n"  
                                      "QUE DESEJA EXECUTAR\n"
                                      " 0   1   2   3   4 \n"          //NESSA ETAPA SERIA DESEJAVEL QUE APARECESSE APENAS OS ESPAÃOS DISPONIVEIS
                                      " 5   6   7   8   9 \n");
                            estado = TELA_CARREGA_SEQUENCIA;
                            
             case TELA_CARREGA_SEQUENCIA:
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case '0': 
                                case '1': 
                                case '2': 
                                case '3':
                                case '4': 
                                case '5': 
                                case '6': 
                                case '7':
                                case '8': 
                                case '9':
                                           buscar_dado(10, tecla % 0x30, &init_cond);       //Busca o endereço de memoria onde foi salvo a condicao inicial dos atuadores e guarda na variavel "init_cond"                                         
                                           manipula_atuadores_init(init_cond);              //Realiza a manipulação dos atuadores conforme as condicoes iniciais da sequencia expressa pela variavel "init_cond"
                                           
                                           EEPROM.buscar(tecla % 0x30, vetor_aux);         //Busca a receita pressionada pelo usuario e insere em "vetor_aux"                                     
                                           pt = vetor_aux;
                                           
                                           if(*pt == 0xFF)                                  //Verifica se o primeiro byte do endereço de memoria esta no padrao defalt (0xFF) 
                                           {
                                               EEPROM.deletar(tecla % 0x30);
                                               vetor_aux[0] = 0;
                                           }
                                           
                                          
                                           for(char *ptr = vetor_aux; *ptr != 0; ptr++)     //Varre o vetor_aux e insere os passos na fila (vetor fila)
                                           {                                                  
                                                if(*ptr <= 0x64 || *ptr == 0xFE)
                                                {
                                                    decodifica(&*ptr);
                                                    fifo_add_control(*ptr);
                                                }
                                                else if (*ptr > 0x64 && *ptr <= 0xDC)
                                                {
                                                    decodifica(&*ptr);
                                                    fifo_add_tempo(*ptr);
                                                }
                                                else
                                                {    
                                                    decodifica(&*ptr);
                                                    switch( *ptr )
                                                    {
                                                        case 'A':
                                                        case 'B':
                                                        case 'C':
                                                        case 'D':
                                                        case 'a':
                                                        case 'b':
                                                        case 'c':
                                                        case 'd':                           
                                                                *ptr &= ~0x20;   //Converte os caracteres para maiusculo
                                                                break;
                                                    }            
                                                    alt_atuador(*ptr);                                                  
                                                    fifo_add( ler_atuador(*ptr) ? *ptr : *ptr|0x20 );
                                                }
                                                fifo_print();
                                           }                                      
                                           
                                           dispLCD_clr();
                                           estado = TELA_PRINTFILA;
                                           break;
                                           
                                case TECLA_DELETE:  estado = TELA_MENU;   break;  //Inserido para retornar a pagina anterior         
                            }
                            break;
                    
             case TELA_AJUSTE_INICIAL:
                            IHM.clr();                          
                            IHM.printpos(0, 0, "   AJUSTE O INICIO  ");      
                            IHM.printpos(1, 0, "     DO SISTEMA     ");      
                            estado = TELA_DISPLAY_ATUADORES;                
                            break;
                         
            case TELA_DISPLAY_ATUADORES:                        
                            IHM.printpos(3,0,"   A   B   C   D    ");                               
                            dispLCD_lincol(3, 4); dispLCD_dataReg(ler_atuador('A') ? '+' : '-');    //Inserido para pegar o status atual dos atuadores após retornar da tela posterior (TELA_CONTAR_PASSOS)
                            dispLCD_lincol(3, 8);dispLCD_dataReg(ler_atuador('B') ? '+' : '-');
                            dispLCD_lincol(3, 12);dispLCD_dataReg(ler_atuador('C') ? '+' : '-');
                            dispLCD_lincol(3, 16);dispLCD_dataReg(ler_atuador('D') ? '+' : '-');
                            estado = TELA_EDICAO_ATUADORES;
                            break;
                            
            case TELA_EDICAO_ATUADORES:
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case 'A':   alt_atuador(tecla); dispLCD_lincol(3, 4); dispLCD_dataReg(ler_atuador('A') ? '+' : '-');     break;
                                case 'B':   alt_atuador(tecla); dispLCD_lincol(3, 8); dispLCD_dataReg(ler_atuador('B') ? '+' : '-');     break;
                                case 'C':   alt_atuador(tecla); dispLCD_lincol(3,12); dispLCD_dataReg(ler_atuador('C') ? '+' : '-');     break;
                                case 'D':   alt_atuador(tecla); dispLCD_lincol(3,16); dispLCD_dataReg(ler_atuador('D') ? '+' : '-');     break;
                                case TECLA_ENTER:   estado = TELA_INSERIR_PASSOS;                            break;                        
                                case TECLA_DELETE:  estado = TELA_MENU;                                      break;   //Inserido para retonar a tela anterior
                            }
                            break;

                    // TELA DE EDIÃAO

            case TELA_INSERIR_PASSOS:
                            init_cond = salva_atuadores_init();	         //Guarda as condicoes iniciais dos atuares na variavel "init_cond"
                            IHM.clr();                             
                            IHM.printpos(0, 0, "  INSIRA OS PASSOS  ");     
                            estado = TELA_CONTAR_PASSOS;
                            break;

             case TELA_EDITAR_PASSOS:
                            tecla = teclado_borda();               
                            switch( tecla )
                            {
                                case 'A': 
                                case 'B': 
                                case 'C': 
                                case 'D': 
                                            if( fifo_disponivel() )                 //Se houver espaco disponivel na fila (ind_fila < tam_vetor) ELE EXECUTA:
                                            {   
                                                alt_atuador(tecla);                                     
                                                fifo_add( ler_atuador(tecla) ? tecla : tecla|0x20 );    
                                                estado = TELA_PRINTFILA;
                                            }
                                            break;
                                case TECLA_RIGHT:                                   //TECLA '6'
                                            fifo_indicePrint_inc();
                                            estado = TELA_PRINTFILA;
                                            break;
                                case TECLA_LEFT:                                    //TECLA '4'
                                            fifo_indicePrint_dec();
                                            estado = TELA_PRINTFILA;                        
                                            break;

                                case TECLA_DELETE:                                  //TECLA '*'
                                            if(fifo_indice() == 2 )                 //INserido para retornar a pagina anterior
                                                estado = TELA_AJUSTE_INICIAL;
                                            else
                                            {
                                                fifo_delete();
                                                estado = TELA_PRINTFILA;
                                            }
                                            break;    

                                case TECLA_ENTER:   estado = TELA_SALVAR_SEQUENCIA;          break;  

                                case TECLA_T:       estado = TELA_EDITATEMPO;                break;
                                
                                case TELA_SAIDA_ANL: estado = TELA_EDITACONTROLE;            break;
                            }
                            break; 

             case TELA_PRINTFILA:
                            fifo_print();
                            estado = TELA_CONTAR_PASSOS;
                            break;
                    
             case TELA_CONTAR_PASSOS:
                            IHM.printpos(3,0,"<  /  >");                         
                            IHM.printv(3, 1,( fifo_indice()-2 ), 2);           //EXIBE A MENSAGEM "0" NA  2ª LINHA DO DISPLAY - fifo_indice = 2, logo 2-2 = 0
                            IHM.printv(3, 4,( fifo_tam()-2 ), 2);              //EXIBE A MENSAGEM "18" NA 2ª LINHA DO DISPLAY - fifo_tam = 18, logo 18-2 = 16
                            estado = TELA_EDITAR_PASSOS;                       
                            break;
                    
             case TELA_EXECUTANDO_PASSOS:
                            //dispLCD_clr();                            
                            estado = ME_ESPERA_EXECUCAO;
                            meAtuadores = ME_ATUADORES_START;
                            break;
                 
             case ME_ESPERA_EXECUCAO:
                            if( meAtuadores == 3 && botao != PAUSE )
                            {                                
                                IHM.printpos(0, 0, "       [    ]       ");
                                printExec(1, -1);
                                printExec(9, 0);
                                printExec(14, 1);  
                                IHM.printpos(2, 0, "<  /  > c:          ");
                                IHM.printv(2, 4,( fifo_indice() - 2 ), 2);
                                IHM.printv(2, 1, ( getIndicePassos() - 1 ), 2);                                         
                                IHM.printv(2, 11, ( getContCiclos() + 1 ), 5);                  //getContCiclos retorna o valor de cont_ciclos = 0;                         
                                
                            }
                            if(meAtuadores == 0)
                            {
                                estado = TELA_PRINTFILA;
                                IHM.clr();
                            }
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case TECLA_PLAY:
                                    botao = PLAY;
                                    break;
                                
                                case TECLA_PAUSE:
                                    botao = PAUSE;
                                    break;
                                    
                                case TECLA_STOP:
                                    botao = STOP;
                                    break;
                                   
                                case TECLA_STEP:
                                    if(botao == PAUSE)
                                        botao = STEP;
                                    break;
                            }
                            break;
                 
             case CONFIG_TEMPO:
                            tecla = teclado_borda();             //VARIAVEL RECEBE O VALOR PRESSIONADO PELO USUARIO
                            if(tecla >= '0' && tecla <= '9')     
                            {
                                dignum_conc(tecla, &num);
                                if(num > 120)
                                    num = 120;
                                estado = TELA_EDITATEMPO;                                
                            }
                            if(tecla == TECLA_DELETE)
                            {
                                dignum_apagar(&num);
                                estado = TELA_EDITATEMPO;
                            }
                            if(tecla == TECLA_ENTER)
                                estado = INSERIR_TEMPOFILA;
                            break;
                            
             case TELA_EDITATEMPO:
                            //dispLCD(1, 0, "cfg tempo:     s");
                            IHM.printpos(2, 0, "config tempo:      s");
                            IHM.printv(2, 15, num, 3);
                            estado = CONFIG_TEMPO;
                            break;
                            
             case INSERIR_TEMPOFILA:
                            fifo_add_tempo(num);
                            IHM.clr();
                            estado = TELA_PRINTFILA;
                            break;
                            
//             case TELA_TITULO_REPETICAO:
//                            IHM.clr();
//                            dispLCD(0, 0, "    Tela de     ");
//                            dispLCD(1, 0, "   Repeticao    ");
//                            setT1(2000);
//                            estado = TELA_DIGNUM_REPETICAO;
//                            break;
                 
//             case TELA_DIGNUM_REPETICAO:
//                            if(!statusT1())
//                            {
//                                IHM.clr();
//                                dispLCD(0, 0, "Digite o numero ");
//                                dispLCD(1, 0, " de repeticoes  ");
//                                setT1(2000);
//                                estado = TELA_CONFIG_REPETICAO;
//                            }
//                            break;
                
//             case TELA_DIGNUM_DELAY:
//                            if(!statusT1())
//                            {
//                               dispLCD_clr(); 
//                               estado = TELA_EDITA_REPETICAO;                    
//                            }
//                            break;
                 
                
             case TELA_CONFIG_REPETICAO:
                            tecla = teclado_borda();
                            if(tecla >= '0' && tecla <= '9')
                               {
                                dignum_conc(tecla, &ciclos);
                                if(ciclos > 10000)
                                    ciclos = 10000;
                                estado = TELA_EDITA_REPETICAO;
                                break;
                               }
                            switch(tecla)
                               {
                                   case 'A':
                                   case 'B':
                                   case 'C':
                                   case 'D':
                                       dispLCD_clr();
                                       estado = TELA_PRINTFILA;
                                       break;
                                   case '#':
                                       estado = ME_INSERIR_CICLO_FILA;
                                       break;
                                   case '*':
                                       dignum_apagar(&ciclos);
                                       estado = TELA_EDITA_REPETICAO;
                                       break;
                               }                    
                            break;                
                 
             case TELA_EDITA_REPETICAO: 
                            IHM.clr();
                            IHM.printpos(0, 0, "Ciclos:             ");
                            IHM.printv(0, 15, ciclos, 5);
                            IHM.printpos(2, 0, "0 - Ciclo continuo  ");
                            estado = TELA_CONFIG_REPETICAO;
                            break;
                            
             case TELA_SALVAR_SEQUENCIA:
                            salvar_dado(10, 10, init_cond);          //inclui as condicoes iniciais do atuadores expressa pela variavel "init_cond" na memoria para usuario executar a ultima condi��o inicial realizada
                            EEPROM.salvar(11, fifo_adrs() + 2);      //SALVA A SEQUENCIA PARA USUARIO EXECUTAR A ULTIMA SEQUENCIA REALIZADA 
                            IHM.print("    DESEJA SALVAR   \n"
                                      "     SEQUENCIA ?  \n\n"
                                      "# - SIM      * - NAO");
                            
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case '#':                  estado = SALVAR_SEQUENCIA_MEMORIA;        break;
                                case '*':   dispLCD_clr(); estado = TELA_EDITA_REPETICAO;            break;                        
                            }
                            break;
                            
             case SALVAR_SEQUENCIA_MEMORIA: 
                            IHM.clr();
                            IHM.print(" ESCOLHA A MEMORIA\n"
                                      " QUE DESEJA SALVAR\n"
                                      " 0   1   2   3   4\n"         //NESSA ETAPA SERIA DESEJAVEL QUE APARECESSE APENAS OS ESPAÃOS DISPONIVEIS
                                      " 5   6   7   8   9  ");  
                            estado = ESPERA_TECLA_SALVAR_SEQUENCIA;
                            
             case ESPERA_TECLA_SALVAR_SEQUENCIA:
                            tecla = teclado_borda();
                            switch(tecla)
                            {
                                case '0': 
                                case '1': 
                                case '2': 
                                case '3':
                                case '4': 
                                case '5': 
                                case '6': 
                                case '7':
                                case '8': 
                                case '9':
                                           salvar_dado(10, tecla % 0x30, init_cond);      //inclui as condicoes iniciais do atuadores expressa pela variavel "init_cond" na memoria 
                                           EEPROM.salvar(tecla % 0x30, fifo_adrs() + 2);  //inclui a sequencia na posicao "x" escolhida da memoria
                                           estado = TELA_SALVANDO_SEQUENCIA;
                                           break;
                                           
                                case TECLA_DELETE:  IHM.clr();   estado = TELA_SALVAR_SEQUENCIA;       break;   //Inserido para retonar a tela anterior           
                            }
                            break;
                            
             case TELA_SALVANDO_SEQUENCIA:                                            // TELA DE PROCESSAMENTO INICIAL (CARREGA BARRINHAS)
                            IHM.clr();
                            for(char i = 0; i <= 3; i++)
                            {
                                for(char j = 0; j <= 19; j++)
                                {
                                    IHM.printpos(i, j, "\377");
                                    delay(10);
                                }
                            }

                            for(char i = 0; i <= 3; i++)
                            {
                                for(char j = 0; j <= 19; j++)
                                {
                                    IHM.printpos(i, j, " ");
                                    delay(10);
                                }
                            }
                            IHM.printpos(1, 0, "      SEQUENCIA     ");          //EXIBE A MENSAGEM NA 1Âª LINHA DO DISPLAY
                            IHM.printpos(2, 0, "        SALVA       ");
                            
                            setT1(1000);                                   //INICIA A CONTAGEM DO TEMPO DE 1seg
                            estado = TELA_SALVANDO_SEQUENCIA_DELAY;        //MUDA PARA O PROXIMO ESTADO
                            break; 
                            
             case TELA_SALVANDO_SEQUENCIA_DELAY:
                            if(!statusT1())                         //QUANDO TERMINAR A CONTAGEM PASSA PARA O PROXIMO ESTADO
                                estado = TELA_EDITA_REPETICAO;                    
                            break;             
                     
             case TELA_EDITACONTROLE:
                            IHM.printpos(2, 0, "config contrl:     %");
                            IHM.printv(2, 15, num, 3);
                            estado = CONFIG_CONTROLE;
                            break;
                            
             case CONFIG_CONTROLE:
                            tecla = teclado_borda();             //VARIAVEL RECEBE O VALOR PRESSIONADO PELO USUARIO
                            if(tecla >= '0' && tecla <= '9')     //
                            {
                                dignum_conc(tecla, &num);
                                if(num > 100)
                                    num = 100;
                                estado = TELA_EDITACONTROLE;                                
                            }
                            if(tecla == TECLA_DELETE)
                            {
                                dignum_apagar(&num);
                                estado = TELA_EDITACONTROLE;
                            }
                            if(tecla == TECLA_ENTER)
                                estado = INSERIR_CONTROLEFILA;
                            break;
                            
             case INSERIR_CONTROLEFILA:
                            fifo_add_control(num);
                            IHM.clr();
                            estado = TELA_PRINTFILA;
                            break;
                    
             case ME_INSERIR_CICLO_FILA:
                            fifo_add_ciclo(ciclos);
                            estado = TELA_EXECUTANDO_PASSOS;
                            botao = PLAY;
                            break;
        }
 
        serialIOscan();     
 
        switch(meAtuadores)
        {
            case 0:         break;
            
            case 1:
                            resetIndicePassos();
                            resetContCiclos();                    
                            meAtuadores = 2;
                            break;

            case 2:
                            auxPasso = fifo_lerPos(getIndicePassos()); //auxPasso = fifo[2]
                           
                            if(botao == PLAY || botao == STEP || botao == STOP)
                                meAtuadores = 3;
                            break;

            case 3:
                            if(auxPasso <= 0x64 || auxPasso == 0xFE)
                            {
                                decodifica(&auxPasso);
                                MCP4725((long)((4095L * auxPasso)/100));
                                meAtuadores = 6;
                            } 
                            else if( auxPasso > 0x64 && auxPasso <= 0xDC )   
                            {
                                auxPasso -= 0x64;
                                setT1(auxPasso  * 1000 );
                                meAtuadores = 4;
                            }
                            else
                            {
                                decodifica(&auxPasso);
                                set_passo(auxPasso, vetorOut);
                                wifi.cipsend(1,vetorOut);
                                meAtuadores = 5;
                            }
                            break;

            case 4:
                            if(!statusT1())
                                meAtuadores = 6;
                            break;

            case 5:
                            if(ler_sensor(auxPasso, vetorIn))
                                meAtuadores = 6;
                            break; 
                    
            case 6:
                            if(botao == STEP)
                            {
                                botao = PAUSE;
                            }
                            meAtuadores = 7;
                            
                            break;
                            
            case 7:
                            addIndicePassos();
                            if(getIndicePassos() < fifo_indice())
                                meAtuadores = 2;
                            else
                            {
                                addContCiclos();
                                meAtuadores = 8;
                            }
                            break;
                    
            case 8:
                            if((botao != STOP) && ((getContCiclos() < getCiclos()) || (getCiclos() == 0)))
                            {
                                resetIndicePassos();
                                meAtuadores = 2;
                            }
                            else
                            {
                                meAtuadores = 0;                        
                            }
                            break;               
        }
    }
    return;
}

