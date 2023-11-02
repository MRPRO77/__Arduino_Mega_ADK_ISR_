
/********************************Função que configura Pinos do MCU**************************************

                                   configuração para placa Mega ADK

********************************************************************************************************/
/*******************************************************************************************************/
                                      /* Arquivos de inclusão */

#ifndef Config_Comunc_I2C_h_
#define Config_Comunc_I2C_h_
               
               
// ======================================================================================================
// --- Protótipo das Funções ---

void Config_Comunc_I2C();

void       comunicacao();
void  comunicacao_UNO ();


             void draw();             //função para desenho de string
  void disp_graph_init();             //função de inicialização do display
 void Comunicacao_Oled();

        void disp_umid();
        void disp_temp();

#endif