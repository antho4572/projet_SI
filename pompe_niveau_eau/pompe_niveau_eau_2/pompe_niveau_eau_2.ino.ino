
int pompe = 3,//pompe sur le port D3
    niveauo = 6;// capteur de niveau sur le port D6
boolean etatNivo; // valeur du capteur de niveau eau 
                  // soit 0 soit 1

void setup() {
  pinMode(niveauo, INPUT), (pompe, OUTPUT);
  //Définition des ports Arduino
  digitalWrite(pompe, LOW);//la pompe est désactivée
}

void loop() {
  etatNivo = digitalRead(niveauo);
  //lecture de la valeur du capteur de niveau
  if (etatNivo == HIGH)  {
    Gamelle();
  }
}

void Gamelle() {
  etatNivo = digitalRead(niveauo);
  //lecture de la valeur du capteur de niveau
  while (etatNivo == HIGH) {
    //tant que la valeur n'est pas égale à 0
    etatNivo = digitalRead(niveauo);
    //lecture de la valeur du capteur de niveau
    digitalWrite(pompe, HIGH);//on actionne la pompe
  }
  digitalWrite(pompe, LOW);//on éteint la pompe
}

