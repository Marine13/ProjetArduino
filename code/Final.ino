#include <SoftwareSerial.h> //Software Serial Port
#define RxD 10 //Pin 10 pour RX, PB2 sur votre board, a brancher sur le TX du HC-06
#define TxD 11 //Pin 11 pour TX, PB3 sur votre board, a brancher sur le RX du HC-06
SoftwareSerial BTSerie(RxD,TxD); 

float temp_air ;
int hum_air ;
int nivEau = 2;

//début constantes pH
const int phmetre=1 ;
float tension ;
float pH ;
//fin constantes pH 

//début constantes pour température eau
float tens ;
float res ;
float temp_kel ;
float temp_cel ;
//fin constantes pour température eau

//début constantes pour météo
const byte BROCHE_CAPTEUR = 5;
const byte DHT_SUCCESS = 0;        // Pas d'erreur
const byte DHT_TIMEOUT_ERROR = 1;  // Temps d'attente dépassé
const byte DHT_CHECKSUM_ERROR = 2; // Données reçues erronées
// fin constantes pour météo

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Configuration du bluetooth
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  BTSerie.begin(9600);

}






// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------- DÉBUT DU LOOP ------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() { 
  char recvChar;
  delay(500);
  if (BTSerie.available()) {
  recvChar = BTSerie.read(); //lecture
    /* CAPTEUR MÉTÉO Place la broche du capteur en entrée avec pull-up */
  pinMode(BROCHE_CAPTEUR, INPUT_PULLUP);
  }




/* ---------------------------------------------------------------------- DÉBUT ANALYSE NIVEAU EAU ---------------------------------------------------------------------*/
  Serial.println("\n");
  
  Serial.println(analogRead(nivEau)) ;
  if(analogRead(nivEau)>700){
    BTSerie.write("*n") ; BTSerie.print("R255G000B000") ; BTSerie.write("*" );
    Serial.println("Vide");
  }
  else{
    BTSerie.write("*n") ; BTSerie.print("R000G000B000") ; BTSerie.write("*" );
    Serial.println("Plein");
  }

  Serial.println("\n");
  //delay(5000) ;
  
/* ---------------------------------------------------------------------- FIN ANALYSE NIVEAU EAU ------------------------------------------------------------------------*/




/* ---------------------------------------------------------------------- DÉBUT ANALYSE ET ENVOIE DONNÉES TEMPÉRATURE EAU ---------------------------------------------------------------------*/
  tens=(1023-analogRead(A0))/204.6;
  res=(tens*8300)/(5-tens) ;
  temp_kel=(298.15*3977)/(298.15*log(res/8300)+3977) ;
  temp_cel=temp_kel-273.15 ;
  BTSerie.write("*t") ; BTSerie.print(temp_cel) ; BTSerie.write("*" );
  Serial.print("La température de l'eau est de : ") ;
  Serial.println (temp_cel) ;
  
  BTSerie.write("*t") ; BTSerie.print(temp_cel) ; BTSerie.print("C") ; BTSerie.write("*" );
/* ---------------------------------------------------------------------- FIN ANALYSE ET ENVOIE DONNÉES TEMPÉRATURE EAU ------------------------------------------------------------------------*/



/* ---------------------------------------------------------------------- DÉBUT ANALYSE DONNÉES PH -------------------------------------------------------------------------------------------*/

  tension = analogRead(phmetre) ;
  pH=tension*5/240 ;

/* ---------------------------------------------------------------------- FIN ANALYSE DONNÉES PH --------------------------------------------------------------------------------------------*/




/* ------------------------------------------------------------------------- DÉBUT TRAITEMENT DES DONNÉES PH ----------------------------------------------------------------------------------*/
pH=5.23 ;
Serial.print("Le pH vaut : ") ; Serial.println (pH) ;
BTSerie.write("*m") ; BTSerie.print(pH) ; BTSerie.write("*" );

//Taux de pH correct 
if ((pH>=6.8)&&(pH<=7.8)) {                             
    BTSerie.write("*P") ; BTSerie.print("R000G000B000") ; BTSerie.write("*" );
    delay(500) ;
    }
  
//Taux de pH trop faible --> eau trop acide --> manque de pH+
else if (pH<6.8) {
  BTSerie.write("*P") ; BTSerie.print("R243G146B230") ; BTSerie.write("*" );
  //delay(500) ;
  }

//Taux de pH trop élevé --> eau trop basique --> manque de pH-
else {
  BTSerie.write("*P") ; BTSerie.print("R100G072B250") ; BTSerie.write("*" );
  //delay(500) ;
  }
/*---------------------------------------------------------------------- FIN TRAITEMENT DONNNÉES PH --------------------------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------DÉBUT RECUPERATION ET ENVOIE DONNÉES MÉTÉO ---------------------------------------------------------------------------------*/
/**
 * Exemple de code pour la lecture d'un capteur DHT11
 */




  float temperature, humidity;
 
  /* Lecture de la température et de l'humidité, avec gestion des erreurs */
  switch (readDHT11(BROCHE_CAPTEUR, &temperature, &humidity)) {
  case DHT_SUCCESS: 
     
    /* Affichage de la température et du taux d'humidité */
    Serial.print(F("Humidite (%): "));
    Serial.println(humidity, 2);
    Serial.print(F("Temperature (^C): "));
    Serial.println(temperature, 2);
    break;
 
  case DHT_TIMEOUT_ERROR: 
    Serial.println(F("Pas de reponse !")); 
    break;
 
  case DHT_CHECKSUM_ERROR: 
    Serial.println(F("Pb de communication !")); 
    break;
  }
  
  /* Pas plus d'une mesure par seconde */
  // N.B. Avec le DHT22 il est possible de réaliser deux mesures par seconde
  delay(1000);

  temp_air=10;
  hum_air=10 ;
  BTSerie.write("*H") ; BTSerie.print(humidity, 2) ; BTSerie.print("%") ; BTSerie.write("*" ) ;
  BTSerie.write("*T") ; BTSerie.print(temperature, 2) ; BTSerie.print("C") ; BTSerie.write("*" );

/* --------------------------------------------------------------------- FIN RECUPERATION ET ENVOIE DES DONNÉES MÉTÉO -------------------------------------------------------------------------------*/



/* ----------------------------------------------------------------------DÉBUT ANALYSE ET ENVOIE DONNÉES MÉTÉO ---------------------------------------------------------------------------------*/

  String phrase ;

  

  if (humidity>80) {
    phrase="La pluie n'est pas loin, restez au sec :(" ;
  }
  else if ((temperature>25)&&(humidity<65)){
    phrase="Grand soleil, a l'eau :D" ;

  }
  else if (temperature<15) {
    phrase="Glaglalgla :$" ;
  }
  else {
    phrase="A vous de decider, baignade... ou pas :)" ;
  }
  Serial.println(phrase) ;
  BTSerie.write("*A") ; BTSerie.print(phrase) ; BTSerie.write("*" );

/* --------------------------------------------------------------------- FIN ANALYSE ET ENVOIE DES DONNÉES MÉTÉO -------------------------------------------------------------------------------*/




} // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- FIN DU LOOP ------------------------------------------------------------------------------------------------------ 
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------DÉBUT FONCTION DHT11 --------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/**
 * Lit la température et le taux d'humidité mesuré par un capteur DHT11.
 *
 * @param pin Broche sur laquelle est câblée le capteur.
 * @param temperature Pointeur vers la variable stockant la température.
 * @param humidity Pointeur vers la variable stockant le taux d'humidité.
 * @return DHT_SUCCESS si aucune erreur, DHT_TIMEOUT_ERROR en cas de timeout, ou DHT_CHECKSUM_ERROR en cas d'erreur de checksum.
 */
byte readDHT11(byte pin, float* temperature, float* humidity) {
  
  /* Lit le capteur */
  byte data[5];
  byte ret = readDHTxx(pin, data, 18, 1000);
  
  /* Détecte et retourne les erreurs de communication */
  if (ret != DHT_SUCCESS) 
    return ret;
    
  /* Calcul la vraie valeur de la température et de l'humidité */
  *humidity = data[0];
  *temperature = data[2];

  /* Ok */
  return DHT_SUCCESS;
}

/**
 * Fonction bas niveau permettant de lire la température et le taux d'humidité (en valeurs brutes) mesuré par un capteur DHTxx.
 */
byte readDHTxx(byte pin, byte* data, unsigned long start_time, unsigned long timeout) {
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  // start_time est en millisecondes
  // timeout est en microsecondes
 
  /* Conversion du numéro de broche Arduino en ports / masque binaire "bas niveau" */
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *ddr = portModeRegister(port);   // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port);   // Registre IN (lecture)
  
  /* Conversion du temps de timeout en nombre de cycles processeur */
  unsigned long max_cycles = microsecondsToClockCycles(timeout);
 
  /* Evite les problèmes de pull-up */
  *out |= bit;  // PULLUP
  *ddr &= ~bit; // INPUT
  delay(100);   // Laisse le temps à la résistance de pullup de mettre la ligne de données à HIGH
 
  /* Réveil du capteur */
  *ddr |= bit;  // OUTPUT
  *out &= ~bit; // LOW
  delay(start_time); // Temps d'attente à LOW causant le réveil du capteur
  // N.B. Il est impossible d'utilise delayMicroseconds() ici car un délai
  // de plus de 16 millisecondes ne donne pas un timing assez précis.
  
  /* Portion de code critique - pas d'interruptions possibles */
  noInterrupts();
  
  /* Passage en écoute */
  *out |= bit;  // PULLUP
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT
 
  /* Attente de la réponse du capteur */
  timeout = 0;
  while(!(*in & bit)) { /* Attente d'un état LOW */
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }
    
  timeout = 0;
  while(*in & bit) { /* Attente d'un état HIGH */
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }

  /* Lecture des données du capteur (40 bits) */
  for (byte i = 0; i < 40; ++i) {
 
    /* Attente d'un état LOW */
    unsigned long cycles_low = 0;
    while(!(*in & bit)) {
      if (++cycles_low == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }

    /* Attente d'un état HIGH */
    unsigned long cycles_high = 0;
    while(*in & bit) {
      if (++cycles_high == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }
    
    /* Si le temps haut est supérieur au temps bas c'est un "1", sinon c'est un "0" */
    data[i / 8] <<= 1;
    if (cycles_high > cycles_low) {
      data[i / 8] |= 1;
    }
  }
  
  /* Fin de la portion de code critique */
  interrupts();
 
  /*
   * Format des données :
   * [1, 0] = humidité en %
   * [3, 2] = température en degrés Celsius
   * [4] = checksum (humidité + température)
   */
   
  /* Vérifie la checksum */
  byte checksum = (data[0] + data[1] + data[2] + data[3]) & 0xff;
  if (data[4] != checksum)
    return DHT_CHECKSUM_ERROR; /* Erreur de checksum */
  else
    return DHT_SUCCESS; /* Pas d'erreur */
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------- FIN FONCTION DHT11 --------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
