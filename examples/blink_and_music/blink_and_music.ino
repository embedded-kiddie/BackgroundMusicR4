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

void setup() {
  // pin number connected to the buzzer
  int buzzer_pin = 9;

  // calculate the number of pitch and duration pairs.
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // change this to make the song slower or faster
  int tempo = 70;

  // playback repeatedly
  int repeat = true;

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(buzzer_pin,  OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize and start the BGM player
  BGMusic.begin(buzzer_pin, melody, notes, tempo, repeat);
  BGMusic.start();
}

void loop() {
  // blink LED
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
