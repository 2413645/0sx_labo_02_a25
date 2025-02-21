const int potPin = A1;                   // Broche du potentiomètre
const int buttonPin = 2;                 // Broche du bouton poussoir
const int ledPins[] = { 8, 9, 10, 11 };  // Broches des 4 DELs

unsigned long previousMillis = 0;
const long interval = 100;  // Intervalle d'actualisation en millisecondes



int estClic(unsigned long ct) {
  static unsigned long lastTime = 0;
  static int lastState = HIGH;
  const int rate = 500;
  int clic = 0;

  if (ct - lastTime < rate) {
    return clic; // Trop rapide
  }

  lastTime = ct;

  int state = digitalRead(buttonPin);

  if (state == LOW) {
    if (lastState == !buttonPin) {
      clic = 1;
    }
  }

  lastState = state;

  return clic;
}




void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);  // Activer la résistance pull-up interne du bouton
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);  // Configurer les LEDs en sortie
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int potValue = analogRead(potPin);
    int mappedValue = map(potValue, 0, 1023, 0, 20);  // Convertir en échelle de 0 à 20
    int percentage = map(potValue, 0, 1023, 0, 100);  // Convertir en pourcentage (0-100%)
    int buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) {  // Si le bouton est pressé
      afficherBarreProgression(mappedValue, percentage);
      return 0;
    }

    allumerDEL(percentage);
  }
}

// Fonction pour afficher la barre de progression
void afficherBarreProgression(int mappedValue, int percentage) {
  unsigned long tempMillis = millis();
  
  if (estClic(tempMillis)) {
    Serial.print("[");
    for (int i = 0; i < 20; i++) {
      if (i < mappedValue) {
        Serial.print("#");  // Afficher '#' pour une barre paire
      } else {
        Serial.print(".");
      }
    }

    Serial.print("]");
    Serial.print(percentage);
    Serial.println("%");
  }
}


// Fonction pour gérer l'allumage des DELs
void allumerDEL(int percentage) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);  // Éteindre toutes les LEDs
  }

  if (percentage < 25) {
    digitalWrite(ledPins[0], HIGH);  // Allumer la DEL 1
  } else if (percentage < 50) {
    digitalWrite(ledPins[1], HIGH);  // Allumer la DEL 2
  } else if (percentage < 75) {
    digitalWrite(ledPins[2], HIGH);  // Allumer la DEL 3
  } else {
    digitalWrite(ledPins[3], HIGH);  // Allumer la DEL 4
  }
}
