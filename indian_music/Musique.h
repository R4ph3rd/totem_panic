//BIBLIOTHEQUES

#include "notes.h"



// BROCHES

#define Piezo 11

#define blanche 2
#define noire 4
#define croche 8
#define dbcroche 16
#define triolet 12


// VARIABLES




// MELODIE

// Nombre de notes qui composent la mélodie
long nombreNotesMelodie;

// Définir les notes de la mélodie
int melodie[] = {
  NOTE_MI4, NOTE_SI4, NOTE_MI4, NOTE_DO4, NOTE_MI4, NOTE_RE4, NOTE_DO4, NOTE_SOL4
};

// Définir la durée de chaque note
int dureeNoteMelodie[] = {
  blanche, croche, croche, noire, croche, croche, triolet, triolet, triolet, noire, croche, croche
};



// JOUER LA MUSIQUE

void JouerMelodie() {
 
  nombreNotesMelodie = random(8, 12);
  Serial.println(nombreNotesMelodie);

  // Jouer les notes l'une après l'autre
  for (int note_a_jouer = 0; note_a_jouer < nombreNotesMelodie; note_a_jouer++) {

    // La durée de chaque note est calculée en divisant 1 seconde par le type de note
    // ex : une noire = 1000 millisecondes / 4, une croche = 1000 / 8, etc.

    int rythm = 1000 / dureeNoteMelodie[note_a_jouer];
    tone(Piezo, melodie[note_a_jouer], rythm);

    // Pour distinguer les notes, on laisse une petite pause entre chaque
    //  qui équivaut à la durée de la note + 30%

    int pause_entre_notes = rythm * 1.30;
    delay(pause_entre_notes);

    // Arrêter la note qui joue
    noTone(Piezo);
  }
}



// BANG

// Nombre de notes qui composent la mélodie
int nombreNotesBang = 5;

// Définir les notes de la mélodie
int bang[] = {
  NOTE_FA7, NOTE_MI7, NOTE_RE7, NOTE_DO7, NOTE_SOL4
};

// Définir la durée de chaque note
int dureeNoteBang[] = {
  16, 16, 16, 16, 1
};


void TirerBang() {

  // Jouer les notes l'une après l'autre
  for (int note_a_jouer = 0; note_a_jouer < nombreNotesBang; note_a_jouer++) {

    // La durée de chaque note est calculée en divisant 1 seconde par le type de note
    // ex : une noire = 1000 millisecondes / 4, une croche = 1000 / 8, etc.

    int duree_note_a_jouer = 200 / dureeNoteBang[note_a_jouer];
    tone(Piezo, bang[note_a_jouer], duree_note_a_jouer);

    // Pour distinguer les notes, on laisse une petite pause entre chaque
    //  qui équivaut à la durée de la note + 30%

    int pause_entre_notes = duree_note_a_jouer * 1.30;
    delay(pause_entre_notes);

    // Arrêter la note qui joue
    noTone(Piezo);
  }
}
