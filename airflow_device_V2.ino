/**
 * TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */

#include "Wire.h"
#include "SparkFunMPL3115A2.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

MPL3115A2 myPressure1;
MPL3115A2 myPressure2;

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


void setup(){
    while (!Serial);
    delay(1000);
    Wire.begin();

  myPressure1.begin();
  myPressure1.setModeBarometer(); 
  myPressure1.setOversampleRate(7);
  myPressure1.enableEventFlags();
  myPressure2.begin();
  myPressure2.setModeBarometer(); 
  myPressure2.setOversampleRate(7);
  myPressure2.enableEventFlags(); 
    
    Serial.begin(115200);
    Serial.println("\nTCAScanner ready!");
    
    for (uint8_t t=0; t<8; t++) {
      tcaselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
      
        uint8_t data;
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
           Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
        }
      }
    }
    Serial.println("\ndone");
}

void loop(){
  tcaselect(2);
  float pressure1 = myPressure1.readPressure();
  Serial.print("Pressure 1 (Pa):");
  Serial.print(pressure1, 2);
  float temperature1 = myPressure1.readTempF();
  Serial.print(" Temp(f):");
  Serial.print(temperature1, 2);
  tcaselect(7);
  float pressure2 = myPressure2.readPressure();
  Serial.print("Pressure 2 (Pa):");
  Serial.print(pressure2, 2);
  float temperature2 = myPressure2.readTempF();
  Serial.print(" Temp(f):");
  Serial.print(temperature2, 2);
  float pDifference = myPressure1.readTempF()-myPressure2.readTempF();
  Serial.println();
  Serial.print(" Pressure Difference (Pa):");
  Serial.print(pDifference, 2);
  Serial.println();
}
