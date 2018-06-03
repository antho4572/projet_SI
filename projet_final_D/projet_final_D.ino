//PROJET OK 22-05-18_11h00
#include <RTClib.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "HX711.h"
RTC_DS1307 RTC;
rgb_lcd lcd;

HX711 scale;//capteur de poids
//definition des différentes variables utilisés
int  potar = 0, niveauo = 6, mot = 2, pompe = 3, haut = 4, bas = 8,
     menu = 1, batterie0 = 0, batterie25 = 1, batterie50 = 2,
     batterie75 = 3, batterie100 = 4, bluetooth = 5, valBatterie = 100,
     octetRecu = 0, i, minutes, heures,
     HeurePredefini1, HeurePredefini2, HeurePredefini3;
boolean BTT = false, liste = false, motOn = false,
        urgent = false, etatbphaut, etatbpbas, portion = false,
        etatNivo;
char caractereRecu = 0;
String chaineReception, typeChat;
float valCaptGamelle, valDistribBT;

void setup() {
  //definition des entrées et sorties
  pinMode(haut, INPUT), (bas, INPUT), (mot, OUTPUT),
          (pompe, OUTPUT), (niveauo, INPUT);
  digitalWrite(mot, LOW), (pompe, LOW);
  //pompe et moteur éteint
  lcd.begin(16, 2);//démarrage du lcd
  lcd.setRGB(0, 255, 0);//couleur verte de l'écran
  RTC.begin(); //démarrage du RTC (horloge
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  // envoie date et heure
  creation_caract();
  //Fonction pour créer les caractères Batterie et Bluetooth pour le LCD
  Serial.begin(9600);//demarrage de la liaison serie (bluetooth)
  lcd.setCursor(0, 0);
  scale.begin(A2, A3);//Démarrage du capteur poids
  scale.set_scale(2280.f);//définition coefficient  pour capteur du poids
  scale.tare();//remise à 0 valeur du capteur poids
  initialisation();
  //lancement du sous-prog "initialisation"
}

void initialisation() {
  lcd.print("Initialisation..");
  lcd.setCursor(0, 1);
  lcd.print("...de la pompe   ");
  etatNivo = digitalRead(niveauo);
  //lecture de la valeur du capteur de niveau
  if (etatNivo == HIGH) {
    while (etatNivo == HIGH) {
      //tant que la valeur souhaitée n'est pas atteinte
      etatNivo = digitalRead(niveauo);
      //lecture de la valeur du capteur de niveau
      digitalWrite(pompe, HIGH);//on actionne la pompe
    }
  }
  digitalWrite(pompe, LOW);//on éteint la pompe
  lcd.clear();
}


void creation_caract() {
  //création des caractères avec les "pixels" du LCD
  byte batt0[8] = { //batterie niveau 0%
    B00000,
    B01110,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
  };
  byte batt25[8] = {//batterie niveau 25%
    B00000,
    B01110,
    B11111,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
  };
  byte batt50[8] = {//batterie niveau 50%
    B00000,
    B01110,
    B11111,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
  };
  byte batt75[8] = {//batterie niveau 75%
    B00000,
    B01110,
    B11111,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte batt100[8] = {//batterie niveau 100%
    B00000,
    B01110,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte bluetoothS[8] = {//logo Bluetooth
    B00100,
    B10110,
    B01101,
    B00110,
    B01110,
    B01101,
    B10110,
    B00100,
  };
  lcd.createChar(batterie0, batt0);
  //création logo batterie niveau 0%
  lcd.createChar(batterie25, batt25);
  //création logo batterie niveau 25%
  lcd.createChar(batterie50, batt50);
  //création logo batterie niveau 50%
  lcd.createChar(batterie75, batt75);
  //création logo batterie niveau 75%
  lcd.createChar(batterie100, batt100);
  //création logo batterie niveau 100%
  lcd.createChar(bluetooth, bluetoothS);
  //création logo Bluetooth
}

void date_heure() {
  DateTime now = RTC.now();
  //définition du nom "now" comme
  //étant le moment du RTC
  lcd.setCursor(2, 0);
  heures = int(now.hour());
  //stockage de l'heure dans la variable
  minutes = int(now.minute());
  //stockage des minutes dans la variable
  if (now.day() < 10)
  {
    lcd.print("0");
  }
  lcd.print(now.day());
  lcd.print("/");
  if (now.month() < 10)
  {
    lcd.print("0");
  }
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());
  lcd.setCursor(4, 1);
  if (now.hour() < 10)
  {
    lcd.print("0");
  }
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10)
  {
    lcd.print("0");
  }
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10)
  {
    lcd.print("0");
  }
  lcd.print(now.second());
}

void batterieLogo() {
  lcd.setCursor(14, 1);
  if (valBatterie == 100) {
    //si valeur du potentiomètre est égale à 100
    lcd.write(batterie100); //écrire logo 100% sur le lcd
  }
  else if (valBatterie < 99 && valBatterie > 75) {
    //si valeur du potentiomètre est comprise entre 99 et 75
    lcd.write(batterie75);//écrire logo 75% sur le lcd
  }
  else if (valBatterie < 75 && valBatterie > 50) {
    //si valeur du potentiomètre est comprise entre 75 et 50
    lcd.write(batterie50);//écrire logo 50% sur le lcd
  }
  else if (valBatterie < 50 && valBatterie > 25) {
    //si valeur du potentiomètre est comprise entre 50 et 25
    lcd.write(batterie25);//écrire logo 25% sur le lcd
  }
  else if (valBatterie < 25 && valBatterie > 10) {
    //si valeur du potentiomètre est comprise entre 25 et 0
    lcd.write(batterie0);//écrire logo 0% sur le lcd
  }
  else if (valBatterie < 10) {
    //si valeur du potentiomètre est égale à 100
    lcd.write(batterie0);//écrire logo 0% sur le lcd
  }
}

void Gamelle() {
  etatNivo = digitalRead(niveauo);
  //lecture de la valeur du capteur de niveau
  lcd.clear();
  lcd.print("Rajustement eau");
  lcd.setCursor(0, 1);
  delay(100);
  while (etatNivo == HIGH) {
    //tant que la valeur n'est pas égale à 0
    etatNivo = digitalRead(niveauo);
    //lecture de la valeur du capteur de niveau
    lcd.setCursor(0, 1);
    digitalWrite(pompe, HIGH);//on actionne la pompe
  }
  delay(200);
  digitalWrite(pompe, LOW);//on éteint la pompe
  lcd.clear();
}

void bluetoothConnection() {
  //fonction pour la liaison Bluetooth
  i = 0;
  while (Serial.available() > 0) {
    //si il y a des valeurs a lire
    octetRecu = Serial.read();
    i += 1;
    caractereRecu = char (octetRecu);
    chaineReception += caractereRecu;
    //ces valeurs s'ajoute pour former une chaine de caractère
  }
  //on test si la chaine de caractère est égale aux différents mots
  if (chaineReception == "BTON") {
    BTT = true;
  }
  else if (chaineReception == "BTOFF") {
    BTT = false;
  }
  else if (chaineReception == "MOTON") {
    motOn = true;
  }
  else if (chaineReception == "Liste 1") {
    liste = true;
    HeurePredefini1 = 6;
    HeurePredefini2 = 13;
    HeurePredefini3 = 18;
  }
  else if (chaineReception == "Liste 2") {
    liste = true;
    HeurePredefini1 = 8;
    HeurePredefini2 = 15;
    HeurePredefini3 = 20;
  }
  else if (chaineReception == "Liste 3") {
    liste = true;
    HeurePredefini1 = 10;
    HeurePredefini2 = 17;
    HeurePredefini3 = 22;
  }
  else if (chaineReception == "Liste 4") {
    liste = true;
    HeurePredefini1 = 12;
    HeurePredefini2 = 19;
    HeurePredefini3 = 00;
  }
  else if (chaineReception == "Chat 1") {
    portion = true;
    typeChat = "Chat d'int 2kg";
    valDistribBT = 3.3;
  }
  else if (chaineReception == "Chat 2") {
    valDistribBT = 5.0;
    typeChat = "Chat d'int 3kg";
    portion = true;
  }
  else if (chaineReception == "Chat 3") {
    typeChat = "Chat d'int 4kg";
    valDistribBT = 6.6;
    portion = true;
  }
  else if (chaineReception == "Chat 4") {
    valDistribBT = 3.6;
    typeChat = "Chat actif 2kg";
    portion = true;
  }
  else if (chaineReception == "Chat 5") {
    valDistribBT = 5.5;
    typeChat = "Chat actif 3kg";
    portion = true;
  }
  else if (chaineReception == "Chat 6") {
    valDistribBT = 3.6;
    typeChat = "Chat actif 4kg";
    portion = true;
  }
  else if (chaineReception == "Chat 7") {
    valDistribBT = 2.6;
    typeChat = "Chat sterel 2kg";
    portion = true;
  }
  else if (chaineReception == "Chat 8") {
    valDistribBT = 4.0;
    typeChat = "Chat sterel 3kg";
    portion = true;
  }
  else if (chaineReception == "Chat 9") {
    valDistribBT = 5.2;
    typeChat = "Chat sterel 4kg";
    portion = true;
  }
  chaineReception = "";//on efface la chaine de réception
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
    if (portion == true) {
      lcd.clear();
      lcd.print("Distribution");
      while (valCaptGamelle < valDistribBT) {
        //tant que la valeur souhaitée n'est pas atteinte
        digitalWrite(mot, HIGH);
        //on actionne le moteur
        valCaptGamelle = scale.get_units(10), 1;
        //lecture de la valeur du capteur de poids
      }
      digitalWrite(mot, LOW);//on éteint le moteur
    }
    else {
      lcd.clear();
      lcd.print("pas de portions");
      delay(5000);
    }
    lcd.clear();
  }
  motOn = false;
}

void Batterie() {
  valBatterie = analogRead (potar);
  //on lit la valeur de la batterie du
  //potentiomètre
  if (valBatterie < 1000) {
    valBatterie /=  10;
    //on la divise par 10 pour l'avoir comprise
    //entre 0 et 100
  }
  else if (valBatterie > 1000) {
    valBatterie = 100;
  }
  if (valBatterie < 10) {
    //s'il y a presque plus de batterie, urgent
    //est vrai
    urgent = true;
  }
  else if (valBatterie > 10) {
    //si il y a assez de batterie, urgent se
    //désactive
    urgent = false;
  }
}

void menuLCD() {
  if (urgent == true) { //si etat d'urgence est activé
    lcd.setRGB(255, 0, 0); //couleur du LCD est rouge
  }
  else if (valBatterie == 100) {
    lcd.setRGB(0, 255, 0);
  }
  else {//si etat d'urgence est desactivé
    lcd.setRGB(255, 255, 255);//couleur du LCD est bleu
  }
  etatbphaut = digitalRead(haut), etatbpbas = digitalRead(bas);
  //enregistrement des états des boutons (appuyé/relâché)
  if (etatbpbas == HIGH && menu < 5) {
    // si le bouton haut est activé et la variable menu est
    //plus petite que 6 (dernier menu)
    menu = menu + 1; //
  }
  else if (etatbphaut == HIGH && menu > 1) {
    // si le bouton bas est activé et la variable menu est
    //plus grande que 1 (premier menu)
    menu = menu - 1;
  }
  switch (menu) {
    case 1 :
      //écriture de l'heure sur l'écran 1 + logo
      //Bluetooth s'il est connecté + logo batterie
      lcd.clear();
      date_heure();
      if (BTT == true) {
        lcd.setCursor(14, 0);
        lcd.write(bluetooth);
      }
      batterieLogo();
      break;
    case 2://écriture du niveau de la batterie en %
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Niveau de");
      lcd.setCursor(0, 1);
      lcd.print("batterie : ");
      lcd.print(valBatterie);
      lcd.print("%");
      break;
    case 3 :// écriture du poids de la gamelle
      lcd.clear();
      lcd.setCursor(0, 0);
      if (valCaptGamelle < 10) {
        lcd.print("poids gamelle  ");
      }
      else if (valCaptGamelle < 100) {
        lcd.print("poids gamelle ");
      }
      else {
        lcd.print("poids gamelle");
      }
      lcd.print(valCaptGamelle);
      lcd.setCursor(0, 1);
      lcd.print("g de croquettes");
      break;
    case 4 :
      // écriture des heures de distribution fixées par l'utilisateur
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("heures distrib:");
      if (liste == true) {
        lcd.setCursor(0, 1);
        lcd.print(HeurePredefini1);
        lcd.print("h - ");
        lcd.print(HeurePredefini2);
        lcd.print("h - ");
        lcd.print(HeurePredefini3);
        lcd.print("h");
      }
      else if (liste == false) {
        lcd.setCursor(0, 1);
        lcd.print("pas definies");
      }
      break;
    case 5 :
      //écriture du type de chat défini par l'utilisateur
      lcd.clear();
      lcd.setCursor(0, 0);
      if (portion == true) {
        lcd.print(typeChat);
        lcd.setCursor(0, 1);
        lcd.print("3 port. de ");
        lcd.print(valDistribBT);
        lcd.print("g");
      }
      else if (portion == false) {
        lcd.print("type de chat");
        lcd.setCursor(0, 1);
        lcd.print("pas definies");
      }
      break;
  }
}

void loop() {
  if (Serial.available() > 0) {
    //si des données sont reçu
    //lancer le sous-programme "bluetoothConnection"
    bluetoothConnection();
  }
  else {
    valCaptGamelle = scale.get_units(10), 1;
    //lecture de la valeur du capteur de poids
    valCaptGamelle = abs(valCaptGamelle);
    //mise en valeur absolu de la valeur du capteur poids
    Serial.print(valCaptGamelle, 0);
    //envoie du poids de la gamelle pour l'afficher
    //sur l'application
    menuLCD();
    Batterie();
    distrib();
    //lancement des différents sous-prog
    delay(50);
    etatNivo = digitalRead(niveauo);
    //lecture de la valeur du capteur de niveau
    if (etatNivo == HIGH)  {
      lcd.clear();
      Gamelle();
    }
  }
}
