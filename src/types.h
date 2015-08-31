#pragma once

#include "ofMain.h"

typedef struct Sample {
		ofVec2f position;
		ofFloatColor color;

		Sample(ofVec2f _position, ofFloatColor _color) :
		position(_position), color(_color) {};
} Sample;