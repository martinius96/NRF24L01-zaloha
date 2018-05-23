// nRF24L01 vysílač
#include <SPI.h>
#include "RF24.h"
#define CE 4
#define CS 3
RF24 nRF(CE, CS);
int odezva = 8;
int prijem = 12;
const int buzzer = 2;
const int cervenytim = 5;
const int zltytim = 6;
const int cervenaled = 7;
const int zltaled = 8;
byte adresaPrijimac[]= "prijimac00";
byte adresaVysilac[]= "vysilac00";
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(cervenaled, OUTPUT);
  pinMode(zltaled, OUTPUT);
  pinMode(cervenytim, INPUT_PULLUP);
  pinMode(zltytim, INPUT_PULLUP);
  nRF.begin();
  nRF.setDataRate( RF24_250KBPS );
  nRF.setPALevel(RF24_PA_HIGH);
  nRF.openWritingPipe(adresaVysilac);
  nRF.openReadingPipe(1,adresaPrijimac);
  nRF.startListening();
}

void loop() {
 if(odezva == prijem){
          
  tone( buzzer, 900, 800);
 
  delay(1000);
      }else{
        noTone;
        int vystupcerveny = digitalRead(cervenytim);
  int vystupzlty = digitalRead(zltytim);
  if(vystupcerveny==LOW){
    odezva = 1;
    digitalWrite(cervenaled, HIGH);
    digitalWrite(zltaled, LOW);
    }else if(vystupzlty==LOW){
    odezva = 2;
    digitalWrite(cervenaled, LOW);
    digitalWrite(zltaled, HIGH);
    }
    
    nRF.stopListening();
     nRF.write( &odezva, sizeof(odezva) );
    nRF.startListening();
 while (nRF.available()) {
      nRF.read( &prijem, sizeof(prijem) );
    }
    Serial.println("Prijata volba: ");
    Serial.println(prijem);
    Serial.println("Odozva: ");
    Serial.println(odezva);
    delay(50);
        
        
        }
  
  
  }




