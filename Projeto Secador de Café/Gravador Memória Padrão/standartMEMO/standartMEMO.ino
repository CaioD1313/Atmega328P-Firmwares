#include <EEPROM.h>
void setup() {
  EEPROM.write(0,45); // 45 segundos ligado address 0
  EEPROM.write(1,25); // 25 segundos desligado address 1
  EEPROM.write(2,40); // Alarme a 40ºC
  EEPROM.write(3,45); // Desliga sistema a 45ºC
  
  

}

void loop() {
  

}
