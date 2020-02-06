#include "Ultrasonic.h" // importer la bibliothèque
Ultrasonic ultrasonic(2); // la broche "sig" du capteur est branchée sur "D2" de l'arduino

#include <Servo.h>

#include "DFRobot_VL53L0X.h"
#include "Wire.h"

//#include "indianmusic.h"

#define micro 2
#define airPowerPIN 11
#define hp 11
#define totem1PIN 3
#define totem2PIN 5
#define totem3PIN 6
#define indianScorePIN 9

Servo t1 ;
Servo t2 ;
Servo t3 ;
Servo indianScore ;
Servo totems[3] ;

uint16_t ritualDuration = 120000 ;// 2min de jeu
uint32_t startGame = 0 ;
double inactive = 0 ;
uint8_t inactiveDuration = 3000 ;
boolean looseToConfirm = false ;
double looseTimeToConfirm = 0 ;

// uint8_t score = 0 ;
boolean isGaming = false ;

// totem motors = tm
double tm_storage = 0 ;
double delayMotors = 0 ;
uint16_t tm_timestamp = 2000 ;
boolean totemRotation = true ;
uint8_t tm_angle = 50 ;

DFRobotVL53L0X sensor;

void setup() {
  Serial.begin(115200);
  pinMode(airPowerPIN, OUTPUT);
  pinMode(micro, INPUT);
  pinMode(hp, OUTPUT);
  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(Continuous, High);
  sensor.start();

  t1.attach(totem1PIN);
  t2.attach(totem2PIN);
  t3.attach(totem3PIN);
  indianScore.attach(indianScorePIN);

  t1.write(0);
  t2.write(0);
  t3.write(0);
  indianScore.write(180);
}


void loop() {

  if ( isGaming ) {
    Serial.println(sensor.getDistance());
    if ( (sensor.getDistance() > 299) && (millis() > startGame + 10000)) { // soul ball out : wait 10 sec after the game started to avoid wrong manipulations at the beginning
      Serial.print("ball out : ");
      Serial.println(sensor.getDistance());

      // to avoid sensor fluctuations : be sure that ball is out by expecting 2 sec
      if (!looseToConfirm) { // set timestamp which will confirm the loose
        looseTimeToConfirm = millis() ;
        looseToConfirm = !looseToConfirm ;
      }

      if (millis() > looseTimeToConfirm + 2000 ) { // is loose is recognized, so player is a looser ( bouhouhou )
        looseGame();
      }

    } else { // soul ball in
      //Serial.print("ball in : ");
      //Serial.println(sensor.getDistance());

      ////////////// DISTANCE SENSOR ////////////
      int range = ultrasonic.MeasureInCentimeters();
      //Serial.print("range : ");
      //Serial.println(range);

      if ( range > 150) { // if no hand is recognized in the ultrasonic sensor, set to minimum height
        analogWrite(airPowerPIN, 110);

        Serial.println("no hand is detected. Come back quickly !");

        if ( abs(inactive - millis()) > inactiveDuration ) {
          inactive = millis() ;
          Serial.println("inactive set");
        }

        if (isTimeToAct(inactive, inactiveDuration)) { // if player became inactive, so loose game
          Serial.println("you have been inactive during too long, you lose");
          looseGame();
        }

      } else {   // normal behavior when a hand ( or object ) is in the range
        uint8_t sigma = int(map(range, 0, 40, 110, 255));
        // Serial.print("sigma : ");
        // Serial.println(sigma);
        analogWrite(airPowerPIN, sigma);
      }


      ////////// TOTEM MOTORS ROTATION ///////////////
      if (millis() > startGame + 3000) {
        if ( isTimeToAct(tm_storage, tm_timestamp) ) {
          if (totemRotation) {
            moveTotemMotors_right();
          } else {
            moveTotemMotors_left();
          }
        }
      }

      /////////// SCORE DISPLAY & CHECK SCORE /////////////
      uint8_t score = int(map(millis(), startGame, startGame + ritualDuration, 180, 0)) ;

      if ( isGameWon(score)) {
        wonGame();
      } else {
        indianScore.write(score);
        //Serial.print("score : ");
        //Serial.println(score);
        delay(100);
      }

    }
  }


  ///////// START GAME CHECK ////////////
  if (!isGaming) {
    uint8_t range = ultrasonic.MeasureInCentimeters();

    // Serial.print("range start check : ");
    // Serial.println(range);

    if (range > 2 && range < 20 ) {
      isGaming = !isGaming ;
      startGame = millis() ;
      // Serial.print("startgame : ");
      // Serial.println(startGame);
      Serial.println("game start");
    }
  }

}

void looseGame() {
  moveTotemMotors_right();
  //score = 0 ;
  indianScore.write(180);

  Serial.println("You are such a looser ... be shamed");

  analogWrite(airPowerPIN, 0);
  isGaming = false ;
  looseToConfirm = false ;
}

void wonGame() {
  moveTotemMotors_right();
  //score = 0 ;
  indianScore.write(180);

  analogWrite(airPowerPIN, 0);

  Serial.println("GAME WON !! ");

  soulIsInParadize();
  isGaming = false ;
}


void moveTotemMotors_right() {
  if (isTimeToAct(delayMotors, 0)) {
    t1.write(0);
    Serial.println("totem 1 motor moved to right");
  }

  if (isTimeToAct(delayMotors, 2000) ) {
    t2.write(0);
    Serial.println("totem 2 motor moved to right");
  }

  if (isTimeToAct(delayMotors, 3000)) {
    t3.write(0);
    Serial.println("totem 3 motor moved to right");
    totemRotation = !totemRotation ;
    tm_storage = millis();
  }
}

void moveTotemMotors_left() {
  if (isTimeToAct(delayMotors, 0)) {
    t1.write(tm_angle);
    Serial.println("totem 1 motor moved to left");
  }

  if (isTimeToAct(delayMotors, 2000) ) {
    t2.write(tm_angle);
    Serial.println("totem 2 motor moved to left");
  }

  if (isTimeToAct(delayMotors, 3000)) {
    t3.write(tm_angle);
    Serial.println("totem 3 motor moved to left");
    totemRotation = !totemRotation ;
    tm_storage = millis();
  }
}


boolean isTimeToAct(double timeStorage, uint16_t timestamp) {
  return millis() > timeStorage + timestamp;
}

boolean isGameWon(uint8_t score) {
  return score == 0 ;
}
