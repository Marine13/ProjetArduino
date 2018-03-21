//**********************************************
//Code de reference pour le module bluetooth HC-06  
// envoi de commandes AT et affichage de la rÃ©ponse du module
#include <SoftwareSerial.h> //Software Serial Port
#define RxD 10 //Pin 10 pour RX, PB2 sur votre board, a brancher sur le TX du HC-06
#define TxD 11 //Pin 11 pour TX, PB3 sur votre board, a brancher sur le RX du HC-06
SoftwareSerial BTSerie(RxD,TxD); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Configuration du bluetooth
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  BTSerie.begin(9600);
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  char recvChar;
  delay(500);
  //On lit caractere par caractere sur le BTSerie et on affiche sur le Terminal Serie
  if (BTSerie.available()) {
  recvChar = BTSerie.read(); //lecture
  if(recvChar == '1') {
    digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000); // wait for a second
    digitalWrite(13, LOW); // turn the LED off by making the voltage LO
  }
  Serial.print(recvChar); //ecriture
  }
  //On lit caractere par caractere sur le terminal et on affiche sur le BT Serie
//  if (Serial.available()) {
//  recvChar = Serial.read(); //lecture
  BTSerie.write("Bonjour"); //ecriture
//  }

}
