#include <EEPROM.h>


unsigned int mary_land (void)
{}
void setup() {
  // put your setup code here, to run once:
EEPROM.write(0,0);
EEPROM.write(1,100);
EEPROM.write(2,0);
EEPROM.write(3,5);
EEPROM.write(10,0);
EEPROM.write(4,0);
EEPROM.write(5,5);
EEPROM.write(100,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
