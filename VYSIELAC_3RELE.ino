// nRF24L01 vysielač, ovladanie 3 rele cez tlacidla
// Autor: Martin Chlebovec (martinius96)
// E-mail: martinius96@gmail.com

#include <SPI.h>
#include "RF24.h"
#define CE 4
#define CS 3
RF24 nRF(CE, CS);
int data_na_odoslanie = 0;
int prijate_data = 0;
byte adresaPrijimac[] = "prijimac00";
byte adresaVysielac[] = "vysielac00";

const int buttonPin1 = 6;
const int buttonPin2 = 7;
const int buttonPin3 = 8;

int buttonState1 = HIGH;
int buttonState2 = HIGH;
int buttonState3 = HIGH;

int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int lastButtonState3 = HIGH;

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;

unsigned long debounceInterval = 50;
void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  Serial.begin(115200);
  nRF.begin();
  nRF.setDataRate( RF24_250KBPS );
  nRF.setPALevel(RF24_PA_LOW);
  nRF.openWritingPipe(adresaVysielac);
  nRF.openReadingPipe(1, adresaPrijimac);
  nRF.startListening();
}

void loop() {
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);
  int reading3 = digitalRead(buttonPin3);
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }

  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if (reading3 != lastButtonState3) {
    lastDebounceTime3 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceInterval) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH) {
        nRF.stopListening();
        data_na_odoslanie = 0;
        nRF.write( &data_na_odoslanie, sizeof(data_na_odoslanie) );
        nRF.startListening();
        Serial.println("Odosielam prijimacu toto: ");
        Serial.println(data_na_odoslanie);
      }
    }
  }

  if ((millis() - lastDebounceTime2) > debounceInterval) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == HIGH) {
        nRF.stopListening();
        data_na_odoslanie = 1;
        nRF.write( &data_na_odoslanie, sizeof(data_na_odoslanie) );
        nRF.startListening();
        Serial.println("Odosielam prijimacu toto: ");
        Serial.println(data_na_odoslanie);
      }
    }
  }

  if ((millis() - lastDebounceTime3) > debounceInterval) {
    if (reading3 != buttonState3) {
      buttonState3 = reading3;
      if (buttonState3 == HIGH) {
        nRF.stopListening();
        data_na_odoslanie = 2;
        nRF.write( &data_na_odoslanie, sizeof(data_na_odoslanie) );
        nRF.startListening();
        Serial.println("Odosielam prijimacu toto: ");
        Serial.println(data_na_odoslanie);
      }
    }
  }

  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
  lastButtonState3 = reading3;
  while (nRF.available()) {
    nRF.read( &prijate_data, sizeof(prijate_data) );
    Serial.println("Prijimac odpoveda ze prijal tieto data: ");
    Serial.println(prijate_data);
  }

}
