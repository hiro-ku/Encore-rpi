#include <I2S.h>
#include "audio_data.h"

// I2S Pins
#define I2S_BCLK_PIN 10
#define I2S_LRCLK_PIN 11
#define I2S_DOUT_PIN 12

// Button Pin
#define BUTTON_PIN 14

I2S i2s(OUTPUT);

// Audio parameters (Adjust these to match your WAV export settings)
const int sampleRate = 44100; // or 22050
const int bps = 16;           // 16-bit PCM there is also 8 and 24-bit PCM but 16 is most common and is the default in most cases so use that

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize I2S
  i2s.setBCLK(I2S_BCLK_PIN);
  i2s.setDATA(I2S_DOUT_PIN);
  i2s.setBitsPerSample(bps);
  
  if (!i2s.begin(sampleRate)) {
    while (1); 
  }
}

void playAudio() {
  const uint32_t headerOffset = 44;
  
  if (ohayo_wav_len <= headerOffset) return;

  const uint8_t *audioPtr = ohayo_wav + headerOffset;
  size_t bytesRemaining = ohayo_wav_len - headerOffset;

  while (bytesRemaining > 0) {
    int16_t sample = *(int16_t *)audioPtr;
    
    i2s.write((int32_t)sample);
    i2s.write((int32_t)sample);

    audioPtr += 2;
    bytesRemaining -= 2;
  }
}

void loop() {
  // button press (active LOW)
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce is important to avoid multiple triggers from a single press
    if (digitalRead(BUTTON_PIN) == LOW) {
      playAudio();
      
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
    }
  }
}