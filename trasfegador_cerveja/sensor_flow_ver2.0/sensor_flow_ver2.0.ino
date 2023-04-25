/********************************************************
 * Firmaware desenvolvido por Caio Dutra em 17/07/2019  *
 * **************************************************** *
 * Este firmware colhe pulsos gerados pelo sensor hall  *
 * Yf-S201 para medir a vazão de trasfego na linha de   *
 * produção de cerveja. O firmware também porta de uma  *
 * interface para comunição direta com ser humano.      *
 * **************************************************** *
 ********************************************************/

 /*******************************************************
 * **************************************************** *
 * **************************************************** *
 * Esta versão corrige a imprecisão da medida fazendo   *
 * intervalos de tempo de medida menores (aqui 50ms)    *
 ********************************************************/

/*
 * Bibliotecas
 */
#include<LiquidCrystal.h>

/*
 * #define
 */
#define botao1 8


LiquidCrystal lcd(12, 11, 6, 5, 4, 3); // Cria um objeto lcd e atribui os pinos

/*****************************
********* Variáveis **********
*****************************/


double pulso = 0; // varíavel que conta o número de pulsos do sensor hall num intervalo de tempo
long int cont_init, cont_end;
float  freq = 0.000;
float vazao = 0.000;
float ml=0.00, ml_tot=0.00;
int cont_micro = 0,aux_cont_micro = 0;;
int aux = 0;

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

/*****************************
*********    SETUP  **********
*******   do sistema  ********
*****************************/

void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP); // interrupção externa no pino 2
  pinMode(botao1,INPUT); // botao de comando 1
  
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
  if(aux_cont_micro == 0)
  {
  lcd.clear();
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("      Tupa"); //Escreve no LCD
  lcd.setCursor(0, 1);
  lcd.print("   Automation");
  delay(300);
  }

  else if(aux_cont_micro == 1)
  {
  lcd.clear();
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("   Trasfegador"); //Escreve no LCD
  delay(300);
  
  }

  if((millis() - cont_micro) >= 2000 )
  {
    aux_cont_micro = !(aux_cont_micro);
    cont_micro = millis();
  }
  if(digitalRead(botao1) == HIGH)
   { aux = 1;
    cont_init = millis();
    Serial.println("----------------------------------");
    lcd.clear();
    attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING);
   }
  


while(aux == 1)
{

  // put your main code here, to run repeatedly:
cont_end = millis();


 
if(cont_end >= (cont_init+50))
  {
 detachInterrupt(contapulso);
 
 
 freq = (pulso/(cont_end - cont_init))*1000;
 cont_init = cont_end; // update new init time for millis
 pulso = 0;
 
 vazao = freq/7.5;
 ml = (vazao / (2*60*10)) * 1000;
 ml_tot += ml;
 vazao = 0.00;
 

  lcd.clear();
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("Volume:"); //Escreve no LCD
  lcd.setCursor(6, 1);
  lcd.print("litros");
  lcd.setCursor(0, 1);
  lcd.print(ml_tot/1000);
  
 attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING);
  }

  
  
  
 if(digitalRead(botao1) == LOW)
 {
  aux = 0;
  detachInterrupt(contapulso);
 }
}
/*vazao = ((80/11)*freq) - ((80/11)*16) + 120;
 
  vazao = (vazao / 3600)* tempo_total/1000;

 
 
  Serial.println("litros trasfegados:");
  Serial.println(vazao,6);
  vazao = 0.00;

  */
}

/****************************
****Fim do loop infinito*****
****************************/


