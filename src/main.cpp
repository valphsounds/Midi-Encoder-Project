#include <Arduino.h>
#include <EncoderButton.h>
#include <MIDIUSB.h>

// Setting up encoder buttons with pins
EncoderButton EB1(2, 3, 4);
EncoderButton EB2(5, 6, 7);
EncoderButton EB3(8, 9, 10);

// Saving all encoder buttons to a list
EncoderButton *EBs[]{&EB1, &EB2, &EB3};
byte NUMBER_EBS = 3;

void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void onEbEncoder(EncoderButton &eb)
{
  int16_t inc = eb.increment();
  int16_t value = 64 + inc;
  controlChange(16, byte(eb.userId()), byte(value));
}

void onBtnPressed(EncoderButton &eb)
{
  int16_t control = eb.userId() + 10;
  controlChange(16, byte(control), 127);
}

void onBtnReleased(EncoderButton &eb)
{
  int16_t control = eb.userId() + 10;
  controlChange(16, byte(control), 0);
}

void setup()
{
  for (int i = 0; i < NUMBER_EBS; i++)
  {
    EBs[i]->setUserId(30 + i);
    EBs[i]->setEncoderHandler(onEbEncoder);
    EBs[i]->setPressedHandler(onBtnPressed);
    EBs[i]->setReleasedHandler(onBtnReleased);
  }
}

void loop()
{
  for (int i = 0; i < NUMBER_EBS; i++)
  {
    EBs[i]->update();
  }
}