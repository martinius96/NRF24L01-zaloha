// nRF24L01 vysílač

// připojení knihoven
#include <SPI.h>
#include "RF24.h"
// nastavení propojovacích pinů
#define CE 4
#define CS 3
// inicializace nRF s piny CE a CS
RF24 nRF(CE, CS);
// nastavení adres pro přijímač a vysílač,
// musí být nastaveny stejně v obou programech!
byte adresaPrijimac[]= "prijimac00";
byte adresaVysilac[]= "vysilac00";
void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  // zapnutí komunikace nRF modulu
  nRF.begin();
  nRF.setDataRate( RF24_250KBPS );
  // nastavení výkonu nRF modulu,
  // možnosti jsou RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX,
  // pro HIGH a MAX je nutnýv externí 3,3V zdroj
  nRF.setPALevel(RF24_PA_HIGH);
  // nastavení zapisovacího a čtecího kanálu
  nRF.openWritingPipe(adresaVysilac);
  nRF.openReadingPipe(1,adresaPrijimac);
  // začátek příjmu dat
  nRF.startListening();
}

void loop() {
  // for smyčka pro postupné odeslání
  // hodnot 0 až 3 pro načtení všech dat
  // z přijímače
  for (int i=0; i < 4; i++ ) {
    // ukončení příjmu dat
    nRF.stopListening();
    // vytisknutí aktuální volby po sériové lince
    Serial.print("Posilam volbu ");
    Serial.println(i);
    // uložení startovního času komunikace
    unsigned long casZacatek = micros();
    // odeslání aktuální volby, v případě selhání
    // vytištění chybové hlášky na sériovou linku
    if (!nRF.write( &i, sizeof(i) )){
       Serial.println("Chyba při odeslání!");
    }
    nRF.startListening();
    unsigned long casCekaniOdezvy = micros();
    boolean timeout = false;
    while ( ! nRF.available() ){
    if (micros() - casCekaniOdezvy > 200000 ){
          timeout = true;
          break;
      }      
    }
    if ( timeout ){
      Serial.println("Chyba při prijmu, vyprseni casu na odezvu!");
    }
    else{
     
        unsigned long prijataData;
        nRF.read( &prijataData, sizeof(prijataData) );
        unsigned long casKonec = micros();
        Serial.print("Odeslana volba: ");
        Serial.print(i);
        Serial.print(", prijata data: ");
        Serial.println(prijataData);
        Serial.print("Delka spojeni: ");
        Serial.print(casKonec - casZacatek);
        Serial.println(" mikrosekund.");
    }
    delay(200);
  }
}
