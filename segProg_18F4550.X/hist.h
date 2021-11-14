/*
 * 08:26 16/10/2020 - Luis - cria��o do arquivo de hist�rico
 * 09:33 26/10/2020 - Luis - implementa��o de passos de tempo, utilizando Timer1
 * atualiza��o do codigo no qual foi feito uma limpa de fun��es que nao estavam sendo utilizadas
 * atualizando as existentes com auxilio do professor Jos�, deixando o codigo mais simples de ser compreendido e modificado 
 * 10:35 28/10 - Lucas V - Atualiza��o. Implementa��o da rotina de repeti��o de sequencias de passos. 
 * 08:22 30/11/2020 - Lucas V - Resolu��o de bug no retorno da tela de ciclos.
 * 
 *  
*/

/* Projeto Sequenciador L�gico Program�vel - Vers�o 2021
 * 
 * REALIZAR ATUALIZA��O DO HIST�RICO DIARIAMENTE - falta implementar conteudo
 *
 * /09/2021 - /09/2021 - Pedro - Desenvolvimento da biblioteca EEPROM
 * /09/2021 - /09/2021 - Guilherme - Inicio do desenvolvimento da biblioteca I2C para implementa��o do conversor digital-anal�gico MCP4725 - Pendente
 * /09/2021 - /09/2021 - Pedro e Carlos - Estudo para implementa��o da biblioteca  
 * 
 * Realizado modifica��o da pagina��o do dispositivo, a fim de propiciar uma melhora na intera��o com o usuario e facilidade de de navega��o entre telas
 * 
 * 
 * 16/10/2021 - Guilherme - Inclus�o da biblioteca EEPROM ao programa principal
 *                        - Desenvolvido a possibilidade de salvar e realizar o resgate de sequencias salvas na memoria
 * 18/10/2021 - Guilherme - Corre��o de bug - Corre��o para buscar o tempo correto inserido na sequencia extraida da memoria
 * 18/10/2021 -   Pedro   - Desenvolvimento de biblioteca I2C - upgrade 
 * 20/10/2021 - Guilherme - Corre��o de bug - Busca de sequencias vazias na mem�ria
 * 21/10/2021 - Guilherme - 
 * 22/10/2021 - Guilherme - 
 * 23/10/2021 - Guilherme - Finalizado implementa��o da biblioteca EEPROM, de modo a possibiitar a busca de sequencias salvas na memoria, bem como o estado inicial dos atuadores para cada sequencia desejada
                            Para salvar as condi��es iniciais dos atuadores para cada sequ�ncia foram desenvolvidas e utilizadas as seguintes fun��es:
 * 
 *                          char salva_atuadores_init( void );	    - Funcao criada para retornar a condicao inicial dos atuadores para cada sequencia criada
 *                          void manipula_atuadores_init(char aux); - Funcao criada para manipular os atuadores conforme a condicao inicial buscada na memoria 
 *                          void salvar_dado (char adrs, char col, unsigned char data);  - funcao criada para salvar um unico dado de qualquer endereco de memoria da EEPROM
 *                          void buscar_dado (char adrs, char col, unsigned char *data); - Funcao criada para buscar um unico dado de qualquer endereco de memoria da EEPROM
 * 
 * 23/10/2021 -   Pedro   - Finaliza��o da biblioteca I2C - Testes e simula��o
 * 
 * 
 *                          
 
 */