#include <LiquidCrystal.h> 
#include <EEPROM.h>


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


int prog_menu = 1; // variable used to walk between programs sections
                  // for instance: motor on/off.. alarm temp...


int forward_menu = 0; // variable used to select wheter the user wants to go forward to off motor
                      // time or back to main menu
                       // forward_menu = 0 -> go back to main menu and does not save the ON time in e2prom
                       // forward_menu = 1 -> go forward to OFF Motor time



int reader_menu = 0; //  variable used to see if the left botton is pressed inside the menu
                    
 /**************/

int sec = 0; // variable used to count seconds

/************
 * VARIABLES
 *   USED   *
 *FOR E2PROM*
 ************/

int e2prom_reader = 0; // variable used to hold e2prom read values


 /***********/


 /**************
 * VARIABLES   *
 *  USED FOR   *
 * TEMPERATURE *
 **************/

int temperature = 0; // variable used to read and display temperature
int comparative_temperature =0;
  /***********/



 /*************
 * VARIABLES
 *  USED FOR  *
 * MOTOR TIME *
 ************/

int motor_cont_ON = 1; // variables used count and display the amount of seconds chosen
int motor_cont_OFF = 1; // (ON & OFF)

 /***********/

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
       lcd.noBlink();


      /*****************
       ** PROGRAMMING ** 
       ** MOTOR TIME  **
       *    ON/OFF     *
       *****************/
      
      
      if(digitalRead(left)) // if the left botton is pressed then enters in the programming time motor section
        
        {
          lcd.clear();
          forward_menu = 1; // set condition for the menu to go forward (if the user doesn't want to go forward there is an if that sets this variable to '0')
          lcd.setCursor(0,0);
          lcd.print("Tempo do motor");
          lcd.setCursor(0,1);
          lcd.print("Ligado:      Seg");
          lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
          
          e2prom_reader =  EEPROM.read(0); // reads the position '0' of the e2prom (ON TIME MOTOR)
          lcd.print(e2prom_reader); // print in the 8th colum and '1' line the value read
          motor_cont_ON = e2prom_reader;
          prog_menu = 1; // sets the prog menu for the first option (ON Motor time)
          delay(500); // delay used to stop the menu to go back to main menu (cause the left botton is also used to go back as well to enter here)
          lcd.blink(); // makes the cursor to blink
          while(prog_menu == 1) // holds the ON motor section
            {
            lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear

              if(digitalRead(up)==1) // if the up botton is pressed then increment the amount of seconds for the motor to be ON
                {
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Tempo do motor");
                  lcd.setCursor(0,1);
                  lcd.print("Ligado:      Seg");
                  lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                  motor_cont_ON++; // increment the variable used to count and display the time chosen for the motor to be ON
                  lcd.print(motor_cont_ON); // displays the current ammount of motor_cont
                  lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                  delay(300); // a little delay so the count doesn't increment more than one at a time
                  lcd.blink(); // turns on the blink again
                }

                if(digitalRead(down)==1) // if the down botton is pressed then decrement the amount of seconds for the motor to be ON
                {
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Tempo do motor");
                  lcd.setCursor(0,1);
                  lcd.print("Ligado:      Seg");
                   lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                  motor_cont_ON--;// increment the variable used to count and display the time chosen for the motor to be ON

                  if(motor_cont_ON == 0)
                  {
                  motor_cont_ON = 1; // the lowest value that this variable can be (there isn't negative time)  
                  }
                  lcd.print(motor_cont_ON); // displays the current ammount of motor_cont
                  lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                  delay(300); // a little delay so the count doesn't increment more than one at a time
                  lcd.blink(); // turns on the blink again
                }

                if(digitalRead(left)==1) // if the left botton is pressed then go backward in the menu section and doesn't writes the ON Motor Time in the e2prom
                  {
                    delay(500); // delay used to stop the main menu to go back here (cause the condition is the same -> the left botton is used to do so)
                    prog_menu=0; // condition for noone program section (goes back to main menu)
                    lcd.clear();
                    forward_menu = 0; // doesn't go to the OFF Motor time (It's condition would be '1')
                    
                  }

                
               if( (digitalRead(right) == 1) && (forward_menu == 1)) // if the right botton is pressed and the forward_menu is set to 1 then enters in the OFF Motor Time Section
              {
                lcd.noBlink();
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Tempo do motor");
                lcd.setCursor(0,1);
                lcd.print("Deslig:      Seg");
                lcd.setCursor(8,1); // sets the cursor to the 7th position where the numbers will appear

                

                e2prom_reader =  EEPROM.read(1); // reads the position '1' of the e2prom (OFF TIME MOTOR)
                lcd.print(e2prom_reader); // print in the 8th colum and '1' line the value read
                motor_cont_OFF = e2prom_reader;
                prog_menu = 2; // sets the condition for the hold of the  OFF Motor Time Section
                delay(500); // delay used to stop the menu to go back to main menu (cause the left botton is also used to go back as well to enter here)
                lcd.blink(); // makes the cursor to blink
                
                while(prog_menu == 2)

                  {

                    lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear

                   if(digitalRead(up)==1) // if the up botton is pressed then increment the amount of seconds for the motor to be OFF
                      {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Tempo do motor");
                        lcd.setCursor(0,1);
                        lcd.print("Deslig:      Seg");
                        lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                        motor_cont_OFF++; // increment the variable used to count and display the time chosen for the motor to be OFF
                        lcd.print(motor_cont_OFF); // displays the current ammount of motor_cont
                        lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                        delay(300); // a little delay so the count doesn't increment more than one at a time
                        lcd.blink(); // turns on the blink again
                      }

                    if(digitalRead(down)==1) // if the down botton is pressed then decrement the amount of seconds for the motor to be OFF
                      {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("Tempo do motor");
                        lcd.setCursor(0,1);
                        lcd.print("Deslig:      Seg");
                        lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                        motor_cont_OFF--;// increment the variable used to count and display the time chosen for the motor to be ON

                       if(motor_cont_OFF == 0)
                          {
                             motor_cont_OFF = 1; // the lowest value that this variable can be (there isn't negative time)  
                          }
                       lcd.print(motor_cont_OFF); // displays the current ammount of motor_cont
                       lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                       delay(300); // a little delay so the count doesn't increment more than one at a time
                       lcd.blink(); // turns on the blink again
                     }

                     if(digitalRead(left)==1) // if the left botton is pressed then go back to the ON Motor Time Menu
                        {
                         // delay(500);
                         // lcd.clear();
                          prog_menu = 1; // sets the condition to go back to ON Motor TIME

                          lcd.clear();
                          forward_menu = 1; // set condition for the menu to go forward (if the user doesn't want to go forward there is an if that sets this variable to '0')
                          lcd.setCursor(0,0);
                          lcd.print("Tempo do motor");
                          lcd.setCursor(0,1);
                          lcd.print("Ligado:      Seg");
                          lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
          
                          e2prom_reader =  EEPROM.read(0); // reads the position '0' of the e2prom (ON TIME MOTOR)
                          lcd.print(e2prom_reader); // print in the 8th colum and '1' line the value read
                          motor_cont_ON = e2prom_reader;
                          prog_menu = 1; // sets the prog menu for the first option (ON Motor time)
                          lcd.noBlink();
                          delay(500); // delay used to stop the menu to go back to main menu (cause the left botton is also used to go back as well to enter here)
                          lcd.blink(); // makes the cursor to blink
                           
                      
                        }

                     if(digitalRead(right)==1)
                        {
                        EEPROM.write(0,motor_cont_ON); // saves in the e2prom the amount chosen for the motor to be ON on its adress (0)
                        EEPROM.write(1,motor_cont_OFF); // saves in the e2prom the amount chosen for the motor to be OFF on its adress (1)  
                        prog_menu = 0; // sets the condition to leave the ON/OFF Menu and goes back to the main menu
                        lcd.clear(); // clear lcd
                        }
                    
                  
                  
                  
                  }
                
              
              
              
              }  
            
            
            }


                      
          
        }


        /****************
        **************** 
        ***************/
       
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
       lcd.noBlink();
      
      
       /*****************
       ** PROGRAMMING ** 
       **    ALARM    **
       *  TEMPERATURE   *
       *****************/

        if(digitalRead(left)) // if the left botton is pressed then enters in the programming alarm temperature

          {
            
            lcd.clear();
            
            lcd.setCursor(0,0);
            lcd.print("Temperatura de");
            lcd.setCursor(0,1);
            lcd.print("Alarme:     oC");
            lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
          
            e2prom_reader =  EEPROM.read(2); // reads the position '2' of the e2prom (temperature)
            lcd.print(e2prom_reader); // print in the 8th colum and '1' line the value read
            temperature = e2prom_reader;
            prog_menu = 1; // sets the prog menu for the hold of the menu entering section
            delay(500); // delay used to stop the menu to go back to main menu (cause the left botton is also used to go back as well to enter here)
            lcd.blink(); // makes the cursor to blink
            
            while(prog_menu == 1)

               {

                 lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear

                 if(digitalRead(up)==1) // if the up botton is pressed then increment the amount of the temperature desired by the user
                      {
                        lcd.setCursor(0,0);
                        lcd.print("Temperatura de");
                        lcd.setCursor(0,1);
                        lcd.print("Alarme:     oC");
                        lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                        temperature++; // increment the variable used to count and display the temperature chosen for the alarm
                        comparative_temperature = EEPROM.read(3); // reads the switch off temperature to compare with the alarm temperature
                        if(comparative_temperature <= temperature)
                          {
                            temperature--;
                            lcd.clear();
                            lcd.noBlink();
                            lcd.setCursor(0,0);
                            lcd.print("Temp. de alarme");
                            lcd.setCursor(0,1);
                            lcd.print("deve ser menor");
                            delay(2500);
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("que a temp. de");
                            lcd.setCursor(0,1);
                            lcd.print("desligamento!");
                            delay(2500);
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Temperatura de");
                            lcd.setCursor(0,1);
                            lcd.print("Alarme:     oC");
                            lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                            
                          }

                        if(temperature == 141)
                          {
                            temperature = 140;
                          }
                        
                        lcd.print(temperature); // displays the current ammount of temperature
                        lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                        delay(300); // a little delay so the count doesn't increment more than one at a time
                        lcd.blink(); // turns on the blink again
                      }
                 
                 
                 if(digitalRead(down)==1) // if the down botton is pressed then decrement the amount of temperature chosen by the user
                      {
                        lcd.setCursor(0,0);
                        lcd.print("Temperatura de");
                        lcd.setCursor(0,1);
                        lcd.print("Alarme:     oC");
                        lcd.setCursor(8,1); // sets the cursor to the 8th position where the numbers will appear
                        temperature--;// increment the variable used to count and display the temperature chosen for the alarm

                        if(temperature == -1)
                           {
                                temperature = 0; // the lowest value that this variable can be (we're talkin' about a heating system, so even this low value is unlikely)  
                           }
                        lcd.print(temperature); // displays the current ammount of temperature
                        lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                        delay(300); // a little delay so the count doesn't increment more than one at a time
                        lcd.blink(); // turns on the blink again
                      }    
                 
                
               
                   if(digitalRead(left)==1)
                    {
                      delay(500);
                      prog_menu = 0;
                      lcd.clear();
                    }

                    if(digitalRead(right)==1)
                      {
                        delay(300);
                        EEPROM.write(2,temperature); // saves in the e2prom the amount chosen for the alarm temperature on its adress (2)
                        prog_menu = 0; // sets the condition to leave the ALARM TEMPERATURE Menu and goes back to the main menu
                        lcd.clear();
                      }
               }
            
          }
       


       /****************
        **************** 
        ***************/
       
      
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
       lcd.noBlink();
      
      
      /*******************
       ** PROGRAMMING   ** 
       **  SWITCH OFF   **
       *  TEMPERATURE    *
       *****************/

        if(digitalRead(left)) // if the left botton is pressed then enters in the programming switch off temperature

          {
            
            lcd.clear();
            
            lcd.setCursor(0,0);
            lcd.print("Temperatura de");
            lcd.setCursor(0,1);
            lcd.print("Deslig.:      oC");
            lcd.setCursor(9,1); // sets the cursor to the 9th position where the numbers will appear
          
            e2prom_reader =  EEPROM.read(3); // reads the position '3' of the e2prom (temperature)
            lcd.print(e2prom_reader); // print in the 9th colum and '1' line the value read
            temperature = e2prom_reader;
            prog_menu = 1; // sets the prog menu for the hold of the menu entering section
            delay(500); // delay used to stop the menu to go back to main menu (cause the left botton is also used to go back as well to enter here)
            lcd.blink(); // makes the cursor to blink
            
            while(prog_menu == 1)

               {

                 lcd.setCursor(9,1); // sets the cursor to the 9th position where the numbers will appear

                 if(digitalRead(up)==1) // if the up botton is pressed then increment the amount of the temperature desired by the user
                      {
                        lcd.setCursor(0,0);
                        lcd.print("Temperatura de");
                        lcd.setCursor(0,1);
                        lcd.print("Deslig.:      oC");
                        lcd.setCursor(9,1); // sets the cursor to the 9th position where the numbers will appear
                        temperature++; // increment the variable used to count and display the temperature chosen for the switch of

                        if(temperature == 141)
                          {
                            temperature = 140;
                          }
                        
                        lcd.print(temperature); // displays the current ammount of temperature
                        lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                        delay(300); // a little delay so the count doesn't increment more than one at a time
                        lcd.blink(); // turns on the blink again
                      }
                 
                 
                 if(digitalRead(down)==1) // if the down botton is pressed then decrement the amount of temperature chosen by the user
                      {
                        lcd.setCursor(0,0);
                        lcd.print("Temperatura de");
                        lcd.setCursor(0,1);
                        lcd.print("Deslig.:      oC");
                        lcd.setCursor(9,1); // sets the cursor to the 9th position where the numbers will appear
                        temperature--;// increment the variable used to count and display the temperature chosen for the switch off
                        comparative_temperature = EEPROM.read(2); // reads the alarm temperature to compare with the switch off temperature
                        if(comparative_temperature >= temperature)
                          {
                            temperature++;
                            lcd.clear();
                            lcd.noBlink();
                            lcd.setCursor(0,0);
                            lcd.print("Temp. de deslig.");
                            lcd.setCursor(0,1);
                            lcd.print("deve ser maior");
                            delay(2500);
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("que a temp. de");
                            lcd.setCursor(0,1);
                            lcd.print("alarme!");
                            delay(2500);
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("Temperatura de");
                            lcd.setCursor(0,1);
                            lcd.print("Deslig.:      oC");
                            lcd.setCursor(9,1); // sets the cursor to the 9th position where the numbers will appear
                            
                          }
                        if(temperature == -1)
                           {
                                temperature = 0; // the lowest value that this variable can be (we're talkin' about a heating system, so even this low value is unlikely)  
                           }
                        lcd.print(temperature); // displays the current ammount of temperature
                        lcd.noBlink(); // turns off the blink so it wont go to the next room of the display
                        delay(300); // a little delay so the count doesn't increment more than one at a time
                        lcd.blink(); // turns on the blink again
                      }    
                 
                
               
                   if(digitalRead(left)==1)
                    {
                      delay(500);
                      prog_menu = 0;
                      lcd.clear();
                    }

                    if(digitalRead(right)==1)
                      {
                        delay(300);
                        EEPROM.write(3,temperature); // saves in the e2prom the amount chosen for the alarm temperature on its adress (3)
                        prog_menu = 0; // sets the condition to leave the SWITCH OFF TEMPERATURE Menu and goes back to the main menu
                        lcd.clear();
                      }
               }
            
          }
       


       /****************
        **************** 
        ***************/
       
      
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
       lcd.noBlink();
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
  lcd.setCursor(0,1); // colum and line
  lcd.print(sec);
  lcd.setCursor(9,1); // colum and line
  lcd.print("Seconds");
  delay(200);
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

