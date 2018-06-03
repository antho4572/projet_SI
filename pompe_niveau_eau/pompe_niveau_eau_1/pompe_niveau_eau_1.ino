
int pompe = 3,//pompe sur le port D3
    niveauo = 1,// capteur de niveau sur le port A1
    valeurniv = 0, // valeur du capteur de niveau eau
    niveauEau = 200; //niveau d'eau limite

void setup() {
  pinMode (pompe, OUTPUT);
  //Définition du port arduino comme sortie
  digitalWrite(pompe, LOW);//la pompe est désactivée
}

void loop() {
  valeurniv = analogRead(niveauo);
  //lecture de la valeur du capteur de niveau
  if (valeurniv > niveauEau)  {
    Gamelle();
  }
}

void Gamelle() {
  valeurniv = analogRead(niveauo);
  //lecture de la valeur du capteur de niveau
  while (valeurniv > niveauEau) {
    //tant que la valeur souhaitée n'est pas atteinte
    valeurniv = analogRead(niveauo);
    //lecture de la valeur du capteur de niveau
    digitalWrite(pompe, HIGH);//on actionne la pompe
  }
  digitalWrite(pompe, LOW);//on éteint la pompe
  if (niveauEau == 500) {
    niveauEau = 150;
  }
  else {
    niveauEau += 10;
  }
}

