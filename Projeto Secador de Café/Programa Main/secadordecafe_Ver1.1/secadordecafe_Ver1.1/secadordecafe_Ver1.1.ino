#include <LiquidCrystal.h> 



LiquidCrystal lcd(13,12,11,10,9,8); // LiquidCrystal lcd(RS, E, D4, D5, D6, D7); 



#define up 5 // pin 5 tactil bottom 'up'
#define down 4 // pin 4 tactil bottom 'down'
#define left 3 // pin 3 tactil bottom 'left'
#define right 2 // pin 2 tactil bottom 'right'

/****************
 *Menu Variables* 
 ***************/

int hold_menu = 0; // variable used to hold the menu on screen until the user 
                    // wishes to leave it. if it's 0 stay inside menu. if it's 1 leaves the menu.
                    
int select_menu = 1; // variable use to select the option desired inside the menu.
                    // it stars with the value 1 so when the up botton is pressed
                    // the menu starts at the first option (Motor Time On/Off)
                    
 /**************/

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

// Timer1 Configuration              
  TCCR1A = 0;                        // Sets timer for normal operation - OC1A & OC1B are unattacheds
  TCCR1B = 0;                        // Clear Register
  TCCR1B |= (1<<CS10)|(1 << CS12);   // configure prescaler to 1024: CS12 = 1 e CS10 = 1
 
  TCNT1 = 0xC2F7;                    // sets timer for one interrupt per second
                                     // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
  
  TIMSK1 |= (1 << TOIE1);           // ables timer1's interruput

/***********************************************
 *********************************************** 
 **********************************************/
 
}










/************************************************
 ************************************************
 *******          MENU ROUTINE          *********
 ************************************************/

void menu  (void) //  rotine for the menu
                 // Interface between man-machine

{
  if(digitalRead(up) == HIGH) // if the user presses the up botton, then the menu stars to run
  {
    lcd.clear(); //  clears the display
    
    while(hold_menu == 0) // while the variable 'hold_menu' is on the value '0' the program stays inside the menu
    
    {


      /**************************************
       ************************************** 
       ****** FIRST OPTION IN THE MENU ******
       ********* TIME ON/OFF MOTOR **********
       **************************************/
      while(select_menu == 1)
      {
       lcd.setCursor(0,0);
       lcd.print("Menu: Programar");
       lcd.setCursor(0,1);
       lcd.print("tempo do motor.");

      /*
       * ROUTINE GOES HERE
       */
       
      if(digitalRead(down)==1) // if the down botton is pressed then goes to the second option in the menu
      {
        delay(500);
        lcd.clear();
        select_menu=2; // selects the second option in the menu
        
      } // if down bottem pressed

        
      }

      /**************************************
       ************************************** 
       *************************************/



       /**************************************
       ************************************** 
       ****** SECOND OPTION IN THE MENU ******
       ********* ALARM TEMPERATURE **********
       **************************************/
      while(select_menu == 2)
      {
       lcd.setCursor(0,0);
       lcd.print("Prog temperatura");
       lcd.setCursor(0,1);
       lcd.print("de alarme.");

      /*
       * ROUTINE GOES HERE
       */
       
      
      if(digitalRead(up)==1) // if the up botton is pressed then goes to the first option in the menu
      {
        delay(500);
        lcd.clear();
        select_menu=1; // selects the first option in the menu
        
      } // if down bottem pressed
      
      
      if(digitalRead(down)==1) // if the down botton is pressed then goes to the third option in the menu
      {
        delay(500);
        lcd.clear();
        select_menu=3; // selects the third option in the menu
        
      } // if down bottem pressed

        
      }

      /**************************************
       ************************************** 
       *************************************/


       /**************************************
       ************************************** 
       ****** THIRD OPTION IN THE MENU ******
       ********* SWITCH OFF TEMPERATURE *****
       **************************************/
      while(select_menu == 3)
      {
       lcd.setCursor(0,0);
       lcd.print("Prog temperatura");
       lcd.setCursor(0,1);
       lcd.print("de desligamento");

      /*
       * ROUTINE GOES HERE
       */
       
      
      if(digitalRead(up)==1) // if the up botton is pressed then goes to the second option in the menu
      {
        delay(500);
        lcd.clear();
        select_menu=2; // selects the second option in the menu
        
      } // if down bottem pressed
      
      
      if(digitalRead(down)==1) // if the down botton is pressed then goes to the fourth option in the menu
      {
        delay(500);
        lcd.clear();
        select_menu=4; // selects the fourth option in the menu
        
      } // if down bottem pressed

        
      }

      /**************************************
       ************************************** 
       *************************************/

        /**************************************
       ************************************** 
       ****** FOURTH OPTION IN THE MENU ******
       ************* LEAVE MENU **************
       **************************************/
      while(select_menu == 4)
      {
       lcd.setCursor(0,0);
       lcd.print("Sair do menu");
       lcd.setCursor(0,1);
       lcd.print("de opcoes.");

      /*
       * ROUTINE GOES HERE
       */
       
      
      if(digitalRead(up)==1) // if the up botton is pressed then goes to the third option in the menu
      {
        delay(500);
        lcd.clear();
        select_menu=3; // selects the third option in the menu
        
      } // if down bottem pressed
      
      
      if(digitalRead(left)==1) // if the left botton is pressed then leaves the menu
      {
        delay(500);
        lcd.clear();
        select_menu = 6; // sets a inexistent condition for the menu option(sixth option), guarantees not to enter in any other menu option
        hold_menu = 1 ; // sets the condition to quit the menu (first while of the section)
      } // if down bottem pressed

        
      }

      /**************************************
       ************************************** 
       *************************************/
      
      
      
    }
    
  }
  
}

/**********************************************
 ********************************************** 
 **********************************************/









 

void loop() {
  lcd.setCursor(0,0); // colum and line
  lcd.print("Time");
  lcd.setCursor(8,0); // colum and line
  lcd.print(sec);
  delay(100);
  lcd.clear();

  select_menu = 1; // sets the inital condition for the menu option (fisrt option)
  hold_menu = 0; // sets the condition for the menu stay active until the user wishes to leave it
  menu();
  

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

