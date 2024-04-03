/*
  BGMusic.h - Background Music player using tone() for Arduino UNO R4

  Copyright (c) 2024 embedded-kiddie All Rights Reserved.

  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef BGMUSIC_H
#define BGMUSIC_H

#include "Arduino.h"
#include "CBTimer.h"

#ifndef debug_begin
#define debug_begin(...)
#endif
#ifndef debug_print
#define debug_print(...)
#endif
#ifndef debug_println
#define debug_println(...)
#endif

// Music score
typedef struct {
  int pin;
  int *notes;
  int *notes_bigin;
  int *notes_end;
  int wholenote;
  int loop;
} MusicScore_t;

class BGMusic_t {
private:
  static MusicScore_t score;
  static void music_callback(void);
  int (*duration_function)(int wholenote, int duration) = nullptr;

public:
  bool begin(int pin, int notes[], int n_notes, int tempo, bool loop);
  bool start(void);
  bool stop(void);
  void end(void);

  void set_duration_function(int (*calc_duration_function)(int wholenote, int duration)) {
    duration_function = calc_duration_function;
  }
};

extern BGMusic_t BGMusic;
extern CBTimer_t CBTimer;

#endif
