/**************************************************************************
       Title:   dacTone Example #1
        Date:   05 Dec 2020
      Author:   Bruce E. Hall, w8bh.net
       Legal:   Open Source under the terms of the MIT License 
                
                    
 Description:   Simple "beep" sketch to show dacTone output
                Add piezo or audio amp/headphones to GPIO25
          
***************************************************************************/

#include "DacTone.h"           // this is the DAC audio library

DacTone audio;                 // create audio object, output is GPIO25

void setup() {                 // so simple, nothing in setup.
}

void loop() {
  audio.tone();                // turn on 880Hz sine wave
  delay(500);                  // wait  
  audio.noTone();              // now turn off audio
  delay(500);                  // wait
}
