#pragma once

#include "ofMain.h"

#include "types.h"

class TransformFunction {
public:
		float probability;
		float xx, xy, xt;
		float yx, yy, yt;
		ofFloatColor color;

		TransformFunction();
		TransformFunction(float _probability, float _A, float _B, float _C, float _D, float _E, float _F, ofFloatColor _color);
		TransformFunction(TransformParameters);

		Sample apply(const Sample & input);
};