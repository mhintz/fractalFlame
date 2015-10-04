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
	bool hasReflection;
	bool ratioFixed;
	float colorMin;
	float colorMax;

	TransformParameters() {
		maxRotAngle = TWO_PI;
		maxSkewAngle = ofRandom(0.f, PI / 4.f);
		minScale = 0.4f;
		maxScale = 1.f;
		hasReflection = false;
		ratioFixed = false;

		// Color settings
		colorMin = 48.f / 255.f;
		colorMax = 1.f;
	}

	static TransformParameters rigidMaps() {
		TransformParameters retVal;

		// rigid maps parameters
		retVal.maxRotAngle = TWO_PI;
		retVal.maxSkewAngle = 0.f;
		retVal.minScale = 0.4f;
		retVal.maxScale = 1.f;
		retVal.hasReflection = false;
		retVal.ratioFixed = true;

		return retVal;
	}
};