#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
  while (!Serial) {
    /*** Arguardando o estabelecimento da conexão ***/
  }
  // Serial.println("Slave send");
  // Serial.println(EEPROM.read(0));
  Serial.println("Slave received");
  Serial.println(EEPROM.read(1));
}

void loop() {
  // put your main code here, to run repeatedly:

}
