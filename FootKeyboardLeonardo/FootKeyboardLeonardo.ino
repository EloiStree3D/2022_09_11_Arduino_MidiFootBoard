

#include "MIDIUSB.h"
#include "PitchToNote.h"
#include "IR2Midi.h"
#include "SerialLog.cpp"

#define NUM_BUTTONS  5
const uint8_t button1 = 12;
const uint8_t button2 = 10;
const uint8_t button3 = 8;
const uint8_t button4 = 6;
const uint8_t button5 = 4;

const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5};
const byte notePitches[NUM_BUTTONS] = {pitchC3, pitchD3, pitchE3, pitchF3, pitchG3 };
bool noteCurrentValue[NUM_BUTTONS];
bool notePreviousValue[NUM_BUTTONS];
uint8_t intensity;
IR2Midi irToMidi;

uint32_t lastIrId;
int cChannel;
int cNote;
int cVelocity;
int lastIrIdCountDown;
long timeMidiDown ;
long timeMidiDownPrevious ;
int deltaTime ;

void setup() {
  Serial.begin(9600);
  irToMidi.InitIR();
  for (int i = 0; i < NUM_BUTTONS; i++){
    
    pinMode(buttons[i], INPUT_PULLUP);
    readButtons();
  }
  m_useLog=true;
  timeMidiDown= millis();
  timeMidiDownPrevious=timeMidiDown; 
}


void loop() {

  timeMidiDownPrevious=timeMidiDown; 
timeMidiDown= millis();
deltaTime= (int)(timeMidiDown-timeMidiDownPrevious);
  readButtons();
  triggerButtons();
  
  irToMidi.ReadAndConvertToMidi();
  if (irToMidi.HasReceivedIR()) {

    if(lastIrId!= irToMidi.m_peviousRaw){ 
      lastIrIdCountDown=0;
      lastIrId=0;
      noteOffAndFlush(cChannel, cNote,cVelocity);
    }
    if(lastIrId!=0 && lastIrId== irToMidi.m_peviousRaw)
      lastIrIdCountDown=220;
    if(lastIrId==0|| lastIrIdCountDown<=0){
      
      noteOn(irToMidi.m_peviousChannel,irToMidi.m_peviousNote,irToMidi.m_peviousVelocity);
          MidiUSB.flush();
      delay(1);
      irToMidi.DisplayPreviousIRToMidiIfFound();
      lastIrIdCountDown=220;
      lastIrId=(irToMidi.m_peviousRaw);
      cChannel =irToMidi.m_peviousChannel;
      cNote =irToMidi.m_peviousNote;
      cVelocity =irToMidi.m_peviousVelocity;
    }
  }
  delay(1);
  //Serial.print('.');
  //Serial.print(lastIrId);
  //Serial.print('_');
  //Serial.print(lastIrIdCountDown);
  //Serial.println();
  if(lastIrIdCountDown>0){
    if(deltaTime<=0)
      deltaTime=1;
    lastIrIdCountDown-=deltaTime;
    if(lastIrIdCountDown<=0){
      lastIrIdCountDown=0;
      lastIrId=0;
      noteOffAndFlush(cChannel, cNote,cVelocity);
      //irToMidi.DisplayPreviousIRToMidiIfFound();
    
    }
  }
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).
void controlChange(byte channel, byte control, byte value) {

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void readButtons()
{
  //Serial.print(">");
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    notePreviousValue[i]=noteCurrentValue[i];
    noteCurrentValue[i]=digitalRead(buttons[i]) == LOW;
  //Serial.print(notePreviousValue[i]?'1':'0');
  //Serial.print("|");
  //Serial.print(noteCurrentValue[i]?'1':'0');
  //Serial.print(" ");
  }
  //Serial.println();
  //noteCurrentValue
  //notePreviousValue
}
void triggerButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if(notePreviousValue[i]!=noteCurrentValue[i]){
      if(noteCurrentValue[i]){
        noteOn(0, notePitches[i], 127);
        MidiUSB.flush();
      }
      else {
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }

    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  Serial.print("M,on:");
  LogPrint(channel);
  LogPrint("-");
  LogPrint( pitch);
  LogPrint("-");
  LogPrint( velocity);
  LogPrintLn();
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
 
}

void noteOff(byte channel, byte pitch, byte velocity) {
  Serial.print("M,off:");
  LogPrint(channel);
  LogPrint("-");
  LogPrint( pitch);
  LogPrint("-");
  LogPrint( velocity);
  LogPrintLn();
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
void noteOnAndFlush(byte channel, byte pitch, byte velocity) {
        noteOn(channel, pitch, velocity);
        MidiUSB.flush();
}
void noteOffAndFlush(byte channel, byte pitch, byte velocity) {
        noteOff(channel, pitch, velocity);
        MidiUSB.flush();


}