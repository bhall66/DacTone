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

#include "Arduino.h"
#include "DacTone.h"


// ================== kyztchb code starts here ===================================

/*
 * Enable cosine waveform generator on a DAC channel
 */
void DacTone::dac_cosine_enable()
{
  // Enable tone generator common to both channels
  SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
  switch(_channel) {
    case DAC_CHANNEL_1:
      // Enable / connect tone tone generator on / to this channel
      SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
      // Invert MSB, otherwise part of waveform will have inverted
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2, SENS_DAC_INV1_S);
      break;
    case DAC_CHANNEL_2:
      SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN2_M);
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, 2, SENS_DAC_INV2_S);
      break;
    }
}

/*
 * Set frequency of internal CW generator common to both DAC channels
 *
 * clk_8m_div: 0b000 - 0b111
 * frequency_step: range 0x0001 - 0xFFFF
 *
 */
void DacTone::dac_frequency_set()
{
  REG_SET_FIELD(RTC_CNTL_CLK_CONF_REG, RTC_CNTL_CK8M_DIV_SEL, _div);
  SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL1_REG, SENS_SW_FSTEP, _step, SENS_SW_FSTEP_S);
}

/*
 * Scale output of a DAC channel using two bit pattern:
 *
 * - 00: no scale
 * - 01: scale to 1/2
 * - 10: scale to 1/4
 * - 11: scale to 1/8
 *
 */
void DacTone::dac_scale_set()
{
  switch(_channel) {
    case DAC_CHANNEL_1:
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE1, _scale, SENS_DAC_SCALE1_S);
      break;
    case DAC_CHANNEL_2:
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE2, _scale, SENS_DAC_SCALE2_S);
      break;
  }
}

/*
 * Offset output of a DAC channel
 *
 * Range 0x00 - 0xFF
 *
 */
void DacTone::dac_offset_set()
{
  switch(_channel) {
    case DAC_CHANNEL_1:
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC1, _offset, SENS_DAC_DC1_S);
      break;
    case DAC_CHANNEL_2:
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC2, _offset, SENS_DAC_DC2_S);
      break;
  }
}

/*
 * Invert output pattern of a DAC channel
 *
 * - 00: does not invert any bits,
 * - 01: inverts all bits,
 * - 10: inverts MSB,
 * - 11: inverts all bits except for MSB
 *
 */
void DacTone::dac_invert_set()
{
  switch(_channel) {
    case DAC_CHANNEL_1:
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, _invert, SENS_DAC_INV1_S);
      break;
    case DAC_CHANNEL_2:
      SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, _invert, SENS_DAC_INV2_S);
      break;
  }
}

// ================== kyztchb code ends here ========================================

// ================== dacTone library code folows ===================================

/*
 *  setShape() is provided for completeness sake, but useful only for sound effects
 *  There are 4 shapes: 0,1,2,3.
 *  Shape 2, the default, is a sine wave.  The other shapes are somewhat unusual.
 * 
 */

void DacTone::setShape(int shape) {
  _invert = shape && 0x03;                     // use only lowest two bits
  dac_invert_set();                            // set the shape 
}

/*  Two low-level routines:
 *   
 *  setFrequency() is a high-speed method of setting the audio output frequency.
 *  This routine is much faster than tone(), which takes 6mS to process.
 *  But you must already know the clock divider and frequency step parameters.
 *  
 *  getFrequency() returns the actual frequency output, after a call to
 *  tone() or setFrequency.  Actual frquency may differ from the desired frequency
 *  by several hertz.  It also returns the last clock divisor and frequency step
 *  parameters used
 *   
 */
void DacTone::setFrequency(int clkDiv, int freqStep) {
  _div = clkDiv; _step = freqStep;
  dac_frequency_set(); 
  dac_output_enable(_channel);
}

int DacTone::getFrequency(int *clkDiv, int *freqStep) {
  *clkDiv = _div;
  *freqStep = _step; 
  return(125.6*(float)_step /(1 + (float)_div));
}

/*
 * setFreqParams() finds the optimal _div and _step parameters for Frequency selection
 * 
 * Called by tone() and determines: _div, _step, _fActual
 *
 * The highest clock divisor (/8) results in 16 Hz steps, which is too wide for adequate
 * musical note selection.   Therefore, a search must be performed to determine the
 * combination of clock divisor and frequency step that comes closet to the desired frequency.
 * 
 * The following routine examines 250 steps at all 8 clock divisors (2000 combinations)
 * to see which results in the best frequency approximation.  Experiments show a combination
 * will be found within 6 milliseconds, with deviation from expected frequency usually under 6 Hz
 * This is good enough for tonal generation and simple music applications.
 *
 */
 
 void DacTone::setFreqParams() {
  float f, fTarget, delta, delta_min=999.0;
  int stepTarget=0, divTarget=0;
  for (int step=1; step<250; step++) {
    for (int divi=0; divi<8; divi++) {            // try 2000 combinations
      f = 125.6*(float)step/(float)(divi+1);      // what is the resulting frequency?
      delta= abs((_freq - f));                    // compare it to desired frequency
      if (delta  < delta_min) {                   // is it closer than last best combo?
        delta_min  = delta;                       // yes, so remember how close it is
        stepTarget = step;                        // and remember the step
        divTarget  = divi;                        // and remember the divisor
        fTarget    = f;                           // and remember the resulting freq
      }
    }
  }
  _div = divTarget;                               // save the best combination
  _step = stepTarget;
  _fActual = fTarget;
}


/* 
 *  tone() and noTone()
 * 
 * The major routines for this library, tone() is called
 * with the desired audio output frequency in Hz.
 * 
 * Acceptable input values for tone() are 0..5000 Hz, 
 * where 0 = no output.
 * 
 * tone() optionally returns with the actual output frequency  
 * noTone() disables any current output
 * 
 */
 
int DacTone::tone(int freqHz) {
  int result = 0;
  if (freqHz==0) noTone();
  else if ((freqHz >= DACTONE_MIN_FREQUENCY)
  && (freqHz <= DACTONE_MAX_FREQUENCY)) {
    _freq = freqHz;
    setFreqParams();
    dac_frequency_set(); 
    dac_output_enable(_channel);
    result = _fActual;
  } 
  return(result);
}

void DacTone::noTone() {
  dac_output_disable(_channel);
}

/*  
 *  setVolume() sets the audio output volume
 *   
 *  There are five volume levels:
 *  100 (full) volume
 *   50 = 50% volume
 *   25 = 25% volume
 *   12 = 12.5% volume
 *    0 = 0% volume
 *    
 *  Each volume level corresponds to the voltage swing of the sine wave output.
 *  Louder volumes have larger voltage swings.
 *  
 *  On an oscilloscope, these are typically measured as Vpp, or "peak-to-peak"
 *  In other words, from the highest voltage at the top of the waveform to the
 *  lowest voltage at the base of the waveform.
 *  
 *  Another way to measure amplitude is from the center of the waveform to the
 *  peak, V(peak), which is exactly half of Vpp.
 *  
 *  Here are the 4 volume levels and their appoximate voltage outputs: 
 *     Volume   Vpp     Vpk
 *       100    3.20    1.60
 *        50    1.70    0.85
 *        25    1.00    0.50
 *        12    0.56    0.28
 *        
 *  On the ESP32, which runs at 3.3V, the highest voltage output is 3.30V and the
 *  lowest voltage is 0V, so the largest possible Vpp is 3.30V.
 */
 
void DacTone::setVolume (int volume) {
  if (volume <= 0) noTone();
  else if (volume <  25) _scale = 3;
  else if (volume <  50) _scale = 2;
  else if (volume < 100) _scale = 1;
  else _scale = 0;
  dac_scale_set();
}

/*  
 *  setOffset() sets the DC offset
 *   
 *  input range: -128 to 128 (smallest to largest)
 *  each unit add/subracts ~0.013 volts from 1.65V waveform centerline
 *  
 *  A center offset(0) = 1.65v DC bias,
 *  which allows full volume 3.2Vpp waveforms without clipping
 *  
 *  Choosing a non-zero offset may result in waveform clipping 
 *  and "fuzzy" non-sinusoidal sound.
 *  This should usually be avoided, but can be a useful
 *  modulation technique (think: guitar fuzz pedal).
 *  
 *  To avoid clipping, choose offset magnitudes of less than 
 *  61 @ 50% volume, < 88 @ 25% volume, and < 105 @ 12% volume.
 *  
 *  The routine isClipped() returns true if the current waveform
 *  is being clipped.
 *  
 */
 
void DacTone::setOffset (int ofs) {
  _offset = constrain(ofs,-128,127);           // input range -128..127
  dac_offset_set();                            // set the DC bias
}

bool DacTone::isClipped() {
  int vPk = 0;
  switch (_scale) {
    case 0: vPk = 1600; break;                 // 100% volume = vPeak 1.60V
    case 1: vPk =  850; break;                 //  50% volume = vPeak 0.85V
    case 2: vPk =  500; break;                 //  25% volume = vPeak 0.50V
    case 3: vPk =  280; break;                 //  12% volume = vPeak 0.28V
  };
  int vCenter = 1650 + (_offset*13);           //  DC voltage @ waveform center
  int vTop  = vCenter + vPk;                   //  DC voltage @ waveform top
  int vBase = vCenter - vPk;                   //  DC voltage @ waveform base
  return ((vTop>3300)||(vBase<0));             //  anything >3.3V or <0V is clipped
}


/*
 *  DacTone object constructor
 *  
 *  Call with channel number:
 *  1 = DAC_CHANNEL_1 (GPIO 25)
 *  2 = DAC_CHANNEL_2 (GPIO 26)
 *  
 *  The default channel is 1, so instantiating
 *  the object without any parameter will use GPIO25
 *  for its output.
 *  
 *  You may have two objects, each with its own channel,
 *  However they share the same CW generator:  changing
 *  the frequency of one will have a simulantous effect
 *  on the other
 *  
 */


DacTone::DacTone (int channel) {
  _channel = (dac_channel_t)channel;         
  _scale   = DACTONE_DEFAULT_SCALE;
  _offset  = DACTONE_DEFAULT_OFFSET;
  _invert  = DACTONE_DEFAULT_SHAPE;                               
  _step    = 0; 
  _div     = 0;
  _freq    = 0;
  _fActual = 0;
  
  dac_cosine_enable();                         // configure for CW
  dac_scale_set();                             // 100% volume
  dac_offset_set();                            // no offset
  dac_invert_set();                            // Sine wave
}
