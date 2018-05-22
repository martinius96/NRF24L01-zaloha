// nRF24L01 prijimac
#include <SPI.h> //arduino kniznica pre SPI zbernicu
#include "RF24.h" //kniznica pre moduly
int odezva; //globalna premenna pre odosielany stav timu
int prijem; //globalna premenna pre citany stav timu
const int buzzer = 2; //pin pre hlasic
const int cervenytim = 5; //pin pre cerveny button
const int zltytim = 6;  //pin pre zlty button
const int cervenaled = 7;
const int zltaled = 8;
#define CE 4 //chip state pin
#define CS 3 //chip select pin
RF24 nRF(CE, CS); //nrf s volitelnymi pinmi CE a CS
byte adresaPrijimac[] = "prijimac00"; //adresa prijimac
byte adresaVysilac[] = "vysilac00"; //adresa vysielac
void setup() {
  Serial.begin(9600); //Serial monitor na 9600 baud/s
  pinMode(buzzer, OUTPUT); //hlasic nastavit ako vystup
    pinMode(cervenaled, OUTPUT);
  pinMode(zltaled, OUTPUT);
  pinMode(cervenytim, INPUT_PULLUP); //tlacidla ako vstup na pullup rezistor
  pinMode(zltytim, INPUT_PULLUP); //tlacidla ako vstup na pullup rezistor
  nRF.begin(); //zapnut komunikaciu s nRF modulom
  nRF.setDataRate( RF24_250KBPS ); //najnizsia rychlost pre najvacsiu moznu vzidalenost
  nRF.setPALevel(RF24_PA_HIGH); //napajacie napatie na high moznost MAX potrebuje externy zdroj na 5v (3.3v pri napojeni napriamo)
  nRF.openWritingPipe(adresaPrijimac); //zapisujeme na prijimac na jeho adresu
  nRF.openReadingPipe(1, adresaVysilac); //citame na svojej adrese na kanale XX
  nRF.startListening();
}

void loop() {
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
    if(odezva == prijem){
      tone(buzzer, 10000, 1000);
      
      }else{
        noTone(buzzer);
        }  
  if ( nRF.available()) {
    // čekání na příjem dat
    while (nRF.available()) {
      nRF.read( &prijem, sizeof(prijem) );
    }
    Serial.println("Prijata volba: ");
    Serial.println(prijem);
    Serial.println("Odozva: ");
    Serial.println(odezva);
    nRF.stopListening();
    nRF.write( &odezva, sizeof(odezva) );
    nRF.startListening();
  }
}
