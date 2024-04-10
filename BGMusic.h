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
  const int *notes;
  const int *notes_bigin;
  const int *notes_end;
  int wholenote;
  int loop;
} MusicScore_t;

class BGMusic {
private:
  static MusicScore_t score;
  static void music_callback(void);
  static int (*duration_function)(int duration);

public:
  BGMusic();
  ~BGMusic();
  bool begin(int pin, const int notes[], int n_notes, int tempo, bool loop = false, bool start = false);
  static bool start(void);
  static bool stop(void);
  void end(void);

  void set_duration_function(int (*calc_duration_function)(int duration)) {
    duration_function = calc_duration_function;
  }
};

#endif
