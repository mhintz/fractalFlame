#pragma once

#include "ofMain.h"

#include "types.h"

class TransformFunction {
public:
		float probability;
		float pA, pB, pC;
		float pD, pE, pF;
		ofFloatColor color;

		TransformFunction();
		TransformFunction(float _probability, float _A, float _B, float _C, float _D, float _E, float _F, ofFloatColor _color);

		Sample apply(const Sample & input);
};