
/********************************Função que configura Pinos do MCU**************************************

                                   configuração para placa Mega ADK

********************************************************************************************************/
/*******************************************************************************************************/
                                      /* Arquivos de inclusão */

#ifndef Config_Sete_seg_h_
#define Config_Sete_seg_h_
               
               
// ======================================================================================================
// --- Protótipo das Funções ---

void config_Sete_seg();
 int display7seg(int number);                 //Retorna o dado para o barramento do display
void multiplex(int value);                   //Faz a multiplexação do display
void changeCounter();  
void Supervisionamento();

#endif