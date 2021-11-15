/*
 * 08:26 16/10/2020 - Luis - criação do arquivo de histórico
 * 09:33 26/10/2020 - Luis - implementação de passos de tempo, utilizando Timer1
 * atualização do codigo no qual foi feito uma limpa de funções que nao estavam sendo utilizadas
 * atualizando as existentes com auxilio do professor José, deixando o codigo mais simples de ser compreendido e modificado 
 * 10:35 28/10 - Lucas V - Atualização. Implementação da rotina de repetição de sequencias de passos. 
 * 08:22 30/11/2020 - Lucas V - Resolução de bug no retorno da tela de ciclos.
 * 
 *  
*/

/* Projeto Sequenciador Lógico Programável - Versão 2021
 * 
 * REALIZAR ATUALIZAÇÃO DO HISTÓRICO DIARIAMENTE - falta implementar conteudo
 *
 * /09/2021 - /09/2021 - Pedro - Desenvolvimento da biblioteca EEPROM
 * /09/2021 - /09/2021 - Guilherme - Inicio do desenvolvimento da biblioteca I2C para implementação do conversor digital-analógico MCP4725 - Pendente
 * /09/2021 - /09/2021 - Pedro e Carlos - Estudo para implementação da biblioteca  
 * 
 * Realizado modificação da paginação do dispositivo, a fim de propiciar uma melhora na interação com o usuario e facilidade de de navegação entre telas
 * 
 * 
 * 16/10/2021 - Guilherme - Inclusão da biblioteca EEPROM ao programa principal
 *                        - Desenvolvido a possibilidade de salvar e realizar o resgate de sequencias salvas na memoria
 * 18/10/2021 - Guilherme - Correção de bug - Correção para buscar o tempo correto inserido na sequencia extraida da memoria
 * 18/10/2021 -   Pedro   - Desenvolvimento de biblioteca I2C - upgrade 
 * 20/10/2021 - Guilherme - Correção de bug - Busca de sequencias vazias na memória
 * 21/10/2021 - Guilherme - 
 * 22/10/2021 - Guilherme - 
 * 23/10/2021 - Guilherme - Finalizado implementação da biblioteca EEPROM, de modo a possibiitar a busca de sequencias salvas na memoria, bem como o estado inicial dos atuadores para cada sequencia desejada
                            Para salvar as condições iniciais dos atuadores para cada sequência foram desenvolvidas e utilizadas as seguintes funções:
 * 
 *                          char salva_atuadores_init( void );	    - Funcao criada para retornar a condicao inicial dos atuadores para cada sequencia criada
 *                          void manipula_atuadores_init(char aux); - Funcao criada para manipular os atuadores conforme a condicao inicial buscada na memoria 
 *                          void salvar_dado (char adrs, char col, unsigned char data);  - funcao criada para salvar um unico dado de qualquer endereco de memoria da EEPROM
 *                          void buscar_dado (char adrs, char col, unsigned char *data); - Funcao criada para buscar um unico dado de qualquer endereco de memoria da EEPROM
 * 
 * 23/10/2021 -   Pedro   - Finalização da biblioteca I2C - Testes e simulação
 * 
 * 12/11/2021 - Guilherme - Realizado a troca do microcontrolador PIC16F887 pelo microcontrolador PIC18F4550, de modo a dispor de mais memória para execução do programa
 *                          * Realizado readequação das bibliotecas para o novo componente utilizado conforme datasheet  
 * 
 *                          
 
 */
