# Background music player using the tone() function for Arduino UNO Minima/WiFi.

This library allows you to play background music by `tone()` while letting **Arduino UNO R4 Minima/WiFi** do something.

## Usage

A typical example that uses tone() to play notes is as follows. In this program, you cannot do anything inside `loop()` until the melody finishes playing due to the blocking function `delay()`.

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

Using this BGMusic library, you can add something to do inside `loop()` like this:

```C++
/*
  Example of music score player in background
*/
#include "Arduino.h"
#include "BGMusic.h"

// notes (pairs of pitch and duration) in musical score.
// duration: 4 = quarter note, 8 = eighth note, etc.
#include "pitches.h"
static int score[] = {
  NOTE_C4, 4, NOTE_G3, 8, NOTE_G3, 8, NOTE_A3, 4, NOTE_G3, 4, REST, 4, NOTE_B3, 4, NOTE_C4, 4
};

// parameters for begin()
#define BUZZER_PIN  11    // pin number connected to the buzzer.
#define TEMPO       140   // change this to make the song slower or faster.
#define REPEAT      true  // playback repeatedly
#define N_NOTES(s)  (sizeof(s) / sizeof((s)[0]) / 2)  // number of notes in musical score.

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(BUZZER_PIN,  OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize and start BGM player
  static BGMusic music;
  music.begin(BUZZER_PIN, score, N_NOTES(score), TEMPO, REPEAT);
  music.start();
}

void loop() {
  // blink LED
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
```

## Dependency
[CallbackTimerR4](https://github.com/embedded-kiddie/CallbackTimerR4 "embedded-kiddie/CallbackTimerR4: Arduino UNO R4 timer library using FspTimer class.") >= 1.0.0

## API

### bool begin(int pin, const int notes[], int n_notes, int tempo, bool loop = false, bool start = false)

- `int pin` ... the pin number connected to piezo buzzer or a speaker.

- `const int notes[]` ... An array of musical notes made up of flat pairs of frequency and duration. (same format as in [arduino-songs](https://github.com/robsoncouto/arduino-songs "robsoncouto/arduino-songs") by [Robson Couto](https://github.com/robsoncouto "robsoncouto (Robson Couto)"))

- `int n_notes` ... Number of notes in the array. This is half the number of elements in the array.

- `int tempo` ... The metronome number. For example, `60` means **60 quarter-note beats per minute**.

- `bool loop` ... When `true`, play repeatedly.

- `bool start` ... When `true`, play immediately. 

### bool start(void)
Start playing after calling the "begin" method.

### void stop(void)
Stop playing.

### void end(void)
Calling this method followed by `stop()` will release the timer resource.

### void set_duration_function(int (*calc_duration_function)(int duration))

Register a function to calculate the duration of `tone()`. The default calculating method is as follows:

```C++
duration = wholenote / duration;
```

where `wholenote` is `(60000 * 4) / tempo`. You can use numbers to specify duration, such as `4` for a quarter note, `8` for an 8th note, and `16` for a 16th note like this:

| Nth note | 16th     | 8th     |quarter  | half    | whole  | 
|:-------- |:--------:|:-------:|:-------:|:-------:|:------:|
| duration | 16       | 8       | 4       | 2       | 1      |

But this description can not be specify the [**tie**](https://en.wikipedia.org/wiki/Tie_(music) "Tie (music) - Wikipedia") that two or more same notes are played in succession.

As an alternative, you can define some symbols as follows:

| Nth note | 16th     | 8th     |quarter  | half    | whole  | 
|:-------- |:--------:|:-------:|:-------:|:-------:|:------:|
| duration | 1        | 2       | 4       | 8       | 16     |
| symbol   | N16      | N8      | N4      | N2      | N1     |

Then you can define the symbols like this:

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
#define TEMPO         155   // change this to make the song slower or faster
#define QUARTER_NOTE  ((60000 / N4) / TEMPO)  // quarter note duration in milliseconds based on the number of beats in 60 second

// Calculate note length from note length symbol
int calc_duration(int duration) {
  return QUARTER_NOTE * duration;
}
...
void setup() {
  ...
  // initialize and start the BGM player
  static BGMusic music;
  music.set_duration_function(calc_duration);
  music.begin(buzzer_pin, melody, notes, tempo, repeat);
  music.start();
  ...
}
```

By this method, you can specify a "tie" like "quarter note" + "8th note" as `N4 + N8` Check the sample at [background_music](https://github.com/embedded-kiddie/arduino-uno/tree/main/background_music "arduino-uno/background_music at main · embedded-kiddie/arduino-uno") :+1:
