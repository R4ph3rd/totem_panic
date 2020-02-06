#include "notes.h"

#define hp 11

// Note duration is based on 1 sec because tempo is intially 60
// So a "blanche" is played twice by second
// But tempo is accelerated as things progress
 
#define blanche 2
#define noire 4
#define croche 8
#define dbcroche 16
#define triolet 12

//uint16_t ritualDuration = 120000 ;// 2min de jeu
uint8_t tempo = 1000 ;

uint16_t melody[] = { MI4,  SI4, MI4, DO4, MI4, RE4, DO4, SOL4 };
int rythmMelody[] = { blanche, croche, croche, noire, croche, croche, triolet, triolet, triolet, noire, croche, croche };
double rythmTimestamp = 0 ;

uint16_t progressiveTempo(){
  return uint16_t(map(millis(), 0, ritualDuration, 60, 90));
}

void soulRitualRythm() {
 
  uint8_t phrase = random(8, 12);
  //Serial.println(phrase);

  for (uint8_t note = 0; note < phrase ; note++) {
    
    int rythm = progressiveTempo() / rythmMelody[note];
    tone(hp, melody[note], rythm);

    int pause_entre_notes = rythm * 1.30;
    delay(pause_entre_notes);
    
    noTone(hp);
  }
}

void soulIsInParadize(){
  //Serial.println("soul is in paradise !");
}

boolean isRitualEnded(uint32_t startGame, uint16_t ritualDuration) {
  return millis() > startGame + ritualDuration ;
}
