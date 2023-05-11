
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define up 8

#define reset 9


#define MISO 0  // Master INPUT Slave OUTPUT
#define MOSI 1 // Master OUTPUT Slave INPUT
#define SS 6   // Slave Select (Enable)
#define SCK 7  // Clock

byte test = 170;
int aux = 0;

int Rx = 0;

int SPI_Delay_Time = 1000;

unsigned char SPI_Com(unsigned char send);

void setup() {
  // put your setup code here, to run once:

  lcd.begin(16,2); // inicializa lcd
  lcd.print("Value = ");

  pinMode(up,INPUT);
  pinMode(reset,INPUT);

  lcd.setCursor(9, 0);
  lcd.print(aux);

  // Defining SPI pin's

  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH); // Set Slave Select to HIGH, so it is inactive
  digitalWrite(SCK, LOW); // Gurantees SCK is set to LOW so when the COM starts the Master is on Sending mode
  digitalWrite(MOSI,LOW); // Gurantees SCK is set to LOW so when the COM starts the Master is on Sending mode
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(up) == HIGH)
{
delay(150);
aux = aux + 17;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Value = ");
lcd.setCursor(9, 0);
lcd.print(aux);
}


//if(digitalRead(reset) == HIGH && aux != 0) // This Button Will Call SPI_Com();
if(digitalRead(reset) == HIGH) // This Button Will Call SPI_Com();
{


  digitalWrite(SS, LOW); // Sets Slave Select to LOW so it is active;
  Rx = int(SPI_Com(byte(aux))); // sends aux as a byte to Slave to make the transform; (receives it as an int variable so it can be easily writen on the LCD)
  digitalWrite(SS, HIGH); // Sets Slave Select to HIGH to it is inactive
  delay(500);

// lcd.clear(); 
// lcd.setCursor(6, 0);
// lcd.print("LCD");
// lcd.setCursor(5, 1);
// lcd.print("RESET");
// lcd.setCursor(13, 1);
// lcd.print("2.0");
// delay(500);
// lcd.setCursor(13, 1);
// lcd.print("1.5");
// delay(500);
// lcd.setCursor(13, 1);
// lcd.print("1.0");
// delay(500);
// lcd.setCursor(13, 1);
// lcd.print("0.5");
// delay(500);
// aux = 0;
// lcd.clear();
// lcd.setCursor(0, 0);
// lcd.print("Value = ");
// lcd.setCursor(9, 0);
// lcd.print(aux);

}

// else if (digitalRead(reset) == HIGH && aux == 0){


// lcd.clear();
// lcd.setCursor(0,0);
// lcd.print("Cannot reset if");
// lcd.setCursor(0,1);
// lcd.print("Value = 0 !!!");
// delay(2000);
// lcd.clear();
// lcd.setCursor(0, 0);
// lcd.print("Value = ");
// lcd.setCursor(9, 0);
// lcd.print(aux);

// }



}



byte SPI_Com(byte send)
  {
    
    byte received = 0;
    byte  k = 0;
    for(k = 0x80; k != 0; k /= 2) // \frac{k}{2} shifts the kth bit to the right >>
    {

      if ((k & send) !=0x00) // tests if the kth send bit is 1 or 0 with the k mask
        {digitalWrite(MOSI,HIGH);} // Writes the kth bit of the sending word on the MOSI PIN
      else
        {digitalWrite(MOSI,LOW);}

      delay(SPI_Delay_Time); // COM delay
      digitalWrite(SCK, HIGH); // Sets SCK (Clock) to HIGH so the master is on the reading period
      
      if(digitalRead(MISO)) // if the Master Input is in HIGH state (a bit has been sent), then stores this bit in the kth position of the received var
        {received |= k ;}  

        
      // lcd.clear();
      delay(SPI_Delay_Time); // COM delay
      digitalWrite(SCK, LOW); // Sets SCK (Clock) to LOW so the master is on the sending period again
      // lcd.setCursor(0,0);
      // lcd.print(k);


    }
    aux = 0; // resets aux 
    digitalWrite(MOSI,LOW);
    return received; // returns the received 

  }



