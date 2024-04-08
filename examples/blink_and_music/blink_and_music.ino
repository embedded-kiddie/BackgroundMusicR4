/*
  Example of music score player in background

  Copyright (c) 2024 embedded-kiddie All Rights Reserved.

  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/
#include "Arduino.h"
#include "BGMusic.h"

// pitch and duration pairs in the melody:
// duration: 4 = quarter note, 8 = eighth note, etc.
#include "pitches.h"
static int melody[] = {
// Special thanks for the memody data from: https://github.com/robsoncouto/arduino-songs
#include "Greensleeves.h"
};

#define BUZZER_PIN  9     // pin number connected to the buzzer
#define TEMPO       70    // change this to make the song slower or faster
#define REPEAT      true  // playback repeatedly
#define N_NOTES(s)  (sizeof(s) / sizeof((s)[0]) / 2)  // number of notes in musical score.

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(BUZZER_PIN,  OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize and start the BGM player
  BGMusic.begin(BUZZER_PIN, melody, N_NOTES(melody), TEMPO, REPEAT);
  BGMusic.start();
}

void loop() {
  // blink LED
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
