//Fonctionnement: La valeur de la tension de sortie du capteur pH 
//         se convertit en pH et s'affiche ensuite sur le moniteur série.
#define Vref 4.95
void setup()
{ 
  Serial.begin(9600);
}
void loop()
{
  int sensorValue;
  int m;
  long sensorSum;
  for(m=0;m<50;m++)
  {
    sensorValue=analogRead(A0);//Connecter le capteur de pH au port A0
    sensorSum += sensorValue;
  }
   sensorValue =   sensorSum/50;
 Serial.print(" the PH value is "); 
 Serial.println(7-1000*(sensorValue-372)*Vref/59.16/1023);

}
