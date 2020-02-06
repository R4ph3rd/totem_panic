//BIBLIOTHEQUES
#ifdef __AVR__
  #include <avr/power.h>
#endif


#include <Servo.h>

#include "Musique.h"



//BR

#define boutonRouge 2
#define boutonBleu 3

#define ServoBleu 8
#define ServoRouge 9
Servo servoB;
Servo servoR;

#define rubanLed 6
#define nombrePixel 5

// VARIABLES

boolean mancheFinie = false;
boolean manche = false;
boolean victoireDuel = false;

int posServoInitiale = 0;
int posServoMort = 90;

int numLedR; // Numéro de la led dans l'avancement des points du joueur rouge
int numLedB; // Numéro de la led dans l'avancement des points du joueur bleu
//int lumR = 30; // Lumière joueur rouge
//int lumB = 30; // Lumière joueur bleu

int ledVictoire; // Animation des leds lors d'une victoire
float a;



// SETUP

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));


  servoB.attach(ServoBleu);
  servoR.attach(ServoRouge);

  servoR.write(posServoInitiale);
  servoB.write(posServoInitiale);

  pinMode(Piezo, OUTPUT);
JouerMelodie();
}



// LOOP

void loop() {
JouerMelodie();
}



void PresseBoutonReset() {

  // On lance le jeu

  // On lit la musique qui a une durée random
  JouerMelodie();

  // On lance la première manche
  manche = true;

}
