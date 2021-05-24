// Potrebne biblioteke za program
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Oderđivanje na kojim se pinovima nalazi CE i CSN pinovi modula
RF24 radio(7, 8); 
// Određivanje kanala preko kojeg će moduli komunicirat
const byte address[6] = {"00001"}; 
Servo myServo; // Deklariranje serva 
Servo ESC; // Deklariranje kontrolera za motor ESC
int kutSkretanja = 0; // Deklariranje varijable za kut skretanja
int vrijednostBrzine = 0; // Deklariranje varijabli za vrijednost brzine

// Void koji će se odvrtiti samo jednom nakon pokretanja Arduina
void setup() {
  Serial.begin(9600);
  myServo.attach(5); // Deklaracija pina od serva
  // Deklaracija pina od ESC, minimalna širina impulsa, maksimalna širina impulsa
  ESC.attach(6,1000,2000); 
  radio.begin(); // Započinjanje radio veze
  // Postavljanje adrese od kanala za prijemnik preko kojeg će komunicirat
  radio.openReadingPipe(0, address); 
  // Postavljanje maksimalne razine pojačanja snage 
  radio.setPALevel(RF24_PA_MAX); 
}

// Void koji će se vrtiti cijelo vrijeme dok Arduino radi
void loop() {
  radio.startListening(); // Postavljanje modula kao prijemink
  if ( radio.available()) { // Provjera dali je radio veza uspostavljana
    // Vrti sljedeču petlju sve dok je radio veze uspostavljana
    while (radio.available()) {

      // Slušanje i spremanje vrijednost
      radio.read(&kutSkretanja, sizeof(kutSkretanja)); 
      // Pomicanje serva za primljenu vrijednost
      myServo.write(kutSkretanja);  
      Serial.print("Skretanje: "); Serial.println(kutSkretanja);

      // Slušanje i spremanje vrijednost 
      radio.read(&vrijednostBrzine, sizeof(vrijednostBrzine)); 
      // Postavljanje brzine motora za primljanu vrijednost
      ESC.write(vrijednostBrzine);
      Serial.print("Brzina: "); Serial.println(vrijednostBrzine);
      //delay(500);
    }
  }
}
