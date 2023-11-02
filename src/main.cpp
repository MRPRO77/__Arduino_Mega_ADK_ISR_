/*******************************************************************************
   Título:      TCC  Engenharia Mecatrônica.

   Descrição:   C L P   ---  I H M  -- Estação Metereologica

   Processador: ARDUINO MEGA ADK +  ROBOCORE ESP32 + Arduino Uno
   
   Geração Multifolhas


   Desenvolvido: MRPRO Tecnologia e Automação

   Autor: Tecnologo. Marcelo Rodrigues     Crea-SP 5070505617

   Data de Inicio: 02/11/2023       Versão:   2023.000.00

        

 
*******************************************************************************/  
/* ============================================================================  
                                                              
                                       _                      
                                      / \                     
                                     |oo >                    
                                     _\=/_                    
                    ___         #   /  _  \   #               
                   /<> \         \\//|/.\|\\//                
                 _|_____|_        \/  \_/  \/                 
                | | === | |          |\ /|                    
                |_|  0  |_|          \_ _/                    
                 ||  0  ||           | | |                    
                 ||__*__||           | | |                    
                |* \___/ *|          []|[]                    
                /=\ /=\ /=\          | | |                    
________________[_]_[_]_[_]_________/_]_[_\___________________________________
                                                                             
============================================================================== */

/******************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Wire.h"




#include "Config_Sete_seg.h"
#include "Config_Comunc_I2C.h"
#include "Config_Teclado_Matricial.h"

extern int counter;

// --- Configurações Iniciais ---
void setup()
{

    config_Sete_seg();
  Config_Comunc_I2C();





} //end setup


// --- Loop Infinito ---
void loop()
{
   
      Comunicacao_Oled();
           comunicacao();
       comunicacao_UNO();
         changeCounter();
      multiplex(counter);
     Supervisionamento();


} //end loop


