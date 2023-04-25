/********************************************************
 * Firmaware desenvolvido por Caio Dutra em 17/07/2019  *
 * **************************************************** *
 * Este firmware colhe pulsos gerados pelo sensor hall  *
 * Yf-S201 para medir a vazão de trasfego na linha de   *
 * produção de cerveja. O firmware também porta de uma  *
 * interface para comunição direta com ser humano.      *
 * **************************************************** *
 ********************************************************/

 /* Ver.3.0 *********************************************
 * **************************************************** *
 * Esta versão implementa a primeira parte de interface *
 * do menu com o usuário. SOMENTE as primeiras telas.   *
 ********************************************************/

/*
 * Bibliotecas
 */
#include<LiquidCrystal.h>

/*
 * #define
 */
#define botao1 7 // cima
#define botao2 8 // baixo
#define botao3 9 // esqueda
#define botao4 10 // direita


LiquidCrystal lcd(12, 11, 6, 5, 4, 3); // Cria um objeto lcd e atribui os pinos

/*****************************
********* Variáveis **********
*****************************/

/*
 * Double
 */
double pulso = 0; // varíavel que conta o número de pulsos do sensor hall num intervalo de tempo

/*
 * Float
 */
float  freq = 0.000;
float vazao = 0.000;
float ml=0.00, ml_tot=0.00; // variáveis que contam a quantidade de volume trasfegado (em litros, apesar de chamarem 'ml')

/*
 * int
 */
long int cont_init, cont_end;
long int cont_micro = 0,aux_cont_micro = 0;

int menu_opt = 1; // Menu_Option => 1- primeira opção do menu () | 2 - segunda opção do menu () | 3 - terceira opção do menu () | 4 - quarta opção do menu ()
int aux_menu = 0; // aux_menu = 0 -> Sai do laço do menu | aux_menu = 1 -> fica preso no laço de menu 

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
 unsigned int aux = 1; // variável que segura laço de contagem 
 
 attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING);// liga interrupção externa
 
 while(aux == 1) // condição para a contagem ficar repetindo
{

  
cont_end = millis(); // vê em quanto está a contagem atual de millis()


 
if(cont_end >= (cont_init+50)) //  se tiver passado 50 ms, faz a contagem de quantos litros passaram pelo medidor
  {
 detachInterrupt(contapulso); // desliga interrupção
 
 
 freq = (pulso/(cont_end - cont_init))*1000; // frequência  =[ número de pulsos / intervalo de tempo (50ms) ] * 1000(fator para colocar a frequência em hertz HZ, pois o delta de t está em ms)  
 cont_init = cont_end; // atualiza variável que é o início da contagem
 pulso = 0; // zera a variável que conta os pulsos do sensor
 
 vazao = freq/7.5; // equação do sensor Q (l/min) = freq (Hz) / 7.5 (Hz*min/l) -> Valor de calibração para 1 segundo de leitura
 ml = (vazao / (20*60)) * 1000; // transforma a vazao em quantidade de volume trasfegado num intervalo de 50ms -> cabem 20 50ms em 1 s, por isso o fator de 20 no denominador
 ml_tot += ml; // acumula o valor de volume trasfegado a cada 50ms
 vazao = 0.00; // reseta o valor da varíavel para uma nova leitura
 

  lcd.clear(); // limpa lcd
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("Volume:"); //Escreve Volume:
  lcd.setCursor(6, 1); //Posiciona o cursor na coluna 6 e linha 1 do LCD
  lcd.print("litros");//Escreve litros (unidade de medida da contagem)
  lcd.setCursor(0, 1);  //Posiciona o cursor na coluna 0 e linha 1 do LCD
  lcd.print(ml_tot/1000); // printa o valor total de volume trasfegado até o momento
  
 attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING); // liga a interrupção para uma nova contagem de pulsos
  }

  
  
  
 if(digitalRead(botao1) == HIGH)
 {
  aux = 0;
  detachInterrupt(contapulso);
 }
}
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
       *      DA OPÇÃO 4    *
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
       *   Fim da Opção 5  *
       *********************/
       
      } // cahves de else if(menu_opt == 5) #quinta opção do menu
      
    } // chaves de while(aux_menu == 1) # laço que garante loop para o menu
    
    
   }

   
  
  /***********************
   * Fim Menu de Opções  *
   ***********************/



  
}

/****************************
****Fim do loop infinito*****
****************************/
