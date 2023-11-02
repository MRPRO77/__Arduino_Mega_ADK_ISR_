/************************************************************************************************************

                                      configurações Mega ADK
************************************************************************************************************/

                                   /* Arquivos de Bibliotecas */

 // ========================================================================================================
// --- Bibliotecas Auxiliares --- //

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Wire.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BusIO_Register.h>
#include <TCA9548A.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SSD1306.h>
#include <U8glib-HAL.h>


/**********************************************************************************************************/
                                        /* Arquivos de inclusão */


#include "Config_Comunc_I2C.h"
#include "Config_Sete_seg.h"

TCA9548A I2CMux;                            // Address can be passed into the constructor


// ========================================================================================================
//                                   --- Definições e Objetos ---
 


  
/*   Configuração ESP32*/  
#define buttonPin 12 // numero do pino onde o botao esta conectado ESP32

boolean buttonState;             // estado atual do botao
boolean lastButtonState = LOW;   // valor da ultima leitura do botao
boolean ledState = HIGH;         // estado atual do LED

// as variaveis a seguir sao do tipo long por conta que o tempo, medido 
// em milessegundos alcancara rapidamente um numero grande demais para 
// armazenar em uma variavel do tipo int
unsigned long lastDebounceTime = 0;  // tempo da ultima modificacao do estado do LED

// tempo de debounce; aumentar se o LED oscilar; espera-se que o LED acenda
// apenas se o botao for pressionado por mais de 50ms
unsigned long debounceDelay = 2;    
/*    ------------------------------------------------------  -------- --------------*/



/*   Configuração Arduino Uno */  
#define buttonPin_UNO 14 // numero do pino onde o botao esta conectado Arduino Uno

boolean buttonState_UNO;             // estado atual do botao
boolean lastButtonState_UNO = LOW;   // valor da ultima leitura do botao
boolean ledState_UNO = HIGH;         // estado atual do LED

// as variaveis a seguir sao do tipo long por conta que o tempo, medido 
// em milessegundos alcancara rapidamente um numero grande demais para 
// armazenar em uma variavel do tipo int
unsigned long lastDebounceTime_UNO = 0;  // tempo da ultima modificacao do estado do LED

// tempo de debounce; aumentar se o LED oscilar; espera-se que o LED acenda
// apenas se o botao for pressionado por mais de 50ms
unsigned long debounceDelay_UNO = 2;    


/*    ------------------------------------------------------  -------- --------------*/






#define ledpin_MEGA_2560           13                  // numero do pino onde o LED Verde supervisionamento do funcionamento do codigo

#define ledpin_Oled_Blue            6                  //  Led_Blue  supervisão Interrupção
//#define ledpin_Oled_Green        23                  //  Led_Green supervisão Interrupção

#define slaveAdress       0x07                         // ESP32
#define slaveAdress_1     0x08                         // Arduino Uno

#define i2clcd          0                              // Display Lcd
#define i2cOled         1                              // Display Oled
#define i2cESP32        2                              // ESP32
#define i2cArduino_uno  7                              // Arduino Uno






#define usalcd              I2CMux.openChannel(i2clcd)
#define usaOled             I2CMux.openChannel(i2cOled)
#define usaESP32            I2CMux.openChannel(i2cESP32)
#define usaArduino_uno      I2CMux.openChannel(i2cArduino_uno)


#define OLED_RESET   3
Adafruit_SSD1306 display(OLED_RESET);               // configura tela de Oled

LiquidCrystal_I2C lcd(0x27,20,4);                   // set the LCD address to 0x27 for a 16 chars and 2 line display
 
U8GLIB_ST7920_128X64_1X u8g( 6,  //E
                             5,  //R/W
                             4,  //RS
                             7); //RST

     boolean control = 0x00;        //flag de controle
    int counter_Oled = 0x00;        //contador auxiliar


float temperatura = 0x00;
byte byte1, byte2, byte3, byte4;
unsigned int aux_temp;


float umidade = 0x00;
byte byte5, byte6, byte7, byte8;
unsigned int aux_umid;

// --- Protótipo das Funções ---
void disp_temp(); //mostrar a temperatura em Graus Celsius
void disp_umid(); //mostrar a umidade relativa do ar

// ========================================================================================================
// --- Interrupção  Oled_1 ---

ISR(TIMER2_OVF_vect)
{
    TCNT2=100;                //Reinicializa o registrador do Timer2

    counter_Oled++;              //incrementa counter
    
    if(counter_Oled == 400 )     //counter igual a 400?
    {                           //sim...
    
       counter_Oled = 0x00;      //reinicia counter
          
           
       
                      digitalWrite(ledpin_Oled_Blue,  !digitalRead(ledpin_Oled_Blue));    //Inverte o estado da saída
                   // digitalWrite(ledpin_Oled_Green, !digitalRead(ledpin_Oled_Green));   //Inverte o estado da saída
      
       control = !control;


    
    } //end if counter
    
} //end ISR

// ========================================================================================================
// --- Desenvolvimento das Funções ---
void disp_temp()
{
                       usaOled; 
        display.clearDisplay();
        display.setTextSize(1);
   display.setTextColor(WHITE);
       display.setCursor(30,0);
  display.print("Temperatura");
        display.setTextSize(2);
      display.setCursor(16,14);
   display.setTextColor(WHITE);
    display.print(temperatura);
           display.print(" *C");
             display.display();
        Wire.endTransmission();                      // encerra a transmissao                     
        I2CMux.closeChannel(1);            
 
} //end disp_temp


void disp_umid()
{
                       usaOled;
        display.clearDisplay();
        display.setTextSize(1);
   display.setTextColor(WHITE);
       display.setCursor(40,0);
      display.print("Umidade");
        display.setTextSize(2);
   display.setTextColor(WHITE);
      display.setCursor(19,14);
        display.print(umidade);
           display.print(" %");
             display.display();
        Wire.endTransmission();                      // encerra a transmissao                     
        I2CMux.closeChannel(1); 
} //end umid_temp


void Config_Comunc_I2C()
{
     
                                                    Wire.begin(); 
                                            Serial.begin(115200);
                                              I2CMux.begin(Wire);               // Wire instance is passed to the library 
                                           

                                          

// ========================================================================================================
// --- Inicialização da Interrupção do OLed ---

  TCCR2A = 0x00;   //Timer operando em modo normal
  TCCR2B = 0x07;   //Prescaler 1:1024
  TCNT2  = 100;    //10 ms overflow again
  TIMSK2 = 0x01;   //Habilita interrupção do Timer2

                                                         usaESP32;
                                                       delay(100);
                                           I2CMux.closeChannel(2); 

                                                           usalcd;
                                                       lcd.init();
                                                      lcd.clear(); 
                                                  lcd.backlight();               
                                           I2CMux.closeChannel(0);

                                                           usalcd;
                                                      lcd.clear(); 
                                               lcd.setCursor(2,1);
                                    lcd.print("MRPRO TECNOLOGIA");
                                                       delay(200);
                                           I2CMux.closeChannel(0);                                           

                        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);               // initialize with the I2C addr 0x3C (for the 128x64)
                                                      delay(3000);
                                           display.clearDisplay();

                                                          usaOled;
                        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
                                           display.clearDisplay();
                                           I2CMux.closeChannel(1);          
 
                                                          usaOled;
                                           display.clearDisplay(); 
                                      display.setTextColor(WHITE);
                                          display.setCursor(25,1);
                                 display.println("Loja Saravati");
                                      display.setTextColor(WHITE);
                                         display.setCursor(25,15);
                                 display.println("Mestre Felipe");
                                                      delay(2000);
                                                display.display();
                                           I2CMux.closeChannel(1);

                                                   usaArduino_uno;
                                                       delay(100);
                                           I2CMux.closeChannel(3);                                            
                                      
                                                           draw();                //função para desenho de string
                                                disp_graph_init();                //função de inicialização do display                                              

                                pinMode (ledpin_MEGA_2560, OUTPUT);                // configura o pino do LED 13 como saida
                                digitalWrite(ledpin_MEGA_2560,LOW);  




                               pinMode (ledpin_Oled_Blue, OUTPUT);                // configura o pino 22  como saida
                               digitalWrite(ledpin_Oled_Blue,LOW);

                         //     pinMode (ledpin_Oled_Green, OUTPUT);                // configura o pino 23  como saida
                         //     digitalWrite(ledpin_Oled_Green,LOW);
                            
                              
  // configura o pino do botao como entrada com resistor de pullup interno ESP32
                                 pinMode(buttonPin, INPUT_PULLUP);                             


  // configura o pino do botao como entrada com resistor de pullup interno  Arduino Uno
                                 pinMode(buttonPin_UNO, INPUT_PULLUP);   


}


// ======================================================================================================
// --- Função para desenho de strings ---
void draw() 
{
  
  u8g.setFont(u8g_font_unifont);
 
  u8g.drawStr( 0, 25, "MRPRO TECNOLOGIA");
  
} //end draw


// ======================================================================================================
// --- Função de inicialização do Display ---
void disp_graph_init()
{
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     //branco
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         //máxima intensidade
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

    u8g.firstPage();  
  do {
                        draw();
     } while( u8g.nextPage() );
  
                    delay(100);

} //end disp_graph_init





void comunicacao_UNO ()
{


   // le o estado do botao e salva em uma variavel local
  int reading_UNO = digitalRead(buttonPin_UNO);

  // verifica se voce apenas apertou o botao (i.e. se a entrada foi de LOW 
  // to HIGH), e se ja esperou tempo suficiente para ignorar qualquer ruido

  // se a entrada foi alterada devido ao ruido ou botao ter sido pressionado:
  if (reading_UNO != lastButtonState_UNO) {
    // reseta o tempo do debounce
    lastDebounceTime_UNO = millis();
  }
  
  if ((millis() - lastDebounceTime_UNO) > debounceDelay_UNO) {
    // qualquer que seja a leitura atual, ela se manteve por um tempo maior
    // que o nosso debounce delay, então atualizemos o estado atual:

    // se o estado do botao foi alterado:
    if (reading_UNO != buttonState_UNO) {
      buttonState_UNO = reading_UNO;

      // apenas altera o estado do LED se o novo estado do botao e HIGH
      if (buttonState_UNO == HIGH) {
        ledState_UNO = !ledState_UNO;
        // incia a transmissao para o endereco 0x07 (slaveAdress)
                                                        usaArduino_uno;
                                 Wire.beginTransmission(slaveAdress_1);
                                              Wire.write(ledState_UNO); // envia um byte contendo o estado do LED
                                                Wire.endTransmission(); // encerra a transmissao          
                                                I2CMux.closeChannel(7); 
      }
    }
  }
  // salva a leitura. No proximo laco este sera o ultimo 
  // estado do botao (lastButtonState)
  lastButtonState_UNO = reading_UNO;
 
}










void comunicacao ()
{


   // le o estado do botao e salva em uma variavel local
  int reading = digitalRead(buttonPin);

  // verifica se voce apenas apertou o botao (i.e. se a entrada foi de LOW 
  // to HIGH), e se ja esperou tempo suficiente para ignorar qualquer ruido

  // se a entrada foi alterada devido ao ruido ou botao ter sido pressionado:
  if (reading != lastButtonState) {
    // reseta o tempo do debounce
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // qualquer que seja a leitura atual, ela se manteve por um tempo maior
    // que o nosso debounce delay, então atualizemos o estado atual:

    // se o estado do botao foi alterado:
    if (reading != buttonState) {
      buttonState = reading;

      // apenas altera o estado do LED se o novo estado do botao e HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        // incia a transmissao para o endereco 0x07 (slaveAdress)
                                                              usaESP32;
                                   Wire.beginTransmission(slaveAdress);
                                                  Wire.write(ledState); // envia um byte contendo o estado do LED
                                                Wire.endTransmission(); // encerra a transmissao          
                                                I2CMux.closeChannel(2); 
      }
    }
  }
  // salva a leitura. No proximo laco este sera o ultimo 
  // estado do botao (lastButtonState)
  lastButtonState = reading;
 
}




void Comunicacao_Oled()
{

                                                     usaESP32;
                                       
                             Wire.requestFrom(slaveAdress, 8);
                                          byte1 = Wire.read();                            // Lê os 4 bytes enviados pelo mestre 
                                          byte2 = Wire.read(); 
                                          byte3 = Wire.read(); 
                                          byte4 = Wire.read();
                                          byte5 = Wire.read();                            // Lê os 4 bytes enviados pelo mestre 
                                          byte6 = Wire.read(); 
                                          byte7 = Wire.read(); 
                                          byte8 = Wire.read();

                                                                       // Ajustando os bytes recebidos para obetr a variavel_float
                                          aux_temp = (byte3<<8) | byte4;                       // Ajusta a parte fracionáia (depois da vírgula)
                                          temperatura = (float) (aux_temp*0.0001);             // Atribui a parte fracionária, depois da vírgula 
                                          aux_temp = (byte1<<8) | byte2;                       // Ajusta a parte inteira (antes da vírgula)
                                          temperatura += aux_temp;                             // Atribui a parte iteira


                                                                       // Ajustando os bytes recebidos para obetr a variavel_float
                                          aux_umid = (byte7<<8) | byte8;                       // Ajusta a parte fracionáia (depois da vírgula)
                                          umidade = (float) (aux_umid*0.0001);                 // Atribui a parte fracionária, depois da vírgula 
                                          aux_umid = (byte5<<8) | byte6;                       // Ajusta a parte inteira (antes da vírgula)
                                          umidade += aux_umid;                                 // Atribui a parte iteira
                                      
                                           Wire.endTransmission();                             // encerra a transmissao                     
                                           I2CMux.closeChannel(2);  


                                                         
                                         if (control) disp_temp();
                                         else         disp_umid();
                                         
  
                                         




}