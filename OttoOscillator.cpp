//--------------------------------------------------------------
//-- Oscillator.pde
//-- Generate sinusoidal oscillations in the servos
//--------------------------------------------------------------
//-- (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
//-- GPL license
//--------------------------------------------------------------

#include "Arduino.h"
#include "OttoOscillator.h"
#include <Servo.h>


bool Oscillator::next_sample(){
  _currentMillis = millis();
  if(_currentMillis - _previousMillis > _TS) {
    _previousMillis = _currentMillis;   
    return true;
  }
  return false;
}


void Oscillator::attach(int pin, bool rev){
  if(!_servo.attached()){
      _servo.attach(pin);
      _servo.write(90);
      _TS=30;
      _T=2000;
      _N = _T/_TS;
      _inc = 2*M_PI/_N;
      _previousMillis=0;
      _A=45;
      _phase=0;
      _phase0=0;
      _O=0;
      _stop=false;
      _rev = rev;
  }
}


void Oscillator::detach(){
  if(_servo.attached())
        _servo.detach();
}


void Oscillator::SetT(unsigned int T){
  _T=T;
  _N = _T/_TS;
  _inc = 2*M_PI/_N;
};


void Oscillator::SetPosition(int position){
  _servo.write(position+_trim);
};


// This function should be periodically called to maintain the oscillations.
void Oscillator::refresh(){
  if (next_sample()) {
      if (!_stop) {
         _pos = round(_A * sin(_phase + _phase0) + _O);
	       if (_rev) _pos=-_pos;
         _servo.write(_pos+90+_trim);
      }
      _phase = _phase + _inc;
  }
}
