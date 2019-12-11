// nRF24L01 prijimac
#include <SPI.h> //arduino kniznica pre SPI zbernicu
#include "RF24.h" //kniznica pre moduly
int odozva = 1; //globalna premenna pre odosielany stav
int prijem = 0; //globalna premenna pre citany stav
#define CE 4 //chip state pin
#define CS 3 //chip select pin
RF24 nRF(CE, CS); //nrf s volitelnymi pinmi CE a CS
byte adresaPrijimac[] = "prijimac00"; //adresa prijimac
byte adresaVysielac[] = "vysielac00"; //adresa vysielac
void setup() {
  Serial.begin(9600); //Serial monitor na 9600 baud/s
  nRF.begin(); //zapnut komunikaciu s nRF modulom
  nRF.setDataRate( RF24_250KBPS );
  nRF.setPALevel(RF24_PA_HIGH);
  nRF.openWritingPipe(adresaPrijimac); //zapisujeme na prijimac na jeho adresu
  nRF.openReadingPipe(1, adresaVysielac); //citame na svojej adrese na kanale XX
  nRF.startListening();
}

void loop() {
  if ( nRF.available()) {
    while (nRF.available()) {
      nRF.read( &prijem, sizeof(prijem) );
    }
    Serial.println("Prijata volba: ");
    Serial.println(prijem);
    Serial.println("Odozva: ");
    Serial.println(odozva);
    nRF.stopListening();
    nRF.write( &odozva, sizeof(odozva) );
    nRF.startListening();
  }
}
