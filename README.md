# Arduino background music player with tone using CallbackTimerR4 library.

This library allows you to play background music by `tone()` while letting Arduino do something.

## Usage

A typical example that uses tone() to play notes is as follows. In this program, you cannot do anything inside `loop()` until the melody finishes playing.

```C++
/*
  Plays a melody
  https://www.arduino.cc/en/Tutorial/Tone
*/
#include "Arduino.h"

// notes (pair of pitch and duration) in the melody.
// duration: 4 = quarter note, 8 = eighth note, etc.
#include "pitches.h"
int melody[] = {
  NOTE_C4, 4, NOTE_G3, 8, NOTE_G3, 8, NOTE_A3, 4, NOTE_G3, 4, REST, 4, NOTE_B3, 4, NOTE_C4, 4
};

void setup() {
  int buzzer_pin = 11;  // pin number connected to the buzzer.
  int notes = sizeof(melody) / sizeof(melody[0]); // number of pitches and durations.

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < notes; thisNote += 2) {
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000/4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / melody[thisNote + 1];

    tone(buzzer_pin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 10[msec] works well.
    delay(noteDuration + 10);

    // stop the tone playing
    noTone(buzzer_pin);
  }
}

void loop() {
  // no need to repeat the melody.
}
```

Using this BGMplay library, you can add something to do inside `loop()` like this:

```C++
/*
  Example of music score player in background
*/
#include "Arduino.h"
#include "BGMplay.h"

// notes (pair of pitch and duration) in the melody.
// duration: 4 = quarter note, 8 = eighth note, etc.
#include "pitches.h"
static int melody[] = {
  NOTE_C4, 4, NOTE_G3, 8, NOTE_G3, 8, NOTE_A3, 4, NOTE_G3, 4, REST, 4, NOTE_B3, 4, NOTE_C4, 4
};

void setup() {
  int buzzer_pin = 11;  // pin number connected to the buzzer.
  int notes = sizeof(melody) / sizeof(melody[0]) / 2; // number of notes (pair of pitch and duration).
  int tempo = 140;      // change this to make the song slower or faster
  int repeat = true;    // playback repeatedly

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(buzzer_pin,  OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize and start BGM player
  BGMplay.begin(buzzer_pin, melody, notes, tempo, repeat);
  BGMplay.start();
}

void loop() {
  // blink LED
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
```

## API

### bool begin(int pin, int notes[], int n_notes, int tempo, bool loop)

- int pin ... the pin number connected to piezo buzzer or a speaker.

- int notes[] ... An array of note that is a pair of pitch frequency and duration.

- int n_notes ... Number of notes in the array. This is half the number of elements in the array.

- int tempo ... The metronome number. For example, `60` means **60 quarter-note beats per minute**.

- bool loop ... If `true`, play repeatedly.

### bool start(void)
Start playing after calling the "begin" method.

### void stop(void)
Stop playing.

### void end(void)
Calling this method followed by `stop()` will release the timer resource.

### void set_duration_function(int (*calc_duration_function)(int wholenote, int duration))

Register a function to calculate the duration of `tone()`. The default calculating method is as follows:

```C++
duration = wholenote / duration;
```

where `wholenote` is `(60000 * 4) / tempo`. You can use numbers to specify duration, such as `4` for a quarter note, `8` for an 8th note, and `16` for a 16th note.

But this description can not be specify the **slur** that two or more notes are played in succession.

As an alternative, you can define some symbols as follows:

```C++
/*----------------------------------------------------------------------
 * Musical note length
 * Define the length of each note when "16th note" is 1
 *----------------------------------------------------------------------*/
#define  N16     1   //   16th note
#define  N8      2   //    8th note
#define  N8x     3   //  8.5th note
#define  N4      4   //quarter note
#define  N4x     6   //  4.5th note
#define  N2      8   //   half note
#define  N2x     12  //  2.5th note
#define  N1      16  //  Whole note
```

And register your function of calculating duration like this:

```C++
// Calculate note length from note length symbol
int calc_duration(int wholenote, int duration) {
  return (wholenote  / 16) * duration;
}
...
void setup() {
  ...
  // initialize and start the BGM player
  BGMusic.set_duration_function(calc_duration);
  BGMusic.begin(buzzer_pin, melody, notes, tempo, repeat);
  BGMusic.start();
  ...
}
```

Then, you can specify a slur like "quarter note" + "8th note" as `N4 + N8` :+1:
