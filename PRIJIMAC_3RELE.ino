// nRF24L01 prijimac, ovladanie 3 rele od vysielaca tlacidlami
// Autor: Martin Chlebovec (martinius96)
// E-mail: martinius96@gmail.com

#include <SPI.h>
#include "RF24.h"
#define CE 4
#define CS 3
RF24 nRF(CE, CS);
int odozva = 0;
int prijem = 0;
byte adresaPrijimac[] = "prijimac00"; //adresa prijimac
byte adresaVysielac[] = "vysielac00"; //adresa vysielac
const int rele1 = 6;
const int rele2 = 7;
const int rele3 = 8;

int rele1_state = LOW;
int rele2_state = LOW;
int rele3_state = LOW;

void setup() {
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  digitalWrite(rele1, rele1_state);
  digitalWrite(rele2, rele2_state);
  digitalWrite(rele3, rele3_state);
  Serial.begin(115200);
  nRF.begin();
  nRF.setDataRate( RF24_250KBPS );
  nRF.setPALevel(RF24_PA_LOW);
  nRF.openWritingPipe(adresaPrijimac);
  nRF.openReadingPipe(1, adresaVysielac);
  nRF.startListening();
}

void loop() {
  if ( nRF.available()) {
    while (nRF.available()) {
      nRF.read( &prijem, sizeof(prijem) );
    }
    Serial.println("Prijate data od vysielaca: ");
    Serial.println(prijem);
    Serial.println("Odozva k vysielacu: ");
    Serial.println(odozva);
    nRF.stopListening();
    nRF.write( &odozva, sizeof(odozva) );
    nRF.startListening();
    switch (prijem) {
      case 0:
        rele1_state = !rele1_state;
        digitalWrite(rele1, rele1_state);
        break;
      case 1:
        rele2_state = !rele2_state;
        digitalWrite(rele2, rele2_state);
        break;
      case 2:
        rele3_state = !rele3_state;
        digitalWrite(rele3, rele3_state);
        break;
      default:
        Serial.print("Nepridelena akcia");
        break;
    }
  }
}
