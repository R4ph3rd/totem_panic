
byte vol = 0;


/* Constantes pour la communication avec le module KT403A */
const byte COMMAND_BYTE_START = 0x7E;
const byte COMMAND_BYTE_VERSION = 0xFF;
const byte COMMAND_BYTE_STOP = 0xEF;

/* Constantes pour la commande setEqualizer() */
const byte EQUALIZER_NORMAL = 0x00;
const byte EQUALIZER_POP = 0x01;
const byte EQUALIZER_ROCK = 0x02;
const byte EQUALIZER_JAZZ = 0x03;
const byte EQUALIZER_CLASSIC = 0x04;
const byte EQUALIZER_BASS = 0x05;

/* Constantes pour la commande selectSourceDevice() */
const byte DEVICE_UDISK = 0x01;
const byte DEVICE_SDCARD = 0x02;
const byte DEVICE_AUX = 0x03; // Not used
const byte DEVICE_PC = 0x04; // Debug only
const byte DEVICE_FLASH = 0x05;
const byte DEVICE_SLEEP = 0x06;


/** Play the next song (all folders) WARNING: files are ordered by creation date! */
void playNextSong() {
  send_kt_command(0x01, 0, 0, 10);
}

/** Play the previous song (all folders) WARNING: files are ordered by creation date! */
void playPreviousSong() {
  send_kt_command(0x02, 0, 0, 10);
}

/** Play the given track number (0 ~ 2999) (all folders) WARNING: files are ordered by creation date! */
void playTrack(uint16_t track_number) {
  send_kt_command(0x03, highByte(track_number), lowByte(track_number), 10);
}

/** Increase the volume */
void volumeUp() {
  send_kt_command(0x04, 0, 0, 10);
}

/** Decrease the volume */
void volumeDown() {
  send_kt_command(0x05, 0, 0, 10);
}

/** Set the volume to the given level (0 ~ 30) */
void setVolume(byte volume) {
  if (volume > 30) volume = 30;
  send_kt_command(0x06, 0, volume, 10);
}

/** Set the equalizer to the given audio mode */
void setEqualizer(byte mode) {
  send_kt_command(0x07, 0, mode, 20);
}

/** Repeat a single track (0 ~ 2999) (all folders) WARNING: files are ordered by creation date! */
void repeatSingleTrack(uint16_t track_number) {
  send_kt_command(0x08, highByte(track_number), lowByte(track_number), 10);
}

/** Select the source device for playing */
void selectSourceDevice(byte device) {
  send_kt_command(0x09, 0, device, 200);
}

/** Enter standby / low power mode */
void enterStandbyMode() {
  send_kt_command(0x0A, 0, 0, 20);
}

/** Exit standby / low power mode */
void exitStandbyMode() {
  send_kt_command(0x0B, 0, 0, 20);
}

/** Reset the MP3 module */
void resetPlayer() {
  send_kt_command(0x0C, 0, 0, 100);
}

/** Play/resume the music */
void playResumeMusic() {
  send_kt_command(0x0D, 0, 0, 20);
}

/** Pause the music */
void pauseMusic() {
  send_kt_command(0x0E, 0, 0, 20);
}

/** Select the source folder and track for playing */
void selectSourceFolderAndTrack(byte folder_number, byte track_number) {
  send_kt_command(0x0F, folder_number, track_number, 10);
}

// 0x10 "Volume adjust set" - NOT SUPPORTED

/** Enable or disable looping of all music files */
void loopAllMusic(byte enabled) {
  send_kt_command(0x11, 0, !!enabled, 10);
}

/** Select the source track from the "MP3" (case insensitive) folder */
void selectSourceTrackInMp3Directory(uint16_t track_number) {
  // Assert 0 ~ 9999
  send_kt_command(0x12, highByte(track_number), lowByte(track_number), 10);
}

/** Pause the current track and insert an advertisement song from the "ADVERT" (case insensitive) folder */
void insertSongFromAdvertDirectory(uint16_t track_number) {
  // Assert 0 ~ 9999
  send_kt_command(0x13, highByte(track_number), lowByte(track_number), 10);
}

/** Play the given song in the given directory (big directory version) */
void selectSourceBigFolderAndTrack(byte folder, uint16_t track_number) {
  // Assert folder 0 ~ 15, track number 0 ~ 9999
  // 0b1100 011111001111 = 4bits folder, 12 bits track number
  send_kt_command(0x14, (folder << 4) | highByte(track_number) & 0x0f, lowByte(track_number), 10);
}

/** Stop playing the inserted advertisement song and resume playing the user track */
void stopInsertedSongAndResumePlaying() {
  send_kt_command(0x15, 0, 0, 20);
}

/** Stop the music */
void stopMusic() {
  send_kt_command(0x16, 0, 0, 20);
}

/** Loop all music in the given folder */
void loopFolder(byte folder_number) {
  send_kt_command(0x17, 0, folder_number, 10);
}

/** Random playing */
void shufflePlay() {
  send_kt_command(0x18, 0, 0, 20);
}

/** single repeat the currently playing track track */
void repeatCurrentTrack(byte enabled) {
  send_kt_command(0x19, 0, !enabled, 10);
}

/** Mute sound output **/
void muteSound(byte enabled) {
  send_kt_command(0x1A, 0, !!enabled, 10);
}


/** Fonction de bas niveau pour envoyer une commande au module KT403A */
void send_kt_command(byte command, byte data_h, byte data_l, unsigned long cmd_delay) {
  mp3.write(COMMAND_BYTE_START);
  mp3.write(COMMAND_BYTE_VERSION);
  mp3.write((byte) 0x06);
  mp3.write(command);
  mp3.write((byte) 0x00);
  mp3.write(data_h);
  mp3.write(data_l);
  mp3.write(COMMAND_BYTE_STOP);
  // 16-bits checksum is optionnal
  delay(cmd_delay);
}
