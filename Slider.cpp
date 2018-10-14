#include "Arduino.h"
#include "Slider.h"

Slider::Slider(int size, int digitalPin[]) {
	_size = size;
	_delta = 100/(size*2-1);
	_buttons = new Button *[size];

	for(int i=0; i<size; i++) {
		_buttons[i] = new Button(digitalPin[i]);
	}
}

Slider::Slider(int size, uint8_t* input) {
	_size = size;
	_delta = 100/(size*2-1);
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
	for(int i=0; i<_size; i++) {
		Transition t = (*_buttons[i]).getTransition();
		if(t == PRESSED && !_pressed) {
			_pressed = true;
			return PRESSED;
		}
		else if(t == RELEASED && getState() == OFF) {
			_pressed = false;
			return RELEASED;
		}
	}

	return NONE;
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
				progress = (2*i+1)*_delta;
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
		_holdCounter++;
		if(_holdCounter > _holdThreshold) {
			_progress = progress;
			_holdCounter = 0;
		}
	}
	return static_cast<int>(_progress);
}