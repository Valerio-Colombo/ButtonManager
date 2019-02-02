#ifndef Button_h
#define Button_h

#include "Arduino.h"
#include "Transition.h"
#include "State.h"

class Button {
	private:
		void (*_func)(); 				// Function called by doAction()

		bool _shifted; 					// True if button is attached to a 8-Stage Static Shift Register
		uint8_t _pos; 					// Position of the button in the 8-Stage Static Shift Register
		uint8_t* _input;				// Byte which contains 8-Stage Static Shift Register readings
		bool _isPressed();


		bool _state; 					// Register state of the button
		uint8_t _pin; 					// Digital pin of the button

		bool _debounce = true;
		int16_t _timeElapsed;
		int16_t _timeThreshold = 100; 	// Number of milliseconds that must elapse to accept a change in the button state

	public:
		/*
		* Button has two constructors:
		* one accapting the digital pin where the button is connected
		* the other accepting a pointer to a byte and a position relative to the byte
		*/
		Button(int pin);
		Button(int pos, uint8_t* in);

		/*
 		* Return if the button is:
 		* PRESSED(transition between not pressed to pressed), 
 		* RELEASED(transition between pressed to not pressed),
 		* or simply not touched. 
 		*/
		Transition getTransition();

		/*
 		* Return if the button is:
 		* ON(User is currently pressing the sensor), 
 		* OFF(User is currently not pressing the sensor), 
 		*/
		State getState();

		/*
		* Ties an action to a button
		*/
		void setAction(void (*func)());

		/*
		* Call the action tied to the button
		*/
		void doAction();
};

#endif