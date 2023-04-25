#include<LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 6, 5, 4, 3); // Cria um objeto lcd e atribui os pinos

double pulso = 0;
long int cont_init, cont_end;
float  freq = 0.000;
long int tempo_total = 0;
float vazao = 0.000;
float ml=0.00, ml_tot=0.00;
int cont_micro = 0,aux_cont_micro = 0;;
int aux = 0;


void contapulso ()
{
 pulso++;
}



void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP);
  pinMode(8,INPUT);
  Serial.begin(9600); // inicia serial
  Serial.println("Pressione o botao para comecar");
  lcd.begin(16, 2); // Define o display com 16 colunas e 2 linhas
  
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("      Tupa"); //Escreve no LCD
  lcd.setCursor(0, 1);
  lcd.print("Automation"); 
  cont_init = millis();
  cont_micro = millis();
//  sei();
}

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
  if(digitalRead(8) == HIGH)
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


 
if(cont_end >= (cont_init+1000))
  {
 detachInterrupt(contapulso);
  Serial.println("diferença");
 Serial.println(cont_end - cont_init);
 tempo_total = tempo_total + cont_end - cont_init;
 Serial.println("tempo total");
 Serial.println(tempo_total);
  Serial.println("pulsos");
 Serial.println(pulso);
 freq = (pulso/(cont_end - cont_init))*1000;
 cont_init = cont_end; // update new init time for millis
 pulso = 0;
 Serial.println("Freq");
 Serial.println(freq);


  vazao = freq/7.5;
  ml = (vazao / 60) * 1000;
  ml_tot += ml;
  Serial.println("ml trasfegados:");
  Serial.println(ml_tot,6);
  vazao = 0.00;
  Serial.println("----------------------------------");

  lcd.clear();
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna e linha do LCD
  lcd.print("Volume:"); //Escreve no LCD
  lcd.setCursor(6, 1);
  lcd.print("litros");
  lcd.setCursor(0, 1);
  lcd.print(ml_tot/1000);
  
 attachInterrupt(digitalPinToInterrupt(2),contapulso,RISING);
  }

  
  
  
 if(digitalRead(8) == LOW)
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


