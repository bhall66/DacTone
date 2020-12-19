# DacTone
 Audio sine wave generator for ESP32

## Introduction
 
This Arduino library is meant for creating audio tones on an ESP32 microcontroller its internal digital-to-audio (DAC) converter.  The result is a well-formed sine wave on either of its two DAC pins, GPIO25 or GPIO26.  

## Basic Usage

First, include DacTone in your sketch:

```
#include "DacTone.h"
```

Next, instantiate an DacTone object:

```
DacTone audio;
```

Finally, call tone() to start playing a tone, and noTone() to silence it:

```
audio.tone(1200);    // start 1200 Hz sine wave output
delay(1000);         // play it for a second, then
audio.noTone();      // silence it
```

## Provided Examples

1. Beep - A simple blink sketch, audio-style
2. Scales - How to generate notes using frequency calculations
3. PlayBach - Play a short musical piece using defined notes
4. SoundEffects - How to use lower-level code for sound generation
5. FastTones - Shows accuracy of sine wave frequency

## Low-level control

DacTone allows you to control the amplitude (volume) and offset (DC bias) of the waveform.  If the volume and/pr bias exceed to output capabilities of the DAC, waveform clipping will result.  The isClipping() method will tell you if the current waveform is clipping.  Changing the amound of clipping can result in interesting sounds!

## Code Documentation

Each public method in DacTone.cpp is fully documented.

## Known Issues

1. DacTone seems to interferes with digitalInput under certain conditions.
2. DacTone output may differ from requested frequency by several Hz.  This is usually not an issue, but a good musical ear may hear the difference! 

## Credits

The low-level code for enabling sine wave output on the ESP32 is provided by [krzychb](https://github.com/krzychb).  See his [dac-cosine library](https://github.com/krzychb/dac-cosine) for more information.


