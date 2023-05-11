
// #include <LiquidCrystal.h>

// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// #define up 8

// #define reset 9
#include <EEPROM.h>

#define MISO 10  // Master INPUT Slave OUTPUT
#define MOSI 8 // Master OUTPUT Slave INPUT
#define SS 6   // Slave Select (Enable)
#define SCK 2  // Clock

int A = 1;

// byte test = 170;
int aux = 0; //  aux = 0 means SS was turned to LOW, aux = 1 CC 

byte Slave_received = 0x00;

byte Slave_send = 0x00;

byte h = 0x00; // h is the mask used to set the hth bit on the Slave_received var
byte m = 0x00; // m is the mask used to set the mth bit on the Slave_send var


int sum_aux = 0; // sum_aux is a flag to make the sum (to be sent back) one time

int SPI_Delay_Time = 500;

// unsigned char SPI_Com(unsigned char send);

void setup() {
  // put your setup code here, to run once:

  // lcd.begin(16,2); // inicializa lcd
  // lcd.print("Value = ");

  // pinMode(up,INPUT);
  // pinMode(reset,INPUT);

  // lcd.setCursor(9, 0);
  // lcd.print(aux);

  // Defining SPI pin's

  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);


  digitalWrite(MISO, LOW); // Set Master Input Slave Output to LOW.
  // digitalWrite(SS, HIGH); // Set Slave Select to HIGH, so it is inactive
  // digitalWrite(SCK, LOW); // Gurantees SCK is set to LOW so when the COM starts the Master is on Sending mode
  // digitalWrite(MOSI,LOW); // Gurantees SCK is set to LOW so when the COM starts the Master is on Sending mode
  delay(200); // delay so the master have time to turn SS to HIGH
 

}

void loop() {
  // put your main code here, to run repeatedly:

  if((digitalRead(SS) == LOW) && (aux == 0))
    {
      aux = 1;
      h = 0x80; // sets h to 1000 000 (MSB to be shifted right)
      m = 0x80; // sets m to 1000 000 (MSB to be shifted right)
      Slave_received = 0x00; // resets the Slave_Received byte
      // sum_aux = 1; // sets the sum_aux to 1 so the send var gets updated
 
      attachInterrupt(digitalPinToInterrupt(SCK),SCK_Check,CHANGE);
      // attachInterrupt(digitalPinToInterrupt(SCK),SCK_Check_HighToLow,FALLING);
      // attachInterrupt(digitalPinToInterrupt(SCK),SCK_Check_LowToHigh,RISING);
    
    }

  while(aux == 1)
    {
      
      if(digitalRead(SS) == HIGH)
      {
        aux = 0;
        detachInterrupt(SCK_Check);
        Slave_send = Slave_received;
        // detachInterrupt(SCK_Check_HighToLow);
        // detachInterrupt(SCK_Check_LowToHigh);
      }

    } 

    // if(sum_aux == 1)
    // {
    //   // adds 10 (dec) to the Slave received to be sent back to the master (fisrt time will send 0x00) 
    //   sum_aux = 0; // guarantees this Send var gets updated only once at new info burst 
    //   // EEPROM.write(0,Slave_send);
    //   // EEPROM.update(1,Slave_received);

    // }  


}




void SCK_Check (void)
  {

    //tests if the interrupt is being entered (OK!!!)
    
    // if(A == 1)
    // {
    //   digitalWrite(12,HIGH);
    //   A = 0;
    // }

    // else if(A == 0)
    // {
    //    digitalWrite(12,LOW);
    //   A = 1;
    // }


    if(digitalRead(SCK) == LOW) // if SCK is LOW, the master is sending data
   {

    if(digitalRead(MOSI) == HIGH)
    {Slave_received = Slave_received | h;} // if MOSI has a bit 1 on it, stores on the hth bit on Slave_received
    h/=2;  // shifst h by one bit to the right
   
   }
  
    else if (digitalRead(SCK) == HIGH) // if SCK is HIGH, the Slave is sending data
    {

      if((Slave_send & h) != 0x00) // if there is a bit on the mth bit to be sent, sends it
      {digitalWrite(MISO, HIGH);}
      else
      {digitalWrite(MISO, LOW);}
     // m/=2; // shifst m by one bit to the right
       

    }








  }



  // void SCK_Check_HighToLow (void){

  // //      if(digitalRead(SCK) == LOW) // if SCK is LOW, the master is sending data
  // //  {

  //    if(digitalRead(MOSI) == HIGH)
  //   {Slave_received = Slave_received | h;} // if MOSI has a bit 1 on it, stores on the hth bit on Slave_received
  //  // h/=2; // shifst h by one bit to the right

  // //  }

  // }


  // void SCK_Check_LowToHigh (void){

  //     if((Slave_send & h) != 0x00) // if there is a bit on the mth bit to be sent, sends it
  //     {digitalWrite(MISO, HIGH);}
  //     else
  //     {digitalWrite(MISO, LOW);}
  //    // m/=2; // shifst m by one bit to the right
  //   h/=2;

  // }



// byte SPI_Com(byte send)
//   {
    
//     byte received = 0;
//     byte  k = 0;
//     for(k = 0x80; k != 0; k /= 2) // \frac{k}{2} shifts the kth bit to the right >>
//     {

//       if ((k & send) !=0x00) // tests if the kth send bit is 1 or 0 with the k mask
//         {digitalWrite(MOSI,HIGH);} // Writes the kth bit of the sending word on the MOSI PIN
//       else
//         {digitalWrite(MOSI,LOW);}

//       delay(SPI_Delay_Time); // COM delay
//       digitalWrite(SCK, HIGH); // Sets SCK (Clock) to HIGH so the master is on the reading period
      
//       if(digitalRead(MISO)) // if the Master Input is in HIGH state (a bit has been sent), then stores this bit in the kth position of the received var
//         {received |= k ;}  

        
//       // lcd.clear();
//       delay(SPI_Delay_Time); // COM delay
//       digitalWrite(SCK, LOW); // Sets SCK (Clock) to LOW so the master is on the sending period again
//       // lcd.setCursor(0,0);
//       // lcd.print(k);


//     }
//     aux = 0; // resets aux 
//     digitalWrite(MOSI,LOW);
//     return received; // returns the received 

//   }









