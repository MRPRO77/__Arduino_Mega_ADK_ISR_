/************************************************************************************************************

                                      configurações Arduino Mega ADK
************************************************************************************************************/

                                   /* Arquivos de Bibliotecas */

 // ========================================================================================================
// --- Bibliotecas Auxiliares --- //

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Wire.h"




/**********************************************************************************************************/
                                        /* Arquivos de inclusão */


#include "Config_Sete_seg.h"
#include "Config_Comunc_I2C.h"

#define disp1    1                             //Display unidade   1
#define disp2    2                             //Display dezena    2
#define disp3    3                             //Display centena   3
#define disp4    4                             //Display milhar    4

#define mais    50                            //Botão para incremento no digital A0
#define menos   51                            //Botão para decremento no digital A1

#define ledpin_MEGA_2560           13         //numero do pino onde o LED Verde supervisionamento do funcionamento do codigo



// --- Variáveis Globais ---
int  mil, 
    cent,
     dez,                                       //Armazena o valor da dezena
     uni,                                       //Armazena o valor da unidade
    counter = PORTF;                            //Variável para contagem
    
boolean flagMais, 
        flagMenos;




 void config_Sete_seg ()
 {

   for(char i= A0; i< A8; i++)    pinMode(i,       OUTPUT); //Saídas (barramento do display no PORTL)
   for(char j=  1; j<  4; j++)    pinMode(j,       OUTPUT); //Saídas (controle do display)
   for(char k= 50; k< 51; k++)    pinMode(k, INPUT_PULLUP); //Entrada botões
   



   digitalWrite(disp1, LOW);
   digitalWrite(disp2, LOW);
   digitalWrite(disp3, LOW);
   digitalWrite(disp4, LOW);


   
   flagMais  = 0x00; 
   flagMenos = 0x00;

  pinMode (ledpin_MEGA_2560, OUTPUT);                // configura o pino do LED 13 como saida
  digitalWrite(ledpin_MEGA_2560,LOW);  

 

 }


// --- Desenvolvimento das Funções Auxiliares ---
int display7seg(int number)
{
  int catodo;                                  //Variável que receberá o número indexado ao vetor
  
  //Declaração e inicialização do vetor segmento
  int segmento[10] = {0x3f,                    //Dado referente ao BCD '0'
                      0x06,                    //Dado referente ao BCD '1'
                      0x5b,                    //Dado referente ao BCD '2'
                      0x4f,                    //Dado referente ao BCD '3'
                      0x66,                    //Dado referente ao BCD '4'
                      0x6d,                    //Dado referente ao BCD '5'
                      0x7d,                    //Dado referente ao BCD '6'
                      0x07,                    //Dado referente ao BCD '7'
                      0x7f,                    //Dado referente ao BCD '8'
                      0x67};                   //Dado referente ao BCD '9'
  
  catodo = segmento[number];  
  
  return(catodo);

} //end display7seg


void multiplex(int value)
{
   mil = (value/1000);
   PORTF = display7seg(mil);
   digitalWrite(disp1, LOW);   
   digitalWrite(disp2, LOW);
   digitalWrite(disp3, LOW);   
   digitalWrite(disp4, HIGH);
   delay(1);
   digitalWrite(disp4, LOW);

   cent = ((value/100)%10);
   PORTF = display7seg(cent);
   digitalWrite(disp4, LOW);   
   digitalWrite(disp1, LOW);
   digitalWrite(disp2, LOW);   
   digitalWrite(disp3, HIGH);
   delay(1);
   digitalWrite(disp3, LOW);

   dez = ((value/10)%10);
   PORTF = display7seg(dez);
   digitalWrite(disp3, LOW);   
   digitalWrite(disp4, LOW);
   digitalWrite(disp1, LOW);   
   digitalWrite(disp2, HIGH);
   delay(1);
   digitalWrite(disp2, LOW);
   
   uni = ((value/1)%10);
   PORTF = display7seg(uni);
   digitalWrite(disp2, LOW);   
   digitalWrite(disp3, LOW);
   digitalWrite(disp4, LOW);   
   digitalWrite(disp1, HIGH);
   delay(1);
   digitalWrite(disp1,  LOW);

} //end multiplex


void changeCounter()
{

   
   if(!digitalRead( mais)) flagMais  = 0x01;
   if(!digitalRead(menos)) flagMenos = 0x01;
   
   if(digitalRead(mais) && flagMais)
   {
      flagMais = 0x00;
      counter++;
      
      if(counter == 9999) counter = 0;
   }
   
   if(digitalRead(menos) && flagMenos)
   {
      flagMenos = 0x00;
      counter--;
      
      if(counter == 0) counter = 9999;
   }


} //end changeCounter



// ======================================================================================================
// --- Função de Supervisionamento do programa ---

void Supervisionamento()
{
   

  static unsigned long ultimaMudanca = 0;
 
  if (millis() - ultimaMudanca >= 1000)
  {
    int estado = !digitalRead (ledpin_MEGA_2560);
    digitalWrite(ledpin_MEGA_2560, estado);
    ultimaMudanca = millis();

  }

    
}




