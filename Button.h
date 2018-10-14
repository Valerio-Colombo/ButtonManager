#ifndef Button_h
#define Button_h

#include "Arduino.h"
#include "Transition.h"
#include "State.h"

class Button {
	private:
		void (*_func)();

		bool _shifted;
		uint8_t _pos;
		uint8_t* _input;
		bool _isPressed();


		bool _state;
		uint8_t _pin;

		bool _debounce = true;
		int16_t _timeElapsed;
		int16_t _timeThreshold = 100;

	public:
		Button(int pin);
		Button(int pos, uint8_t* in);
		Transition getTransition();
		State getState();

		void setAction(void (*func)());
		void doAction();
};

#endif