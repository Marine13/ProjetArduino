float tens ;
float res ;
float temp_kel ;
float temp_cel ;


void setup() {
  Serial.begin (9600) ;
}

void loop() {  
  tens=analogRead(A0)/204.6;
  res=(tens*8300)/(5-tens) ;
  temp_kel=(298.15*3977)/(298.15*log(res/8300)+3977) ;
  temp_cel=temp_kel-273,15 ;
  Serial.println (temp_cel) ;

}
