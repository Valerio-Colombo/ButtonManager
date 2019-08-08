#include <Button.h>
#include <Slider.h>
#include <State.h>
#include <Transition.h>

#define SLIDER_SIZE 8

// Sensor
uint8_t ledPin = 3;                          // Led strip pin
uint8_t dataPin = 5;                         // Q8
uint8_t clockPin = 6;                        // CLK
uint8_t latchPin = 7;                        // PSC
uint8_t inputSlider = 0;             // Byte where is stored 8-Stage Static Shift Register output
Slider slider(SLIDER_SIZE, &inputSlider, latchPin, dataPin, clockPin);
int progress = 0;
int currentLuminosity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  slider.setHoldThreshold(100);
}

void loop() {
  // Get Slider status
  slider.updateStatus();

  Transition transition = slider.getTransition();
  State state = slider.getState();

  if (state == State::ON) {
    int p = slider.getProgress(); // update slider status
    Serial.println(p);
    if (p != (-1)) progress = p;
  }
  if (transition == Transition::RELEASED) {
    Serial.print("Led set to: ");
    Serial.println(progress);
  }

  // Light output
  setBedLight(progress);

  delay(1);        // delay in between reads for stability
}

void setBedLight(int p) {
  if (currentLuminosity < p) currentLuminosity++;
  else if (currentLuminosity > p) currentLuminosity--;
  analogWrite(ledPin, map(currentLuminosity, 0, 100, 0 , 1024));
}

