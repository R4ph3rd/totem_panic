#define micro 2
#define power 10
#define ledOUT 8
#define hp 11

#define totem1PIN 3
#define totem2PIN 5
#define totem3PIN 6

#include <Servo.h> 
#include "DFRobot_VL53L0X.h"
#include "Wire.h"

int charge = 10 ;
uint8_t threshold = 500 ;

#include "Ultrasonic.h" // importer la bibliothèque
Ultrasonic ultrasonic(2); // la broche "sig" du capteur est branchée sur "D2" de l'arduino

Servo t1 ;
Servo t2 ;
Servo t3 ;

DFRobotVL53L0X sensor;

double tm_storage = 0 ;
uint16_t tm_timestamp = 2000 ;
boolean totemRotation = true ;
uint8_t tm_angle = 55 ;

void setup() {
  Serial.begin(115200);
  pinMode(power, OUTPUT);
  pinMode(micro, INPUT);
  pinMode(hp, OUTPUT);
  pinMode(totem1PIN, OUTPUT);
  pinMode(totem2PIN, OUTPUT);
  pinMode(totem3PIN, OUTPUT);
  
  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(Continuous,High);
  sensor.start();
  

  t1.attach(totem1PIN);
  t2.attach(totem2PIN);
  t3.attach(totem3PIN);

  t1.write(0); // bas
  t2.write(0); // centre
  t3.write(0); // haut
}

void loop() {
  int range = ultrasonic.MeasureInCentimeters(); // mesurer la distance en cm
  //Serial.print("range : ");
  //Serial.println(range);//0~400cm
  delay(100); // attendre un peu entre deux mesures.

  if ( range > 150){
    analogWrite(power,0);
  } else {

    uint16_t sigma = int(map(range, 0, 40, 110, 255));
    //Serial.println("sigma : " );
    //Serial.println(sigma);
    analogWrite(11, sigma);
    
  }


 if ( isTimeToAct(tm_storage, tm_timestamp) ){
        if (totemRotation){
          moveTotemMotors_right();
        } else {
          moveTotemMotors_left();
        }
      } 

  Serial.print("Distance: ");Serial.println(sensor.getDistance());
}

void moveTotemMotors_right(){
  tm_storage = millis(); 
  t1.write(10);
  t2.write(10);
  t3.write(10);
  
  Serial.println("totem motors moved to right");
  totemRotation = !totemRotation ;
}

void moveTotemMotors_left(){
  tm_storage = millis(); 
  t1.write(tm_angle);
  t2.write(tm_angle);
  t3.write(tm_angle);

  Serial.println("totem motors moved to left");
  totemRotation = !totemRotation ;
}


boolean isTimeToAct(double timeStorage, uint16_t timestamp){
    return millis() >= timeStorage + timestamp;
}
