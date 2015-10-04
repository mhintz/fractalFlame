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
	float colorMin;
	float colorMax;

	TransformParameters() {
		maxRotAngle = TWO_PI;
		maxSkewAngle = ofRandom(0.f, PI / 4.f);
		minScale = 0.33f;
		maxScale = 0.67f;
		colorMin = 48.f / 255.f;
		colorMax = 1.f;
	}
};