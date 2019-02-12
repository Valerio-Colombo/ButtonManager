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

Slider::Slider(int size, uint8_t* input, uint8_t latchPin, uint8_t dataPin, uint8_t clockPin) {
	_latchPin = latchPin;
	_dataPin = dataPin;
	_clockPin = clockPin;
	_input = input;
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

void Slider::updateStatus() {
	int iterations = (_size/8);

	for(int i=0; i<=iterations; i++) {
		/*
		* Pulse the latch pin:
  		* set it to 1 to collect parallel data
  		*/
  		digitalWrite(_latchPin, 1);
  		//set it to 1 to collect parallel data, wait
  		digitalWrite(_clockPin, 1);
  		delayMicroseconds(50);
  		//set it to 0 to transmit data serially
  		digitalWrite(_latchPin, 0);

  		/*
  		* While the shift register is in serial mode
  		* collect each shift register into a byte
  		* the register attached to the chip comes in first
  		*/
  		*_input = shiftIn(_dataPin, _clockPin);

  		//Debuging print statements
  		//Serial.print(*input, BIN);
  		//Serial.print(" - ");
  	}
}

uint8_t Slider::shiftIn(int myDataPin, int myClockPin) {
	int i;
  	int temp = 0;
  	int pinState;
  	byte myDataIn = 0;

  	pinMode(myClockPin, OUTPUT);
  	pinMode(myDataPin, INPUT);
  	/*
  	* We will be holding the clock pin high 8 times (0,..,7) at the
  	* end of each time through the for loop
	*
  	* At the begining of each loop when we set the clock low, it will
  	* be doing the necessary low to high drop to cause the shift
  	* register's DataPin to change state based on the value
  	* of the next bit in its serial information flow.
  	* The register transmits the information about the pins from pin 7 to pin 0
  	* so that is why our function counts down
  	*/
  	for (i = 7; i >= 0; i--) {
	    digitalWrite(myClockPin, 0);
	    delayMicroseconds(0.2);
	    temp = digitalRead(myDataPin);
	    if (temp) {
      		pinState = 1;
      		//set the bit to 0 no matter what
      		myDataIn = myDataIn | (1 << i);
    	}
    	else {
      		pinState = 0;
    	}

    //Debuging print statements
    //Serial.print(pinState);
    digitalWrite(myClockPin, 1);

  }
  return myDataIn;
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
