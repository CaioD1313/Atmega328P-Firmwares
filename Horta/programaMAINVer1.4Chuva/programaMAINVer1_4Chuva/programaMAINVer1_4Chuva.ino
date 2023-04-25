

#include <HT1380.h>
#include <EEPROM.h>



#include <LiquidCrystal.h>
#define valv 7 // solenoid valve attached to the 7nd pin
#define rain_sensor A5 // sensor de chuva attached to the A5 pin
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
int programacao; // variavel auxiliar para os letreiros de programação
int able; // variavel que guardara se o dia está habilitado para irrigação 0 -> Nao 1-> Sim
int deltaH,deltaM; // variaveis que auxiliarão nas horas e minutos máximos que poderá ser imposto dependendo da hora da irrigação escolhida. (para a irrigação de um dia não atravessar no outro).
int delta_flag; // variável auxiliar para decrementação da hr/min dependendo se está programando o inicio ou duração da irrigação
int cont; // variável do for que zera memória da duração da irrigação
int dia_irrig; // variável para saber em qual dia se está na hora da irrigação
int hour_irrig,minu_irrig,hour_irrig1,minu_irrig1;
int hour_irrig_duration,minu_irrig_duration;
int flag_irrig; // liga~desliga valvula
int pause,aux_pause; // variaveis para controlar a parada do sistema por interferencia do usuario -> pause para e aux_pause manda letreiro de religação da irrigação
int clock_reader_flag;
int sensor; // variavel que lerá a entrada analógica do sensor de chuva
int cicle,cicle_aux; //variavel responsavel por selecionar ciclo 1 ou 2
int first,second,h;

HT1380 rtc = HT1380(8, 9, 10);







/*******************************************
 * 
 *******************************************/

 void valvula_open_close (void)
 {
  first = hour_irrig*60+(minu_irrig);
  second = ((hour_irrig+hour_irrig1)*60)+(minu_irrig+minu_irrig1);
  h=hour*60+minu; 
  if(sensor>350)
      {
         
     // if(((hour>=hour_irrig )&& (minu>=minu_irrig)) || (hour<=(hour_irrig+hour_irrig1) && (minu<=(minu_irrig+minu_irrig1))))
        if(h>=first && h<=second)
     {
     
     
     
     
    // hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     hour=hour*60+minu;
     if((hour<(hour_irrig_duration)) && (aux_pause==1))
     {
      aux_pause=0;
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("RETOMANDO");
      lcd.setCursor(3,1);
      lcd.print("IRRIGACAO");
      delay(3000);
      lcd.clear();
      
      
     }
     
     while(flag_irrig==0)
     {
     // if(digitalRead(down)==HIGH)
     //  pause=1;
       
      clockreader();
      clockshower();
      sensor=analogRead(rain_sensor);
      delay(30);
      hour=hour*60+minu;
     
      
      if((hour>(hour_irrig_duration-1)) && sec>=0)
       {
       flag_irrig=1;
       clock_reader_flag=0;
       }
      if(flag_irrig==0)
      {
        digitalWrite(valv,HIGH); 
        clock_reader_flag=1;
      }
      if(sensor<=350)
        flag_irrig=1;
     // if(pause==1)
       // flag_irrig=1; 

    if((digitalRead(down)==1) && (hour<hour_irrig_duration))
      {
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("SYSTEM");
        lcd.setCursor(5,1);
        lcd.print("PAUSED");
        pause=1;
        flag_irrig=1;
        digitalWrite(valv,LOW);
        
        delay(2000);
        
      }
       
     }
    }
   }
   hour=hour*60+minu;
   while ((sensor<=350) && (hour<(hour_irrig_duration)) )
   {
    sensor=analogRead(rain_sensor);
    
    clock_reader_flag=2;
    clockreader();
    clockshower();
    
    hour=hour*60+minu;
    digitalWrite(valv,LOW);
    delay(40);
    if((digitalRead(down)==1) && (hour<hour_irrig_duration))
      {
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("SYSTEM");
        lcd.setCursor(5,1);
        lcd.print("PAUSED");
        pause=1;
        sensor=2000;
        digitalWrite(valv,LOW);
        
        delay(2000);
        
      }
   }
 }






/*******************************************
 *************ROTINA DE IRRIGAÇÃO***********
 *******************************************/

 void irrigacao (void)
 {
  
  clockreader();
  clock_reader_flag=0;
  flag_irrig=0;
 
  switch(day)
  {
    case 1:  // domingo
     /*hour_irrig=EEPROM.read(0); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(1); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(14); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(15); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;*/

    if(EEPROM.read(56)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(0); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(1); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(28); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(29); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();
    
     
     clockreader();


     hour_irrig=EEPROM.read(14); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(15); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(42); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(43); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }
    
    
    break;

    
    case 2: // segunda
 if(EEPROM.read(57)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(2); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(3); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(30); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(31); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();

    clockreader();
  
     hour_irrig=EEPROM.read(16); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(17); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(44); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(45); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }
    

    
    break;

    case 3: // terça 

if(EEPROM.read(58)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(4); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(5); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(32); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(33); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();

     clockreader();


     hour_irrig=EEPROM.read(18); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(19); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(46); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(47); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }
     

   
     
   
    break;

    case 4: // quarta

if(EEPROM.read(59)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(6); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(7); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(34); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(35); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();

     clockreader();


     hour_irrig=EEPROM.read(20); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(21); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(48); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(49); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }


    break;

    case 5:  // quinta

if(EEPROM.read(60)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(8); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(9); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(36); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(37); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();


     clockreader();


     hour_irrig=EEPROM.read(22); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(23); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(50); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(51); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }
    

    break;

    case 6: // sexta

     if(EEPROM.read(61)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(10); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(11); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(38); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(39); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();


     clockreader();


     hour_irrig=EEPROM.read(24); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(25); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(52); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(53); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }
    

    break;

    case 7: // sábado

    if(EEPROM.read(62)==1) // se o dia estiver habilitado então entra aqui
    {
      
      
     hour_irrig=EEPROM.read(12); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(13); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(40); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(41); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();


     clockreader();


     hour_irrig=EEPROM.read(26); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig=EEPROM.read(27); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig1=EEPROM.read(54); // guarda em hour_irrig a hora que começa a irrigação (dom)
     minu_irrig1=EEPROM.read(55); // guarda em minu_irrig o min que começa a irrigação
     hour_irrig_duration=hour_irrig+hour_irrig1;
     minu_irrig_duration=minu_irrig+minu_irrig1;
     hour_irrig_duration=hour_irrig_duration*60+minu_irrig_duration;
     valvula_open_close();



     
      
    }  
     

    break;
  }
  
 }





/*******************************************
 ****Rotina de hab/desabilitar irrigação****
 *******************************************/
 void hab_irrig (void)
 {

  while(aux_setup==0) // hold da atualização da hora/min/seg
      {
       
       lcd.setCursor(curs0r,1);
       if(digitalRead(up)== HIGH && curs0r==8)
       {
        
         lcd.clear();
         lcd.setCursor(curs0r,1);
         lcd.print("Sim");
         able=1;    
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
         lcd.noBlink();
         delay(200); 
         lcd.blink();
        
       }

       


       

       if(digitalRead(right)==HIGH)
       {
        curs0r++;
        if(curs0r>10)
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
        if(curs0r<8)
          curs0r=8; //segura cursor no canto esquerdo da tela
        delay(500); 
       }


       if(digitalRead(down)== HIGH && curs0r==8)
       {
        
        lcd.clear();
        lcd.setCursor(curs0r,1);
        lcd.print("Nao");
        able=0;
        
        lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
        lcd.noBlink();
         delay(200); 
         lcd.blink();
        
       }


      

         

       
       
       


       
      } // while aux_setup
  
  
 }





/***********************************
 *ROTINA AUXILIAR ESCRITA NO DISPLAY*
 ****DURANTE PROG DIA E HORA IRRIG***/

void lcd_aux_prog_irrig (void)
{
         lcd.setCursor(0,0);
         if(programacao==0)
            lcd.print("Dia & Hora Irrig");

         else if (programacao==1)
            lcd.print("Irrigar por:");

         else if(programacao==2)
            lcd.print("Hab. este dia?"); 
         
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


         if(programacao==0 || programacao ==1)
         {
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
 *******HORA DA IRRIGAÇÃO 1***********/

 void dia_hora_irrig_1 (void) // ciclo 1 
 {

  while(aux_setup==0) // hold da atualização da hora/min/seg
      {
       
       lcd.setCursor(curs0r,1);
       if(digitalRead(up)== HIGH && curs0r==7)
       {
        hour++; //incrementa hora
        
        if(hour==deltaH && minu>deltaM) // se a hora chega na máxima zera o minuto para nao ter conflito de min>minuto max
          {
          minu=0;
          lcd.clear();
          }
        if(hour>deltaH)
        {
         hour=0;
         lcd.clear();
        }
        
        
        if(hour>11) // se a hora for maior que 11 zera a hora
         {
         hour=0;
         lcd.clear();
         
         }
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
        
       }

       if(digitalRead(up)== HIGH && curs0r==14)
       {
        
        minu++; //incrementa minuto
        
        if(hour==deltaH)
        {
        if(minu>deltaM)
        {
         minu=0;
         lcd.clear();
        }
        }
        
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
         
         if(delta_flag==0)
           hour=11;
         if(delta_flag==1)
           hour=deltaH;
         if(minu>deltaM)
            minu=0;   
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
          if(delta_flag==0)
           minu=59;
         if(delta_flag==1)
           {
           if(hour==deltaH)
            minu=deltaM; 
           else
            minu=59;  
           }
         }
         lcd.clear();
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
       }

         

       
       
       


       
      } // while aux_setup
  
  
 }



 /***********************************
 **ROTINA AUXILIAR PARA PROG DIA***
 *******HORA DA IRRIGAÇÃO 2***********/

 void dia_hora_irrig_2 (void)
 {

  while(aux_setup==0) // hold da atualização da hora/min/seg
      {
       
       lcd.setCursor(curs0r,1);
       if(digitalRead(up)== HIGH && curs0r==7)
       {
        hour++; //incrementa hora
        
        if(hour==deltaH && minu>deltaM) // se a hora chega na máxima zera o minuto para nao ter conflito de min>minuto max
          {
          minu=0;
          lcd.clear();
          }
        if(hour>deltaH)
        {
         hour=0;
         lcd.clear();
        }
        
        
        if(hour>23) // se a hora for maior que 23 zera a hora
         {
         hour=12;
         lcd.clear();
         
         }
         lcd_aux_prog_irrig(); // chama rotina pra rescrever os dados na tela
        
       }

       if(digitalRead(up)== HIGH && curs0r==14)
       {
        
        minu++; //incrementa minuto
        
        if(hour==deltaH)
        {
        if(minu>deltaM)
        {
         minu=0;
         lcd.clear();
        }
        }
        
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
        if(hour<12) // se a hora for menor que zero seta hora para 23
         {
         
         if(delta_flag==0)
           hour=23;
         if(delta_flag==1)
           hour=deltaH;   
         if(minu>deltaM)
            minu=0;   
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
          if(delta_flag==0)
           minu=59;
         if(delta_flag==1)
           {
           if(hour==deltaH)
            minu=deltaM; 
           else
            minu=59;  
           }
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
 **************HORA***************

 void Write_clock (void)
 {
  rtc.setHour(hour);
  rtc.setMin(minu);
  rtc.setSec(sec);
  
  rtc.setWP(1);
  rtc.writeBurst();
 }*/

/**********************************
 ***ROTINA DE ESCRITA NO RELOGIO***
 **************DATA***************/
 void Write_clock_date (void)
 {
  rtc.setDate(date);
  rtc.setMonth(month);
  rtc.setDay(day);
  rtc.setHour(hour);
  rtc.setMin(minu);
  rtc.setSec(sec);
  
  rtc.setWP(1);
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
 if(clock_reader_flag==0)
 lcd.print("Irrigacao");
 if(clock_reader_flag==1)
 lcd.print("Irrigando");
 if(clock_reader_flag==2)
 lcd.print("ChuvaValvOFF");
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
  pinMode(valv,OUTPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  /*************************************
   *************************************
   **Definindo Pino do sensor de chuva**/
   pinMode(rain_sensor,INPUT);

   

  /*************************************
   *Seta condições inicias da programação 
   **************************************/
  aux_menu=0; // seta condição inicial do menu
  exit_menu=0; // seta condição de hold no menu
  aux_setup=0; // seta condição de hold do setup
  aux_prog=1; // seta condição do primeiro dia de progamação da irrigação
  clock_reader_flag=0;
  pause=0;
  aux_pause=0;
  sensor=1023; // seta sensor para valor **SEM CHUVA** <Chuva Máx -> Sensor =0 >
}

void loop() {
  
  //letter();
  
  clockreader();
  clockshower();
  aux_menu=0; // seta condição inicial do menu
  exit_menu=0; // seta condição de hold no menu
  aux_setup=0; // seta condição de hold do setup
  
  digitalWrite(valv,LOW);
  delay(40);
  lcd.clear();
  sensor = analogRead(rain_sensor);
  irrigacao();
  
  
  /*********************************
   ******AQUI ENTRA NO MENU********* 
   ************DE OPÇÕES************
   *********************************/

   
  
  
  
  
  if((digitalRead(up) == HIGH) || (pause == 1)) // aperte para cima para entrar no menu
  {
   
    
    lcd.clear(); // limpa display
    aux_setup=0; // seta confirmação para zero
   
    if(pause==1)
    {
    pause=0;
    aux_pause=1;
    }
    
    
    
    while(exit_menu==0){
      
    
  
   
   /*********************************
   ******PRIMEIRA OPÇÃO DO MENU****** 
   **AJUSTAR HORA/MIN/SEG DO CLOCK***
   *********************************/
    while(aux_menu==0)
    {
    
    lcd.setCursor(0,0);
    lcd.print("Menu: Atualizar");
    lcd.setCursor(0,1);
    lcd.print("Data e Hora");
    
    if(digitalRead(left) == HIGH) // se o botao direito for pressinado entao ira fazer a programação das horas e minutos da irrigação
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
          aux_setup=1; // sai do hold da programação da data e vai pra hora
          lcd.clear();
          curs0r=2;
          delay(300);
          hourreader();
          letter();
          //lcd.noBlink();
          //hourreader();
         // Write_clock_date();
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
        
      }
      
      
      while(aux_setup==1) // hold da atualização da hora/min/seg
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
          aux_setup=2; // sai do hold da programação da hora
          lcd.clear();
          lcd.noBlink();
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
        aux_menu=2;
        
      } // if down bottem pressed
      /********************************************************************/
    
    } //while menu = 0 <ou seja, primeira condição no display>
  
  
  
  
  
  
  
  
  /*********************************
   ******SEGUNDA OPÇÃO DO MENU****** 
   *AJUSTAR DIA DA SEMANA DO CLOCK**
   *********************************
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
       
       

       
       
       


       
      } // while aux_setup
      
    }*/
    

    /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
    /*if(digitalRead(up)==HIGH) // se up bottom pressed go back to option 1
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

    /********************************************************************
    
   }*/ // WHILE AUX_MENU == 1 <OPÇAO 2>

   
   
   
   
   
   
   
   
   /*********************************
   *****PROGRAMAÇÃO DO DIA************ 
   ***********E DA HORA****************
   *********DA IRRIGAÇÃO*************/
  while(aux_menu==2)
  {
    lcd.setCursor(0,0);
    lcd.print("Prog. Dia & Hora");
    lcd.setCursor(3,1);
    lcd.print("Da Irrigacao");
  
   
    if(digitalRead(left)==HIGH) // se apertar botao esquerdo entra na programação do dia e hora da irrigação
    {

      //lcd.blink();
      lcd.clear();
      aux_setup=0; // condição de hold do while da programação
      aux_prog=1;
      curs0r=7;
      programacao=0;
      delta_flag = 0; // programação do dia e hora
      cicle=0;
      cicle_aux=1; // seta para o ciclo 1
      lcd.clear();
      while(cicle==0)
      {
        delay(300);
        while(cicle_aux==1)
        {
          lcd.setCursor(4,0);
          lcd.print("Ciclo 1");
          lcd.setCursor(1,1);
          lcd.print("00:00 - 11:59");
          delay(40);
          if(digitalRead(down)==1)
            cicle_aux=2; // seta para o ciclo 2
          if(digitalRead(left)==1) // seleciona o ciclo 1 aqui
            {
            cicle_aux=0; // sai do loop de ciclos (escolha)
            cicle=1;// sai do loop de ciclo
            lcd.blink();
            lcd.clear();
            deltaH=100;
            deltaM=100;
            
            
            }
        }

          while(cicle_aux==2)
        {
          lcd.setCursor(4,0);
          lcd.print("Ciclo 2");
          lcd.setCursor(1,1);
          lcd.print("12:00 - 23:59");
          delay(40);
          if(digitalRead(up)==1)
            cicle_aux=1; // seta para o ciclo 1
          if(digitalRead(left)==1) // seleciona o ciclo 2 aqui
            {
            cicle_aux=0; // sai do loop de ciclos (escolha)
            cicle=2;// sai do loop de ciclo
            lcd.blink();
            lcd.clear();
            deltaH=100;
            deltaM=100;
            
            
            }
        }
        
      }
      lcd.setCursor(0,0);
      lcd.print("Dia & Hora Irrig");
      delay(300);
      

     
     
     
     
     if(cicle==1)
     {
     
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
        dia_hora_irrig_1();
        
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
        
        dia_hora_irrig_1();
        
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
        
        dia_hora_irrig_1();
        
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
       dia_hora_irrig_1();
        
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
        dia_hora_irrig_1();
        
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
        dia_hora_irrig_1();
        
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
        dia_hora_irrig_1();
        
        EEPROM.write(12,hour); // salva a hora programada na posição doze(Sab)
        EEPROM.write(13,minu); // salva o minuto programado na posição treze (Sab)
        aux_setup=0;
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

         for(cont=28;cont<=41;cont++)
            EEPROM.write(cont,0);
         
        
      

         
        
     }


      if(cicle==2) // programação do ciclo 2
     {
     
     /*****************************************************************************
      ***************************PROGRAMAÇÃO DO DOMINGO****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Dom");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=14; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=15; // posição do min do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        lcd.setCursor(curs0r,1);
        dia_hora_irrig_2();
        
        EEPROM.write(14,hour); // salva a hora programada na posição zero (domingo)
        EEPROM.write(15,minu); // salva o minuto programado na posição um (doming)
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
        
        e2prom_r=16; // posição da hora do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=17; // posição do min do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        
        dia_hora_irrig_2();
        
        EEPROM.write(16,hour); // salva a hora programada na posição dois (Seg)
        EEPROM.write(17,minu); // salva o minuto programado na posição tres (Seg)
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
        
        e2prom_r=18; // posição da hora do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=19; // posição do min do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        
       dia_hora_irrig_2();
        
        EEPROM.write(18,hour); // salva a hora programada na posição quatro (ter)
        EEPROM.write(19,minu); // salva o minuto programado na posição cinco (ter)
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
        
        e2prom_r=20; // posição da hora da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=21; // posição do min da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        dia_hora_irrig_2();
        
        EEPROM.write(20,hour); // salva a hora programada na posição seis (quarta)
        EEPROM.write(21,minu); // salva o minuto programado na posição sete (quarta)
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
        
        e2prom_r=22; // posição da hora da QUINTA na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=23; // posição do min da quinta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
       dia_hora_irrig_2();
        
        EEPROM.write(22,hour); // salva a hora programada na posição oito (Quinta)
        EEPROM.write(23,minu); // salva o minuto programado na posição nove (Quinta)
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
        
        e2prom_r=24; // posição da hora da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=25; // posição do min da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
        dia_hora_irrig_2();
        
        EEPROM.write(24,hour); // salva a hora programada na posição dez(Sexta)
        EEPROM.write(25,minu); // salva o minuto programado na posição onze (sexta)
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
        
        e2prom_r=26; // posição da hora do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=27; // posição do min do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();
       dia_hora_irrig_2();
        
        EEPROM.write(26,hour); // salva a hora programada na posição doze(Sab)
        EEPROM.write(27,minu); // salva o minuto programado na posição treze (Sab)
        aux_setup=0;
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

         for(cont=42;cont<=55;cont++)
            EEPROM.write(cont,0);
         
        
      

         
        
     }

     
      
    }
   
   
   
   
   /******LAÇO QUE TESTA EM QUAL OPÇÃO DO MENU DESEJA ESTAR***********/
   if(digitalRead(up)==1) // se up bottom pressed go back to option 2
    {
      delay(500);
      lcd.clear();
      aux_menu=0;
      
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
   ******DURACAO DA IRRIGACAO********
   **********************************
   *********************************/
  while(aux_menu==3)
  {
   lcd.setCursor(0,0);
   lcd.print("Duracao");
   lcd.setCursor(4,1);
   lcd.print("Da irrigacao");

   if(digitalRead(left)==HIGH) // se apertar botao esquerdo entra na programação do dia e hora da irrigação
    {

     // lcd.blink();
      lcd.clear();
      aux_setup=0; // condição de hold do while da programação
      aux_prog=1;
      curs0r=7;
      programacao=1;
      delta_flag=1; // programação da duração
      
      cicle=0;
      cicle_aux=1; // seta para o ciclo 1
      lcd.clear();
      while(cicle==0)
      {
        delay(300);
        while(cicle_aux==1)
        {
          lcd.setCursor(0,0);
          lcd.print("Irrigar");
          lcd.setCursor(8,1);
          lcd.print("Ciclo 1");
          delay(40);
          if(digitalRead(down)==1)
            cicle_aux=2; // seta para o ciclo 2
          if(digitalRead(left)==1) // seleciona o ciclo 1 aqui
            {
            cicle_aux=0; // sai do loop de ciclos (escolha)
            cicle=1;// sai do loop de ciclo
            lcd.blink();
            lcd.clear();
            deltaH=100;
            deltaM=100;
            
            
            }
        }

          while(cicle_aux==2)
        {
          lcd.setCursor(0,0);
          lcd.print("Irrigar");
          lcd.setCursor(8,1);
          lcd.print("Ciclo 2");
          delay(40);
          if(digitalRead(up)==1)
            cicle_aux=1; // seta para o ciclo 1
          if(digitalRead(left)==1) // seleciona o ciclo 2 aqui
            {
            cicle_aux=0; // sai do loop de ciclos (escolha)
            cicle=2;// sai do loop de ciclo
            lcd.blink();
            lcd.clear();
            deltaH=100;
            deltaM=100;
            
            
            }
        }
        
      }
      
      
      
      lcd.setCursor(0,0);
      lcd.print("Irrigar por:");
      delay(300);
      

     if(cicle==1)
     {
     
     /*****************************************************************************
      ***************************PROGRAMAÇÃO DO DOMINGO****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Dom");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=28; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=29; // posição do min do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        lcd.setCursor(curs0r,1);

        e2prom_r=0; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=1; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_1();
        
        EEPROM.write(28,hour); // salva a hora programada na posição quatorze (domingo)
        EEPROM.write(29,minu); // salva o minuto programado na posição quinze (doming)
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
        lcd.print("Irrigar por:");
       
        lcd.setCursor(0,1);
        lcd.print("Seg");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=30; // posição da hora do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=31; // posição do min do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=2; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=3; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_1();
        
        EEPROM.write(30,hour); // salva a hora programada na posição dezesseis(Seg)
        EEPROM.write(31,minu); // salva o minuto programado na posição dezessete (Seg)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Ter");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=32; // posição da hora do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=33; // posição do min do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=4; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=5; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_1();
        
        EEPROM.write(32,hour); // salva a hora programada na posição dezoito (ter)
        EEPROM.write(33,minu); // salva o minuto programado na posição dezenove (ter)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Qua");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=34; // posição da hora da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=35; // posição do min da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=6; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=7; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
       dia_hora_irrig_1();
        
        EEPROM.write(34,hour); // salva a hora programada na posição vinte (quarta)
        EEPROM.write(35,minu); // salva o minuto programado na posição vinte e um (quarta)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Qui");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=36; // posição da hora da QUINTA na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=37; // posição do min da quinta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=8; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=9; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_1();
        
        EEPROM.write(36,hour); // salva a hora programada na posição vinte e dois (Quinta)
        EEPROM.write(37,minu); // salva o minuto programado na posição vinte e três (Quinta)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Sex");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=38; // posição da hora da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=39; // posição do min da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=10; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=11; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
       dia_hora_irrig_1();
        
        EEPROM.write(38,hour); // salva a hora programada na posição vinte e quatro(Sexta)
        EEPROM.write(39,minu); // salva o minuto programado na posição vinte e cinco (sexta)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Sab");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=40; // posição da hora do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=41; // posição do min do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=12; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=11-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=13; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_1();
        
        EEPROM.write(40,hour); // salva a hora programada na posição vinte e seis (Sab)
        EEPROM.write(41,minu); // salva o minuto programado na posição vinte e sete (Sab)
        aux_setup=0;
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/
         
        
      

         
        
      
     }


     if(cicle==2)
     {
     
     /*****************************************************************************
      ***************************PROGRAMAÇÃO DO DOMINGO****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Dom");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=42; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=43; // posição do min do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        lcd.setCursor(curs0r,1);

        e2prom_r=14; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=15; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_2();
        
        EEPROM.write(42,hour); // salva a hora programada na posição quatorze (domingo)
        EEPROM.write(43,minu); // salva o minuto programado na posição quinze (doming)
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
        lcd.print("Irrigar por:");
       
        lcd.setCursor(0,1);
        lcd.print("Seg");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=44; // posição da hora do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=45; // posição do min do segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=16; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=17; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_2();
        
        EEPROM.write(44,hour); // salva a hora programada na posição dezesseis(Seg)
        EEPROM.write(45,minu); // salva o minuto programado na posição dezessete (Seg)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Ter");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=46; // posição da hora do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=47; // posição do min do terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=18; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=19; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_2();
        
        EEPROM.write(46,hour); // salva a hora programada na posição dezoito (ter)
        EEPROM.write(47,minu); // salva o minuto programado na posição dezenove (ter)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Qua");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=48; // posição da hora da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=49; // posição do min da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=20; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=21; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
       dia_hora_irrig_2();
        
        EEPROM.write(48,hour); // salva a hora programada na posição vinte (quarta)
        EEPROM.write(49,minu); // salva o minuto programado na posição vinte e um (quarta)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Qui");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=50; // posição da hora da QUINTA na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=51; // posição do min da quinta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=22; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=23; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_2();
        
        EEPROM.write(50,hour); // salva a hora programada na posição vinte e dois (Quinta)
        EEPROM.write(51,minu); // salva o minuto programado na posição vinte e três (Quinta)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Sex");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=52; // posição da hora da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=53; // posição do min da Sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=24; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=25; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
       dia_hora_irrig_2();
        
        EEPROM.write(52,hour); // salva a hora programada na posição vinte e quatro(Sexta)
        EEPROM.write(53,minu); // salva o minuto programado na posição vinte e cinco (sexta)
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
        lcd.print("Irrigar por:");
        lcd.setCursor(0,1);
        lcd.print("Sab");
        lcd.setCursor(5,1);
        lcd.print("H:");

        lcd.setCursor(12,1);
        lcd.print("M:");
        
        e2prom_r=54; // posição da hora do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        hour=e2prom_r_val;
        
        lcd.setCursor(7,1);
        lcd.print(e2prom_r_val);

        e2prom_r=55; // posição do min do sábado na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        minu=e2prom_r_val;
        
        lcd.setCursor(14,1);
        lcd.print(e2prom_r_val);

        curs0r=7;
        lcd.setCursor(curs0r,1);
        
        lcd.blink();

        e2prom_r=26; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaH=23-e2prom_r_val; // ve a hora máxima que pode ser imprimida na irrigação

        e2prom_r=27; // posição da hora do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        deltaM=59-e2prom_r_val; // ve o minuto máximo que pode ser imprimido na irrigação
        
        dia_hora_irrig_2();
        
        EEPROM.write(54,hour); // salva a hora programada na posição vinte e seis (Sab)
        EEPROM.write(55,minu); // salva o minuto programado na posição vinte e sete (Sab)
        aux_setup=0;
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/
         
        
      

         
        
      
     }
     
    }

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
   ***********HAB/DESAB***************
   ************IRRIG*****************/
  while(aux_menu==4)
  {
    lcd.setCursor(0,0);
    lcd.print("Habilitar ou");
    lcd.setCursor(0,1);
    lcd.print("Desabilitar dia");


 if(digitalRead(left)==HIGH) // se apertar botao esquerdo entra na programação do dia e hora da irrigação
     {
      
      lcd.blink();
      lcd.clear();
      aux_setup=0; // condição de hold do while da programação
      aux_prog=1;
      curs0r=8;
      programacao=2;
      lcd.setCursor(0,0);
      lcd.print("Hab. este dia?");
      delay(300);

       /*****************************************************************************
      ***************************PROGRAMAÇÃO DO DOMINGO****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Dom");
        lcd.setCursor(8,1);
        
        
        
        e2prom_r=56; // posição da hab/desab do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        EEPROM.write(56,able); // salva a hora programada na posição vinte e oito (domingo)
        
        aux_prog=2;
        aux_setup=0;
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/


      /*****************************************************************************
      ***************************PROGRAMAÇÃO DA SEGUNDA****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Seg");
        
        curs0r=8;
        lcd.setCursor(0,0);
        lcd.print("Hab. este dia?");
        lcd.setCursor(8,1);
        delay(300);
        
        
        
        e2prom_r=57; // posição da hab/desab da segunda na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        EEPROM.write(57,able); // salva a hora programada na posição vinte e nove (segunda)
        
        aux_prog=3;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      /*****************************************************************************
      ***************************PROGRAMAÇÃO DA TERÇA****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Ter");
        curs0r=8;
        lcd.setCursor(0,0);
        lcd.print("Hab. este dia?");
        lcd.setCursor(8,1);
        delay(300);
        
        
        
        
        e2prom_r=58; // posição da hab/desab da terça na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        EEPROM.write(58,able); // salva a hora programada na posição trinta (Terça)
        
        aux_prog=4;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      /*****************************************************************************
      ***************************PROGRAMAÇÃO DA QUARTA****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Qua");
        curs0r=8;
        lcd.setCursor(0,0);
        lcd.print("Hab. este dia?");
        lcd.setCursor(8,1);
        delay(300);
        
        
        
        
        e2prom_r=59; // posição da hab/desab da quarta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        EEPROM.write(59,able); // salva a hora programada na posição trinta e um (Quarta)
        
        aux_prog=5;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      /*****************************************************************************
      ***************************PROGRAMAÇÃO DA QUINTA****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Qui");
        curs0r=8;
        lcd.setCursor(0,0);
        lcd.print("Hab. este dia?");
        lcd.setCursor(8,1);
        delay(300);
        
        
        
        
        e2prom_r=60; // posição da hab/desab da quinta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        EEPROM.write(60,able); // salva a hora programada na posição trinta e dois (Quinta)
        
        aux_prog=6;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

       /*****************************************************************************
      ***************************PROGRAMAÇÃO DA Sexta****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Sex");
        curs0r=8;
        lcd.setCursor(0,0);
        lcd.print("Hab. este dia?");
        lcd.setCursor(8,1);
        delay(300);
        
        
        
        
        e2prom_r=61; // posição da hab/desab da sexta na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        EEPROM.write(61,able); // salva a hora programada na posição trinta e três (Sexta)
        
        aux_prog=7;
        aux_setup=0;
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/

      /*****************************************************************************
      ***************************PROGRAMAÇÃO DO SABADO****************************
      ****************************************************************************/
        lcd.setCursor(0,1);
        lcd.print("Sab");
         curs0r=8;
        lcd.setCursor(0,0);
        lcd.print("Hab. este dia?");
        lcd.setCursor(8,1);
        delay(300);
        
        
        
        e2prom_r=62; // posição da hab/desab do domingo na memoria
        e2prom_read(); // le a memoria com a posição dada da variavel e2prom_r
        able=e2prom_r_val;

        if(able==0)
         lcd.print("Nao");
        else if(able==1)
         lcd.print("Sim");
        
        lcd.setCursor(curs0r,1);
        lcd.blink();
        hab_irrig();
        
        
        EEPROM.write(62,able); // salva a hora programada na posição trinta e quatro (Sábado)
        
        
        
        
        /************************************************************************
         ************************************************************************ 
         ************************************************************************
         ***********************************************************************/
      
      
     }
    

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
      
      /**************
      clockreader();
      
      delay(1000);
      clockshower(); 
      ***************/  
    }
    
    
  } // WHILE AUX_MENU == 5 <OPÇAO 6>

  
  }// while do menu (EXIT)
    
     
  }
  

}
