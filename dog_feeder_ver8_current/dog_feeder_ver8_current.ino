#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <DS1307.h>

// initialize the library with the numbers of the interface pinslo
LiquidCrystal lcd(14, 15, 16, 17, 2, 3);
DS1307 rtc(A4,A5);
String hrm; // variável que é responsálvel por pegar hora, min e seg do DS1307 <-- É uma STRING
String week_day; // variável que le o dia da semana 
String WEEK_DAY; //variável que lê dia da semana de forma inteira 0-> DOM ; 1-> SEG; 2-> TER; etc...
String data; // variável que lê a data
int DATA [6]; // variavel que transforma o formato da data DD/MM/YYYY >>>> DD/MM
int HRM [10]; // variável que é responsálvel por pegar hora, min e seg do DS1307 <-- É uma INT
int i = 0; // variável que conta o for da conversão de char pra int hrm>>HRM
int hora,minu,seg; // variáveis que aramazenarão os valores da hora, min e seg;
int dia,mes; // variáveis que mostrarao dia e mes no display
int dia_da_semana = 0; // variavel inteira que codifica o dia da semana 
int tecla = 0;
char letra;
int aux_menu = 0; // variável que prende o laço WHILE 1 -> Inicia-se em zero para a primeira condição

int aux_inner_menu = 0; // variável que seleta qual opção de menu o usuário deseja

int aux_inner_tela=0;// variável que prende o WHILE 2 ou WHILE 3

int navegador = 0; // variavel que navegará o user pelos menus

int hor_aliment[4], min_aliment [4], hor_total_aliment= 0; // variavel do horário de alimentação
int gramas = 0, gramasLSB = 0, gramasMSB = 0; // variável que guardará a qntdade de gramas por alimentação
int apontador = 0; // variável que posicionará o cursor para o user navegar no display
int aliment_aux=0;

unsigned int aux_time_feed = 0; // variável que calcula delay necessário para alimentação
unsigned long time_feed = 0;
float tempo=0;

int aux_come1 = 0, aux_come2 = 0; // variável que sinaliza se está na hora de alimentação

//int hora,minute,seg,day,month;

void le_hora (void); // função que le a data e hora e troca os nomes dos dias EN -> PT

void transform_ASCII_to_int (void);

void mostra_hora_data_atualizar (void);
void varredura_de_teclado()
{
  /************************************************
  *************************************************
  ************ Varredura Primeira Linha ***********
  *************************************************
  ************************************************/
  
 digitalWrite(8,HIGH); // energiza a primeira linha
  
  if(digitalRead(4) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("A");
  tecla = 1;
  aux_menu = 1; // seta aux_menu para prender o laço WHILE 1 do menu
  delay(300);
  
  }
  
  if(digitalRead(5) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("B");
  tecla= 2;
  delay(300);
  }
  
  
  if(digitalRead(6) == 1)
   { 
   //  lcd.clear();
  //  lcd.setCursor(0,0);   
    // lcd.print("cachorro");
     delay(300);
     tecla=3;
   }
 
 if(digitalRead(7) ==1)
  { 
  //  lcd.clear();
 // lcd.setCursor(0,0);  
  // lcd.print("deway");
   tecla=10; // simbolo hexadecima para A
   letra = 'A';
    delay(300); 
  }
  digitalWrite (8,LOW); // desliga primeira linha


  /************************************************
  *************************************************
  *************************************************
  *************************************************
  ************************************************/


  /************************************************
  *************************************************
  ************ Varredura Segunda linha ************
  *************************************************
  ************************************************/

digitalWrite(9,HIGH); // energiza a segunda linha

  if(digitalRead(4) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("A");
  tecla = 4;
  aux_menu = 1; // seta aux_menu para prender o laço WHILE 1 do menu
  delay(300);
  
  }
  
  if(digitalRead(5) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("B");
  tecla=5;
  delay(300);
  }
  
  
  if(digitalRead(6) == 1)
   { 
   //  lcd.clear();
  //  lcd.setCursor(0,0);   
    // lcd.print("cachorro");
     delay(300);
     tecla=6;
   }
 
 if(digitalRead(7) ==1)
  { 
  //  lcd.clear();
 // lcd.setCursor(0,0);  
  // lcd.print("deway");
   tecla=11;
   letra = 'B';
    delay(300); 
  }

digitalWrite(9,LOW); // desliga segunda linha

 /************************************************
  *************************************************
  *************************************************
  *************************************************
  ************************************************/

 /************************************************
  *************************************************
  ************ Varredura terceira Linha ***********
  *************************************************
  ************************************************/

digitalWrite(10,HIGH); // energiza a terceira linha


  if(digitalRead(4) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("A");
  tecla = 7;
  aux_menu = 1; // seta aux_menu para prender o laço WHILE 1 do menu
  delay(300);
  
  }
  
  if(digitalRead(5) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("B");
  tecla= 8;
  delay(300);
  }
  
  
  if(digitalRead(6) == 1)
   { 
   //  lcd.clear();
  //  lcd.setCursor(0,0);   
    // lcd.print("cachorro");
     delay(300);
     tecla=9;
   }
 
 if(digitalRead(7) ==1)
  { 
  //  lcd.clear();
 // lcd.setCursor(0,0);  
  // lcd.print("deway");
   tecla=12;
   letra = 'C';
    delay(300); 
  }

digitalWrite(10,LOW); // desliga terceira linha

 /************************************************
  *************************************************
  *************************************************
  *************************************************
  ************************************************/
  
  /************************************************
  *************************************************
  ************ Varredura quarta Linha ***********
  *************************************************
  ************************************************/

digitalWrite(11,HIGH); // energiza a quarta linha


  if(digitalRead(4) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("A");
  letra = '*';
  tecla = 14;
  aux_menu = 1; // seta aux_menu para prender o laço WHILE 1 do menu
  delay(300);
  
  }
  
  if(digitalRead(5) == 1)
  {
  //lcd.clear();
  //lcd.setCursor(0,0);  
 // lcd.print("B");
  tecla=0;
  delay(300);
  }
  
  
  if(digitalRead(6) == 1)
   { 
   //  lcd.clear();
  //  lcd.setCursor(0,0);   
    // lcd.print("cachorro");
     delay(300);
     tecla=15;
     letra = '#';
   }
 
 if(digitalRead(7) ==1)
  { 
  //  lcd.clear();
 // lcd.setCursor(0,0);  
  // lcd.print("deway");
   tecla=13;
   letra = 'D';
    delay(300); 
  }

digitalWrite(11,LOW); // desliga quarta linha

 /************************************************
  *************************************************
  *************************************************
  *************************************************
  ************************************************/

}




void setup() {
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Set the clock to run-mode
  rtc.halt(false);
  
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  //delay(200);
  // Print a message to the LCD.
 // pinMode(A0,OUTPUT);//      O
  //pinMode(A1,OUTPUT);//      l<=========o
  //pinMode(A5,INPUT);//      / \
  //pinMode(A4,INPUT);
  
  /******************
  *******************
  ** Saidas Teclado**
  ******************/
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
   pinMode(12,OUTPUT);
  
  /******************
  *******************
  **Entradas Teclado**
  ******************/
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  
  
 // EEPROM.write(6,16); 
 // EEPROM.write(7,15);

}

void loop() {

 /*****************************************************************/
 // CONDIÇÃO QUE RESETA AUX_COME PRA LIBERAR COMIDA PRO PROXIMO DIA
  if (hora == 23 && minu == 59)
  {
   aux_come1 = 0 ; 
   aux_come2 = 0;
  }
  /*****************************************/

  
  varredura_de_teclado();
  
  /***********************************
  ************************************
  *******Primeira tela de descanso****
  ***********************************/
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dog feeder");
  
  lcd.setCursor(8,1);
  lcd.print(rtc.getTimeStr());
  transform_ASCII_to_int ();
  lcd.setCursor(11,0);
  lcd.print(dia);
  lcd.print("/");
  lcd.print(mes);
  lcd.setCursor(0,1);
  lcd.print(WEEK_DAY);
  
  delay(200);


  /**************************************************
   * ***********************************************
   **************** ALIMENTAÇÃO MOTOR **************
   ************************************************* 
   ************************************************/

    if(hora == EEPROM.read(6) && minu == EEPROM.read(7) && aux_come1 == 0 )
    {
    
       gramasLSB = EEPROM.read(4); // Byte menos significativo
       gramasMSB = EEPROM.read(5); // Byte mais significativo
       gramasMSB = gramasMSB * 256; // desloca 8 posições para a esquerda 
       
       gramas = gramasLSB+gramasMSB;
        
        tempo = 0.289*(gramas) - 0.843; // função de controle de malha aberta calculada empiricamente 
       
        digitalWrite(12,HIGH);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alimentando");

        lcd.setCursor(0,1);
        lcd.print(gramas);
        lcd.print(" ");
        lcd.print("g");
        
        aux_time_feed = int(tempo);
        time_feed = aux_time_feed * 1000; // passa de seg pra mili-seg (função delay usa mili-seg)
       // lcd.print(time_feed);
        delay(time_feed );

        lcd.clear();

        digitalWrite(12,LOW);
        aux_come1 = 1; // seta para 1, assim ele nao entra mais no if no mesmo dia
        
    }

     if(hora == EEPROM.read(8) && minu == EEPROM.read(9) && aux_come2 == 0 )
    {
    
       gramasLSB = EEPROM.read(4); // Byte menos significativo
       gramasMSB = EEPROM.read(5); // Byte mais significativo
       gramasMSB = gramasMSB * 256; // desloca 8 posições para a esquerda 
       
       gramas = gramasLSB+gramasMSB;
        
        tempo = 0.289*(gramas) - 0.843;
       
        digitalWrite(12,HIGH);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alimentando");

        lcd.setCursor(0,1);
        lcd.print(gramas);
        lcd.print(" ");
       
        
        aux_time_feed = int(tempo);
        time_feed = aux_time_feed * 1000;
       // lcd.print(time_feed);
        lcd.print("g");
        delay(time_feed );

        lcd.clear();

        digitalWrite(12,LOW);
        aux_come2 = 1; // seta para 1, assim ele nao entra mais no if no mesmo dia
        
    }

  /************************************************
   * **********************************************
   ***********************************************/
 // varredura_de_teclado();
  
  /***********************************
  ************************************
  *******Segunda tela de descanso*****
  ***********************************
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Prox.Alimentacao:");
  
  lcd.setCursor(0,1);
  lcd.print("feed");
  delay(2000); */
  
  
  if(tecla == 14) // tecla 14 significa que o usuário deseja entrar no menu '*'
  {
    lcd.clear();
    lcd.setCursor(0,0);
    aux_inner_menu = 0; // variável que seleta a opção de menu desejado pelo usuário
    tecla = 0;
    delay(200);
    
  while(aux_menu == 1) //(WHILE 1)
  {
    varredura_de_teclado();
    
    /********************************************************
    *********************************************************
    ***************1ª OPÇÃO DE MENU********************
    ***************ATUALIZAR DATA E HORA*********************
    ********************************************************/ 
    if(aux_inner_menu == 0) // primeira opção de tela
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Atualizar data");
      lcd.setCursor(0,1);
      lcd.print("e hora.");
      delay(200);
      
      
      if(tecla == 11)// mover a tela para a próxima
      {
        aux_inner_menu ++;
        tecla = 0;
        delay(200);
      }
      
      
      if (tecla== 14)
     { 
       aux_inner_tela=0;
       tecla=0;
       delay(200);
       transform_ASCII_to_int ();
       lcd.blink();
       lcd.clear();
       mostra_hora_data_atualizar ();
       apontador = 0; // seta apontador para zero (começar no DD da data)
      while (aux_inner_tela==0)
      { 
        varredura_de_teclado();
        
        if (aux_inner_tela==0)
        {      
               
            lcd.clear();   
            mostra_hora_data_atualizar ();
            delay (100);   
            
         // seção que anda para direita e esquerda no menu
        // C para direita e D para a esquerda     
         if (tecla == 12)
         {
           apontador ++;
           if(apontador >=5)
             apontador = 5;
           tecla = 0;
         }
         
         if (tecla == 13)
         {
           apontador --;
           if(apontador <=0)
             apontador = 0;
             
           tecla = 0;
         }
         /*****************************************/
         
         // interface HM -> valor do apontador indica qual dado está sendo atualizado
         // 0: DD dia do mes  | 1: MM mês  | 2: Dia Semana | 3: HH horas | 4: MM Minutos | 5: SS Segundos
         
         
         // APONTADOR em DIA
         if(apontador == 0)
             {
               
               
          
               
               
               if(tecla == 10)
               {
               
               dia++;
               if(dia>31)
                 dia =1;
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               dia--;
               if(dia<1)
                 dia =31;
              
               tecla =0;
               
               
               }
               
             }
             
             // APONTADOR em MÊS
             
            else if(apontador == 1)
             {
               
               
          
               
               
               if(tecla == 10)
               {
               
               mes++;
               if(mes>12)
                 mes =1;
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               mes--;
               if(mes<1)
                 mes =12;
              
               tecla =0;
               
               
               }
               
             }
             
             // Apontador em DIA DA SEMANA
             else if(apontador == 2)
             {
               
               
          
               
               
               if(tecla == 10)
               {
               
               dia_da_semana++;
               if(dia_da_semana>6)
                 dia_da_semana=0;
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               dia_da_semana--;
               if(dia_da_semana<0)
                 dia_da_semana =6;
              
               tecla =0;
               
               
               }
               
             }
             
            // APONTADOR em HORA
             
             else if(apontador == 3)
             {
               
               
          
               
               
               if(tecla == 10)
               {
               
               hora++;
               if(hora>23)
                 hora =0;
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               hora--;
               if(hora<0)
                 hora=23;
              
               tecla =0;
               
               
               }
               
             }
          
           // APONTADOR em MINUTO
             
           else if(apontador == 4)
             {
               
               
          
               
               
               if(tecla == 10)
               {
               
               minu++;
               if(minu>59)
                 minu = 0;
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               minu--;
               if(minu<0)
                 minu=59;
              
               tecla =0;
               
               
               }
               
             }
             
              // APONTADOR em SEGUNDOS
             
            else if(apontador == 5)
             {
               
               
          
               
               
               if(tecla == 10)
               {
               
               seg++;
               if(seg>59)
                 seg =0;
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               seg--;
               if(seg<0)
                 seg=59;
              
               tecla =0;
               
               
               }
               
             }
          
          /**************************************************************/
          
         if (tecla== 15) // sai do menu sem gravar as alterações
         { 
           aux_inner_tela ++;
           tecla=0;
           lcd.noBlink();
         }
         
         if (tecla== 14) // sai do menu e grava as alterações
         { 
           aux_inner_tela ++;
           tecla=0;
           lcd.noBlink();
           
           if (dia_da_semana == 0)
             rtc.setDOW(SUNDAY); 
         else if (dia_da_semana == 1)
             rtc.setDOW(MONDAY); 
         else if (dia_da_semana == 2)
             rtc.setDOW(TUESDAY); 
         else if (dia_da_semana == 3)
             rtc.setDOW(WEDNESDAY); 
         else if (dia_da_semana == 4)
             rtc.setDOW(THURSDAY); 
         else if (dia_da_semana == 5)
             rtc.setDOW(FRIDAY); 
         else if (dia_da_semana == 6)
             rtc.setDOW(SATURDAY); 
          
           
           rtc.setTime(hora, minu, seg);     
           rtc.setDate(dia, mes, 2023);   
         }
         
         
         
         
        }  
      }
     }
    }
    

    
    /********************************************************
    *********************************************************
    ***************2ª OPÇÃO DE MENU**************************
    ***************HORARIO DA ALIMENTAÇÃO*******************
    ********************************************************/ 
    
    if(aux_inner_menu == 1) // segunda opção de tela
    {
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ajustar horario");
      lcd.setCursor(0,1);
      lcd.print("de alimentacao");
      delay(200);

       if(tecla == 11)// mover a tela para a próxima
      {
        aux_inner_menu ++;
        tecla = 0;
        delay(200);
      }
      
      if(tecla == 10)// mover a tela para a anterior
      {
        aux_inner_menu --;
        tecla = 0;
        delay(200);
      }
      if (tecla==14)
      {
        
        aux_inner_tela = 0;
        tecla=0;
        delay(200);
        //lcd.blink();
        lcd.clear();
        apontador = 0;
       hor_aliment[0] = EEPROM.read(6); 
       min_aliment[1] = EEPROM.read(7);
       lcd.setCursor (0,1);
       aliment_aux = 1;
        while (aux_inner_tela == 0)
        {
        varredura_de_teclado();
        
        if (aux_inner_tela==0)
        { 
          lcd.noBlink();
          delay(10);
          lcd.clear();
          lcd.setCursor(3,0);
          
          if(aliment_aux ==1)
          {
           lcd.print("Horario 1");
          lcd.setCursor(0,1);
          lcd.print(hor_aliment[0]);
          lcd.print(":");
          lcd.print (min_aliment[1]);
          }
         
          if(aliment_aux ==2)
          {
           lcd.print("Horario 2");
          lcd.setCursor(0,1);
          lcd.print(hor_aliment[2]);
          lcd.print(":");
          lcd.print (min_aliment[3]);
          }
         
          
          if(tecla == 1)
          {
          hor_aliment[0] = EEPROM.read(6); 
          min_aliment[1] = EEPROM.read(7);
          aliment_aux = 1;
          }
          
          if(tecla == 2)
          {
          hor_aliment[2] = EEPROM.read(8); 
          min_aliment[3] = EEPROM.read(9);
          aliment_aux = 2;
          }
          
          
             if (tecla == 12)
         {
           apontador ++;
           if(apontador >=1)
             apontador = 1;
           tecla = 0;
         }
         
         if (tecla == 13)
         {
           apontador --;
           if(apontador <=0)
             apontador = 0;
             
           tecla = 0;
         }
      
          if(apontador == 0)
             {
               lcd.setCursor(0,1);
             if(tecla == 10)
               {
               
               if(aliment_aux == 1)
             {
                hor_aliment[0]++;
               if(hor_aliment[0]>23)
                 hor_aliment[0] =0;
             }  
              
              if(aliment_aux == 2)
             {
                hor_aliment[2]++;
               if(hor_aliment[2]>23)
                 hor_aliment[2] =0;
             }   
               
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
               if(aliment_aux == 1)
             {
              hor_aliment[0]--;
               if(hor_aliment[0]<0)
                 hor_aliment[0]=23;
             }  
                 
              if(aliment_aux == 2)
             {
              hor_aliment[2]--;
               if(hor_aliment[2]<0)
                 hor_aliment[2]=23;
             }
              
               tecla =0;
               
               
               }
               
             }
                else if(apontador == 1)
             {
               lcd.setCursor(3,1);
              if(tecla == 10)
               {
               
               if(aliment_aux == 1)
             
             {
               min_aliment[1]++;
              if(min_aliment[1]>59)
              min_aliment[1] = 0;
             }  
               
               if(aliment_aux == 2)
             
             {
               min_aliment[3]++;
              if(min_aliment[3]>59)
              min_aliment[3] = 0;
             } 
               
               
               
               
               tecla =0;
               
               
               }
               if(tecla == 11)
               {
               
                 if(aliment_aux == 1)
                 {
                 
                 min_aliment[1]--;
                 if(min_aliment[1]<0)
                 min_aliment[1]=59;
                 }
               
              
                if(aliment_aux == 2)
                 {
                 
                 min_aliment[3]--;
                 if(min_aliment[3]<0)
                 min_aliment[3]=59;
                 }
               
               
              
               tecla =0;
               
               
               }
               
             }
            if (tecla== 15) // sai do menu sem gravar as alterações
         { 
           aux_inner_tela ++;
           tecla=0;
           lcd.noBlink();
         }
         
          if (tecla== 14) // sai do menu e grava as alterações
         { 
           aux_inner_tela ++;
           tecla=0;
           lcd.noBlink();
           
           EEPROM.write(6,hor_aliment[0]);
           EEPROM.write(7,min_aliment[1]);
           EEPROM.write(8,hor_aliment[2]);
           EEPROM.write(9,min_aliment[3]);
         }
         
         lcd.blink();
         delay(100);
        }
      }
    }
    }
  
    
    /********************************************************
    *********************************************************
    ***************3ª OPÇÃO DE MENU********************
    ***************QTDADE DE RAÇÃO*********************
    ********************************************************/ 
     
      if(aux_inner_menu == 2) // terceira opção de tela
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Quantidade de");
      lcd.setCursor(0,1);
      lcd.print("racao");
      delay(200);
     
      if(tecla == 11)// mover a tela para a próxima
      {
        aux_inner_menu ++;
        tecla = 0;
        delay(200);
      }
      
      if(tecla == 10)// mover a tela para a anterior
      {
        aux_inner_menu --;
        tecla = 0;
        delay(200);
      }
      
      /***************************************
      ****************************************
      *** Sub opção do menu - quantidade em **
      *** gramas de ração ********************
      ****************************************/
      
       if (tecla== 14) // o usuario apertou '*' e deseja entrar no submenu
     { 
       aux_inner_tela=0; // variável que seta o laço do submenu (prende o laço)
       tecla=0;
       delay(200);
       lcd.blink();
       gramasLSB = EEPROM.read(4); // Byte menos significativo
       gramasMSB = EEPROM.read(5); // Byte mais significativo
       gramasMSB = gramasMSB * 256; // desloca 8 posições para a esquerda 
       lcd.setCursor(4,1);
       gramas = gramasLSB+gramasMSB;
       lcd.print(gramas);
       
      while (aux_inner_tela==0)
      { 
        varredura_de_teclado();
        
        if (aux_inner_tela==0)
       { //lcd.noBlink();
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Liberar porcoes");
         lcd.setCursor(0,1);
         lcd.print("de: ");
         lcd.setCursor(10,1);
         lcd.print("gramas");
         lcd.setCursor(4,1);
         lcd.print(gramas);
         delay(100);
         
         
         if(tecla == 10) 
         {
         gramas = gramas + 50; // step escolhido de 50g
         tecla = 0;
         delay(50);
         if(gramas > 9950) // até 9,95 kg
           gramas = 9950;
         }
         
         else if(tecla == 11) 
         {
         gramas = gramas - 50;
         tecla = 0;
         delay(50);
         if(gramas<0)
           gramas = 0;
         }
         
         
          
          
         else if (tecla== 15)
         { 
           aux_inner_tela ++;
           tecla=0;
           lcd.noBlink();
         }
         
         else if (tecla== 14)
         { 
           aux_inner_tela ++;
           tecla=0;
           gramasLSB = gramas & 0x00FF; // máscara com 1's a direita -> byte menos significativo
           gramasMSB = gramas & 0xFF00; // máscara com 1's a esquerda -> byte mais significativo
           gramasMSB = gramasMSB / 256; // shift à direita para byte mais significativo ser guardado na E2PROM
           EEPROM.write(4,gramasLSB);
           EEPROM.write(5,gramasMSB);
           lcd.noBlink();       
         }
           
         
    }
      }
     }
     
     /*****************************************
     ******************************************
     ******************************************
     *****************************************/
       
       
    }
  
    /********************************************************
    *********************************************************
    ***************4ª OPÇÃO DE MENU********************
    ***************SAIDA DO MENU DE OPÇOES*********************
    ********************************************************/ 
    
    if(aux_inner_menu == 3) // terceira opção de tela
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sair do menu");
      lcd.setCursor(0,1);
      lcd.print("de opcoes.");
      delay(200);
     
      
      if(tecla == 10)// mover a tela para a anterior
      {
        aux_inner_menu --;
        tecla = 0;
      }
       delay(200);
        if (tecla==14)// sair das opções de tela
        { 
        aux_menu=0;
        tecla=0;
        lcd.clear();
        lcd.noBlink();
        }
    }
     
     
    /********************************************************
    *********************************************************
    ***************FIM DO MENU DE OPÇÕES**********************
    *********************************************************
    ********************************************************/ 
    
  }
    
  }
}
  
 
 
 
 /****************************************/
 
 /***************************************/
 
void transform_ASCII_to_int (void)
{

 /******************************
  ******************************
  *****Leitura da HORA**********
  ******************************
  *****************************/

 hrm = rtc.getTimeStr();


     

 for (i= 0 ; i<8; i++)
 {

    if(i != 2 && i != 5 )
    {
      HRM[i] = (int)hrm[i];

      if(HRM[i] == 0x30)
          HRM[i] = 0;

      else if(HRM[i] == 0x31)
          HRM[i] = 1;

      else if(HRM[i] == 0x32)
          HRM[i] = 2;    

      else if(HRM[i] == 0x33)
          HRM[i] = 3;
          
      else if(HRM[i] == 0x34)
          HRM[i] = 4;
      
      else if(HRM[i] == 0x35)
          HRM[i] = 5;
      
      else if(HRM[i] == 0x36)
          HRM[i] = 6;

      else if(HRM[i] == 0x37)
          HRM[i] = 7;

      else if(HRM[i] == 0x38)
          HRM[i] = 8;
     
      else if(HRM[i] == 0x39)
          HRM[i] = 9;
      
      switch(i)
      {
        case 0:
        hora = HRM[i]*10;
        break;

        case 1:
        hora = hora + HRM[i];
        break;

        case 3:
        minu = HRM[i]*10;
        break;

        case 4:
        minu = minu + HRM[i];
        break;

        case 6:
        seg = HRM[i]*10;
        break;

        case 7:
        seg = seg + HRM[i];
        break;
      }
      
    }
    
  
 }


 /******************************
  ******************************
  ******************************
  ******************************
  *****************************/

 /******************************
  ******************************
  *****Leitura do Dia **********
  *********da Semana************
  *****************************/

week_day = rtc.getDOWStr(FORMAT_SHORT);
 

if(week_day == "Sunday")
{
    WEEK_DAY = "Dom";
    dia_da_semana = 0; // posição do apontador em DOMINGO
}

else if(week_day == "Monday")
{
    WEEK_DAY = "Seg";
    dia_da_semana = 1; // posição do apontador em Segunda
}

else if(week_day == "Tuesday")
{
    WEEK_DAY = "Ter";
    dia_da_semana = 2; // posição do apontador em TERÇA
}

else if(week_day == "Wednesday")
{
    WEEK_DAY = "Qua";
    dia_da_semana = 3; // posição do apontador em QUARTA
}

else if(week_day == "Thursday")
{
    WEEK_DAY = "Qui";
    dia_da_semana = 4; // posição do apontador em QUINTA
}

else if(week_day == "Friday")
{
    WEEK_DAY = "Sex";
    dia_da_semana = 5; // posição do apontador em SEXTA
}

else if(week_day == "Saturday")
{
    WEEK_DAY = "Sab";
    dia_da_semana = 6; // posição do apontador em SÁBADO
}

 
 /******************************
  ******************************
  ******************************
  ******************************
  *****************************/



 /******************************
  ******************************
  *****Leitura do Dia **********
  *********da Semana************
  *****************************/

data= rtc.getDateStr();

for (i= 0 ; i<5; i++)
{
   if(i != 2)
    DATA[i] = (int)data[i];

     if(DATA[i] == 0x30)
          DATA[i] = 0;

      else if(DATA[i] == 0x31)
          DATA[i] = 1;

      else if(DATA[i] == 0x32)
          DATA[i] = 2;    

      else if(DATA[i] == 0x33)
          DATA[i] = 3;
          
      else if(DATA[i] == 0x34)
          DATA[i] = 4;
      
      else if(DATA[i] == 0x35)
          DATA[i] = 5;
      
      else if(DATA[i] == 0x36)
          DATA[i] = 6;

      else if(DATA[i] == 0x37)
          DATA[i] = 7;

      else if(DATA[i] == 0x38)
          DATA[i] = 8;
     
      else if(DATA[i] == 0x39)
          DATA[i] = 9;
    

     switch(i)
      {
        case 0:
        dia = DATA[i]*10;
        break;

        case 1:
        dia = dia +DATA[i];
        break;

        case 3:
        mes = DATA[i]*10;
        break;

        case 4:
        mes = mes +DATA[i];
        break;

       
      }
}


   /******************************
  ******************************
  ******************************
  ******************************
  *****************************/
  
}


void mostra_hora_data_atualizar (void)
{
         lcd.noBlink();
         //delay(100);
         
         lcd.setCursor(0,0);
         lcd.print("data");
         lcd.setCursor(5,0);
         lcd.print(dia);
         lcd.print("/");
         lcd.print(mes);
         lcd.setCursor(0,1);
         lcd.print("hora");
         lcd.setCursor(5,1);
         lcd.print(hora);
         lcd.print(":");
         lcd.print(minu);
         lcd.print(":");
         lcd.print(seg);
         lcd.setCursor(13,0);
         
         if (dia_da_semana == 0)
             lcd.print("Dom");
         else if (dia_da_semana == 1)
             lcd.print("Seg");
         else if (dia_da_semana == 2)
             lcd.print("Ter");
         else if (dia_da_semana == 3)
             lcd.print("Qua");
         else if (dia_da_semana == 4)
             lcd.print("Qui");
         else if (dia_da_semana == 5)
             lcd.print("Sex");
         else if (dia_da_semana == 6)
             lcd.print("Sab");
            
         
         lcd.blink();
         
         if (apontador == 0) 
           lcd.setCursor(5,0);
         
         else if (apontador == 1) 
           lcd.setCursor(8,0);
         
         else if (apontador == 2) 
           lcd.setCursor(13,0);
           
         else if (apontador == 3) 
           lcd.setCursor(5,1);
           
        else if (apontador == 4) 
           lcd.setCursor(8,1);
         
        else if (apontador == 5) 
           lcd.setCursor(11,1);
}
 
