/**************************************************************************
       Title:   Sound Effects (dacTone Example)
        Date:   05 Dec 2020
      Author:   Bruce E. Hall, w8bh.net
       Legal:   Open Source under the terms of the MIT License 
                                
 Description:   Demonstrate a few sound effects
                using the super-fast setFrequency() routine
                Listen with piezo or audio amp/headphones on GPIO25
          
***************************************************************************/

#include "DacTone.h"

DacTone audio;                                 // create object, audio on GPIO25

void setup() {                                 // so simple, nothing in setup.
} 

void whoop() {
   for (int i=10; i<210; i++) {                // 200 tones in all
     audio.setFrequency(6,i);                  // output a tone
     delay(1);                                 // and wait 1mS
   }
}

void unWhoop() {
   for (int i=210; i>10; i--) {                // 200 tones in all
     audio.setFrequency(6,i);                  // output a tone
     delay(1);                                 // and wait 1mS
   }  
}

void siren(int count=4) {                      // specify number of cycles
  for (int i=1; i<count; i++) {               
    whoop();                                   // sound goes up
    unWhoop();                                 // and sound goes down
  }
  audio.noTone();                              // off
}

void alarm(int count=4) {                      // specify number of cycles
  for (int i=0; i<count; i++) 
    whoop();                                   // phaser sound
  audio.noTone();
}

void computer(int count=8) {                   // specify number of cycles
  for (int i=0; i<count; i++) {
    audio.setFrequency(6,random(10,80));       // a few random beeps
    delay(150);
  }
  audio.noTone();
}

void ticks(int count) {
  for (int i=0; i<count; i++) {                // for the desired count
    for (int j=0; j<25; j++) {                 // emit a brief,
      audio.setFrequency(6,random(200));       // static-like noise
      delay(1);
    }
    audio.noTone();   
    delay(1000);                               // once a second
  }
}

void loop() {
 siren(); delay(1000);                         // sound effect #1
 alarm(); delay(1000);                         // sound effect #2
 computer(); delay(1000);                      // sound effect #3
 ticks(5);                                     // sound effect #4
}
