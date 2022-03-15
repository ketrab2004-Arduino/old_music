#include <Arduino.h>

#define CHOSEN_SONG 3
// 2 Vader Jacob
// 3 Papegaaitje leef je nog
// 4 Slaap kindje slaap
// 5 Jingle Bells

#if CHOSEN_SONG == 0
const String songNotes =
"[octave = 4; bpm = 120]"
"c1/1"
"d e"
"_1/1"
"f g a"
"_1/1"
"a g f e g e d";

#elif CHOSEN_SONG == 1
const String songNotes =
"[octave = 4; bpm = 120]"
"c d"
"c d";

#elif CHOSEN_SONG == 2    // Vader Jacob
const String songNotes =
"[octave = 4; bpm = 120]"
"cdec cdec"
"ef g1/2"
"ef g1/2"
"g1/8 a1/8 g1/8 f1/8 e c"
"g1/8 a1/8 g1/8 f1/8 e c"
"c [oct=3]g[oct=4] c1/2"
"c [oct=3]g[oct=4] c1/2";

#elif CHOSEN_SONG == 3    // Papegaaitje leef je nog?
const String songNotes = //PROGMEM
"[octave = 4; bpm = 114]"
"d d1/8 d F1/8 a F1/8 d3/8"
"[img = /MUSIC/NEVERG~1/0.BMP]"
"e3/8 e3/8 C3/8 [oct=3]a3/8[oct=4]"
"d d1/8 d F1/8 a F1/8 d3/8"
"e3/8 e3/8 C3/8 [oct=3]a3/8[oct=4]"
"d d1/8 d d1/8 d d1/8 d d1/8 d d1/8 d d1/8 d d1/8 d3/8"
"d3/8 F3/8 a3/8 F3/8 [oct=5]d3/8 _3/8";

#elif CHOSEN_SONG == 4    // Slaap kindje slaap
const String songNotes =
"[octave = 4; bpm = 95]"
"e1/2 d1/2 d c1/2"
"egg ffe"
"eggg feff"
"feee dcdd"
"effg"
"agfe dc";

#elif CHOSEN_SONG == 5    // Jingle bells
const String songNotes =
"[octave = 4; bpm = 104]"
"eee1/2 eee1/2"
"egc d1/8 e1/1"
"fff f1/8f eee e1/8e1/8"
"edde d1/2g1/2 eee1/2 eee1/2"
"egc d1/8 e1/1 ffff"
"fee e1/8e1/8 ggfd c1/1";

#endif
