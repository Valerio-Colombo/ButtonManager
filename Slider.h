#ifndef Slider_h
#define Slider_h

#include "Arduino.h"
#include "Button.h"
#include "Transition.h"
#include "State.h"

class Slider {
	private:
		int _size;
		Button **_buttons;
		float _delta;

		bool _pressed = false;
		float _progress = 0;
		int _holdCounter = 0;
		int _holdThreshold = 15;

	public:
		/*
 		*Slider has two constructor, one accepting the array containing digital pins attached to the touch sensor,
		*the other one accept a byte or an array where is stored the output of a/multiple 8-Stage Static Shift Register.
 		*Use of the class is the same in both cases
 		*/
		Slider(int size, int digitalPin[]);
		Slider(int size, uint8_t* input);

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
		Return curret value of slider if pressed or return last progress value if not pressed
		*/
		int getProgress();
};

#endif