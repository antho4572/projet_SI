#include "rgb_lcd.h"
#include "HX711.h"
rgb_lcd lcd;
HX711 scale;

float valCaptGamelle, valDistribBT;
int heures, minutes, mot = 2;
int HeurePredefini1, HeurePredefini2, HeurePredefini3;
bool motOn = false, portion = false;

void setup() {
  pinMode(mot, OUTPUT);
  digitalWrite(mot, LOW);
  scale.begin(A2, A3);
  scale.set_scale(2280.f);
  scale.tare();
}

void loop() {
  distrib();
}

void distrib() {
  if (heures == HeurePredefini1 && minutes == 0
      || heures == HeurePredefini2 && minutes == 0
      || heures == HeurePredefini3 && minutes == 0
      || motOn == true) {
    valCaptGamelle = scale.get_units(10), 1;
    if (portion == true || motOn == true) {
      lcd.clear();
      lcd.print("Distribution");
      while (valCaptGamelle < valDistribBT) {
        digitalWrite(mot, HIGH);
        valCaptGamelle = scale.get_units(10), 1;
      }
      digitalWrite(mot, LOW);//ajouté
    }
    else {
      lcd.clear();
      lcd.print("pas de portions");
      delay(2000);
    }
    lcd.clear();
  }
  motOn = false;
}
