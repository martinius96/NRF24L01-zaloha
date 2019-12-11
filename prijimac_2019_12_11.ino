// nRF24L01 prijimac
#include <SPI.h>
#include "RF24.h"
int odozva = 1;
int prijem = 0;
#define CE 4
#define CS 3
RF24 nRF(CE, CS);
byte adresaPrijimac[] = "prijimac00"; //adresa prijimac
byte adresaVysielac[] = "vysielac00"; //adresa vysielac

void setup() {
  Serial.begin(9600);
  nRF.begin();
  nRF.setDataRate( RF24_250KBPS );
  nRF.setPALevel(RF24_PA_HIGH);
  nRF.openWritingPipe(adresaPrijimac);
  nRF.openReadingPipe(1, adresaVysielac);
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
    switch (prijem) {
      case 0:
        //PRE HODNOTU STATE 0
        Serial.println("ZAPNEM RELE");
        break;
      case 1:
        //PRE HODNOTU STATE 1
        Serial.println("VYPINAM RELE");
        break;
      default:
        //PRE HODNOTU STATE 2 alebo inu
        Serial.print("HODNOTA Z ROZSAHU 2-10 --> nepridelena akcia");
        break;
    }
  }
}
