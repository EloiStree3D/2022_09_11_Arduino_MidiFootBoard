#ifndef IR2Midi_CPP
#define IR2Midi_CPP

#include "Arduino.h"
#include <string.h>
#include "IR2Midi.h"
#include <IRremote.hpp>
#include "SerialLog.cpp"


IR2Midi::IR2Midi() {
  m_irToMidiFound = false;
  m_peviousNote = 0;
  m_peviousVelocity = 0;
  m_peviousChannel = 0;
  m_peviousIrAddress = 0;
  m_peviousIrCommand = 0;
  m_peviousRaw = 0;
}

void IR2Midi::InitIR() {
  m_irToMidiFound = false;
  m_peviousNote = 0;
  m_peviousVelocity = 0;
  m_peviousChannel = 0;
  m_peviousIrAddress = 0;
  m_peviousIrCommand = 0;
  m_peviousRaw = 0;
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  //IrSender.begin();
  printActiveIRProtocols(&Serial);
}

// void IR2Midi::PushMidiAsIR(byte note, byte velocity, byte channel){
// if(velocity%2==1)
//   note+=127;
// uint16_t sAddress = (velocity-126)/2;
// uint8_t sCommand = note;
// uint8_t sRepeats = 0;
//     LogPrintLn();
//     LogPrint(F("Send now: address=0x"));
//     LogPrint(sAddress, HEX);
//     LogPrint(F(" command=0x"));
//     LogPrint(sCommand, HEX);
//     LogPrint(F(" repeats="));
//     LogPrint(sRepeats);
//     LogPrintLn();
//     IrSender.sendNEC(sAddress, sCommand, sRepeats);
// }

void IR2Midi::ReadAndConvertToMidi() {

  m_irToMidiFound = false;
  if (IrReceiver.decode()) {

    // Print a short summary of received data
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      if(m_useDebugLog){
        LogPrintLn(F("Received noise or an unknown (or not yet enabled) protocol"));
        IrReceiver.printIRResultRawFormatted(&Serial, true);
      }
    } else if (IrReceiver.decodedIRData.protocol != UNKNOWN) {

        m_peviousIrAddress = IrReceiver.decodedIRData.address;
        m_peviousIrCommand = IrReceiver.decodedIRData.command;

        m_peviousNote = m_peviousIrCommand;
        m_peviousVelocity =126-( m_peviousIrAddress * 2);
        // a= 126-(b*2) b= (a-126)/2 ??
        m_peviousChannel = m_defaultChannel;

        if (m_peviousNote > 127) {
          m_peviousVelocity = +1;
          m_peviousNote = m_peviousNote % 127;
        }

        if (m_peviousVelocity > 127) {
          m_peviousVelocity = 127;
        }
        if (m_peviousNote > 127) {
          m_peviousNote = 127;
        }
        m_peviousRaw = m_peviousChannel*10000+m_peviousNote*100+m_peviousVelocity;
        m_irToMidiFound = true;
      
    }
    IrReceiver.resume();  // Enable receiving of the next value
  }
}

bool IR2Midi::HasReceivedIR(){
return m_irToMidiFound;

}

void IR2Midi::DisplayPreviousIRToMidiIfFound() {
  if (m_irToMidiFound) {
    DisplayPreviousIRToMidi();
  }
}

void IR2Midi::DisplayPreviousIRToMidi() {

  LogPrintLn("## Display IR 2 MIDI ##");
  
  LogPrint(" Address: ");
  LogPrint(m_peviousIrAddress);
  LogPrint(" Command: ");
  LogPrint(m_peviousIrCommand);
  LogPrintLn();

  LogPrint(" Note: ");
  LogPrint(m_peviousNote);
  LogPrint(" Velocity: ");
  LogPrint(m_peviousVelocity);
  LogPrint(" Channel: ");
  LogPrint(m_peviousChannel);
  LogPrintLn();
}

#endif