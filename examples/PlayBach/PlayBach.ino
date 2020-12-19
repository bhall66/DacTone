/**************************************************************************
       Title:   Play Bach (dacTone Example)
        Date:   05 Dec 2020
      Author:   Bruce E. Hall, w8bh.net
       Legal:   Open Source under the terms of the MIT License 
                             
 Description:   Play a small tune using the dacTone library
                
                The tune is "Prelude in C", from the Well Tempered Clavier,
                Book One, by J. S. Bach

                Listen with piezo or audio amp/headphones on GPIO25
                Review data output on serial monitor.
          
***************************************************************************/

#include "DacTone.h"
#define   NOTECOUNT      544                   //  length of array
DacTone   audio;                               // instantiate dacTone library


int notes[] =                                  // this array hold entire tune
{
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5, //measures 1-2, notes 1-32
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5,
  NOTE_C4, NOTE_D4, NOTE_A4, NOTE_D5, NOTE_F5, NOTE_A4, NOTE_D5, NOTE_F5,
  NOTE_C4, NOTE_D4, NOTE_A4, NOTE_D5, NOTE_F5, NOTE_A4, NOTE_D5, NOTE_F5,

  NOTE_B3, NOTE_D4, NOTE_G4, NOTE_D5, NOTE_F5, NOTE_G4, NOTE_D5, NOTE_F5, //measures 3-4, notes 33-64
  NOTE_B3, NOTE_D4, NOTE_G4, NOTE_D5, NOTE_F5, NOTE_G4, NOTE_D5, NOTE_F5, 
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5,
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G4, NOTE_C5, NOTE_E5,
  
  NOTE_C4, NOTE_E4, NOTE_A4, NOTE_E5, NOTE_A5, NOTE_A4, NOTE_E5, NOTE_A5, //measures 5-6, notes 65-96
  NOTE_C4, NOTE_E4, NOTE_A4, NOTE_E5, NOTE_A5, NOTE_A4, NOTE_E5, NOTE_A5,
  NOTE_C4, NOTE_D4, NOTE_FS4,NOTE_A4, NOTE_D5, NOTE_FS4,NOTE_A4, NOTE_D5,
  NOTE_C4, NOTE_D4, NOTE_FS4,NOTE_A4, NOTE_D5, NOTE_FS4,NOTE_A4, NOTE_D5,
  
  NOTE_B3, NOTE_D4, NOTE_G4, NOTE_D5, NOTE_G5, NOTE_G4, NOTE_D5, NOTE_G5, //measures 7-8, notes 97-128
  NOTE_B3, NOTE_D4, NOTE_G4, NOTE_D5, NOTE_G5, NOTE_G4, NOTE_D5, NOTE_G5,
  NOTE_B3, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E4, NOTE_G4, NOTE_C5,
  NOTE_B3, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E4, NOTE_G4, NOTE_C5,
  
  NOTE_A3, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E4, NOTE_G4, NOTE_C5, //measures 9-10, notes 129-160
  NOTE_A3, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E4, NOTE_G4, NOTE_C5,
  NOTE_D3, NOTE_A3, NOTE_D4, NOTE_FS4,NOTE_C5, NOTE_D4, NOTE_FS4,NOTE_C5,
  NOTE_D3, NOTE_A3, NOTE_D4, NOTE_FS4,NOTE_C5, NOTE_D4, NOTE_FS4,NOTE_C5,
  
  NOTE_G3, NOTE_B3, NOTE_D4, NOTE_G4, NOTE_B4, NOTE_D4, NOTE_G4, NOTE_B4, //measures 11-12, notes 161-192
  NOTE_G3, NOTE_B3, NOTE_D4, NOTE_G4, NOTE_B4, NOTE_D4, NOTE_G4, NOTE_B4, 
  NOTE_G3, NOTE_AS3,NOTE_E4, NOTE_G4, NOTE_CS5,NOTE_E4, NOTE_G4, NOTE_CS5,
  NOTE_G3, NOTE_AS3,NOTE_E4, NOTE_G4, NOTE_CS5,NOTE_E4, NOTE_G4, NOTE_CS5,  
 
  NOTE_F3, NOTE_A3, NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_A4, NOTE_D5, //measures 13-14, notes 193-224
  NOTE_F3, NOTE_A3, NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_A4, NOTE_D5,
  NOTE_F3, NOTE_GS3,NOTE_D4, NOTE_F4, NOTE_B4, NOTE_D4, NOTE_F4, NOTE_B4,
  NOTE_F3, NOTE_GS3,NOTE_D4, NOTE_F4, NOTE_B4, NOTE_D4, NOTE_F4, NOTE_B4,
  
  NOTE_E3, NOTE_G3, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_C4, NOTE_G4, NOTE_C5, //measures 15-16, notes 225-256
  NOTE_E3, NOTE_G3, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_C4, NOTE_G4, NOTE_C5,
  NOTE_E3, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4, NOTE_A3, NOTE_C4, NOTE_F4,   
  NOTE_E3, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4, NOTE_A3, NOTE_C4, NOTE_F4, 
  
  NOTE_D3, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4, NOTE_A3, NOTE_C4, NOTE_F4, //measures 17-18, notes 257-288
  NOTE_D3, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4, NOTE_A3, NOTE_C4, NOTE_F4,
  NOTE_G2, NOTE_D3, NOTE_G3, NOTE_B3, NOTE_F4, NOTE_G3, NOTE_B3, NOTE_F4, 
  NOTE_G2, NOTE_D3, NOTE_G3, NOTE_B3, NOTE_F4, NOTE_G3, NOTE_B3, NOTE_F4, 
 
  NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G3, NOTE_C4, NOTE_E4, //measures 19-20, notes 289-320 
  NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G3, NOTE_C4, NOTE_E4, 
  NOTE_C3, NOTE_G3, NOTE_AS3,NOTE_C4, NOTE_E4, NOTE_AS3,NOTE_C4, NOTE_E4,
  NOTE_C3, NOTE_G3, NOTE_AS3,NOTE_C4, NOTE_E4, NOTE_AS3,NOTE_C4, NOTE_E4,

  NOTE_F2, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A3, NOTE_C4, NOTE_E4, //measures 21-22, notes 321-352
  NOTE_F2, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A3, NOTE_C4, NOTE_E4,
  NOTE_FS2,NOTE_C3, NOTE_A3, NOTE_C4, NOTE_DS4,NOTE_A3, NOTE_C4, NOTE_DS4,
  NOTE_FS2,NOTE_C3, NOTE_A3, NOTE_C4, NOTE_DS4,NOTE_A3, NOTE_C4, NOTE_DS4,

  NOTE_GS2,NOTE_F3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_C4, NOTE_D4, //measures 23-24, notes 353-384
  NOTE_GS2,NOTE_F3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_C4, NOTE_D4, 
  NOTE_G2, NOTE_F3, NOTE_G3, NOTE_B3, NOTE_D4, NOTE_G3, NOTE_B3, NOTE_D4,
  NOTE_G2, NOTE_F3, NOTE_G3, NOTE_B3, NOTE_D4, NOTE_G3, NOTE_B3, NOTE_D4,
  
  NOTE_G2, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G3, NOTE_C4, NOTE_E4, //meaures 25-26, notes 385-416
  NOTE_G2, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G3, NOTE_C4, NOTE_E4,
  NOTE_G2, NOTE_D3, NOTE_G3, NOTE_C4, NOTE_F4, NOTE_G3, NOTE_C4, NOTE_F4,
  NOTE_G2, NOTE_D3, NOTE_G3, NOTE_C4, NOTE_F4, NOTE_G3, NOTE_C4, NOTE_F4,

  NOTE_G2, NOTE_D3, NOTE_G3, NOTE_B3, NOTE_F4, NOTE_G3, NOTE_B3, NOTE_F4, //measures 27-28, notes 417-448 
  NOTE_G2, NOTE_D3, NOTE_G3, NOTE_B3, NOTE_F4, NOTE_G3, NOTE_B3, NOTE_F4,
  NOTE_G2, NOTE_DS3,NOTE_A3, NOTE_C4, NOTE_FS4,NOTE_A3, NOTE_C4, NOTE_FS4,
  NOTE_G2, NOTE_DS3,NOTE_A3, NOTE_C4, NOTE_FS4,NOTE_A3, NOTE_C4, NOTE_FS4,

  NOTE_G2, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_G4, NOTE_G3, NOTE_C4, NOTE_G4, //measures 29-30, notes 449-480 
  NOTE_G2, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_G4, NOTE_G3, NOTE_C4, NOTE_G4,
  NOTE_G2, NOTE_F3, NOTE_G3, NOTE_C4, NOTE_F4, NOTE_G3, NOTE_C4, NOTE_F4,
  NOTE_G2, NOTE_F3, NOTE_G3, NOTE_C4, NOTE_F4, NOTE_G3, NOTE_C4, NOTE_F4,

  NOTE_G2, NOTE_F3, NOTE_G3, NOTE_B3, NOTE_F4, NOTE_G3, NOTE_B3, NOTE_F4, //measures 31-32, notes 481-512
  NOTE_G2, NOTE_F3, NOTE_G3, NOTE_B3, NOTE_F4, NOTE_G3, NOTE_B3, NOTE_F4, 
  NOTE_C2, NOTE_C3, NOTE_G3, NOTE_AS3,NOTE_E4, NOTE_G3, NOTE_AS3,NOTE_E4,
  NOTE_C2, NOTE_C3, NOTE_G3, NOTE_AS3,NOTE_E4, NOTE_G3, NOTE_AS3,NOTE_E4, 

  NOTE_C2, NOTE_C3, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_A3, //measures 33-34, notes 513-544
  NOTE_C4, NOTE_A3, NOTE_F3, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_F3, NOTE_D3,
  NOTE_C2, NOTE_B2, NOTE_G4, NOTE_B4, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_B4,
  NOTE_D5, NOTE_B4, NOTE_G4, NOTE_B4, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_D4, 
};

void playBach() {
  int len = 190;                               // modify for speed
  for (int i=0; i<NOTECOUNT; i++) {
    audio.tone(notes[i]);                      // grab a note
    delay(len);                                // and play it for a while
    if ((i>512) and (i<540))                   // slow down (rit.) at end.
      len += 5;
  }
  audio.tone(NOTE_C4);                         // final note
  delay(2400);
  audio.noTone();                              // done
}


void setup() {
  playBach();                                  // Music!
}

void loop() {
  // once is enough.  Really.
}
