#include <Button.h>
#include <Slider.h>
#include <State.h>
#include <Transition.h>

#define BUTTON_SIZE 1
#define SLIDER_SIZE 7

// Sensor
uint8_t ledPin = D2;                          // Led strip pin
uint8_t latchPin = D7;                        // PSC
uint8_t dataPin = D5;                         // Q6
uint8_t clockPin = D6;                        // CLK
uint8_t inputSlider = 0;                      // Byte where is stored 8-Stage Static Shift Register output
Button buttonArray[BUTTON_SIZE] = {D0};       // Don't use D3, D4 and D8! They are reserved pin for boot
Slider slider(SLIDER_SIZE, &inputSlider, latchPin, dataPin, clockPin);
int progress = 0;
int currentLuminosity = 0;

void buttonArraySetup() {
  buttonArray[0].setAction(roomLightButton);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  buttonArraySetup();
}

void loop() {
  // Get buttons status
  for (int i = 0; i < BUTTON_SIZE; i++) {
    Transition transition = buttonArray[i].getTransition();
    if (transition == Transition::RELEASED) {
      buttonArray[i].doAction();
    }
  }

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

void roomLightButton() {
  Serial.println(progress);
  progress = 0;
}

