#include "Wire.h"
#include <Adafruit_MPL115A2.h>
 
Adafruit_MPL115A2 bmp;
//==== Branchements du module BMP085 avec 4 fils :
// GND sur la masse de l'Arduino
// +3.3 sur le +3.3 de l'Arduino
// SDA (=data) vers AnalogPin4
// SCL (=clock) vers AnalogPin5
//
// Pas besoin de brancher CLR (reset) ni OEC (end of conversion)
// Mais EOC peut servir si on cherche à accélérer son code Arduino
 
void setup() {
  Serial.begin(9600);  //Port série
  bmp.begin(); 
 
}
 
void loop() {
    Serial.print("Temperature = ");   //En degrés Celsius (SI)
    Serial.print(bmp.getTemperature());
    Serial.print(" *C");
 
    Serial.print("   Pression = ");  //En Pascal (SI)
    Serial.print(bmp.getPressure());
    Serial.print(" Pa ");
   
    // 1 Pa = 10-5 bar = 0.0075 mm Hg = 9.87.10-6 atm
    float PressionPascal;
    float PressionAtm;
    float PressionHg;
    PressionPascal = bmp.getPressure();  //Lire le capteur
    PressionAtm = PressionPascal*9.81*0.000001;
    PressionHg = PressionPascal*0.0075;
   
    Serial.print(PressionPascal);
    Serial.print(" Pa     ");
    Serial.print(PressionAtm);
    Serial.print(" Atm     ");
    Serial.print(PressionHg);
    Serial.println(" mm Hg     ");     

    delay(500);  //2 mesures par seconde
}
