/**************************************************************************
       Title:   FastTones (dacTone Example)
        Date:   05 Dec 2020
      Author:   Bruce E. Hall, w8bh.net
       Legal:   Open Source under the terms of the MIT License 
                
                    
 Description:   Demonstrate use of low-level routines for debugging,
                fast tone generation, etc.

                Listen with piezo or audio amp/headphones on GPIO26
                Review data output on serial monitor.
          
***************************************************************************/

#include "DacTone.h"

#define RATIO    1.059463          // frequency ratio of adjacent notes

DacTone audio;                     // instantiate dacTone library


void setup() { 
  Serial.begin(115200);            // will need serial monitor
  delay(1000);                    
} 

void loop() {
  Serial.println("\nNotes from A4 to A7:");
  showFrequencies();
  delay(3000);
  
  Serial.println("\nFast tones!");
  fastTones();
  Serial.println();
  delay(3000);
}


 
//  Three octaves of notes are generated in the usual way via audio.tone().
//
//  getFrequency() is used to retrieve 3 internally used parameters:
//    a) the actual frequency generated
//    b) the clock divisor
//    c) the frequency step
//
//  The divisor and frequency step can be useful if fast tone generation is needed
//  See fastTones(), below. 


void showFrequencies() {
  int clkDiv, freqStep, freq;

  for (float f=NOTE_A3; f<=NOTE_A7; f*=RATIO) {
    audio.tone(f);
    freq = audio.getFrequency(&clkDiv,&freqStep);
    Serial.printf("div: %d, step: %4d, f(desired): %4.0f Hz, f(actual): %4d Hz\n",
      clkDiv, freqStep, f, freq);
    delay(100);                     
  }
  
  audio.noTone();                   // stop audio
}


//  The following routine generates 200 extremely fast tones, using
//  the routine setFrequency().  The advantage of this routine is
//  its speed.  The disadvantage is that you cannot specifiy the 
//  desired frequency directly, you must supply two low-level frequency
//  parameters:  clkDiv and freqStep.  These parameters determine the
//  output frequency according to the equation:
// 
//    f(actual) = RTC_FAST_CLK_FREQ_APPROX / (1 + clkDiv) * freqStep/65536
//              = 125.6 * freqStep / (clkDivi+1)
// 
//  A one millisecond delay is added so that you can hear the tones.
//  Here is a speed comparison of the two tone-generating routines
//
//    Procedure               Elapsed Time (mS)
//    audio.tone()                1223
//    audio.setFrequency()          <1*
//
//  With the one-millisecond delay, 200 setFrequency calls takes
//  about 200 milliseconds.
//


 void fastTones() {
   int t=millis(),stopT=0;                     // Lets time this:
   
   for (int i=10; i<210; i++) {                // 200 tones in all
     audio.setFrequency(6,i);                  // output a tone
     delay(1);                                 // and wait 1mS
   }
   
   stopT = millis();                           // stop the timer
   audio.noTone();                             // stop the audio
   Serial.print("Elapsed time (mS): ");        // print elapsed time
   Serial.println(stopT-t);
 }
