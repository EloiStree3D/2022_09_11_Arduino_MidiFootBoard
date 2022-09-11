#ifndef IR2Midi_H
#define IR2Midi_H

#include "Arduino.h"
#include <string.h>
#define IR_SEND_PIN A1
#define IR_RECEIVE_PIN A0
class IR2Midi {
public:
  bool m_irToMidiFound ;
  bool m_useDebugLog=true;
  int m_peviousNote ;
  int m_peviousVelocity ;
  int m_peviousChannel ;
  int m_peviousIrAddress ;
  int m_peviousIrCommand ;
  int m_peviousRaw ;
  int m_defaultChannel=3;

  IR2Midi();
  void PushMidiAsIR(byte note, byte velocity, byte channel);
  void InitIR();
  void ReadAndConvertToMidi();
  void DisplayPreviousIRToMidi();
  void DisplayPreviousIRToMidiIfFound();
  bool HasReceivedIR();
};


#endif