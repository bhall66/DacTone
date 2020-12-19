/**************************************************************************
       Title:   Scales (dacTone Example)
        Date:   05 Dec 2020
      Author:   Bruce E. Hall, w8bh.net
       Legal:   Open Source under the terms of the MIT License 
                                
 Description:   Demonstrate volume control & musical scales
                Listen with piezo or audio amp/headphones on GPIO26
          
***************************************************************************/

#include "DacTone.h"
#define RATIO 1.059463             // freq. ratio of adj. notes = 2**(1/12)

DacTone audio;                     // create audio object, output is GPIO25

void setup() {                     // so simple, nothing in setup.
} 

void loop() {
  beeps();                         // play 4 beeps, loud to soft
  goingUp();                       // play 3-octave scale, low to high
  goingDown();                     // play 3-octave scale, high to low
}


// output 4 beeps:  the first one at full volume, the next 3 gradually softer

void beeps() {
  int vol = 100;                   // start at full volume
  for (int i=0; i<4; i++) {        // 4 notes
    audio.setVolume(vol);          // set volume
    audio.tone(NOTE_A5);           // play the note
    delay(500);                    // for a while
    audio.tone(0);                 // then silence
    delay(500);                    // for a while
    vol /=2;                       // volume decreases to 50%,25%,12%
  } 
}


// play a fast 3-octave scale from low to high
// the scale starts at A3 (220 Hz) and stops at A7 (3220 Hz)
// the frequency of each note is f(previous note) * 1.059463,

void goingUp() {
  audio.setVolume(100);            // full volume
  for (float f=NOTE_A3; f<=NOTE_A7; f*=RATIO) {
    audio.tone(f);                 // play a note in the scale
    delay(70);                     // and hold it ever so briefly
  }  
}


// play a fast 3-octave scale from high to low

void goingDown() {
  for (float f=NOTE_A7; f>=NOTE_A3; f/=RATIO) {
    audio.tone(f);                 // play a note in the scale
    delay(70);                     // and hold it ever so briefly
  }  
}
