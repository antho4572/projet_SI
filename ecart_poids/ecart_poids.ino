#include "rgb_lcd.h"
#include "HX711.h"
rgb_lcd lcd;
HX711 scale;
int mot = 2, haut = 4, bas = 8;
float valCaptGamelle, valDistribBT;
boolean  etatbphaut, etatbpbas;
void setup() {
  pinMode(haut, INPUT), (bas, INPUT), (mot, OUTPUT);
  digitalWrite(mot, LOW);
  lcd.begin(16, 2);//démarrage du lcd
  lcd.setRGB(0, 255, 0);//couleur verte de l'écran
  lcd.setCursor(0, 0);
  scale.begin(A2, A3);//Démarrage du capteur poids
  scale.set_scale(2280.f);//définition coefficient  pour capteur du poids
  scale.tare();
}

void loop() {
  lcd.clear();
  etatbphaut = digitalRead(haut), etatbpbas = digitalRead(bas);
  if (etatbpbas == HIGH) {
    digitalWrite(mot, LOW);
  }
  else if (etatbphaut == HIGH) {
    digitalWrite(mot, HIGH);
  }
  valCaptGamelle = scale.get_units(10), 1;
  valCaptGamelle = abs(valCaptGamelle);
  lcd.print(valCaptGamelle, 0);
  lcd.print("g"); 
  delay(500);
}
