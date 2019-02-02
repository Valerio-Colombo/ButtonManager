#ifndef Slider_h
#define Slider_h

#include "Arduino.h"
#include "Button.h"
#include "Transition.h"
#include "State.h"

class Slider {
	private:
		// Initialization variables
		int _size; // Size of the slider
		uint8_t* _input; // Byte where slider reading are stored
		Button **_buttons; // Slider buttons
		float _delta; // Value quantum

		// Pin variables
		uint8_t* _latchPin;
		uint8_t* _dataPin;
		uint8_t* _clockPin;

		// Measurements variables
		bool _pressed = false;
		float _progress = 0; // Actual value of the slider
		int _holdCounter = 0; //Current time
		int _holdThreshold = 150; // Number of milliseconds that must elapse to accept a change in the progress of the slider

		uint8_t shiftIn(int myDataPin, int myClockPin);

	public:
		/*
 		*Slider has two constructor, one accepting the array containing digital pins attached to the touch sensor,
		*the other one accept a byte or an array where is stored the output of a/multiple 8-Stage Static Shift Register.
 		*Use of the class is the same in both cases
 		*/
		Slider(int size, int digitalPin[]);
		Slider(int size, uint8_t* input, uint8_t* latchPin, uint8_t* dataPin, uint8_t* clockPin);

		/*
 		*Return if the slider is:
 		*PRESSED(transition between not pressed to pressed), 
 		*RELEASED(transition between pressed to not pressed),
 		*or simply not touched. 
 		*/
		Transition getTransition();

		/*
 		*Return if the slider is:
 		*ON(User is currently pressing the sensor), 
 		*OFF(User is currently not pressing the sensor), 
 		*/
		State getState();

		/*
		*Return curret value of slider if pressed or return last progress value if not pressed
		*If an invalid number of button is pressed the function returns -1
		*/
		int getProgress();

		/*
		*Set _holdThresholdValue
		*/
		int setHoldThreshold(int value);

		/*
		*Update slider status 
		*/
		void updateStatus();
};

#endif