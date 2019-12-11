// nRF24L01 vysielac
#include <SPI.h>
#include "RF24.h"
#define CE 4
#define CS 3
RF24 nRF(CE, CS);
int odezva = 1;
int prijem = 0;
byte adresaPrijimac[] = "prijimac00";
byte adresaVysielac[] = "vysielac00";

void setup() {
  Serial.begin(9600);
  nRF.begin();
  nRF.setDataRate( RF24_250KBPS );
  nRF.setPALevel(RF24_PA_HIGH);
  nRF.openWritingPipe(adresaVysielac);
  nRF.openReadingPipe(1, adresaPrijimac);
  nRF.startListening();
}

void loop() {
  nRF.stopListening();
  nRF.write( &odezva, sizeof(odezva) );
  nRF.startListening();
  while (nRF.available()) {
    nRF.read( &prijem, sizeof(prijem) );
  }
  Serial.println("Prijate data: ");
  Serial.println(prijem);
  Serial.println("Odozva: ");
  Serial.println(odezva);
  delay(50);
}
