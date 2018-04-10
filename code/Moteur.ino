const int moteur = 3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(moteur,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(moteur,HIGH);
  delay(5000);
  digitalWrite(moteur,LOW);
  delay(5000);
}

/*Programme en partie tiré du projet bar arduino*/
