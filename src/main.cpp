#include <Arduino.h>
#include <EncoderButton.h>
#include <MIDIUSB.h>

EncoderButton eb1(2, 3, 4);

void setup()
{
  eb1.setUserId(30);
  eb1.setEncoderHandler(onEbEncoder);
  eb1.setPressedHandler(onBtnPressed);
  eb1.setReleasedHandler(onBtnReleased);
}

void loop()
{
  eb1.update();
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

void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}