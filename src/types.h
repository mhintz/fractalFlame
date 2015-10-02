#pragma once

#include "ofMain.h"

class Sample {
public:
	ofVec2f position;
	ofFloatColor color;

	Sample(ofVec2f _position) :
	position(_position) {};

	Sample(ofVec2f _position, ofFloatColor _color) :
	position(_position), color(_color) {};
};

class TransformParameters {
public:
	float maxRotAngle;
	float maxSkewAngle;
	float maxScale;
	float minScale;

	TransformParameters() {
		maxRotAngle = TWO_PI;
		maxSkewAngle = ofRandom(PI / 4.f);
		minScale = 0.33f;
		maxScale = 0.67f;
	}
};