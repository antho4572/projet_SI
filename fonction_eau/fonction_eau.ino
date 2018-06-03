#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
int pompe = 6;
int niveauo = 1;
int valeurniv = 0;
int niveauEau = 150;

void setup() {
  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255);
}
void loop() {
  valeurniv = analogRead(niveauo);
  if (valeurniv > niveauEau)  {
    Gamelle();
  }
}
void Gamelle() {
  lcd.clear();
  lcd.print("Rajustement eau");
  valeurniv = analogRead(niveauo);
  delay(100);
  while (valeurniv > niveauEau) {
    valeurniv = analogRead(niveauo);
    digitalWrite(pompe, HIGH);
    delay(50);
  }
  digitalWrite(pompe, LOW);
  lcd.clear();

  if (niveauEau == 500) {
    niveauEau = 150;
  }
  else {
    niveauEau += 10;
  }
}
