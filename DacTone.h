/**************************************************************************
       Title:   dacTone - Arduino audio sine wave library for ESP32
        Date:   18 Dec 2020
      Author:   Bruce E. Hall, w8bh.net
       Legal:   Open Source under the terms of the MIT License 
                Portions (c) kyztchb under Apache License 2.0
                (see https://github.com/krzychb/dac-cosine)
                    
 Description:   Instantiate dacTone with channel# (1=GPIO25, 2=GPIO26)
                Start audio with tone(pitch)
                Stop audio with noTone()
                Four volume levels:  100 (loudest), 50, 25, 12, 0 (off)
                DacTone.cpp contains notes on each public routine.

    Examples:   DacTone audio;              // audio on GPIO25 (default)
                DacTone audio(2);           // audio on GPIO26
                audio.tone();               // output A5 = 880Hz sine wave
                audio.tone(NOTE_D5);        // output D5 = 587Hz sine wave
                audio.tone(1200);           // output 1200Hz sine wave
                audio.tone(0);              // stop output
                audio.noTone();             // stop output
                audio.setVolume(50);        // set output volume at 50%
                audio.setVolume(100);       // set output full volume
                audio.setOffset(0);         // set DC bias at midpoint (1.65V)
                audio.setOffset(-50);       // set DC bias at 1.00V
                audio.isClipping();         // returns true if waveform is clipped
          
***************************************************************************/

#ifndef DacTone_h
#define DacTone_h

#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc.h"
#include "driver/dac.h"
#include "pitches.h"

#define DACTONE_DEFAULT_CHANNEL  1          // range 0..1; 1 = output on GPIO25
#define DACTONE_DEFAULT_SCALE    0          // range 0..3; 0=100% vol, 1=50%, 2=25%, 3=12.5%
#define DACTONE_DEFAULT_OFFSET   0          // range -128..127; 0 = DC offset ~1.65 volts
#define DACTONE_DEFAULT_SHAPE    2          // range 0..3;  2 = sine wave
#define DACTONE_DEFAULT_PITCH    880        // range 1..5000; 880 Hz = Musical note "A5"
#define DACTONE_MIN_FREQUENCY    1          // 1 Hz minimum
#define DACTONE_MAX_FREQUENCY    5000       // 5000 Hz maximum

class DacTone
{
  public:
    DacTone        (int channel = DACTONE_DEFAULT_CHANNEL);
    int  tone      (int freqHz  = DACTONE_DEFAULT_PITCH);
    void noTone    ( );
    void setVolume (int volume  = 100);
    void setOffset (int ofs     = DACTONE_DEFAULT_OFFSET);
    void setShape  (int shape   = DACTONE_DEFAULT_SHAPE);
    bool isClipped ( );
    
    void setFrequency(int clkDiv, int freqStep);
    int  getFrequency(int *clkDiv, int *freqStep); 
 
  private:
    int _scale;
    int _offset;
    int _invert;
    int _step;
    int _div;
    int _freq;
    int _fActual;
    dac_channel_t _channel;
    void setFreqParams();
    void dac_cosine_enable();
    void dac_frequency_set();
    void dac_scale_set();
    void dac_offset_set();
    void dac_invert_set();
};

#endif
