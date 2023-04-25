/********************************************************
 * Firmaware desenvolvido por Caio Dutra em 17/07/2019  *
 * **************************************************** *
 * Este firmware colhe pulsos gerados pelo sensor hall  *
 * Yf-S201 para medir a vazão de trasfego na linha de   *
 * produção de cerveja. O firmware também porta de uma  *
 * interface para comunição direta com ser humano.      *
 * **************************************************** *
 ********************************************************/

 /* Ver.3.2 *********************************************
 * **************************************************** *
 * Esta versão implementa a segunda opção do menu       *
 * 2 - Trasfego Programado, de modo completo.           *
 * Esta versão também corrige o problema de volumes     *
 * programados com casas decimais, utilizando mais uma  *
 * posição de memória para isso (Ver nas rotinas de     *
 * escrita e leitura de memória)                        *
 ********************************************************/

/*
 * Bibliotecas
 */
#include<LiquidCrystal.h>
#include <EEPROM.h> // EEPROM.write(ADDRESS,DATA); EEPROM.read(ADDRESS);

/*
 * #define
 */
#define botao1 7 // cima
#define botao2 8 // baixo
#define botao3 9 // esqueda
#define botao4 10 // direita
#define driver_pin A5 // pino do driver que aciona bomba 


LiquidCrystal lcd(12, 11, 6, 5, 4, 3); // Cria um objeto lcd e atribui os pinos
/*********************************
 * ******************************
 * DECLARAÇÕES DE FUNÇÕES *******
 * ******************************
 ********************************/



/*****************************
********* Variáveis **********
*****************************/

/*
 * Double
 */
double pulso = 0; // varíavel que conta o número de pulsos do sensor hall num intervalo de tempo
double VOL_prog; // variável que auxiliará cálculo de correção de unidade (ml para litros) na opção 2, por exemplo
/*
 * Float
 */
float  freq = 0.000;
float vazao = 0.000;
float ml=0.00, ml_tot=0.00; // variáveis que contam a quantidade de volume trasfegado (em litros, apesar de chamarem 'ml')

float vol_prog =0.00; // variável que irá segurar o valor lido de memória para trasfego programado
float VOL; // varível que guarda valor lido de volumes msB e lsB

float erro = 0.00; // variável que calcula erro para fazer previsão de quando desligar bomba
float previwed_val = 0.00; // variável que irá contabilizar momento de desligar a bomba
/*
 * int
 */
long cont_init, cont_end;
long cont_micro = 0,aux_cont_micro = 0;

int menu_opt = 1; // Menu_Option => 1- primeira opção do menu () | 2 - segunda opção do menu () | 3 - terceira opção do menu () | 4 - quarta opção do menu ()
int aux_menu = 0; // aux_menu = 0 -> Sai do laço do menu | aux_menu = 1 -> fica preso no laço de menu 
int aux_menu_inner = 0;

unsigned int aux = 0; // variável que segura laço de contagem 
 


// variáveis que leem as memoria
unsigned int D_msB,D_lsB; // data 1 e data 2 respectivamente (Most Significative Byte e Less Significative Byte)
int inc; // variável que lê se unidade de medida do incremento esta em ml ou litros => 0 p/ ml & 1 p/ litros (esta informação será salva na posição 10 da memória EEPROM)


/****************************
*****************************
****************************/

/*****************************
*********   Função  **********
*****   de interrupção *******
*****************************/

void contapulso () // interrupção para contar pulsos
{
 pulso++;
}


/****************************
*****************************
****************************/

/*******************************************************
********************************************************
************************ SUBROTINAS ********************
* ******************************************************
********************************************************/

/*****************************
*********   Função  **********
*** Acionamento & medição ****
*****************************/

void meter_on (void) // função que liga bomba e medidor
{
 
 attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING);// liga interrupção externa
 ml_tot =0.00; // condição inicial
 ml = 0; // condição inicial
 
 

  /*lcd.setCursor(0,0);
  lcd.print(erro);*/

  
 while(aux == 1) // condição para a contagem ficar repetindo
{

  
cont_end = millis(); // vê em quanto está a contagem atual de millis()


 
if(cont_end >= (cont_init+50)) //  se tiver passado 0 ms, faz a contagem de quantos litros passaram pelo medidor
  {
 detachInterrupt(contapulso); // desliga interrupção
 
 
 freq = (pulso/(cont_end - cont_init))*1000; // frequência  =[ número de pulsos / intervalo de tempo (50ms) ] * 1000(fator para colocar a frequência em hertz HZ, pois o delta de t está em ms)  
 cont_init = cont_end; // atualiza variável que é o início da contagem
 pulso = 0; // zera a variável que conta os pulsos do sensor
 
 vazao = freq/7.5; // equação do sensor Q (l/min) = freq (Hz) / 7.5 (Hz*min/l) -> Valor de calibração para 1 segundo de leitura
 ml = (vazao / (60*20)); // transforma a vazao em quantidade de volume trasfegado num intervalo de 50ms -> cabem 20 50ms em 1 s, por isso o fator de 20 no denominador
 ml_tot += ml; // acumula o valor de volume trasfegado a cada 50ms
 vazao = 0.00; // reseta o valor da varíavel para uma nova leitura
 

  lcd.clear(); // limpa lcd
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("Volume:"); //Escreve Volume:
  lcd.setCursor(6, 1); //Posiciona o cursor na coluna 6 e linha 1 do LCD
  lcd.print("litros");//Escreve litros (unidade de medida da contagem)
  lcd.setCursor(0, 1);  //Posiciona o cursor na coluna 0 e linha 1 do LCD
  lcd.print(ml_tot+erro); // printa o valor total de volume trasfegado até o momento

  /*
   * Texto para TESTE
   */

 /* lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("e:");
  lcd.print(erro);
  lcd.setCursor(9,0);
  lcd.print("v+e");
  lcd.print(ml_tot + erro);
  lcd.setCursor(0,1);
  lcd.print("VL:");
  lcd.print(ml_tot);

  lcd.setCursor(8,1);
  lcd.print("VP:");
  lcd.print(vol_prog);*/

  /*
   * FIM Textos para TESTES
   */

  /*
   * Curvas de ERRO
   */

 //reta ajustada de erro
 // erro = 0.0561*(ml_tot) - 0.0271; // calcula erro estimado
 

 //polinômio ajustado de erro
 // erro = (0.0298*(pow(ml_tot,2))) - (0.0183*(ml_tot)) + 0.0101; // até 2 litros
    erro = (-0.0108*pow(ml_tot,3)) + (0.0659*pow(ml_tot,2))- (0.0529*ml_tot)+ 0.0193; // até 3 litros (média para 3 litros com 7 amostras)

  //exponêncial ajustada
 //   erro = 0.0032*(exp(1.65*ml_tot));

  // potência ajustada:
  //  erro = 0.0235*(pow(ml_tot,1.9348));


  /*
   * FIM Curvas de ERRO 
   */
  

  
  
  
 attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING); // liga a interrupção para uma nova contagem de pulsos
  }


  /*
   * Condição de parada para as opções
   */
                  /*
                   * OPÇÃO 2
                   */
                  if(menu_opt == 2)
                  {
                    if(((ml_tot)+erro) >= vol_prog)
                      {
                         digitalWrite(driver_pin,LOW); // desliga driver da bomba
                         aux = 0;
                         
                         detachInterrupt(contapulso);
                         
                         while(digitalRead(botao1) == LOW)// agurda usuário apertar pra cima para liberar equipamento
                         {
                          
                                    /*********************
                                    * Tela de descanso  *
                                     *********************/
                                          
                                           if((millis() - cont_micro) >= 2000 ) // a cada 2 segundos troca o texto da tela
                                            {
                                              lcd.clear();
                                              aux_cont_micro = !(aux_cont_micro);
                                              cont_micro = millis();
                                            }
                                            
                                            if(aux_cont_micro == 0) // texto 1
                                            {
                                            
                                           
                                            lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
                                            lcd.print("Vol. Trasfegado:"); //Escreve Volume:
                                            lcd.setCursor(6, 1); //Posiciona o cursor na coluna 6 e linha 1 do LCD
                                            lcd.print("litros");//Escreve litros (unidade de medida da contagem)
                                            lcd.setCursor(0, 1);  //Posiciona o cursor na coluna 0 e linha 1 do LCD
                                            lcd.print(ml_tot+erro); // printa o valor total de volume trasfegado até o momento
                                            }
                                          
                                            else if(aux_cont_micro == 1) // texo 2
                                            {
                                            
                                            lcd.setCursor(0, 0); 
                                            lcd.print("P/ Sair aperte");
                                            lcd.setCursor(0, 1);
                                            lcd.print("para cima.");
                                            //delay(300);
                                            
                                            }
                                          
                                 /***********************
                                 * Fim Tela de decanso *
                                 ***********************/
                         
                         
                         }
                         
                         
                      }
                    
                  }

                  /*
                   * FIM da OPÇÃO 2
                   */

  /*
   * FIM da Condição de parada para as opções
   */
  
// if(digitalRead(botao1) == HIGH)
// {
//  aux = 0;
//  detachInterrupt(contapulso);
// }
}
}


/****************************
*****************************
****************************/


/*****************************
*********   Função  **********
**** escrita na memória ******
*****************************/
void memory_write (float Vol,int a_1,int a_2, int a_3) // Vol que é para ser guardado na memória ; address 1(msByte); address 2 (lsByte)

{
unsigned int VolLSB, VolMSB,Vol_dec;

VolLSB = int(Vol) & 0x00FF; // máscara para LSB
VolMSB = int(Vol) & 0xFF00; // Máscara para MSB
VolMSB = VolMSB / 256; // desloca 8 bits para direita
           
EEPROM.write(a_1,VolMSB);// grava Byte mais significativo 
EEPROM.write(a_2,VolLSB);// grava Byte menos significativo

Vol_dec = (Vol - int(Vol)) * 10;
EEPROM.write(a_3,Vol_dec); 

  
} // outter chave da função escrita na memória

/****************************
*****************************
****************************/

/*****************************
*********   Função  **********
***** leitura na memória *****
*****************************/
void memory_read (int a_1, int a_2, int a_3)
{

       unsigned int VolLSB, VolMSB;
       float Vol_dec;
       VolLSB = EEPROM.read(a_2); // Byte menos significativo
       VolMSB = EEPROM.read(a_1); // Byte mais significativo
       VolMSB = VolMSB * 256; // desloca 8 posições para a esquerda 
       
       VOL = VolLSB+VolMSB;

      
       Vol_dec = EEPROM.read(a_3);
       VOL = VOL + (Vol_dec / 10); //
       
       
}
/****************************
*****************************
****************************/

/**********************************************************
**********************FIM DAS SUBROTINAS*******************
***********************************************************/

/*****************************
*********    SETUP  **********
*******   do sistema  ********
*****************************/

void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP); // interrupção externa no pino 2
  pinMode(botao1,INPUT); // botao de comando 1 (cima)
  pinMode(botao2,INPUT); // botao de comando 2 (baixo)
  pinMode(botao3,INPUT); // botao de comando 3 (esqueda)
  pinMode(botao4,INPUT); // botao de comando 4 (direita)
  pinMode(driver_pin,OUTPUT); // pino A5 será utilizado como saida digital para saturar ou cortar transistor do driver de tensão para bomba
  
  
  lcd.begin(16, 2); // Define o display com 16 colunas e 2 linhas
  
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("      Tupa"); //Escreve no LCD
  lcd.setCursor(0, 1);
  lcd.print("Automation"); 
  cont_init = millis();
  cont_micro = millis();
//  sei();
}

/****************************
*****************************
****************************/


/*****************************
*********    Loop   **********
*******    infinito   ********
*****************************/
void loop() {

  /*********************
   * Tela de descanso  *
   *********************/

 if((millis() - cont_micro) >= 2000 ) // a cada 2 segundos troca o texto da tela
  {
    lcd.clear();
    aux_cont_micro = !(aux_cont_micro);
    cont_micro = millis();
  }
  
  if(aux_cont_micro == 0) // texto 1
  {
  
  lcd.setCursor(0, 0); 
  lcd.print("      Tupa"); 
  lcd.setCursor(0, 1);
  lcd.print("   Automation");
  //delay(300);
  }

  else if(aux_cont_micro == 1) // texo 2
  {
  
  lcd.setCursor(0, 0); 
  lcd.print("   Trasfegador");
  //delay(300);
  
  }

  /***********************
   * Fim Tela de decanso *
   ***********************/


  /*********************
   * Menu de opções    *
   *********************/
 
  if(digitalRead(botao1) == HIGH) // botao 1 entra no menu de opções
   { 
   /*Esse bloco é para utilizar pra chamar a rotina de medição*/
   /* meter_on ();
    cont_init = millis();
    lcd.clear();*/ 
    aux_menu = 1; // garante menu em loop
    menu_opt = 1; // garante primeira opção primeiro
    aux_cont_micro = 0; // seta variável para primeiro texto "menu de opções"
    lcd.clear(); // limpa lcd
    
    while(aux_menu == 1) // laço do menu
    {
      //lcd.clear(); // limpa lcd

      /**********************************************************************
       **********************************************************************
       **********************************************************************
       ************************** MENU - OPÇÃO 1 ****************************
       * ********************************************************************
       **********************************************************************/
      
      if(menu_opt == 1) // primeira opção => user pode escolher o valor de incremento
      {


          /*********************
           * TEXTOS DA OPÇÃO 1 *
           *********************/
          
          if((millis() - cont_micro) >= 2000 ) // a cada 2 segundos troca o texto da tela
             {
               lcd.clear();
               aux_cont_micro = !(aux_cont_micro);
               cont_micro = millis();
             }
  
          if(aux_cont_micro == 0) // texto 1
             {
       
                lcd.setCursor(0,0);
                lcd.print("Menu de");
                lcd.setCursor(0,1);
                lcd.print("Opcoes");
              }

            else if(aux_cont_micro == 1) // texo 2
             {
  
               lcd.setCursor(0, 0); 
               lcd.print("1- Ajustar");
               lcd.setCursor(0,1);
               lcd.print("incremento vol");
  
              }
           /*********************
           *    FIM DOS TEXTOS  *
           *      DA OPÇÃO 1    *
           *********************/

                       /*********************
                      * FUNÇÃO DA OPÇÃO 1  *
                       *********************/  

                       if(digitalRead(botao4) == 1) // se pressionado para direita entra na opção 2 do menu (Ajuste de incremento de vol)
                       {
                           aux_menu_inner = 1; // seta aux_menu_inner para 1, segurando em loop essa opção
                           // condições iniciais:
                           inc = EEPROM.read(10);// verifica se o incremento está em ml ou litros
                           
                           if(inc == 1) // condição para LITROS
                            memory_read(2,3,100); // lê posições 2 e 3 (msB e lsB para litros, respectivamente)
                           

                           else if(inc == 0) // condição para MILILITROS
                            memory_read(0,1,100); // lê posições 0 e 1 (msB e lsB para mililitros, respectivamente)
                           
                           while(aux_menu_inner == 1) // laço da função da opção 1 (Ajustar Incremento Vol)
                           {
                                  /*
                                   * Textos
                                   *       sub área -> aumenta ou diminui incremento
                                   */
                                 lcd.clear();
                                 lcd.setCursor(0,0);
                                 lcd.print("Passo do inc.:");
                                 
                                 if(inc == 1) // em litros
                                 {
                                 lcd.setCursor(5,1);
                                 lcd.print(int(VOL)); 
                                    
                                    if(VOL == 1) // se for 1 litro, escreve no singular
                                    {
                                     lcd.setCursor(11,1);
                                     lcd.print("litro");
                                    }

                                    else // se for mais de 1 litro escreve no plural
                                    {
                                    lcd.setCursor(10,1);
                                    lcd.print("litros");  
                                    }


                                            /*
                                            * Aumenta ou diminui incremento
                                            */
                                    
                                            if(digitalRead(botao2) == 1) // diminuindo o valor do incremento
                                            {
                                            VOL -= 1;
                                              if(VOL<= 1)
                                              {
                                                VOL = 900; //Garante valor mínimo de 100 mililitros
                                                inc = 0; // seta para mililitros
                                              }
                                      
                                            }

                                            if(digitalRead(botao1) == 1) // aumentando o valor do incremento
                                            {
                                            VOL += 1;
                                              if(VOL >= 20)
                                                VOL = 20; //Garante valor mínimo de 100 mililitros
                                                
                                              
                                      
                                            }

                                            /*
                                             * Fim do Aumenta ou diminui incremento
                                             */
                                 }

                                 else if(inc == 0) // em mililitros
                                 {
                                 lcd.setCursor(6,1);
                                 lcd.print("mililitros");
                                 lcd.setCursor(0,1);
                                 lcd.print(int(VOL));
                                 
                                            /*
                                            * Aumenta ou diminui incremento
                                            */
                                    
                                            if(digitalRead(botao2) == 1) // diminuindo o valor do incremento
                                            {
                                            VOL -= 100;
                                              if(VOL<= 100)
                                                VOL = 100; //Garante valor mínimo de 100 mililitros
                                      
                                            }

                                            if(digitalRead(botao1) == 1) // aumentando o valor do incremento
                                            {
                                            VOL += 100;
                                              if(VOL >= 1000)
                                              {
                                                VOL = 1; //Garante valor mínimo de 100 mililitros
                                                inc = 1; // seta para litros
                                              }
                                      
                                            }

                                            /*
                                             * Fim do Aumenta ou diminui incremento
                                             */
                                 }
                                 
                                 delay(200);

                                 /*
                                  * Fim dos Textos
                                  */



                                           

                                  /*
                                   * Liga ou Sai da Opção
                                   */
                                   if(digitalRead(botao3) == 1) // sai dessa opção de ajustar o incremento
                                   {
                                   VOL = 0; // reseta variável de VOL
                                   aux_menu_inner = 0;// condição para sair do laço dessa opção inner

                                   }

                                   if(digitalRead(botao4) == 1) // grava a opção selecionada para ajuste de incremento
                                   {
                                      if(inc == 1) // opção para litros
                                      {
                                        EEPROM.write(10,1); // salva como litro na memória
                                        memory_write(VOL,2,3,100); // grava o valor de vol de incremento desejado nas posições 2 e 3 (msB e lsB respectivamente)
                                        
                                      }

                                      else if (inc == 0) // opção para mililitros
                                      {
                                        EEPROM.write(10,0); // salva como mililitros na memória
                                        memory_write(VOL,0,1,100); // grava o valor de vol de incremento desejado nas posições 2 e 3 (msB e lsB respectivamente)
                                      }

                                      VOL = 0; // reseta variável de VOL
                                      aux_menu_inner = 0;// condição para sair do laço dessa opção inner
                                      delay(300);
                                   
                                   }

                                  /*
                                   * Fim Liga ou Sai da Opção
                                   */
                           }
                        
                       }


                      /*********************
                      *    FIM DA FUNÇÃO   *
                      *      DA OPÇÃO  1   *
                       *********************/

           /*********************
           *  Mundaça de Opção  *
           *********************/
           if(digitalRead(botao2) == HIGH) // botao pra baixo selecionado -> muda para opção numero 2
          
           {
            menu_opt = 2; // seta variável para segunda opção de menu => Trasfego programado
            lcd.clear(); // limpa lcd para proximo texto
           }

           /*********************
           * Fim da Opção 1 *
           *********************/
     
      
      } // cahves de if(menu_opt == 1) #primeira opção do menu

      /**********************************************************************
       **********************************************************************
       ****************************    FIM    *******************************
       ************************** MENU - OPÇÃO 1 ****************************
       * ********************************************************************
       **********************************************************************/

/************************************************************************************/

       /**********************************************************************
       **********************************************************************
       **********************************************************************
       ************************** MENU - OPÇÃO 2 ****************************
       * ********************************************************************
       **********************************************************************/

      else if(menu_opt == 2) // segunda opção => user pratica trasfego com valor programado por ele

      {

      /*********************
       * TEXTOS DA OPÇÃO 2 *
       *********************/
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("2- Trasfego");
            lcd.setCursor(0,1);
            lcd.print("    Programado");
            delay(200);

       /*********************
       *    FIM DOS TEXTOS  *
       *      DA OPÇÃO 2    *
       *********************/

                     /*********************
                      * FUNÇÃO DA OPÇÃO 2  *
                      *********************/

                       if(digitalRead(botao4) == HIGH) // se o user apertou para direita então entra na opção
                       {
                           memory_read(4,5,11); // lê valor desejador pelo user para trasfego programado (em litros); (4-MSByte ; 3 LSByte) + valor decimar (posição 11)
                           vol_prog = VOL; //armazena esse valor lidor em vol_prog, pois VOL será necessário para o incremento
                           
                           aux_menu_inner = 1; // seta aux_menu_inner para 1, segurando em loop essa opção
                          
                           // condições iniciais:
                           inc = EEPROM.read(10);// verifica se o incremento está em ml ou litros
                           
                           if(inc == 1) // condição para LITROS
                            memory_read(2,3,100); // lê posições 2 e 3 (msB e lsB para litros, respectivamente)
                           

                           else if(inc == 0) // condição para MILILITROS
                            memory_read(0,1,100); // lê posições 0 e 1 (msB e lsB para mililitros, respectivamente)

                           VOL_prog = VOL; // guarda valor da memoria lida em VOL_prog -> VOL_prog é uma double, para poder fazer divisão por 1000 para ajusar a unidade de medida de ml para litros
                           while(aux_menu_inner == 1) // laço da função da opção 2 (Volume programado em litros)
                           {

                                  /*
                                   * Textos
                                   *       sub área -> aumenta ou diminui volume programado total
                                   */

                                   lcd.clear();
                                   
                                   lcd.setCursor(0,0);
                                   lcd.print("Vol programado:");
                                   if(vol_prog == 1) // se for 1 litro, mostra no singular
                                      {
                                        lcd.setCursor(11,1);
                                        lcd.print("litro");
                                        
                                      }
                                   else // se não, mostra no plural
                                   {
                                    lcd.setCursor(10,1);
                                    lcd.print("litros");
                                    
                                   }
                                   lcd.setCursor(0,1);
                                   lcd.print(vol_prog);
                                   


                                   /*
                                    * Aumenta ou diminui volume programado
                                    */
  

                                             /*
                                              * Aumenta
                                              */
                                            if(digitalRead(botao1)== HIGH) // se apertar botao pra cima, então aumenta vol programado (máx 1000 litros)
                                            {

                                                if(inc == 1) // se a base de incremento for litros, então não é necessário fazer conversão de unidades
                                                {

                                                   vol_prog +=VOL_prog; // incrementa vol_prog no passo definido pelo usuário na opção 1 do menu (Ajustar incremento de vol)
                                                   if(vol_prog >= 1000)
                                                      vol_prog = 1000; // máx de 1000 litros 
                                                
                                                }

                                                else if(inc == 0)// se a base de incremento for mililitros, necessário fazer conversão para o incremento
                                                {
                                                   vol_prog = vol_prog + (VOL_prog/1000); // divide por mil, para passar de ml para litros
                                                   if(vol_prog >= 1000)
                                                      vol_prog = 1000; // máx de 1000 litros 
                                                }
                                            
                                            }

                                            /*
                                             * Fim do Aumenta
                                             */


                                             /*
                                              * Diminui
                                              */
                                            if(digitalRead(botao2)== HIGH) // se apertar botao pra baixp, então diminui vol programado (min 1 litro ou 100 ml)
                                            {

                                                if(inc == 1) // se a base de decremento for litros, então não é necessário fazer conversão de unidades
                                                {

                                                   vol_prog -=VOL_prog; // decrementa vol_prog no passo definido pelo usuário na opção 1 do menu (Ajustar incremento de vol)
                                                   if(vol_prog <= VOL_prog)
                                                      vol_prog = VOL_prog; // min de 1 litro
                                                
                                                }

                                                else if(inc == 0)// se a base de incremento for mililitros, necessário fazer conversão para o incremento
                                                {
                                                   vol_prog = vol_prog - (VOL_prog/1000); // divide por mil, para passar de ml para litros
                                                   if(vol_prog <= (VOL_prog/1000))
                                                      vol_prog = (VOL_prog/1000); // min de 100 ml 
                                                }
                                            
                                            }

                                            /*
                                             * Fim do Diminui
                                             */

                                    /*
                                     * Fim Aumenta ou diminui volume programado
                                     */

                                       lcd.blink();
                                       delay(200);
                                   
                                       lcd.noBlink();


                                   /*
                                    * Liga o equipamento no volume programado ou sai
                                    */

                                    if(digitalRead(botao3) == HIGH) // se apertar para esquerda, então sai da função sem executar nem salvar nada
                                    {
                                        lcd.clear();
                                        VOL = 0; // reseta variável de VOL
                                        vol_prog = 0; // reseta variável vol_prog
                                        VOL_prog =0 ;// reseta variável VOL_prog
                                        aux_menu_inner = 0;// condição para sair do laço dessa opção inner

                                      
                                    }

                                    if(digitalRead(botao4) == HIGH) // se apertar o botao da direita, então aceita a opção, liga a bomba, salva vol programado
                                    {
                                          
                                          memory_write(vol_prog,4,5,11); // grava na memória valor de volume programado desejado
                                          digitalWrite(driver_pin,HIGH); // liga driver da bomba
                                          aux = 1;
                                          meter_on ();
                                          aux_menu_inner = 0;
                                          cont_init = millis();
                                          lcd.clear();
                                          
                                          
                                      
                                    }

                                   

                                   /*
                                    * FIM Liga o equipamento no volume programado ou sai
                                    */
                                     
                              // pisca display pra usuário entender que é para interagir com o sistema
                              
                             
                              
                           }// chaves do while para segurar a entrada a opção
                           
                       
                       } // chaves  if(digitalRead(botao4) == HIGH) (apertou para direita para entrar na opção)

                      
                      /*********************
                      *    FIM DA FUNÇÃO   *
                      *      DA OPÇÃO 2    *
                       *********************/

       /*********************
        *  Mundaça de Opção  *
        *********************/
           
           if(digitalRead(botao2) == HIGH) // botao pra baixo selecionado -> muda para opção numero 3
          
           {
            menu_opt = 3; // seta variável para terceira opção de menu => Trasfego livre
            lcd.clear(); // limpa lcd para proximo texto
           }

           if(digitalRead(botao1) == HIGH) // botao pra cima selecionado -> muda para opção numero 1
          
           {
            menu_opt = 1; // seta variável para primeira opção de menu => ajustar incremento de volume
            lcd.clear(); // limpa lcd para proximo texto
           }
      
      /*********************
       *   Fim da Opção 2  *
       *********************/
      }// cahves de else if(menu_opt == 2) #segunda opção do menu


       /*********************************************************************
       **********************************************************************
       ****************************    FIM    *******************************
       ************************** MENU - OPÇÃO 2 ****************************
       * ********************************************************************
       **********************************************************************/

/*******************************************************************************************************/

       /*********************************************************************
       **********************************************************************
       **********************************************************************
       ************************** MENU - OPÇÃO 3 ****************************
       * ********************************************************************
       **********************************************************************/


       else if(menu_opt == 3) // terceira opção => User pratica trafego de modo livre, o proprio user deve interagir para parar a bomba e a contagem
       {

      /*********************
       * TEXTOS DA OPÇÃO 3 *
       *********************/
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("3- Trasfego");
            lcd.setCursor(0,1);
            lcd.print("      Livre");
            delay(200);

       /*********************
       *    FIM DOS TEXTOS  *
       *      DA OPÇÃO 3    *
       *********************/

       /*********************
        *  Mundaça de Opção  *
        *********************/
           
           if(digitalRead(botao2) == HIGH) // botao pra baixo selecionado -> muda para opção numero 4
          
           {
            menu_opt = 4; // seta variável para quarta opção de menu => Esvaziar Tanque
            lcd.clear(); // limpa lcd para proximo texto
           }

           if(digitalRead(botao1) == HIGH) // botao pra cima selecionado -> muda para opção numero 2
          
           {
            menu_opt = 2; // seta variável para segunda opção de menu => Trasfego Programado
            lcd.clear(); // limpa lcd para proximo texto
           }
      
      /*********************
       *   Fim da Opção 3  *
       *********************/
      }// cahves de else if(menu_opt == 3) #terceira opção do menu

       /*********************************************************************
       **********************************************************************
       ****************************    FIM    *******************************
       ************************** MENU - OPÇÃO 3 ****************************
       * ********************************************************************
       **********************************************************************/

/*******************************************************************************************************/

       /*********************************************************************
       **********************************************************************
       **********************************************************************
       ************************** MENU - OPÇÃO 4 ****************************
       * ********************************************************************
       **********************************************************************/
 


      else if(menu_opt == 4) // quarta opção => esvaziamento de tanque, o próprio sistema detecta que não tem liquido trafegando mais na tubulação e desliga bomba
      {
       /*********************
       * TEXTOS DA OPÇÃO 4 *
       *********************/
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("4- Esvaziar");
            lcd.setCursor(0,1);
            lcd.print("      Tanque");
            delay(200);

       /*********************
       *    FIM DOS TEXTOS  *
       *      DA OPÇÃO 4    *
       *********************/

       /*********************
        *  Mundaça de Opção  *
        *********************/
           
           if(digitalRead(botao2) == HIGH) // botao pra baixo selecionado -> muda para opção numero 5
          
           {
            menu_opt = 5; // seta variável para quinta opção de menu => Sair do Menu de Opções
            lcd.clear(); // limpa lcd para proximo texto
           }

           if(digitalRead(botao1) == HIGH) // botao pra cima selecionado -> muda para opção numero 3
          
           {
            menu_opt = 3; // seta variável para terceira opção de menu => Trasfego livre
            lcd.clear(); // limpa lcd para proximo texto
           }
      
      /*********************
       *   Fim da Opção 4  *
       *********************/
      } // cahves de else if(menu_opt == 4) #quarta opção do menu

       /*********************************************************************
       **********************************************************************
       ****************************    FIM    *******************************
       ************************** MENU - OPÇÃO 4 ****************************
       * ********************************************************************
       **********************************************************************/

/*******************************************************************************************************/

       /*********************************************************************
       **********************************************************************
       **********************************************************************
       ************************** MENU - OPÇÃO 5 ****************************
       * ********************************************************************
       **********************************************************************/

      if(menu_opt == 5) // Quinta opção => Sair do menu de opções
      {

        /*********************
       * TEXTOS DA OPÇÃO 5 *
       *********************/
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("5- Sair do Menu");
            lcd.setCursor(0,1);
            lcd.print("  de Opcoes ");
            delay(200);

       /*********************
       *    FIM DOS TEXTOS  *
       *      DA OPÇÃO 5   *
       *********************/

       /*********************
        *  Mundaça de Opção  *
        *********************/

           if(digitalRead(botao1) == HIGH) // botao pra cima selecionado -> muda para opção numero 4
          
           {
            menu_opt = 4; // seta variável para quarta opção de menu => Esvaziar tanque
            lcd.clear(); // limpa lcd para proximo texto
           }


        /*********************
        *  Aceite  de Opção  *
        *********************/

        if(digitalRead(botao4) == HIGH) // usuário quer sair do menu de opções
          aux_menu = 0; // sai do menu de opções
            
      
      /*********************
       *   Fim da Opção 5  *
       *********************/
       
      } // cahves de else if(menu_opt == 5) #quinta opção do menu

      /*********************************************************************
       **********************************************************************
       ****************************    FIM    *******************************
       ************************** MENU - OPÇÃO 5 ****************************
       * ********************************************************************
       **********************************************************************/

/*******************************************************************************************************/
      
    } // chaves de while(aux_menu == 1) # laço que garante loop para o menu
    
    
   }

   
  
  /***********************
   * Fim Menu de Opções  *
   ***********************/



  
}

/****************************
****Fim do loop infinito*****
****************************/
