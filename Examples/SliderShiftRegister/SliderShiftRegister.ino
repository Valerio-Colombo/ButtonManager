/* Valerio Colombo - 08/2019
 * ##########################################################################################
 * # This example shows how to use the library to treat a group of buttons as a slider      #
 * # (linear potentiometer). This code could work with an arbitrary number of daisy chained #
 * # shift registers. The only parameter that need to change is the SLIDER_SIZE constant to #
 * # fit the size of the slider of your project.                                            #
 * ##########################################################################################
 */

#include <Button.h>
#include <Slider.h>
#include <State.h>
#include <Transition.h>

#define SLIDER_SIZE 8                             // Number of buttons componing the slider

// Sensor
uint8_t ledPin = 9;                               // Led pin
uint8_t latchPin = 7;                             // PSC
uint8_t dataPin = 5;                              // Q8, shift register output
uint8_t clockPin = 6;                             // CLK
Slider slider(SLIDER_SIZE, latchPin, dataPin, clockPin);

int progress = 0;                                 // Value return by the slider

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  slider.setHoldThreshold(100);                   // Set minimum milliseconds that must elapse 
}                                                 // to accept a change in the progress of the slider


void loop() {
  slider.updateStatus();                          // Refresh slider status

  Transition transition = slider.getTransition(); // Get slider transition and actual state
  State state = slider.getState();

  if (state == State::ON) {
    int p = slider.getProgress();                 // Read slider value
    Serial.print("p: ");
    Serial.println(p);
    if (p != (-1)) progress = p;
  }
  if (transition == Transition::RELEASED) {
    Serial.print("Led set to: ");
    Serial.println(progress);
  }

  analogWrite(ledPin, map(progress, 0, 100, 0, 255));

  delay(1);                                       // delay between reads for stability
}

