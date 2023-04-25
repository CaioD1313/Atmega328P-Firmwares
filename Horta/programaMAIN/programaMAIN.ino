#include <HT1380.h>
#include <EEPROM.h>



#include <LiquidCrystal.h>
#define led 7
#define up 14 // pin A0
#define down 15 // PIN A1
#define left 16 // pin A2
#define right 17 // PIN A3

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int year,month,day,hour,minu,sec,date;
int aux_menu; // variável auxiliar para o menu
int aux_setup; // variável auxiliar para programaçao dos setups da irrigação
int exit_menu; // condição de hold do menu 0 fica / 1 sai
int curs0r=0; // auxiliar para andar com o cursor
int aux_prog; // variaual auxiliar para programar dia e hora da irrigação
int e2prom_r; // variavel auxiliar de leitura da eeprom
int e2prom_r_val; // variavel que guarda o valor lido da e2prom

HT1380 rtc = HT1380(8, 9, 10);


/***********************************
 *ROTINA AUXILIAR ESCRITA NO DISPLAY*
 ****DURANTE PROG DIA E HORA IRRIG***/

void lcd_aux_prog_irrig (void)
{
         lcd.setCursor(0,0);
         lcd.print("Dia & Hora Irrig");
         lcd.setCursor(0,1);

         switch(aux_prog)
 {
  case 1:
  lcd.print("Dom");
  break;

  case 2:
  lcd.print("Seg");
  break;

  case 3:
  lcd.print("Ter");
  break;

  case 4:
  lcd.print("Qua");
  break;

  case 5:
  lcd.print("Qui");
  break;

  case 6:
  lcd.print("Sex");
  break;

  case 7:
  lcd.print("Sab");
  break;
   
 }


         
         lcd.setCursor(5,1);
         lcd.print("H:");
         lcd.setCursor(12,1);
         lcd.print("M:");
         lcd.setCursor(7,1);
         lcd.print(hour);
         lcd.setCursor(14,1);
         lcd.print(minu);
         lcd.noBlink();
         delay(500); 
         lcd.blink();
}



/***********************************
 **ROTINA AUXILIAR LEITURA**********
 ************DA EEPROM*************/

 void e2prom_read (void)
 {
  e2prom_r_val= EEPROM.read(e2prom_r);
 }





/***********************************
 **ROTINA AUXILIAR PARA PROG DIA***
 *******HORA DA IRRIGAÇÃO***********/

 void dia_hora_irrig (void)
 {

  while(aux_setup==0) // hold da atualização da hora/min/seg
      {
       
       lcd.setCursor(curs0r,1);
       if(digitalRead(up)== HIGH && curs0r==7)
       {
        hour++; //incrementa hora
        if(hour>23) // se a hora for maior que 23 zera a hora
         {
         hour=0;
         lcd.clear();
         
         }
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
        
       }

       if(digitalRead(up)== HIGH && curs0r==14)
       {
        minu++; //incrementa minuto
        if(minu>59) // se a hora for maior que 23 zera a hora
         {
         minu=0;
         lcd.clear();
          // chama rotina pra rescrever os dados na tela
         
         }
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
        
       }


       

       if(digitalRead(right)==HIGH)
       {
        curs0r++;
        if(curs0r>15)
          {
          aux_setup=1; // sai do hold da programação da hora
          lcd.clear();
          lcd.noBlink();
         // aux_prog=2; // teste para sair da condição de programação OK
          
          }
          
        delay(500);   
       }

       if(digitalRead(left)==HIGH)
       {
        curs0r--;
        if(curs0r<0)
          curs0r=0; //segura cursor no canto esquerdo da tela
        delay(500); 
       }


       if(digitalRead(down)== HIGH && curs0r==7)
       {
        hour--; //decrementa hora
        if(hour<0) // se a hora for menor que zero seta hora para 23
         {
         hour=23;
         //lcd.clear();
         
         }
        lcd.clear();
        lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
        
       }


       if(digitalRead(down)== HIGH && curs0r==14)
       {
        minu--; //decrementa minuto
        if(minu<0) // se a hora for menor que 0 seta minuto para 59
         {
         minu=59;
         
         }
         lcd.clear();
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
       }

         

       
       
       


       
      } // while aux_setup
  
  
 }





/***********************************
 **ROTINA AUXILIAR PARA PROG DATA***
 ************#MODELO 2**************/
void hourreader (void)
{
  //lcd.clear();
  rtc.readBurst();
  hour=(int)rtc.getHour();
  minu=(int)rtc.getMin();
  sec=(int)rtc.getSec();
  
  
  
}



/***********************************
 **ROTINA AUXILIAR PARA PROG DATA***
 **********************************/
void letter_date (void)
{
  
 lcd.setCursor(0,0);
 lcd.print("Atualizar data");
 lcd.setCursor(0,1);
 lcd.print("D:");
 lcd.print(date);
 lcd.setCursor(6,1);
 lcd.print("M:");
 lcd.print(month);
 
 lcd.setCursor(13,1);
 switch(day)
 {
  case 1:
  lcd.print("Dom");
  break;

  case 2:
  lcd.print("Seg");
  break;

  case 3:
  lcd.print("Ter");
  break;

  case 4:
  lcd.print("Qua");
  break;

  case 5:
  lcd.print("Qui");
  break;

  case 6:
  lcd.print("Sex");
  break;

  case 7:
  lcd.print("Sab");
  break;
   
 }
 
  
}

/**********************************
 ***ROTINA DE ESCRITA NO RELOGIO***
 **************HORA***************/

 void Write_clock (void)
 {
  rtc.setHour(hour);
  rtc.setMin(minu);
  rtc.setSec(sec);
  
  rtc.setWP(1);
  rtc.writeBurst();
 }

/**********************************
 ***ROTINA DE ESCRITA NO RELOGIO***
 **************DATA***************/
 void Write_clock_date (void)
 {
  rtc.setDate(date);
  rtc.setMonth(month);
  rtc.setDay(day);
  rtc.setWP(1);
  rtc.setHour(hour);
  rtc.setMin(minu);
  rtc.setSec(sec);
  
  rtc.writeBurst();
  
 }


/********************************
 *ROTINA AUXILIAR PARA PROG HORA*
 ******************************* 
 *******************************/

void letter (void)
{
 lcd.setCursor(0,0);
 lcd.print("Atualizar hora");
 lcd.setCursor(0,1);
 lcd.print("H:");
 lcd.print(hour);
 lcd.setCursor(6,1);
 lcd.print("M:");
 lcd.print(minu);
 lcd.setCursor(12,1);
 lcd.print("S:");
 lcd.print(sec);
  
}

/**********************************
 ***ROTINA DE LEITURA DO RELOGIO***
 **********************************/

void clockreader (void)
{
  lcd.clear();
  rtc.readBurst();
  year=(int)rtc.getYear();
  month=(int)rtc.getMonth();
  date=(int)rtc.getDate();

  hour=(int)rtc.getHour();
  minu=(int)rtc.getMin();
  sec=(int)rtc.getSec();
  
  day=(int)rtc.getDay();
  
}


/**********************************
 ***ROTINA PARA MOSTRAR O RELOGIO***
 **********************************/

void clockshower (void)
{
 
 lcd.setCursor(0,0);
 lcd.print("Irrigacao");
 lcd.setCursor(13,0);

 switch(day)
 {
  case 1:
  lcd.print("Dom");
  break;

  case 2:
  lcd.print("Seg");
  break;

  case 3:
  lcd.print("Ter");
  break;

  case 4:
  lcd.print("Qua");
  break;

  case 5:
  lcd.print("Qui");
  break;

  case 6:
  lcd.print("Sex");
  break;

  case 7:
  lcd.print("Sab");
  break;
   
 }
 
  
 lcd.setCursor(0,1);
 lcd.print(date);
 lcd.print("/");
 lcd.print(month);

 lcd.setCursor(8,1);
 lcd.print(hour);
 lcd.print(":");
 lcd.print(minu);
 lcd.print(":");
 lcd.print(sec);
 
}



/**********************************
 ***************SETUPS*************
 **********************************/

void setup() {
  lcd.begin(16,2); // inicializa lcd
  rtc.init(); // inicializa HT1380 <CLK>

  /*rtc.setYear(EEPROM.read(0)); // seta ano de acordo com adress 0
  rtc.setMonth(EEPROM.read(1)); // seta mês de acordo com address 1
  rtc.setDate(EEPROM.read(2)); // seta ano de acordo com adress 2
  rtc.setHour(EEPROM.read(3)); // seta ano de acordo com adress 3
  rtc.setMin(EEPROM.read(4)); // seta ano de acordo com adress 4
  rtc.setSec(EEPROM.read(5)); // seta ano de acordo com adress 5
  rtc.setDay(EEPROM.read(6)); // seta ano de acordo com adress 0
  rtc.setWP(1);
  rtc.writeBurst();*/

  /************************************
   ******Definindo pinos de controle***
   ***********************************/
  pinMode(led,OUTPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  /*************************************
   *************************************
   ************************************/

  /*************************************
   *Seta condições inicias da programação 
   **************************************/
  aux_menu=0; // seta condição inicial do menu
  exit_menu=0; // seta condição de hold no menu
  aux_setup=0; // seta condição de hold do setup
  aux_prog=1; // seta condição do primeiro dia de progamação da irrigação
}

void loop() {
  
  //letter();
  clockreader();
  clockshower();
  aux_menu=0; // seta condição inicial do menu
  exit_menu=0; // seta condição de hold no menu
  aux_setup=0; // seta condição de hold do setup
  delay(40);
  lcd.clear();
  
  
  /*********************************
   ******AQUI ENTRA NO MENU********* 
   ************DE OPÇÕES************
   *********************************/
  
  
  
  
  if(digitalRead(up) == HIGH) // aperte para cima para entrar no menu
  {
   
    
    lcd.clear(); // limpa display
    aux_setup=0; // seta confirmação para zero
    
    
    while(exit_menu==0){
      
    
  
   
   /*********************************
   ******PRIMEIRA OPÇÃO DO MENU****** 
   **AJUSTAR HORA/MIN/SEG DO CLOCK***
   *********************************/
    while(aux_menu==0)
    {
    
    lcd.setCursor(0,0);
    lcd.print("Menu:");
    lcd.setCursor(0,1);
    lcd.print("Atualizar H/M/S");
    
    if(digitalRead(left) == HIGH) // se o botao direito for pressinado entao ira fazer a programação das horas e minutos da irrigação
     {
      lcd.blink();
      lcd.clear();
      aux_setup=0; // condição de hold do while da programação
      clockreader(); // le o relogio atual
      letter(); // mostra no canto a hora atual somente hora
     // lcd.setCursor(2,1);
      
      curs0r=2; // guarda a posição que o cursor está
      delay(500);
      while(aux_setup==0) // hold da atualização da hora/min/seg
      {
       
       lcd.setCursor(curs0r,1);
       if(digitalRead(up)== HIGH && curs0r==2)
       {
        hour++; //incrementa hora
        if(hour>23) // se a hora for maior que 23 zera a hora
         {
         hour=0;
         lcd.clear();
         letter();
         }
        lcd.setCursor(curs0r,1);
        lcd.print(hour);
        delay(500); 
       }

       if(digitalRead(up)== HIGH && curs0r==8)
       {
        minu++; //incrementa minuto
        if(minu>59) // se a hora for maior que 23 zera a hora
         {
         minu=0;
         lcd.clear();
         letter();
         }
        lcd.setCursor(curs0r,1);
        lcd.print(minu);
        delay(500); 
       }


       if(digitalRead(up)== HIGH && curs0r==14)
       {
        sec++; //incrementa segundo
        if(sec>59) // se o segundo for maior que 59 zera o segundo
         {
         sec=0;
         lcd.clear();
         letter();
         }
        lcd.setCursor(curs0r,1);
        lcd.print(sec);
        delay(500); 
       }

       if(digitalRead(right)==HIGH)
       {
        curs0r++;
        if(curs0r>15)
          {
          aux_setup=1; // sai do hold da programação da hora
          lcd.clear();
          lcd.noBlink();
          Write_clock();
          /*aux_menu=1;
          exit_menu=1;*/ //teste para ver se grava no relogio OK
          }
          
        delay(500);   
       }

       if(digitalRead(left)==HIGH)
       {
        curs0r--;
        if(curs0r<0)
          curs0r=0; // sai do hold da programação da hora
        delay(500); 
       }


       if(digitalRead(down)== HIGH && curs0r==2)
       {
        hour--; //decrementa hora
        if(hour<0) // se a hora for menor que zero seta hora para 23
         {
         hour=23;
         //lcd.clear();
         
         }
        lcd.clear();
        letter();
        lcd.setCursor(curs0r,1);
        lcd.print(hour);
        delay(500); 
       }


       if(digitalRead(down)== HIGH && curs0r==8)
       {
        minu--; //decrementa minuto
        if(minu<0) // se a hora for menor que 0 seta minuto para 59
         {
         minu=59;
         
         }
        lcd.clear();
        letter();
        lcd.setCursor(curs0r,1);
        lcd.print(minu);
        delay(500); 
       }

         if(digitalRead(down)== HIGH && curs0r==14)
       {
        sec--; //decrementa segundo
        if(sec<0) // se o segundo for menor que zero seta segundo para 59
         {
         sec=59;
         
         }
        lcd.clear();
        letter();
        lcd.setCursor(curs0r,1);
        lcd.print(sec);
        delay(500); 
       }

       
       
       


       
      } // while aux_setup
      
     }//if left bottom pressed

      
      /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
      if(digitalRead(down)==1) // se pressionado o botão para baixo vai para a segunda opção de menu
      {
        delay(500);
        lcd.clear();
        aux_menu=1;
        
      } // if down bottem pressed
      /********************************************************************/
    
    } //while menu = 0 <ou seja, primeira condição no display>
  
  
  
  
  
  
  
  
  /*********************************
   ******SEGUNDA OPÇÃO DO MENU****** 
   *AJUSTAR DIA DA SEMANA DO CLOCK**
   *********************************/
   while(aux_menu==1)
   {
    lcd.setCursor(0,0);
    lcd.print("Atualizar");
    lcd.setCursor(0,1);
    lcd.print("Data D/M e Sem");

    if(digitalRead(left)==HIGH)
    {

      lcd.blink();
      lcd.clear();
      aux_setup=0; // condição de hold do while da programação
      clockreader(); // le o relogio atual
      letter_date(); // mostra no canto a hora atual somente hora
     // lcd.setCursor(2,1);
      
      curs0r=2; // guarda a posição que o cursor está
      delay(500);

      while(aux_setup==0)
     {
       
       lcd.setCursor(curs0r,1);

       
       if(digitalRead(up)== HIGH && curs0r==2)
       {
        date++; //incrementa dia
        if(date>31) // se o dia for maior que 31 volta pro dia primeiro
         {
         date=1;
         lcd.clear();
         letter_date();
         }
        lcd.setCursor(curs0r,1);
        lcd.print(date);
        delay(500); 
       }

       if(digitalRead(up)== HIGH && curs0r==8)
       {
        month++; //incrementa mês
        if(month>12) // se o mês for maior que 12 volta para janeiro
         {
         month=1;
         lcd.clear();
         letter_date();
         }
        lcd.setCursor(curs0r,1);
        lcd.print(month);
        delay(500); 
       }


       if(digitalRead(up)== HIGH && curs0r==13)
       {
        day++; //incrementa dia da semana
        if(day>7) // se passar de sabado vira domingo
         {
         day=1;
         lcd.clear();
         letter_date();
         }
        lcd.setCursor(curs0r,1);
        letter_date();
        delay(500); 
       }

       if(digitalRead(right)==HIGH)
       {
        curs0r++;
        if(curs0r>15)
          {
          aux_setup=1; // sai do hold da programação da hora
          lcd.clear();
          lcd.noBlink();
          hourreader();
          Write_clock_date();
          /*aux_menu=1;
          exit_menu=1;*/ //teste para ver se grava no relogio OK
          }
          
        delay(500);   
       }

       if(digitalRead(left)==HIGH)
       {
        curs0r--;
        if(curs0r<0)
          curs0r=0; // matem cursor no canto esquerdo
        delay(500); 
       }


       if(digitalRead(down)== HIGH && curs0r==2)
       {
        date--; //decrementa dia
        if(date<=0) // se o dia for menor que zero volta para 31
         {
         date=31;
         
         }
        lcd.clear();
        letter_date();
        lcd.setCursor(curs0r,1);
        lcd.print(date);
        delay(500); 
       }


       if(digitalRead(down)== HIGH && curs0r==8)
       {
        month--; //decrementa mês
        if(month<=0) // se o mês for menor que zero volta para dezembro
         {
         month=12;
         
         }
        lcd.clear();
        letter_date();
        lcd.setCursor(curs0r,1);
        lcd.print(month);
        delay(500); 
       }

         if(digitalRead(down)== HIGH && curs0r==13)
       {
        day--; //decrementa dia da semana
        if(day<=0) // se o dia da semana for menor que zero volta pra sabado
         {
         day=7;
         
         }
        lcd.clear();
        lcd.setCursor(curs0r,1);
        letter_date();
        delay(500); 
       }

       
       
       


       
      } // while aux_setup
      
    }
    

    /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
    if(digitalRead(up)==HIGH) // se up bottom pressed go back to option 1
    {
      delay(500);
      lcd.clear();
      aux_menu=0;
      
    }

    if(digitalRead(down)==HIGH) // se down bottom pressed go to option 3
    {
      delay(500);
      lcd.clear();
      aux_menu=2;
      
    }

    /********************************************************************/
    
   } // WHILE AUX_MENU == 1 <OPÇAO 2>

   
   
   
   
   
   
   
   
   /*********************************
   *****TERCEIRA OPÇÃO DO MENU******* 
   **********************************
   *********************************/
  while(aux_menu==2)
  {
    lcd.setCursor(0,0);
    lcd.print("Prog. Dia & Hora");
    lcd.setCursor(3,1);
    lcd.print("Da Irrigacao");
  
   
    if(digitalRead(left)==HIGH) // se apertar botao esquerdo entra na programação do dia e hora da irrigação
    {

      lcd.blink();
      lcd.clear();
      aux_setup=0; // condição de hold do while da programação
      aux_prog=1;
      curs0r=7;
      
      lcd.setCursor(0,0);
      lcd.print("Dia & Hora Irrig");
      delay(300);
      

     /*****************************************************************************
      ***************************PROGRAMAÇÃO DO DOMINGO****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Dom");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=0; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=1; // posição do min do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        lcd.setCursor(curs0r,1);
        dia_hora_irrig();
        
        EEPROM.write(0,hour); // salva a hora programada na posição zero (domingo)
        EEPROM.write(1,minu); // salva o minuto programado na posição um (doming)
        aux_prog=2;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      
      
      
      /*****************************************************************************
      **********************PROGRAMAÇÃO Da SEGUNDA-FEIRA****************************
      ****************************************************************************/
        
        lcd.setCursor(0,0);
        lcd.print("Dia & Hora Irrig");
       
        lcd.setCursor(0,1);
        lcd.print("Seg");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=2; // posição da hora do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=3; // posição do min do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        
        dia_hora_irrig();
        
        EEPROM.write(2,hour); // salva a hora programada na posição dois (Seg)
        EEPROM.write(3,minu); // salva o minuto programado na posição tres (Seg)
        aux_prog=3;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

         /*****************************************************************************
      **********************PROGRAMAÇÃO Da TERÇA-FEIRA****************************
      ****************************************************************************/
        lcd.setCursor(0,0);
        lcd.print("Dia & Hora Irrig");
        lcd.setCursor(0,1);
        lcd.print("Ter");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=4; // posição da hora do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=5; // posição do min do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        
        dia_hora_irrig();
        
        EEPROM.write(4,hour); // salva a hora programada na posição quatro (ter)
        EEPROM.write(5,minu); // salva o minuto programado na posição cinco (ter)
        aux_prog=4;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/


      /*****************************************************************************
      **********************PROGRAMAÇÃO Da QUARTA-FEIRA****************************
      ****************************************************************************/
        lcd.setCursor(0,0);
        lcd.print("Dia & Hora Irrig");
        lcd.setCursor(0,1);
        lcd.print("Qua");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=6; // posição da hora da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=7; // posição do min da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        dia_hora_irrig();
        
        EEPROM.write(6,hour); // salva a hora programada na posição seis (quarta)
        EEPROM.write(7,minu); // salva o minuto programado na posição sete (quarta)
        aux_prog=5;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      /*****************************************************************************
      **********************PROGRAMAÇÃO Da QUINTA-FEIRA****************************
      ****************************************************************************/
        lcd.setCursor(0,0);
        lcd.print("Dia & Hora Irrig");
        lcd.setCursor(0,1);
        lcd.print("Qui");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=8; // posição da hora da QUINTA na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=9; // posição do min da quinta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        dia_hora_irrig();
        
        EEPROM.write(8,hour); // salva a hora programada na posição oito (Quinta)
        EEPROM.write(9,minu); // salva o minuto programado na posição nove (Quinta)
        aux_prog=6;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      /*****************************************************************************
      **********************PROGRAMAÇÃO Da Sexta-FEIRA****************************
      ****************************************************************************/
        lcd.setCursor(0,0);
        lcd.print("Dia & Hora Irrig");
        lcd.setCursor(0,1);
        lcd.print("Sex");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=10; // posição da hora da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=11; // posição do min da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        dia_hora_irrig();
        
        EEPROM.write(10,hour); // salva a hora programada na posição dez(Sexta)
        EEPROM.write(11,minu); // salva o minuto programado na posição onze (sexta)
        aux_prog=7;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

         
      /*****************************************************************************
      **********************PROGRAMAÇÃO Do Sabado***********************************
      ****************************************************************************/
        lcd.setCursor(0,0);
        lcd.print("Dia & Hora Irrig");
        lcd.setCursor(0,1);
        lcd.print("Sab");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=12; // posição da hora do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=13; // posição do min do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        dia_hora_irrig();
        
        EEPROM.write(12,hour); // salva a hora programada na posição doze(Sab)
        EEPROM.write(13,minu); // salva o minuto programado na posição treze (Sab)
        aux_setup=0;
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/
         
        
      

         
        
      
      
    }
   
   
   
   
   /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
   if(digitalRead(up)==1) // se up bottom pressed go back to option 2
    {
      delay(500);
      lcd.clear();
      aux_menu=1;
      
    }

    if(digitalRead(down)==1) // se down bottom pressed go to option 4
    {
      delay(500);
      lcd.clear();
      aux_menu=3;
      
    }

    /********************************************************************/
   
  } // WHILE AUX_MENU == 2 <OPÇAO 3>

  
  
  
  
  
  /**********************************
   ******QUARTA OPÇÃO DO MENU********
   **********************************
   *********************************/
  while(aux_menu==3)
  {
   lcd.setCursor(0,0);
   lcd.print("Opcao 4");

   /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
   if(digitalRead(up)==1) // se up bottom pressed go back to option 3
    {
      delay(500);
      lcd.clear();
      aux_menu=2;
      
    }

    if(digitalRead(down)==1) // se down bottom pressed go to option 5
    {
      delay(500);
      lcd.clear();
      aux_menu=4;
      
    }

    /********************************************************************/
   
    
  }// WHILE AUX_MENU == 3 <OPÇAO 4>

  
   
   
   
   
   
   
   /*********************************
   *******QUINTA OPÇÃO DO MENU********
   ***********SAIR DO MENU***********
   *********************************/
  while(aux_menu==4)
  {
    lcd.setCursor(0,0);
    lcd.print("Opcao 5");

    /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
   if(digitalRead(up)==1) // se up bottom pressed go back to option 4
    {
      delay(500);
      lcd.clear();
      aux_menu=3;
      
    }

    if(digitalRead(down)==1) // se down bottom pressed go to option 6
    {
      delay(500);
      lcd.clear();
      aux_menu=5;
      
    }

    /********************************************************************/

    
    
  } // WHILE AUX_MENU == 4 <OPÇAO 5>
   
   
   
   
   
   
   
   
   
   
   /*********************************
   *******SEXTA OPÇÃO DO MENU********
   ***********SAIR DO MENU***********
   *********************************/
  while(aux_menu==5)
  {
    lcd.setCursor(0,0);
    lcd.print("Sair do Menu");
    lcd.setCursor(7,1);
    lcd.print("de opcoes");

    /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
   if(digitalRead(up)==1) // se up bottom pressed go back to option 4
    {
      delay(500);
      lcd.clear();
      aux_menu=4;
      
    }
    /********************************************************************/

    if(digitalRead(left)==1) // se apertar botao esquerdo sai do menu de opções
    {
      delay(500);
      lcd.clear();
      exit_menu=1;
      aux_menu=6;
    }
    
    
  } // WHILE AUX_MENU == 5 <OPÇAO 6>

  
  }// while do menu (EXIT)
    
     
  }
  

}
