#include "Arduino.h"
#include "Button.h"

Button::Button(int pin) {
	_pin = pin;
	pinMode(_pin, INPUT_PULLUP);
    _state = digitalRead(_pin);
    _shifted = false;
}

Button::Button(int pos, uint8_t* in) {
	_pos = pos;
	_input = in;
	_shifted = true;
}

Transition Button::getTransition() {
	bool state = _isPressed();
	if(state != _state) {
		_state = state;
		if(!_state) return RELEASED;
		else return PRESSED;
	}

	return NONE;
}

State Button::getState() {
	if(_isPressed()) return ON;
	return OFF;
}

void Button::setAction(void (*func)()) {
	_func = func;
}

void Button::doAction() {
	_func();
}

bool Button::_isPressed() {
	if(_shifted) {
		if(*_input & (1<<_pos)) return true;
		return false;
	}
	return digitalRead(_pin);
}
