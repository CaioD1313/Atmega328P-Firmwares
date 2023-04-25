#include <LiquidCrystal.h> 



LiquidCrystal lcd(13,12,11,10,9,8); // LiquidCrystal lcd(RS, E, D4, D5, D6, D7); 



#define up 5 // pin 5 tactil bottom 'up'
#define down 4 // pin 4 tactil bottom 'down'
#define left 3 // pin 3 tactil bottom 'left'
#define right 2 // pin 2 tactil bottom 'right'

int sec = 0; // variable used to count seconds

void setup() {
  lcd.begin(16,2); // initialize lcd
  

  /***************************************/
  /* All bottons are pull-up pre defined */
  /*Define all tactil bottons as inputs */
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(left,INPUT);
  pinMode(right,INPUT);
 /****************************************/


/*********************************************
**********************************************
*****************TIMER 1 SETUP****************
**********************************************
**Timer is used to set clock for the engine***
********************************************/

// Configuração do timer1 
  TCCR1A = 0;                        //confira timer para operação normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;                        //limpa registrador
  TCCR1B |= (1<<CS10)|(1 << CS12);   // configura prescaler para 1024: CS12 = 1 e CS10 = 1
 
  TCNT1 = 0xC2F7;                    // incia timer com valor para que estouro ocorra em 1 segundo
                                     // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
  
  TIMSK1 |= (1 << TOIE1);           // habilita a interrupção do TIMER1

/***********************************************
 *********************************************** 
 **********************************************/
 
}

void loop() {
  lcd.setCursor(0,0); // colum and line
  lcd.print("Time");
  lcd.setCursor(8,0); // colum and line
  lcd.print(sec);
  delay(100);
  lcd.clear();
/*
  if(digitalRead(up) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("CIMA");
    delay(1000);
    
  }

  else if(digitalRead(down) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("BAIXO");
    delay(1000);
    
  }

  else if(digitalRead(left) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("ESQUERDA");
    delay(1000);
    
  }

  else if(digitalRead(right) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("DIREITA");
    delay(1000);
    
  }
  
  
  delay(100); // delay used in ms
  lcd.clear(); // clear lcd so the text can be the current one

*/
}


ISR(TIMER1_OVF_vect)                              //interrupção do TIMER1 
{
  TCNT1 = 0xC2F7;                                 // Renicia TIMER
  sec ++ ; 
  if(sec == 60)
  {
    sec = 0 ; // reinitialize sec
  }
}

