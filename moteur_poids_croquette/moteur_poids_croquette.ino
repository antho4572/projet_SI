#include "HX711.h"
//librairie du capteur de poids
HX711 scale;//capteur de poids
float valCaptGamelle, valDistribBT;
//définition nombre flottant valeur du poids gamelle et
//valeur définit par l'utilisateur
int mot = 2; // moteur est sur la pin D2
int heures, minutes;
//définition heures et secondes du RTC comme entier
int HeurePredefini1, HeurePredefini2,
    HeurePredefini3;
//définition comme entier des variables heures pour les intervalles
boolean motOn = false, portion = false;
//défintion variables activation des heures comme booléen

void setup() {
  pinMode(mot, OUTPUT);
  //Défintion du port arduino comme sortie
  digitalWrite(mot, LOW);//moteur désactivé
  scale.begin(A2, A3);//Démarrage du capteur poids
  scale.set_scale(2280.f);
  //définition coeficient pour capteur du poids
  scale.tare();
  //remise à 0 valeur du capteur poids
}

void loop() {
  distrib();
  //on lance le sous-prog "distrib"
}

void distrib () {
  if (heures == HeurePredefini1 && minutes == 0
      || heures == HeurePredefini2 && minutes == 0
      || heures == HeurePredefini3 && minutes == 0
      || motOn == true) {
    //si les heures définies sont égales au temps présent ou
    //si l'utilisateur à choisis de faire une distribution
    valCaptGamelle = scale.get_units(10), 1;
    //lecture de la valeur du capteur de poids
    if (portion == true || motOn == true) {
      //si la portion est définie ou l'utilisateur décide
      //de distribuer une portion
      while (valCaptGamelle < valDistribBT) {
        //tant que la valeur souhaitée n'est pas atteinte
        valCaptGamelle = scale.get_units(10), 1;
        //lecture de la valeur du capteur de poids
        digitalWrite(mot, HIGH);
        //on actionne le moteur
      }
      digitalWrite(mot, LOW);//on éteint le moteur
    }
  }
  motOn = false;
}

