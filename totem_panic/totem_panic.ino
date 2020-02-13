#include "Ultrasonic.h" // importer la bibliothèque

#include <SoftwareSerial.h>
#include <MP3Player_KT403A.h>

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

SoftwareSerial mp3(12, 13);
Ultrasonic ultrasonic(2); // la broche "sig" du capteur est branchée sur "D2" de l'arduino

DFRobotVL53L0X sensor;

Servo t1 ;
Servo t2 ;
Servo t3 ;
Servo indianScore ;

uint16_t ritualDuration = 120000 ;// 2min de jeu
uint32_t startGame = 0 ;
long inactive = 0 ;
uint8_t inactiveDuration = 1000 ;
boolean looseToConfirm = true ;
long looseTimeToConfirm = 0 ;

// uint8_t score = 0 ;
boolean isGaming = false ;

// totem motors = tm
long tm_storage = 0 ;
uint16_t rotation_timestamp = 1000 ;
long delayMotors = 0 ;
boolean rotationDelay = false ;
uint16_t tm_timestamp = 2000 ;
boolean totemRotation = true ;
uint8_t tm_angle = 50 ;

uint8_t maxUltrason = 43 ; // don't know why this value change sometimes between ~ 40 & 150



void setup() {
  Serial.begin(115200);
  pinMode(airPowerPIN, OUTPUT);
  pinMode(micro, INPUT);
  pinMode(hp, OUTPUT);
  pinMode(totem1PIN, OUPUT);
  pinMode(totem2PIN, OUPUT);
  pinMode(totem3PIN, OUPUT);
  pinMode(indianScorePIN, OUPUT);
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

  mp3.begin(115200);
  SelectPlayerDevice(0x02);       // Select SD card as the player device.
  SetVolume(0x1E);   // Set the volume, the range is 0x00 to 0x1E.
  
}


void loop() {

  if ( isGaming ) {
    shufflePlay();
    
    if ( (sensor.getDistance() > 299) && (millis() > startGame + 3000)) { // soul ball out : wait 3 sec after the game started to avoid wrong manipulations at the beginning
      Serial.println("ball out ");

      // to avoid sensor fluctuations : be sure that ball is out by expecting 2 sec
      if (!looseToConfirm) { // set timestamp which will confirm the loose
        looseTimeToConfirm = millis() ;
        looseToConfirm = !looseToConfirm ;
      }

      if (millis() > looseTimeToConfirm + 100 ) { // is loose is recognized, so player is a looser ( bouhouhou )
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
        uint8_t sigma = int(map(range, 0, maxUltrason, 110, 255));
        // Serial.print("sigma : ");
        // Serial.println(sigma);
        analogWrite(airPowerPIN, sigma);
      }


      ////////// TOTEM MOTORS ROTATION ///////////////
      if (millis() > startGame + 3000) {
        if ( isTimeToAct(tm_storage, tm_timestamp) ) {
          
          if (!rotationDelay){ // set base timestamp to create delay between motors
            delayMotors = millis();
            rotationDelay = !rotationDelay ;   
          }
          
          if (totemRotation) {
            moveTotemMotors_left();
          } else {
            moveTotemMotors_right();
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
  analogWrite(airPowerPIN, 0);
  moveTotemMotors_right();
  indianScore.write(180);

  Serial.println("You are such a looser ... be shamed");
  
  // wait for player remove his hand
  while (handInRange()){
    Serial.println("please remove your hand, looser");
  }
  
  isGaming = false ;
  totemRotation = true ;
  looseToConfirm = false ;
  rotationDelay = false ;
  Serial.println("game reset");
}

void wonGame() {
  analogWrite(airPowerPIN, 0);
  moveTotemMotors_right();
  indianScore.write(180);

  Serial.println("GAME WON !! ");
  Serial.println(" you are now the Great Black Hawk, the greatest Native Shaman on Earth");

  // wait for player remove his hand
  while (handInRange()){
   Serial.println("please remove your hand, oh Great Black Hawk");
  }
  
  isGaming = false ;
  totemRotation = true ;
  rotationDelay = false ;
  Serial.println("game reset");

}


void moveTotemMotors_right() {
  if (isTimeToAct(delayMotors, 0)) {
    t1.write(0);
    Serial.println("totem 1 motor moved to right");
  }

  if (isTimeToAct(delayMotors, 1000) ) {
    t2.write(0);
    Serial.println("totem 2 motor moved to right");
  }

  if (isTimeToAct(delayMotors, 2000)) {
    t3.write(0);
    Serial.println("totem 3 motor moved to right");
    
    totemRotation = !totemRotation ;
    rotationDelay = !rotationDelay ;
    tm_storage = millis();
  }
}

void moveTotemMotors_left() {
  if (isTimeToAct(delayMotors, 0)) {
    t1.write(tm_angle);
    Serial.println("totem 1 motor moved to left");
  }

  if (isTimeToAct(delayMotors, 1000) ) {
    t2.write(tm_angle);
    Serial.println("totem 2 motor moved to left");
  }

  if (isTimeToAct(delayMotors, 2000)) {
    t3.write(tm_angle);
    Serial.println("totem 3 motor moved to left");
    
    totemRotation = !totemRotation ;
    rotationDelay = !rotationDelay ;
    tm_storage = millis();
  }
}


boolean isTimeToAct(long timeStorage, uint16_t timestamp) {
  return (millis() > int(timeStorage + timestamp));
}

boolean isGameWon(uint8_t score) {
  return score == 0 ;
}

boolean handInRange(){
  int8_t range = ultrasonic.MeasureInCentimeters();

  return range < maxUltrason - 5 ;
}
