/*
  BGMusic.cpp - Background Music player using tone() for Arduino UNO R4

  Copyright (c) 2024 embedded-kiddie All Rights Reserved.

  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/
#if 0
#define debug_begin(...)  { Serial.begin(__VA_ARGS__); while(!Serial); }
#define debug_print(...)    Serial.print(__VA_ARGS__)
#define debug_println(...)  Serial.println(__VA_ARGS__)
#endif

#include "BGMusic.h"

// Duration between notes.
#define BREATH 10

// Instance of Music score player in background
static CBTimer cbtimer;

// Initialize static data menbers
MusicScore_t BGMusic::score = { 0, 0, 0, 0, false };
int (*BGMusic::duration_function)(int duration) = nullptr;

// Constructor / Destructor
BGMusic::BGMusic() {}
BGMusic::~BGMusic() {
  end();
}

// Initialize music score sequencer
bool BGMusic::begin(int pin, const int notes[], int n_notes, int tempo, bool loop, bool start) {
  debug_begin(9600);

  score.pin = pin;
  score.notes = notes;
  score.notes_bigin = notes;
  score.notes_end = notes + n_notes * 2;
  score.wholenote = (60000 * 4) / tempo;
  score.loop = loop;

  if (start == true) {
    return BGMusic::start();
  } else {
    return true;
  }
}

// Instanciate the FspTimer and start it immediately
bool BGMusic::start(void) {
  if (cbtimer.begin(TIMER_MODE_ONE_SHOT, CBTIMER_START_NOW, music_callback) == true) {
    return cbtimer.start();
  } else {
    return false;
  }
}

// Stop playing
bool BGMusic::stop(void) {
  noTone(score.pin);
  return cbtimer.stop();
}

// Delete the instanciate of FspTimer
void BGMusic::end(void) {
  cbtimer.end();
}

// Play notes in music score sequentially
void BGMusic::music_callback(void) {
  stop();

  if (score.notes < score.notes_end) {
    debug_print("note[0]  = "); debug_println(score.notes[0]);
    debug_print("note[1]  = "); debug_println(score.notes[1]);

    int note = *score.notes++;
    int duration = *score.notes++;

    // select the method of calculating duration
    if (duration_function == nullptr) {
      // https://github.com/robsoncouto/arduino-songs
      duration = score.wholenote / duration;

      // negative value for dotted note
      if (duration < 0) {
        duration = abs(duration) * 3 / 2;
      }
    }

    else {
      // you can select the original method
      duration = duration_function(duration);
    }

    debug_print("duration = "); debug_println(duration);

    tone(score.pin, (unsigned int)note, (unsigned long)(duration * 0.9));

    // Instanciate the FspTimer and start it
    cbtimer.begin(duration, music_callback);
  }

  // For repeated playback
  else if (score.loop) {
    score.notes = score.notes_bigin;
    start();
  }
}
