#include "Arduino.h"
#include "Slider.h"

Slider::Slider(int size, int digitalPin[]) {
	_size = size;
	_delta = 100/((size-1)*2);
	_buttons = new Button *[size];

	for(int i=0; i<size; i++) {
		_buttons[i] = new Button(digitalPin[i]);
	}
}

Slider::Slider(int size, uint8_t* input) {
	_size = size;
	_delta = 100/((size-1)*2);
	_buttons = new Button *[size];

	
	while(size>0) {
		for(int i=0; i<size && i<8; i++) {
			_buttons[i] = new Button(i, input);
		}
		size-=8;
		input+=1;
	}
}

Transition Slider::getTransition() {
	int state = 0;
	bool found = false;
	/*
	* We could return the slider status directly in if statement but this
	* would not scroll all buttons of a slider, giving problem of misalignment
	* between the RELEASED status of the buttons
	*/
	for(int i=0; i<_size; i++) {
		Transition t = (*_buttons[i]).getTransition();
		if(!found) {
			if(t == PRESSED && !_pressed) {
				_pressed = true;
				state = 1;
				found = true;
			}
			else if(t == RELEASED && getState() == OFF) {
				_pressed = false;
				state = 2;
				found = true;
			}
		}
	}
	switch(state) {
		case 0: return NONE; break;
		case 1: return PRESSED; break;
		case 2: return RELEASED; break;
	}
}

State Slider::getState() {
	for(int i=0; i<_size; i++) {
		State s = (*_buttons[i]).getState();
		if(s == ON) return ON;
	}
	return OFF;
}

int Slider::getProgress() {
	float progress;

	bool found = false;
	for(int i=0; i<_size; i++) { // check if non-consecutive button are pressed, and if only two button are pressed at once
		if((*_buttons[i]).getState() == ON ) {
			if(found) return -1;
			else {
				progress = 2*i*_delta;
				if(i+1<_size) {
					if((*_buttons[i+1]).getState() == ON) {
						progress += _delta;
						i++;
					}
				}
				found = true;
			}
		}
	}
	if(!found) return static_cast<int>(_progress); // return value measured previously

	if(_progress != progress) {
		if(millis()-_holdCounter > _holdThreshold) {
			_progress = progress;
			_holdCounter = millis();
		}
	}
	else {
		_holdCounter = millis();
	}
	return static_cast<int>(_progress);
}

int Slider::setHoldThreshold(int value) {
	_holdThreshold = value;
}