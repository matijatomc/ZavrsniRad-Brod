// Potrebne biblioteke za program
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Oderđivanje na kojim se pinovima nalazi CE i CSN pinovi modula
RF24 radio1(7, 8); 
RF24 radio2(7, 8); 
// Određivanje kanala preko kojeg će moduli komunicirat
const byte address1[6] = {"00001"}; 
const byte address2[6] = {"00002"};
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
  radio1.begin(); // Započinjanje radio veze
  radio2.begin();
  // Postavljanje adrese od kanala za prijemnik preko kojeg će komunicirat
  radio1.openReadingPipe(0, address1); 
  radio2.openReadingPipe(1, address2);
  // Postavljanje maksimalne razine pojačanja snage 
  radio1.setPALevel(RF24_PA_MAX); 
  radio2.setPALevel(RF24_PA_MAX);
}

// Void koji će se vrtiti cijelo vrijeme dok Arduino radi
void loop() {
  radio1.startListening(); // Postavljanje modula kao prijemink
  radio2.startListening(); 
  if ( radio1.available() || radio2.available()) { // Provjera dali je radio veza uspostavljana
    // Vrti sljedeču petlju sve dok je radio veze uspostavljana
    while (radio1.available() || radio2.available()) {

      // Slušanje i spremanje vrijednost
      radio1.read(&kutSkretanja, sizeof(kutSkretanja)); 
      // Pomicanje serva za primljenu vrijednost
      myServo.write(kutSkretanja);  
      Serial.print("Skretanje: "); Serial.println(kutSkretanja);

      // Slušanje i spremanje vrijednost 
      radio2.read(&vrijednostBrzine, sizeof(vrijednostBrzine)); 
      // Postavljanje brzine motora za primljanu vrijednost
      ESC.write(vrijednostBrzine);
      Serial.print("Brzina: "); Serial.println(vrijednostBrzine);
      //delay(500);
    }
  }
}
