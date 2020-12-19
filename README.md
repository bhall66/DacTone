# DacTone
 Audio sine wave generator for ESP32

## Introduction
 
This Arduino library is meant for creating audio tones on an ESP32 microcontroller using its internal digital-to-audio (DAC) converter.  The result is a well-formed sine wave on either of its two DAC pins, GPIO25 or GPIO26.  

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

## Waveform control

DacTone allows you to control the amplitude (volume) and offset (DC bias) of the waveform.  If the volume and/or bias exceed to output capabilities of the DAC, waveform clipping will result.  The isClipping() method will tell you if the current waveform is clipping.  Changing the amound of clipping can result in interesting sounds!

## Methods

1. `int tone(int Hz)` - starts audio output at the requested frequency.  Acceptable input values are 0 to 5000.  Calling tone with no parameter `tone()` results in 880 Hz output.  Calling tone(0) silences output. tone() returns the actual frequency output.

2. `noTone()` - silences audio output

3. `setVolume(int volume)` - sets the amplitude of the waveform.  There are 4 levels of amplitude: 100, 50, 25, and 12.  These roughly correspond to voltage amplitudes of 3.2, 1.7, 1.0, and 0.6 volts.

The following methods are less seldomly used, but may be helpful in certain cases:

4. `setOffset(int offset)` - sets the DC bias of the waveform.  Acceptable input values are -128 to +127.  The default offset of zero results in a DC bias of about 1.65V, which will allow full voltage swing at 100% volume without waveform clipping.

5. `isClipped()` - returns true if the current waveform is being clipped.

6. `setFrequency(int div, int step)` - a high-speed method of setting the output frequency.  It is much faster than tone(), but you must already know the clock divisor and frequency step parameters.

7. `int getFrequency(int *div, int *step)` - returns the actual frequency output from a call to tone() or setFrequency().  It also returns the last clock divisor and frequency step parameters used. 

See comments in DacTone.cpp for documentation of each public method.

## Known Issues

1. DacTone seems to interferes with `digitalInput()` under certain conditions.
2. DacTone output may differ from the requested frequency by several Hz.  This is usually not an issue, but a good musical ear may hear the difference! 

## Credits

The low-level code for enabling sine wave output is provided by [krzychb](https://github.com/krzychb).  See his [dac-cosine library](https://github.com/krzychb/dac-cosine) for more information.


